/** Copyright 2020 Alibaba Group Holding Limited.
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

#include "graph/grin/src/predefine.h"
extern "C" {
#include "graph/grin/include/property/topology.h"
}

#ifdef GRIN_WITH_VERTEX_PROPERTY
size_t grin_get_vertex_num_by_type(GRIN_GRAPH g, GRIN_VERTEX_TYPE vtype) {
    auto _g = static_cast<GRIN_GRAPH_T*>(g)->g;
    return _g->GetVerticesNum(vtype);
}
#endif

#ifdef GRIN_WITH_EDGE_PROPERTY
size_t grin_get_edge_num_by_type(GRIN_GRAPH g, GRIN_EDGE_TYPE etype) {
    auto _cache = static_cast<GRIN_GRAPH_T*>(g)->cache;
    return _cache->etables[etype]->num_rows();
}
#endif

#ifdef GRIN_TRAIT_SELECT_TYPE_FOR_VERTEX_LIST
GRIN_VERTEX_LIST grin_select_type_for_vertex_list(GRIN_GRAPH g, GRIN_VERTEX_TYPE vtype, GRIN_VERTEX_LIST vl) {
    auto _vl = static_cast<GRIN_VERTEX_LIST_T*>(vl);
    if (_vl->is_simple && _vl->vtype != vtype) return GRIN_NULL_LIST;

    auto _g = static_cast<GRIN_GRAPH_T*>(g)->g;
    auto fvl = new GRIN_VERTEX_LIST_T();
    fvl->all_master_mirror = _vl->all_master_mirror;
    fvl->vtype = vtype;
    fvl->is_simple = true;
    __grin_init_simple_vertex_list(_g, fvl);
    return fvl;
}
#endif

#ifdef GRIN_TRAIT_SELECT_TYPE_FOR_EDGE_LIST
GRIN_EDGE_LIST grin_select_type_for_edge_list(GRIN_GRAPH, GRIN_EDGE_TYPE, GRIN_EDGE_LIST);
#endif

#ifdef GRIN_TRAIT_SELECT_NEIGHBOR_TYPE_FOR_ADJACENT_LIST
GRIN_ADJACENT_LIST grin_select_neighbor_type_for_adjacent_list(GRIN_GRAPH, GRIN_VERTEX_TYPE, GRIN_ADJACENT_LIST);
#endif

#ifdef GRIN_TRAIT_SELECT_EDGE_TYPE_FOR_ADJACENT_LIST
GRIN_ADJACENT_LIST grin_select_edge_type_for_adjacent_list(GRIN_GRAPH g, GRIN_EDGE_TYPE etype, GRIN_ADJACENT_LIST al) {
    auto _al = static_cast<GRIN_ADJACENT_LIST_T*>(al);
    if (_al->is_simple && _al->etype != etype) return GRIN_NULL_LIST;
    auto _g = static_cast<GRIN_GRAPH_T*>(g)->g;
    auto fal = new GRIN_ADJACENT_LIST_T();
    fal->vid = _al->vid;
    fal->dir = _al->dir;
    fal->etype = etype;
    fal->is_simple = true;
    __grin_init_simple_adjacent_list(_g, fal);
    return fal;
}
#endif