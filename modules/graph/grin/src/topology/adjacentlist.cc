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
#include "graph/grin/include/topology/adjacentlist.h"
}

#ifdef GRIN_ENABLE_ADJACENT_LIST
GRIN_ADJACENT_LIST grin_get_adjacent_list(GRIN_GRAPH g, GRIN_DIRECTION d, GRIN_VERTEX v) {
    if (d == GRIN_DIRECTION::BOTH) return GRIN_NULL_LIST;
    auto _g = static_cast<GRIN_GRAPH_T*>(g)->g;
    auto al = new GRIN_ADJACENT_LIST_T();
    al->vid = v;
    al->dir = d;
    al->etype_begin = 0;
    al->etype_end = _g->edge_label_num();
    __grin_init_adjacent_list(_g, al);
    return al;
}

void grin_destroy_adjacent_list(GRIN_GRAPH g, GRIN_ADJACENT_LIST al) {
    auto _al = static_cast<GRIN_ADJACENT_LIST_T*>(al);
    delete _al;
}

size_t grin_get_adjacent_list_size(GRIN_GRAPH g, GRIN_ADJACENT_LIST al) {
    auto _al = static_cast<GRIN_ADJACENT_LIST_T*>(al);
    if (_al->is_simple) return _al->end_ - _al->begin_;
    return _al->offsets[_al->etype_end - _al->etype_begin];
}

GRIN_VERTEX grin_get_neighbor_from_adjacent_list(GRIN_GRAPH g, GRIN_ADJACENT_LIST al, size_t idx) {
    auto _al = static_cast<GRIN_ADJACENT_LIST_T*>(al);
    if (_al->is_simple) {
        auto nbr = _al->begin_ + idx;
        if (nbr < _al->end_) return nbr->vid;
    } else {
        for (unsigned i = 0; i < _al->etype_end - _al->etype_begin; ++i) {        
            if (idx < _al->offsets[i+1]) {
                auto _idx = idx - _al->offsets[i];
                auto _nbr = _al->data[i].begin() + _idx;
                return _nbr->vid;
            }
        }
    }
    return GRIN_NULL_VERTEX;
}

GRIN_EDGE grin_get_edge_from_adjacent_list(GRIN_GRAPH g, GRIN_ADJACENT_LIST al, size_t idx) {
    auto _al = static_cast<GRIN_ADJACENT_LIST_T*>(al);
    if (_al->is_simple) {
        auto nbr = _al->begin_ + idx;
        if (nbr < _al->end_) {
            auto e = new GRIN_EDGE_T();        
            e->dir = _al->dir;
            e->etype = _al->etype_begin;
            e->eid = nbr->eid;
            if (_al->dir == GRIN_DIRECTION::OUT) {
                e->src = _al->vid;
                e->dst = nbr->vid;
            } else {
                e->src = nbr->vid;
                e->dst = _al->vid;
            }
            return e;
        }
    } else {
        for (unsigned i = 0; i < _al->etype_end - _al->etype_begin; ++i) {        
            if (idx < _al->offsets[i+1]) {
                auto _idx = idx - _al->offsets[i];
                auto _nbr = _al->data[i].begin() + _idx;
                auto e = new GRIN_EDGE_T();        
                e->dir = _al->dir;
                e->etype = _al->etype_begin + i;
                e->eid = _nbr->eid;
                if (_al->dir == GRIN_DIRECTION::OUT) {
                    e->src = _al->vid;
                    e->dst = _nbr->vid;
                } else {
                    e->src = _nbr->vid;
                    e->dst = _al->vid;
                }
                return e;        
            }
        }
    }
    return GRIN_NULL_EDGE;
}
#endif

#ifdef GRIN_ENABLE_ADJACENT_LIST_ITERATOR
GRIN_ADJACENT_LIST_ITERATOR grin_get_adjacent_list_begin(GRIN_GRAPH g, GRIN_ADJACENT_LIST al) {
    auto _g = static_cast<GRIN_GRAPH_T*>(g)->g;
    auto _al = static_cast<GRIN_ADJACENT_LIST_T*>(al);
    auto ali = new GRIN_ADJACENT_LIST_ITERATOR_T();
    ali->vid = _al->vid;
    ali->dir = _al->dir;
    ali->etype_begin = _al->etype_begin;
    ali->etype_end = _al->etype_end;
    ali->etype_current = _al->etype_begin;
    ali->current = 0;

    while (ali->etype_current < ali->etype_end) {
        if (_al->offsets[ali->etype_current - ali->etype_begin + 1] > 0) break;
        ali->etype_current++;
    }

    if (ali->etype_current < ali->etype_end) {
        if (ali->dir == GRIN_DIRECTION::IN) {
            ali->data = _g->GetIncomingRawAdjList(_GRIN_GRAPH_T::vertex_t(ali->vid), ali->etype_current);
        } else {
            ali->data = _g->GetOutgoingRawAdjList(_GRIN_GRAPH_T::vertex_t(ali->vid), ali->etype_current);
        }
    }
    return ali;
}

void grin_destroy_adjacent_list_iter(GRIN_GRAPH g, GRIN_ADJACENT_LIST_ITERATOR ali) {
    auto _ali = static_cast<GRIN_ADJACENT_LIST_ITERATOR_T*>(ali);
    delete _ali;
}

void grin_get_next_adjacent_list_iter(GRIN_GRAPH g, GRIN_ADJACENT_LIST_ITERATOR ali) {
    auto _g = static_cast<GRIN_GRAPH_T*>(g)->g;
    auto _ali = static_cast<GRIN_ADJACENT_LIST_ITERATOR_T*>(ali);
    _ali->current++;
    while (_ali->etype_current < _ali->etype_end) {
        if (_ali->current < _ali->data.size()) break;
        _ali->etype_current++;
        _ali->current = 0;
        if (_ali->etype_current >= _ali->etype_end) break;
        if (_ali->dir == GRIN_DIRECTION::IN) {
            _ali->data = _g->GetIncomingRawAdjList(_GRIN_GRAPH_T::vertex_t(_ali->vid), _ali->etype_current);
        } else {
            _ali->data = _g->GetOutgoingRawAdjList(_GRIN_GRAPH_T::vertex_t(_ali->vid), _ali->etype_current);
        }
    }
}

bool grin_is_adjacent_list_end(GRIN_GRAPH g, GRIN_ADJACENT_LIST_ITERATOR ali) {
    auto _ali = static_cast<GRIN_ADJACENT_LIST_ITERATOR_T*>(ali);
    return _ali->etype_current >= _ali->etype_end;
}

GRIN_VERTEX grin_get_neighbor_from_adjacent_list_iter(GRIN_GRAPH g, GRIN_ADJACENT_LIST_ITERATOR ali) {
    auto _ali = static_cast<GRIN_ADJACENT_LIST_ITERATOR_T*>(ali);
    auto _nbr = _ali->data.begin() + _ali->current;
    return _nbr->vid;
}

GRIN_EDGE grin_get_edge_from_adjacent_list_iter(GRIN_GRAPH g, GRIN_ADJACENT_LIST_ITERATOR ali) {
    auto _ali = static_cast<GRIN_ADJACENT_LIST_ITERATOR_T*>(ali);
    auto _nbr = _ali->data.begin() + _ali->current;
    auto e = new GRIN_EDGE_T();
    e->dir = _ali->dir;
    e->etype = _ali->etype_current;
    e->eid = _nbr->eid;
    if (_ali->dir == GRIN_DIRECTION::OUT) {
        e->src = _ali->vid;
        e->dst = _nbr->vid;
    } else {
        e->src = _nbr->vid;
        e->dst = _ali->vid;
    }
    return e;     
}
#endif