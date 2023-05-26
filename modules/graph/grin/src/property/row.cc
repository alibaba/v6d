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
#include "graph/grin/include/property/row.h"
#include "graph/grin/include/common/error.h"
}

#ifdef GRIN_ENABLE_ROW
void grin_destroy_row(GRIN_GRAPH g, GRIN_ROW r) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    delete _r;
}

int grin_get_int32_from_row(GRIN_GRAPH g, GRIN_ROW r, size_t idx) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    return *static_cast<const int32_t*>((*_r)[idx]);
}

unsigned int grin_get_uint32_from_row(GRIN_GRAPH g, GRIN_ROW r, size_t idx) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    return *static_cast<const uint32_t*>((*_r)[idx]);
}

long long int grin_get_int64_from_row(GRIN_GRAPH g, GRIN_ROW r, size_t idx) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    return *static_cast<const int64_t*>((*_r)[idx]);
}

unsigned long long int grin_get_uint64_from_row(GRIN_GRAPH g, GRIN_ROW r, size_t idx) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    return *static_cast<const uint64_t*>((*_r)[idx]);
}

float grin_get_float_from_row(GRIN_GRAPH g, GRIN_ROW r, size_t idx) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    return *static_cast<const float*>((*_r)[idx]);
}

double grin_get_double_from_row(GRIN_GRAPH g, GRIN_ROW r, size_t idx) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    return *static_cast<const double*>((*_r)[idx]);
}

const char* grin_get_string_from_row(GRIN_GRAPH g, GRIN_ROW r, size_t idx) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    auto s = static_cast<const std::string*>((*_r)[idx]);
    int len = s->length() + 1;
    char* out = new char[len];
    snprintf(out, len, "%s", s->c_str());
    return out;
}

int grin_get_date32_from_row(GRIN_GRAPH g, GRIN_ROW r, size_t idx) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    return *static_cast<const int32_t*>((*_r)[idx]);
}

int grin_get_time32_from_row(GRIN_GRAPH g, GRIN_ROW r, size_t idx) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    return *static_cast<const int32_t*>((*_r)[idx]);
}

long long int grin_get_timestamp64_from_row(GRIN_GRAPH g, GRIN_ROW r, size_t idx) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    return *static_cast<const int64_t*>((*_r)[idx]);
}

GRIN_ROW grin_create_row(GRIN_GRAPH g) {
    auto r = new GRIN_ROW_T();
    return r;
}

bool grin_insert_int32_to_row(GRIN_GRAPH g, GRIN_ROW r, int value) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    _r->push_back(new int32_t(value));
    return true;
}

bool grin_insert_uint32_to_row(GRIN_GRAPH g, GRIN_ROW r, unsigned int value) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    _r->push_back(new uint32_t(value));
    return true;
}

bool grin_insert_int64_to_row(GRIN_GRAPH g, GRIN_ROW r, long long int value) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    _r->push_back(new int64_t(value));
    return true;
}

bool grin_insert_uint64_to_row(GRIN_GRAPH g, GRIN_ROW r, unsigned long long int value) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    _r->push_back(new uint64_t(value));
    return true;
}

bool grin_insert_float_to_row(GRIN_GRAPH g, GRIN_ROW r, float value) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    _r->push_back(new float(value));
    return true;
}

bool grin_insert_double_to_row(GRIN_GRAPH g, GRIN_ROW r, double value) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    _r->push_back(new double(value));
    return true;
}

bool grin_insert_string_to_row(GRIN_GRAPH g, GRIN_ROW r, const char* value) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    _r->push_back(new std::string(value));
    return true;
}

bool grin_insert_date32_to_row(GRIN_GRAPH g, GRIN_ROW r, int value) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    _r->push_back(new int32_t(value));
    return true;}

bool grin_insert_time32_to_row(GRIN_GRAPH g, GRIN_ROW r, int value) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    _r->push_back(new int32_t(value));
    return true;
}

bool grin_insert_timestamp64_to_row(GRIN_GRAPH g, GRIN_ROW r, long long int value) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    _r->push_back(new int64_t(value));
    return true;
}
#endif

#if defined(GRIN_ENABLE_ROW) && defined(GRIN_TRAIT_CONST_VALUE_PTR)
const void* grin_get_value_from_row(GRIN_GRAPH g, GRIN_ROW r, GRIN_DATATYPE dt, size_t idx) {
    auto _r = static_cast<GRIN_ROW_T*>(r);
    switch (dt) {
    case GRIN_DATATYPE::Int32:
        return static_cast<const int32_t*>((*_r)[idx]);
    case GRIN_DATATYPE::UInt32:
        return static_cast<const  uint32_t*>((*_r)[idx]);
    case GRIN_DATATYPE::Int64:
        return static_cast<const int64_t*>((*_r)[idx]);
    case GRIN_DATATYPE::UInt64:
        return static_cast<const uint64_t*>((*_r)[idx]);
    case GRIN_DATATYPE::Float:
        return static_cast<const float*>((*_r)[idx]);
    case GRIN_DATATYPE::Double:
        return static_cast<const double*>((*_r)[idx]);
    case GRIN_DATATYPE::String: {
        auto s = static_cast<const std::string*>((*_r)[idx]);
        return s->c_str();
    }
    case GRIN_DATATYPE::Date32:
        return static_cast<const int32_t*>((*_r)[idx]);
    case GRIN_DATATYPE::Time32:
        return static_cast<const int32_t*>((*_r)[idx]);
    case GRIN_DATATYPE::Timestamp64:
        return static_cast<const int64_t*>((*_r)[idx]);
    default:
        return NULL;
    }
    return NULL;
}
#endif


#if defined(GRIN_WITH_VERTEX_PROPERTY) && defined(GRIN_ENABLE_ROW)
GRIN_ROW grin_get_vertex_row(GRIN_GRAPH g, GRIN_VERTEX v) {
    auto _g = static_cast<GRIN_GRAPH_T*>(g)->g;
    auto _cache = static_cast<GRIN_GRAPH_T*>(g)->cache;
    unsigned vtype = _cache->id_parser.GetLabelId(v);
    auto offset = _cache->id_parser.GetOffset(v);

    auto r = new GRIN_ROW_T();
    r->resize(_g->vertex_property_num(vtype));
    for (auto vp = 0; vp < _g->vertex_property_num(vtype); ++vp) {
        auto array = _cache->vtables[vtype]->column(vp)->chunk(0);
        auto result = vineyard::get_arrow_array_data_element(array, offset);
        (*r)[vp] = result;
    }
    return r;
}
#endif

#if defined(GRIN_WITH_EDGE_PROPERTY) && defined(GRIN_ENABLE_ROW)
GRIN_ROW grin_get_edge_row(GRIN_GRAPH g, GRIN_EDGE e) {
    auto _g = static_cast<GRIN_GRAPH_T*>(g)->g;
    auto _cache = static_cast<GRIN_GRAPH_T*>(g)->cache;
    auto _e = static_cast<GRIN_EDGE_T*>(e);
    auto etype = _e->etype;
    auto offset = _e->eid;

    auto r = new GRIN_ROW_T();
    r->resize(_g->edge_property_num(etype));
    for (auto ep = 1; ep < _g->edge_property_num(etype); ++ep) {
        auto array = _cache->etables[etype]->column(ep)->chunk(0);
        auto result = vineyard::get_arrow_array_data_element(array, offset);
        (*r)[ep-1] = result;
    }
    return r;
}
#endif
