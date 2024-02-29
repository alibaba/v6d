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

#ifndef MODULES_LLM_CACHE_DS_KV_STATE_CACHE_BLOCK_H_
#define MODULES_LLM_CACHE_DS_KV_STATE_CACHE_BLOCK_H_

#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "basic/ds/tensor.h"
#include "client/ds/blob.h"
#include "client/ds/i_object.h"
#include "llm-cache/radix-tree/radix-tree.h"

typedef struct STATE {
  void* data;
  size_t length;
} STATE;

using K_STATE = STATE;
using V_STATE = STATE;

using KV_STATE_WITH_LAYER = std::map<int, std::pair<K_STATE, V_STATE>>;
using LIST_KV_STATE_WITH_LAYER =
    std::vector<std::map<int, std::pair<K_STATE, V_STATE>>>;
using KV_STATE = std::vector<std::pair<K_STATE, V_STATE>>;
using LIST_KV_STATE = std::vector<std::pair<K_STATE, V_STATE>>;

// Set the bit to 1, which means the resource is not being used
#define FREE_BIT_RESOURCE(value, bit) ((value) |= (((uint64_t) 1) << (bit)))

// Set the bit to 0, which means the resource is being used
#define ACQUIRE_BIT_RESOURCE(value, bit) \
  ((value) &= (~(((uint64_t) 1) << (bit))))

struct OffsetData {
  int16_t offset;
};
namespace vineyard {

#define DEFAULT_BLOCK_SIZE 64

/**
 * @brief KVStateCacheBlock is a cache for kv-cache of LLM. When a new prompt
 * comes, LLM can query KVStateCacheBlock to get the state of the kv-cache to
 * avoid calculate the kv-cache again if the new prompt is similar to the
 * previous one.
 *
 * KVStateCacheBlock is stored in vineyard as a vineyard object which contains a
 * radix tree. The token sequence is the key of the radix tree and the value
 * point out the offset of the kv-cache in the tensor list.
 *
 * KVStateCacheBlock can be shared by multiple machines.
 */

class KVStateCacheBlock : public vineyard::Registered<KVStateCacheBlock> {
 private:
  std::vector<std::shared_ptr<Tensor<double>>> keyStateTensorList;
  std::vector<std::shared_ptr<Tensor<double>>> valueStateTensorList;
  uint64_t* bitmap;
  int blockSize;
  int bitmapSize;
  ObjectID id;
  int layer;
  int dimension;

 public:
  static std::unique_ptr<Object> Create() __attribute__((used)) {
    return std::static_pointer_cast<Object>(
        std::unique_ptr<KVStateCacheBlock>{new KVStateCacheBlock()});
  }

  void Construct(const ObjectMeta& meta) override;

  std::string GetBitmapStr();

  uint64_t GetDimension() { return this->dimension; }

  uint64_t* GetBitmap() { return this->bitmap; }

  int GetBlockSize() { return this->blockSize; }

  std::shared_ptr<const Tensor<double>> GetKeyTensor(int layer) {
    return this->keyStateTensorList[layer];
  }

  std::shared_ptr<const Tensor<double>> GetValueTensor(int layer) {
    return this->valueStateTensorList[layer];
  }

  std::vector<std::shared_ptr<Tensor<double>>> GetKeyTensorList() {
    return this->keyStateTensorList;
  }

  std::vector<std::shared_ptr<Tensor<double>>> GetValueTensorList() {
    return this->valueStateTensorList;
  }

  ~KVStateCacheBlock();

  friend class KVStateCacheBlockBuilder;
};

class KVStateCacheBlockBuilder : public ObjectBuilder {
 private:
  std::vector<std::shared_ptr<TensorBuilder<double>>> keyStateTensorBuilderList;
  std::vector<std::shared_ptr<TensorBuilder<double>>>
      valueStateTensorBuilderList;
  // TBD
  // support more than 64 kv-state cache slots
  uint64_t* bitmap;
  int blockSize;
  int bitmapSize;
  int dimension;
  int layer;

  int FindEmptySlot();

 public:
  KVStateCacheBlockBuilder(Client& client, int dimension, int layer,
                           int blockSize);

  KVStateCacheBlockBuilder(
      Client& client, std::shared_ptr<KVStateCacheBlock> kv_state_cache_block);

  /**
   * @brief Update the kv-state using next token.
   *
   * @param client The vineyard client.
   * @param kv_state The kv-state of the prompt. A LLM inference can contain
   * multiple kv-states for each layer.
   */
  void Update(const KV_STATE_WITH_LAYER& kv_state, OffsetData* data);

  void Update(double* keyState, double* valueState, uint64_t dataLength,
              OffsetData* data);

  /**
   * @brief Query the kv-state using the whole token list.
   *
   * @param token_list The token list of the prompt.
   * @param token The token of the prompt.
   * @param kv_state The kv-state of the prompt returned by radix-tree. If the
   * kv-state is not found, the data of kv-state is invalid.
   */
  Status Query(Client& client, int index, KV_STATE_WITH_LAYER& kv_state);

  bool IsFull();

  Status Build(Client& client) override;

  std::shared_ptr<Object> _Seal(Client& client) override;

  int16_t Split(KVStateCacheBlockBuilder* child, int index);

  const std::shared_ptr<TensorBuilder<double>> GetKeyStateBuilder(int layer) {
    return keyStateTensorBuilderList[layer];
  }

  const std::shared_ptr<TensorBuilder<double>> GetValueStateBuilder(int layer) {
    return valueStateTensorBuilderList[layer];
  }

  const std::vector<std::shared_ptr<TensorBuilder<double>>>
  GetKeyStateBuilderList() {
    return keyStateTensorBuilderList;
  }

  const std::vector<std::shared_ptr<TensorBuilder<double>>>
  GetValueStateBuilderList() {
    return valueStateTensorBuilderList;
  }

  void DeleteKVCache(int bit) {
    FREE_BIT_RESOURCE(this->bitmap[bit / 64], bit % 64);
  }

  std::string GetBitmapStr();

  uint64_t* GetBitmap() { return this->bitmap; }

  uint64_t GetDimension() { return this->dimension; }

  int GetBlockSize() { return this->blockSize; }

  void PrintKVStateCacheBlock();

  ~KVStateCacheBlockBuilder();
};

}  // namespace vineyard

#endif  // MODULES_LLM_CACHE_DS_KV_STATE_CACHE_BLOCK_H_
