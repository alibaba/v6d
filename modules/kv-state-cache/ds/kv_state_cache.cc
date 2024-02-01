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

#include <cstdlib>

#include "client/client.h"
#include "common/util/base64.h"
#include "common/util/logging.h"
#include "common/util/status.h"
#include "kv-state-cache/radix-tree/radix-tree.h"
#include "kv-state-cache/radix-tree/radix.h"
#include "kv_state_cache.h"

namespace vineyard {

void KVStateCache::Construct(const ObjectMeta& meta) {
  Object::Construct(meta);
  Resolve();
}

void KVStateCache::Resolve() {
  LOG(INFO) << "Resolve";
  std::string typeName = type_name<KVStateCache>();

  VINEYARD_ASSERT(this->meta_.GetTypeName() == typeName,
                  "Expect typename '" + typeName + "', but got '" +
                      this->meta_.GetTypeName() + "'");

  // 1. construct the radix tree
  this->rootTree = RadixTree::Deserialize(
      base64_decode(this->meta_.GetKeyValue<std::string>("radix_tree")));
  LOG(INFO) << "Resolve RadixTree success" << std::endl;
  raxShow(this->rootTree->GetRootTree());

  // 2. construct the kvStateCacheBlockBuilder list
  size_t numBlocks = this->meta_.GetKeyValue<size_t>("numBlocks");
  LOG(INFO) << "num blocks:" << numBlocks;
  for (size_t i = 0; i < numBlocks; i++) {
    std::shared_ptr<Object> kvStateCacheBlockObject = this->meta_.GetMember(
        "kv_state_cache_block_builder_" + std::to_string(i));
    this->kvStateCacheBlockList.push_back(
        std::dynamic_pointer_cast<KVStateCacheBlock>(kvStateCacheBlockObject));
    this->kvStateCacheBlockMap[kvStateCacheBlockObject->id()] =
        std::dynamic_pointer_cast<KVStateCacheBlock>(kvStateCacheBlockObject);
    LOG(INFO) << "kvStateCacheBlockObject:" << kvStateCacheBlockObject->id();
  }

  // 3. construct the member field
  this->dimension = this->meta_.GetKeyValue<int>("dimension");
  this->version = this->meta_.GetKeyValue<uint64_t>("version");
  LOG(INFO) << "construct the member field success" << std::endl;
}

KVStateCache::~KVStateCache() {
  // TBD
}

KVStateCacheBuilder::KVStateCacheBuilder(Client& client, int dimension,
                                         int cacheCapacity) {
  this->dimension = dimension;
  this->version = 0;
  KVStateCacheBlockBuilder* builder =
      new KVStateCacheBlockBuilder(client, this->dimension);

  this->rootTree = std::make_shared<RadixTree>(cacheCapacity);

  TreeData* treeData = new TreeData();
  treeData->kvStateCacheBlockBuilder = builder;
  treeData->isPtr = true;

  std::shared_ptr<NodeData> rootTreeHeader = this->rootTree->GetRootNode();
  rootTreeHeader->treeData->data = treeData;
  rootTreeHeader->treeData->dataLength = sizeof(TreeData);
  this->rootTree->SetSubtreeData(treeData, sizeof(TreeData));
  LOG(INFO) << "set builder:" << builder
            << " to tree:" << this->rootTree->GetRootTree()->head;
  LOG(INFO) << "data:" << treeData
            << " custom data:" << rootTreeHeader->treeData;
}

KVStateCacheBuilder::KVStateCacheBuilder(Client& client,
                                         std::shared_ptr<KVStateCache> cache) {
  // TBD
  this->dimension = cache->GetDemension();
  this->version = cache->GetVersion();
  // 1. create block builder from block
  std::map<uint64_t, std::shared_ptr<KVStateCacheBlock>> kvStateCacheBlockMap =
      cache->kvStateCacheBlockMap;
  this->rootTree = cache->GetRootTree();
  std::set<void*> subTreeData = cache->rootTree->GetSubTreeDataSet();

  for (auto iter = subTreeData.begin(); iter != subTreeData.end(); ++iter) {
    TreeData* treeData = (TreeData*) ((DataWrapper*) *iter)->data;
    LOG(INFO) << "tree data:" << treeData;
    VINEYARD_ASSERT(treeData->isPtr == false);
    LOG(INFO) << "id:" << treeData->builderObjectID;
    std::shared_ptr<KVStateCacheBlock> kvStateCacheBlock =
        kvStateCacheBlockMap[treeData->builderObjectID];
    KVStateCacheBlockBuilder* kvStateCacheBlockBuilder =
        new KVStateCacheBlockBuilder(client, kvStateCacheBlock);

    treeData->kvStateCacheBlockBuilder = kvStateCacheBlockBuilder;
    treeData->isPtr = true;
  }
}

KVStateCacheBlockBuilder* KVStateCacheBuilder::Split(
    Client& client, KVStateCacheBlockBuilder* kvStateCacheBlockBuilder,
    std::vector<std::shared_ptr<NodeData>> nodeDataList) {
  LOG(INFO) << "split";
  // Split the tree if the list of kvState is full.
  VINEYARD_ASSERT(nodeDataList.size() > 0);
  KVStateCacheBlockBuilder* childKVStateCacheBlockBuilder =
      new KVStateCacheBlockBuilder(client, this->dimension);
  for (size_t i = 0; i < nodeDataList.size(); i++) {
    OffsetData* data = (OffsetData*) nodeDataList[i]->nodeData->data;
    if (data == nullptr)
      continue;
    int index = data->offset;

    // Transfer the data from this builder to the child builder.
    const std::shared_ptr<TensorBuilder<double>> keyStateTensorBuilder =
        kvStateCacheBlockBuilder->GetKeyStateBuilder();
    const std::shared_ptr<TensorBuilder<double>> valueStateTensorBuilder =
        kvStateCacheBlockBuilder->GetValueStateBuilder();
    OffsetData* new_offset_data = new OffsetData();
    childKVStateCacheBlockBuilder->Update(
        keyStateTensorBuilder->data() + index * this->dimension,
        valueStateTensorBuilder->data() + index * this->dimension,
        this->dimension, new_offset_data);
    nodeDataList[i]->nodeData->data = new_offset_data;
    nodeDataList[i]->nodeData->dataLength = sizeof(OffsetData);
    // Clear the bitmap.
    kvStateCacheBlockBuilder->DeleteKVCache(index);
  }
  LOG(INFO) << "builder:" << kvStateCacheBlockBuilder
            << " bitmap:" << kvStateCacheBlockBuilder->GetBitmapStr();
  LOG(INFO) << "child_builder:" << childKVStateCacheBlockBuilder
            << " bitmap:" << childKVStateCacheBlockBuilder->GetBitmapStr();
  return childKVStateCacheBlockBuilder;
}

void KVStateCacheBuilder::Update(Client& client,
                                 const std::vector<int>& tokenList,
                                 int nextToken,
                                 const KV_STATE_WITH_LAYER& kvState) {
  LOG(INFO) << "update";
  std::vector<int> tokenListCopy = tokenList;
  tokenListCopy.push_back(nextToken);

  // Create a empty node of tokens from radix tree.
  std::shared_ptr<NodeData> evictedNodeData = nullptr;
  std::shared_ptr<NodeData> nodeData =
      this->rootTree->Insert(tokenListCopy, evictedNodeData);
  if (nodeData == nullptr) {
    LOG(INFO) << "insert failed";
    return;
  }
  LOG(INFO) << "insert end";
  KVStateCacheBlockBuilder* kvStateCacheBlockBuilder =
      (KVStateCacheBlockBuilder*) ((TreeData*) nodeData->treeData->data)
          ->kvStateCacheBlockBuilder;
  LOG(INFO) << "try to delete";
  if (evictedNodeData != nullptr) {
    Delete(evictedNodeData);
  }

  // TBD
  // Use lock to protect the kv_state_cache_builder
  LOG(INFO) << "data:" << nodeData->treeData->data
            << " custom data:" << nodeData->treeData;
  LOG(INFO) << "kvStateCacheBlockBuilder:" << kvStateCacheBlockBuilder;
  if (kvStateCacheBlockBuilder->IsFull()) {
    /**
     * If the kv-state cache of the tree is full, triggle split. Delete the
     * empty node from the radix tree and split the tree. Then, kv-state cache
     * split according to the new tree.
     */
    LOG(INFO) << "triggle splits";
    std::shared_ptr<NodeData> evictedNodeData = nullptr;
    this->rootTree->Delete(tokenListCopy, evictedNodeData);

    std::shared_ptr<NodeData> subTreeHeader;
    std::vector<std::shared_ptr<NodeData>> nodeDataList =
        rootTree->Split(tokenListCopy, subTreeHeader);
    KVStateCacheBlockBuilder* newKVStateCacheBlockBuilder =
        Split(client, kvStateCacheBlockBuilder, nodeDataList);

    TreeData* newTreeData = new TreeData();
    newTreeData->kvStateCacheBlockBuilder = newKVStateCacheBlockBuilder;
    newTreeData->isPtr = true;

    subTreeHeader->treeData->data = newTreeData;
    subTreeHeader->treeData->dataLength = sizeof(TreeData);
    rootTree->SetSubtreeData(newTreeData, sizeof(TreeData));
    LOG(INFO) << "block split success";

    // kv_state_cache_builder->UnLock();
    Update(client, tokenList, nextToken, kvState);
  } else {
    // Update the kv-state cache.
    OffsetData* data = new OffsetData();
    kvStateCacheBlockBuilder->Update(kvState, data);
    nodeData->nodeData->data = data;
    nodeData->nodeData->dataLength = sizeof(OffsetData);
  }

  LOG(INFO) << "builder:" << kvStateCacheBlockBuilder
            << " bitmap:" << kvStateCacheBlockBuilder->GetBitmapStr();
}

static std::shared_ptr<NodeData> node;

KV_STATE_WITH_LAYER KVStateCacheBuilder::Query(
    Client& client, const std::vector<int>& tokenList, int token) {
  std::vector<int> tokenListCopy = tokenList;
  tokenListCopy.push_back(token);

  KV_STATE_WITH_LAYER kvState;
  std::shared_ptr<NodeData> nodeData = this->rootTree->Query(tokenListCopy);

  if (nodeData != nullptr) {
    OffsetData* data = (OffsetData*) nodeData->nodeData->data;
    int offset = data->offset;

    KVStateCacheBlockBuilder* kvStateCacheBlockBuilder =
        (KVStateCacheBlockBuilder*) ((TreeData*) nodeData->treeData->data)
            ->kvStateCacheBlockBuilder;

    LOG(INFO) << "offset:" << offset;
    LOG(INFO) << "kvStateCacheBlockBuilder:" << kvStateCacheBlockBuilder;
    kvStateCacheBlockBuilder->Query(client, offset, kvState);
  }
  return kvState;
}

void KVStateCacheBuilder::Delete(std::shared_ptr<NodeData> evictedNodeData) {
  LOG(INFO) << "stage1";
  KVStateCacheBlockBuilder* kvStateCacheBlockBuilder =
      (KVStateCacheBlockBuilder*) ((TreeData*) evictedNodeData->treeData->data)
          ->kvStateCacheBlockBuilder;
  LOG(INFO) << "stage2, builder:" << kvStateCacheBlockBuilder;
  OffsetData* data = (OffsetData*) evictedNodeData->nodeData->data;
  LOG(INFO) << "stage3";
  kvStateCacheBlockBuilder->DeleteKVCache(data->offset);
  LOG(INFO) << "stage4";
  delete data;
}

void KVStateCacheBuilder::Merge(Client& client,
                                std::shared_ptr<KVStateCache> kvStateCache) {
  if (kvStateCache == nullptr) {
    return;
  }

  std::shared_ptr<KVStateCacheBuilder> globalCacheBuilder =
      std::make_shared<KVStateCacheBuilder>(client, kvStateCache);
  std::shared_ptr<RadixTree> globalCacheTree = kvStateCache->GetRootTree();

  std::set<std::vector<int>> insertTokenList;
  std::vector<std::vector<int>> evicted_token_list;
  mergeTree(this->rootTree->GetRootTree(), globalCacheTree->GetRootTree(),
            evicted_token_list, insertTokenList,
            this->rootTree->GetCacheCapacity());

  for (size_t i = 0; i < evicted_token_list.size(); i++) {
    std::vector<int> tokenList = evicted_token_list[i];
    std::shared_ptr<NodeData> evictedNodeData;
    this->rootTree->Delete(tokenList, evictedNodeData);
    Delete(evictedNodeData);
  }

  for (auto it = insertTokenList.begin(); it != insertTokenList.end(); ++it) {
    std::vector<int> tokenList = *it;
    KV_STATE_WITH_LAYER kvState = globalCacheBuilder->Query(
        client, std::vector<int>(tokenList.begin(), tokenList.end() - 1),
        tokenList.back());
    this->Update(client, tokenList, tokenList[tokenList.size() - 1], kvState);
  }
  return;
}

Status KVStateCacheBuilder::Build(Client& client) {
  // TBD
  return Status::OK();
}

std::shared_ptr<Object> KVStateCacheBuilder::_Seal(Client& client) {
  LOG(INFO) << "cache seal";
  this->Build(client);

  std::shared_ptr<KVStateCache> kvStateCache = std::make_shared<KVStateCache>();

  // 1. store the member variables to cache object meta
  kvStateCache->meta_.AddKeyValue("dimension", this->dimension);
  kvStateCache->meta_.AddKeyValue("version", this->version);

  // 2. seal all the block and put object id to cache object and
  // change the tree data from pointer to object id

  int count = 0;
  LOG(INFO) << "count:" << count;
  std::set<void*> subTreeDataSet = rootTree->GetSubTreeDataSet();
  for (auto iter = subTreeDataSet.begin(); iter != subTreeDataSet.end();
       ++iter) {
    TreeData* treeData = (TreeData*) ((DataWrapper*) *iter)->data;
    VINEYARD_ASSERT(treeData != nullptr);
    VINEYARD_ASSERT(treeData->isPtr == true);

    KVStateCacheBlockBuilder* kvStateCacheBlockBuilder =
        (KVStateCacheBlockBuilder*) treeData->kvStateCacheBlockBuilder;
    LOG(INFO) << "builder:" << kvStateCacheBlockBuilder;
    std::shared_ptr<Object> kvStateCacheBlock =
        kvStateCacheBlockBuilder->_Seal(client);
    kvStateCache->meta_.AddMember(
        "kv_state_cache_block_builder_" + std::to_string(count),
        kvStateCacheBlock);
    treeData->builderObjectID = kvStateCacheBlock->id();
    treeData->isPtr = false;
    count++;
  }

  kvStateCache->meta_.AddKeyValue("numBlocks", count);

  // 3. put the serialized sequence radix tree to cache object meta
  kvStateCache->meta_.AddKeyValue("radix_tree",
                                  base64_encode(this->rootTree->Serialize()));

  // 4. put the object type to the meta
  kvStateCache->meta_.SetTypeName(type_name<KVStateCache>());

  VINEYARD_CHECK_OK(
      client.CreateMetaData(kvStateCache->meta_, kvStateCache->id_));
  LOG(INFO) << "KVStateCacheBuilder::_Seal: " << kvStateCache->id_;
  return kvStateCache;
}

KVStateCacheBuilder::~KVStateCacheBuilder() {
  // TBD
  // std::vector<std::shared_ptr<NodeData>> nodeDataList =
  //     RadixTree::TraverseTreeWithoutSubTree(this->rootTree);
  // for (size_t i = 0; i < nodeDataList.size(); i++) {
  //   delete (OffsetData*) nodeDataList[i]->get_node()->get_data();
  // }
}

}  // namespace vineyard
