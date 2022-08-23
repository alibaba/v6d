#! /usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright 2020-2022 Alibaba Group Holding Limited.
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

import numpy as np
import vineyard
import os
import time

env_dist = os.environ
client = vineyard.connect('/var/run/vineyard.sock')

hostname = env_dist['NODENAME']
metaid = env_dist.get(hostname)
meta = client.get_meta(vineyard._C.ObjectID(metaid))
value = client.get(meta['buffer_'].id)

sum = np.sum(value)
print(sum,flush=True)

# avoid CrashLoopBackOff
time.sleep(600)