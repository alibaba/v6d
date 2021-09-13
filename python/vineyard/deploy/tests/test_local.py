#! /usr/bin/env python
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

import vineyard


def test_local_cluster():
    client1 = vineyard.init(etcd_prefix='test')
    client2 = vineyard.init(etcd_prefix='test')
    client3 = vineyard.init(etcd_prefix='test')
    assert client1 != client2
    assert client1 != client3
    assert client2 != client3
    obj_id = client1.put([1024, 1024])
    client1.persist(obj_id)
    meta2 = client2.get_meta(obj_id)
    meta3 = client3.get_meta(obj_id)
    assert str(meta2) == str(meta3)
    vineyard.shutdown()


def test_local_single():
    client = vineyard.init()
    obj_id = client.put(1024)
    assert client.get(obj_id) == 1024
    vineyard.shutdown()
