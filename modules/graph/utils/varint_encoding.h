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

#ifndef MODULES_GRAPH_UTILS_VARINT_ENCODING_H_
#define MODULES_GRAPH_UTILS_VARINT_ENCODING_H_

#include <cstdint>
#include <cstring>
#include <vector>

#include "powturbo/include/ic.h"

#include "common/util/likely.h"

namespace vineyard {

#define UPPER_OF_RANGE_1 185
#define UPPER_OF_RANGE_2 249

#define VARINT_ENCODING_BATCH_SIZE 16

inline uint64_t unaligned_load_u64(const uint8_t* p) {
  uint64_t x;
  std::memcpy(&x, p, 8);
  return x;
}

template <typename T>
inline uint8_t* varint_encode(T input, uint8_t* output) {
  if (input < UPPER_OF_RANGE_1) {
    output[0] = static_cast<uint8_t>(input);
    return output + 1;
  } else if (input <= UPPER_OF_RANGE_1 + 255 +
                          256 * (UPPER_OF_RANGE_2 - 1 - UPPER_OF_RANGE_1)) {
    input -= UPPER_OF_RANGE_1;
    output[0] = UPPER_OF_RANGE_1 + (input >> 8);
    output[1] = input & 0xff;
    return output + 2;
  } else {
    unsigned bits = 64 - __builtin_clzll(input);
    unsigned bytes = (bits + 7) / 8;
    output[0] = UPPER_OF_RANGE_2 + (bytes - 2);
    for (unsigned n = 0; n < bytes; n++) {
      output[n + 1] = input & 0xff;
      input >>= 8;
    }
    return output + 1 + bytes;
  }
}

template <typename T>
inline size_t varint_decode(const uint8_t* input, T& output) {
  const uint8_t* origin_input = input;
  uint8_t byte_0 = *input++;
  if (likely(byte_0 < UPPER_OF_RANGE_1)) {
    output = byte_0;
  } else if (byte_0 < UPPER_OF_RANGE_2) {
    uint8_t byte_1 = *input++;
    output = UPPER_OF_RANGE_1 + byte_1 + ((byte_0 - UPPER_OF_RANGE_1) << 8);
  } else {
    size_t sh = byte_0 - UPPER_OF_RANGE_2;
    output = unaligned_load_u64(input) & ((uint64_t(1) << 8 * sh << 16) - 1);
    input += 2 + sh;
  }
  return static_cast<size_t>(input - origin_input);
}

namespace powturbo {}  // namespace powturbo

}  // namespace vineyard

#endif  // MODULES_GRAPH_UTILS_VARINT_ENCODING_H_
