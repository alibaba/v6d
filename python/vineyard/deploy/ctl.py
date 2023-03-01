#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2020-2023 Alibaba Group Holding Limited.
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

import os
import subprocess
import sys

from ._cobra import click
from .utils import find_vineyardctl_path

_vineyard_ctl = find_vineyardctl_path()


def _register():
    if not hasattr(sys.modules[__name__], 'vineyardctl'):
        click(
            sys.modules[__name__],
            _vineyard_ctl,
            exclude_args=['dump_usage', 'x_version', 'help'],
        )


def _main(args):
    prog = _vineyard_ctl
    if os.name == 'nt':
        try:
            return subprocess.call([prog] + args)
        except KeyboardInterrupt:
            return 0
    else:
        return os.execvp(prog, [prog] + args)


__all__ = [  # noqa: F822
    'vineyardctl',  # pylint: disable=undefined-all-variable
]