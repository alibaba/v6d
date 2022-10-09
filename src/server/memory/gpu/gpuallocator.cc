/** Copyright 2020-2022 Alibaba Group Holding Limited.

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

#include "server/memory/gpu/gpuallocator.h"

#include <cstdio>
#include <cstring>
#include <string>

#include "common/util/env.h"
#include "common/util/logging.h"
#include "server/memory/malloc.h"

#if defined(__linux__) || defined(__linux) || defined(linux) || \
    defined(__gnu_linux__)
#include <sys/mount.h>
#endif

namespace vineyard {

int64_t GPUBulkAllocator::gpu_allocated_ = 0;
int64_t GPUBulkAllocator::gpu_footprint_limit_ = 0;

// GPUBulkAllocator implementation
void* GPUBulkAllocator::Init(const size_t size) {
#if defined(WITH_GPUALLOCATOR)
  return Allocator::Init(size);
#endif
  gpu_allocated_ = 0;
  gpu_footprint_limit_ = size;
  return nullptr;
}

void* GPUBulkAllocator::Memalign(const size_t bytes, const size_t alignment) {
  void* mem = nullptr;
#ifdef ENABLE_GPU
  cudaError_t result = cudaMalloc(&mem, bytes);
  if (result != cudaSuccess) {
    DVLOG(10) << "cudaMalloc Error: " << cudaGetErrorString(result);
    return nullptr;
  }
  gpu_allocated_ += bytes;
#endif
  return mem;
}

void GPUBulkAllocator::Free(void* mem, size_t bytes) {
#ifdef ENABLE_GPU
  cudaError_t result = cudaFree(mem);
  if (result != cudaSuccess) {
    DVLOG(10) << "cudaFree Error: " << cudaGetErrorString(result);
  }
  gpu_allocated_ -= bytes;
#endif
}

void GPUBulkAllocator::SetFootprintLimit(size_t bytes) {
  gpu_footprint_limit_ = static_cast<int64_t>(bytes);
}

int64_t GPUBulkAllocator::GetFootprintLimit() { return gpu_footprint_limit_; }

int64_t GPUBulkAllocator::Allocated() { return gpu_allocated_; }

}  // namespace vineyard
