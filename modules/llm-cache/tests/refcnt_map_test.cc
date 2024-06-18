/** Copyright 2020-2023 Alibaba Group Holding Limited.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <unistd.h>
#include <iostream>
#include <vector>

#include "rax/radix.h"

#include "common/util/logging.h"
#include "llm-cache/ds/kv_cache_manager.h"

using namespace vineyard;  // NOLINT(build/namespaces)

constexpr int tensorNBytes = 80;
constexpr int capacity = 5;
constexpr int layer = 3;
constexpr int block_size = 4;

std::vector<int> round_1_tokens = {1, 2, 3, 4, 5};   // split to two blocks
std::vector<int> round_2_tokens = {1, 2, 4, 9, 10};  // split to two blocks
std::vector<std::vector<int>> round_token_list = {round_1_tokens,
                                                  round_2_tokens};

std::vector<std::shared_ptr<KVCacheManager>> kv_cache_managers;
std::vector<std::shared_ptr<BlobStorage>> blob_storages;
std::string llmCacheObjectName = "refcnt_map_test_cache_object";
std::string llmCacheSyncLock = "refcnt_map_test_cache_lock";
std::string llmRefcntObjectName = "refcnt_map_test_refcnt_object";

Client client[2];

void print_current_tokens(const std::vector<int>& prefix, int next_token) {
  std::string tokens_str = "";
  for (size_t i = 0; i < prefix.size(); ++i) {
    tokens_str += std::to_string(prefix[i]) + " ";
  }
  tokens_str += std::to_string(next_token);
  LOG(INFO) << "Current tokens: " + tokens_str;
}

void print_kv_state(const std::vector<std::pair<LLMKV, LLMKV>>& kv_state) {
  LOG(INFO) << "kv_state: ";
  for (size_t i = 0; i < kv_state.size(); ++i) {
    uint8_t* key_state_data =
        reinterpret_cast<uint8_t*>(kv_state[i].first.data);
    uint8_t* value_state_data =
        reinterpret_cast<uint8_t*>(kv_state[i].second.data);
    // print the first tensorNBytes bytes
    std::string key_state_str = "";
    std::string value_state_str = "";
    for (int j = 0; j < tensorNBytes; j++) {
      key_state_str += std::to_string(key_state_data[j]) + " ";
      value_state_str += std::to_string(value_state_data[j]) + " ";
    }
    LOG(INFO) << "layer " << i << ":";
    LOG(INFO) << "key_state: " << key_state_str;
    LOG(INFO) << "value_state: " << value_state_str;
    LOG(INFO) << "---------------------";
  }
}

// we do not consider the layer.
std::vector<std::pair<LLMKV, LLMKV>> generate_kv_state(int token) {
  std::vector<std::pair<LLMKV, LLMKV>> kv_state;
  for (int currentLayer = 0; currentLayer < layer; currentLayer++) {
    LLMKV key_state;
    LLMKV value_state;
    key_state.data = malloc(tensorNBytes);
    value_state.data = malloc(tensorNBytes);

    key_state.length = tensorNBytes;
    value_state.length = tensorNBytes;

    for (int i = 0; i < tensorNBytes; ++i) {
      (reinterpret_cast<uint8_t*>(key_state.data))[i] =
          (static_cast<uint8_t>(token)) + i + currentLayer;
      (reinterpret_cast<uint8_t*>(value_state.data))[i] =
          (static_cast<uint8_t>(token)) + i + currentLayer;
    }

    kv_state.emplace_back(key_state, value_state);
  }
  return kv_state;
}

void check_kv_state(const std::vector<std::pair<LLMKV, LLMKV>>& kv_state,
                    int& token) {
  VINEYARD_ASSERT(kv_state.size() == (size_t) layer);
  for (size_t index = 0; index < kv_state.size(); ++index) {
    VINEYARD_ASSERT(kv_state[index].first.length == (size_t) tensorNBytes);
    VINEYARD_ASSERT(kv_state[index].second.length == (size_t) tensorNBytes);
    for (int i = 0; i < tensorNBytes; ++i) {
      if ((reinterpret_cast<uint8_t*>(kv_state[index].first.data))[i] !=
          (static_cast<uint8_t>(token)) + i + index) {
        LOG(INFO) << "token:" << token << " tensorNBytes" << tensorNBytes
                  << " layer:" << index;
        LOG(INFO) << "key_state[" << i << "]: "
                  << (reinterpret_cast<uint8_t*>(kv_state[index].first.data))[i]
                  << ". But is should be "
                  << (static_cast<uint8_t>(token)) + i + index;
        throw std::runtime_error("key_state error!");
      }
      if (reinterpret_cast<uint8_t*>(kv_state[index].second.data)[i] !=
          (static_cast<uint8_t>(token)) + i + index) {
        LOG(INFO) << "token:" << token << " tensorNBytes" << tensorNBytes
                  << " layer:" << index;
        LOG(INFO) << "value_state[" << i << "]: "
                  << (reinterpret_cast<uint8_t*>(
                         kv_state[index].second.data))[i]
                  << ". But is should be "
                  << (static_cast<uint8_t>(token)) + i + index;
        throw std::runtime_error("value_state error!");
      }
    }
  }
}

void inference(std::shared_ptr<KVCacheManager>& kv_cache_manager,
               std::vector<int> tokens, size_t begin = 0) {
  std::vector<int> inference_tokens;
  std::vector<std::pair<LLMKV, LLMKV>> kv_state;
  std::vector<std::pair<LLMKV, LLMKV>> kv_state_to_query;
  for (size_t i = 0; i < tokens.size(); ++i) {
    if (i >= begin) {
      kv_state.clear();
      kv_state_to_query.clear();
      for (int current_layer = 0; current_layer < layer; current_layer++) {
        kv_state_to_query.emplace_back(LLMKV{nullptr, 0}, LLMKV{nullptr, 0});
      }
      Status result = kv_cache_manager->Query(inference_tokens, tokens[i],
                                              kv_state_to_query);
      if (!result.ok()) {
        LOG(INFO) << "Can not find the kv_state from cache:";
        print_current_tokens(inference_tokens, tokens[i]);
        LOG(INFO) << "Generate the kv_state and update the cache.";
        kv_state = generate_kv_state(tokens[i]);
        // print_kv_state(kv_state);
        Status status =
            kv_cache_manager->Update(inference_tokens, tokens[i], kv_state);
        if (!status.ok()) {
          // Not a error. May be the cache is full.
          LOG(INFO) << "Put kv state into cache failed:" << status.ToString();
        }
      } else {
        LOG(INFO) << "Find the kv_state from cache:";
        print_current_tokens(inference_tokens, tokens[i]);
        check_kv_state(kv_state_to_query, tokens[i]);
      }
      LOG(INFO) << "--------------------------------------";
    }
    inference_tokens.push_back(tokens[i]);
  }
}

/**
 * After sync:
 * The first kv cache manager will hold the "3, 4, 5" block id and the
 * "1, 2" block id.
 * The second kv cache manager will hold the "4, 9, 10" block ptr and "1, 2"
 * block id. "3, 4, 5" block will be deleted.
 * The second "1, 2" block id is different from the first one. So there
 * exists 4 block id in the global refcnt map.
 */

Status checkRefCnt(std::string ipc_socket) {
  LOG(INFO) << "------------ check ref cnt ---------------";

  std::vector<std::set<void*>> treeDataSets;
  treeDataSets.push_back(blob_storages[0]
                             ->GetKVCacheBuilder()
                             ->GetRootTree()
                             ->GetSubTreeDataSet());
  treeDataSets.push_back(blob_storages[1]
                             ->GetKVCacheBuilder()
                             ->GetRootTree()
                             ->GetSubTreeDataSet());
  LOG(INFO) << raxShow(
      blob_storages[0]->GetKVCacheBuilder()->GetRootTree()->GetRootTree());
  LOG(INFO) << "------------------------------------------";
  LOG(INFO) << raxShow(
      blob_storages[1]->GetKVCacheBuilder()->GetRootTree()->GetRootTree());

  std::shared_ptr<RefcntMapObjectBuilder> refcnt_map =
      std::make_shared<RefcntMapObjectBuilder>(client[0]);
  std::set<ObjectID> blockIDSetToAdd;
  for (size_t i = 0; i < treeDataSets.size(); i++) {
    blockIDSetToAdd.clear();
    for (auto iter = treeDataSets[i].begin(); iter != treeDataSets[i].end();
         ++iter) {
      TreeData* treeData = reinterpret_cast<TreeData*>(*iter);
      if (!treeData->isPtr) {
        blockIDSetToAdd.insert(treeData->builderObjectID);
      }
    }
    refcnt_map->IncSetRefcnt(blockIDSetToAdd);
  }

  ObjectID globalCacheObjectID;
  blockIDSetToAdd.clear();
  VINEYARD_CHECK_OK(client[0].GetName(llmCacheObjectName, globalCacheObjectID));
  std::shared_ptr<KVCache> kvCache = std::dynamic_pointer_cast<KVCache>(
      client[0].FetchAndGetObject(globalCacheObjectID));
  kvCache->GetCurrentBlockIDSet(blockIDSetToAdd);
  refcnt_map->IncSetRefcnt(blockIDSetToAdd);
  if (kvCache->id() != globalCacheObjectID) {
    client[0].DelData(kvCache->id());
  }

  LOG(INFO) << "Prepare refcnt done";

  ObjectID globalRefcntMapId;
  VINEYARD_CHECK_OK(client[0].GetName(llmRefcntObjectName, globalRefcntMapId));
  std::shared_ptr<RefcntMapObject> globalRefcntMap;
  VINEYARD_CHECK_OK(
      client[0].FetchAndGetObject(globalRefcntMapId, globalRefcntMap));

  std::shared_ptr<RefcntMapObjectBuilder> globalRefcntMapBuilder =
      std::make_shared<RefcntMapObjectBuilder>(client[0], globalRefcntMap);
  VINEYARD_ASSERT(globalRefcntMapBuilder->Equals(refcnt_map));
  if (globalRefcntMap->id() != globalRefcntMapId) {
    client[0].DelData(globalRefcntMap->id());
  }
  return Status::OK();
}

void threadFunc(std::string socket, int threadId) {
  LOG(INFO) << "Thread:" << threadId << " start, with socket:" << socket;
  if (threadId == 1) {
    // sleep 4 seconds to wait for the first thread to create the global cache.
    sleep(4);
  }

  std::shared_ptr<KVCacheManager> kv_cache_manager;
  std::shared_ptr<BlobStorage> blob_storage;
  VINEYARD_CHECK_OK(BlobStorage::Make(
      client[threadId], blob_storage, tensorNBytes, capacity, layer, block_size,
      3, llmCacheSyncLock, llmCacheObjectName, llmRefcntObjectName));
  blob_storages.push_back(blob_storage);
  kv_cache_manager = std::make_shared<KVCacheManager>(blob_storage);
  kv_cache_managers.push_back(kv_cache_manager);

  std::vector<int> tokenList = round_token_list[threadId];
  if (threadId == 1) {
    inference(kv_cache_manager, tokenList, 2);
  } else {
    inference(kv_cache_manager, tokenList);
  }

  sleep(5);

  blob_storage->StopSync();
}

void clearGlobalObject() {
  VINEYARD_CHECK_OK(BlobStorage::ClearGlobalCache(
      client[0], llmCacheSyncLock, llmCacheObjectName, llmRefcntObjectName));

  LOG(INFO) << "After clear global object:";
  for (int i = 0; i < 2; i++) {
    std::shared_ptr<InstanceStatus> status;
    VINEYARD_CHECK_OK(client[i].InstanceStatus(status));
    LOG(INFO) << "Client " << client[i].instance_id()
              << " mem usage:" << status->memory_usage;

    if (status->memory_usage != 0) {
      std::vector<ObjectMeta> metas = client[i].ListObjectMeta(".*", true);
      for (auto meta : metas) {
        LOG(INFO) << meta.ToString();
      }
      // VINEYARD_ASSERT(false);
    }

    client[i].Disconnect();
  }
}

int main(int argc, char** argv) {
  std::string sockets[2];
  if (argc < 2) {
    printf(
        "Usage ./kv_cache_test "
        "<ipc_socket_1> <ipc_socket_2> -d \n");
    return 1;
  }

  for (int i = 0; i < 2; i++) {
    sockets[i] = std::string(argv[i + 1]);
    VINEYARD_CHECK_OK(client[i].Connect(sockets[i]));
  }

  std::vector<std::thread> threads;
  for (int i = 0; i < 2; i++) {
    threads.push_back(std::thread(threadFunc, sockets[i], i));
  }

  for (int i = 0; i < 2; i++) {
    threads[i].join();
    LOG(INFO) << "Thread " << i << " exited.";
  }

  VINEYARD_CHECK_OK(checkRefCnt(sockets[0]));

  for (int i = 0; i < 2; i++) {
    kv_cache_managers[i]->Close();
    kv_cache_managers[i] = nullptr;
  }

  LOG(INFO) << "Clear global object";
  clearGlobalObject();
  LOG(INFO) << "Passed refcnt map test.";
  return 0;
}
