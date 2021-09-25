#! /usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright 2020-2021 Alibaba Group Holding Limited.
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

import json
import sys

import pyarrow as pa
import vineyard
from vineyard.io.byte import ByteStreamBuilder


def parse_dataframe(vineyard_socket, stream_id, proc_num, proc_index):
    client = vineyard.connect(vineyard_socket)
    streams = client.get(stream_id)
    if len(streams) != proc_num or streams[proc_index] is None:
        raise ValueError(
            f"Fetch stream error with proc_num={proc_num},proc_index={proc_index}"
        )
    instream = streams[proc_index]
    stream_reader = instream.open_reader(client)

    header_row = instream.params.get("header_row", None) == "1"
    delimiter = instream.params.get("delimiter", ",")

    builder = ByteStreamBuilder(client)
    stream = builder.seal(client)
    client.persist(stream)
    ret = {"type": "return"}
    ret["content"] = repr(stream.id)
    print(json.dumps(ret), flush=True)

    stream_writer = stream.open_writer(client)
    first_write = header_row
    while True:
        try:
            content = stream_reader.next()
        except vineyard.StreamDrainedException:
            stream_writer.finish()
            break
        buf_reader = pa.ipc.open_stream(pa.py_buffer(content))
        while True:
            try:
                batch = buf_reader.read_next_batch()
            except StopIteration:
                break
            df = batch.to_pandas()
            buf = df.to_csv(header=first_write, index=False, sep=delimiter).encode()
            first_write = False
            chunk = stream_writer.next(len(buf))
            buf_writer = pa.FixedSizeBufferWriter(pa.py_buffer(chunk))
            buf_writer.write(buf)
            buf_writer.close()


if __name__ == "__main__":
    if len(sys.argv) < 5:
        print(
            "usage: ./parse_dataframe_to_bytes <ipc_socket> <stream_id> <proc_num> <proc_index>"
        )
        exit(1)
    ipc_socket = sys.argv[1]
    stream_id = sys.argv[2]
    proc_num = int(sys.argv[3])
    proc_index = int(sys.argv[4])
    parse_dataframe(ipc_socket, stream_id, proc_num, proc_index)
