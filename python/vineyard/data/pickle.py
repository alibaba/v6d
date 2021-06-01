#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2021 Alibaba Group Holding Limited.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

'''
Pickle support for arbitrary vineyard objects.
'''

from io import BytesIO

import pickle
if pickle.HIGHEST_PROTOCOL < 5:
    import pickle5 as pickle


class PickledReader:
    ''' Serialize a python object in zero-copy fashion and provides a bytes-like
        read interface.
    '''
    def __init__(self, value):
        self._value = value
        self._buffers = [None]
        self._store_size = 0

        buffers = []
        bs = pickle.dumps(value, protocol=5, fix_imports=True, buffer_callback=buffers.append)

        meta = BytesIO()
        meta.write(b'__VINEYARD__')
        self.poke_int(meta, len(buffers))
        for buf in buffers:
            raw = buf.raw()
            self._buffers.append(raw)
            self._store_size += len(raw)
            self.poke_int(meta, len(raw))

        self.poke_int(meta, len(bs))
        meta.write(bs)
        self._buffers[0] = memoryview(meta.getbuffer())
        self._store_size += len(self._buffers[0])

        self._chunk_index = 0
        self._chunk_offset = 0

    @property
    def value(self):
        return self._value

    @property
    def store_size(self):
        return self._store_size

    def poke_int(self, bs, value):
        bs.write(int.to_bytes(value, length=8, byteorder='big'))

    def read(self, block_size):
        assert block_size >= 0, "The next chunk size to read must be greater than 0"
        if self._chunk_offset == len(self._buffers[self._chunk_index]):
            self._chunk_index += 1
            self._chunk_offset = 0
        if self._chunk_index >= len(self._buffers):
            return b''
        chunk = self._buffers[self._chunk_index]
        offset = self._chunk_offset
        next_offset = min(len(chunk), offset + block_size)
        result = self._buffers[self._chunk_index][offset:next_offset]
        self._chunk_offset += len(result)
        return result


class PickledWriter:
    ''' Deserialize a pickled bytes into a python object in zero-copy fashion.
    '''
    def __init__(self, store_size):
        self._store_size = store_size
        self._value = None
        self._buffers = None
        self._buffer_sizes = None

        self._chunk_index = 0
        self._chunk_offset = 0

    def write(self, bs):
        if self._buffer_sizes = None:
            meta = bytes

    def peek_int(self, bs, offset):
        value = int.from_bytes(bs[offset:offset + 8], byteorder='big')
        self._chunk_offset += 8
        return value


__all__ = []
