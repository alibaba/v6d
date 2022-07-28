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

#include "fuse/fused.h"
#include "fuse/adaptors/formats.h"

#include <limits>
#include <unordered_map>

#include "arrow/status.h"
#include "arrow/api.h"
#include "arrow/io/api.h"
#include "arrow/result.h"
#include "arrow/util/key_value_metadata.h"
#include "arrow/util/macros.h"

#include "basic/ds/array.h"
#include "basic/ds/dataframe.h"
#include "client/client.h"
#include "client/ds/blob.h"
#include "client/ds/core_types.h"
#include "client/ds/i_object.h"
#include "common/util/logging.h"
#include "common/util/uuid.h"
namespace vineyard {

namespace fuse {

struct fs::fs_state_t fs::state {};

static std::string name_from_path(std::string const& path) {
  return path.substr(1, path.length() - 6 /* .arrow */ - 1);
}

static std::shared_ptr<arrow::Buffer> generate_fuse_view(
    std::shared_ptr<Object> object) {
  if (auto dataframe = std::dynamic_pointer_cast<vineyard::DataFrame>(object)) {
    return fuse::arrow_view(dataframe);
  } else if (auto recordbatch =
                 std::dynamic_pointer_cast<vineyard::RecordBatch>(object)) {
    return fuse::arrow_view(recordbatch);
  } else if (auto table = std::dynamic_pointer_cast<vineyard::Table>(object)) {
    return fuse::arrow_view(table);
  }
  VINEYARD_ASSERT(
      false, "Unsupported vineyard data type: " + object->meta().GetTypeName());
  return nullptr;
}

int fs::fuse_getattr(const char* path, struct stat* stbuf,
                     struct fuse_file_info*) {
  VLOG(2) << "fuse: getattr on " << path;
  std::lock_guard<std::mutex> guard(state.mtx_);

  memset(stbuf, 0, sizeof(struct stat));
  if (strcmp(path, "/") == 0) {
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
    return 0;
  }

  stbuf->st_mode = S_IFREG | 0444;
  stbuf->st_nlink = 1;

  {
    auto iter = state.views.find(path);
    if (iter != state.views.end()) {
      stbuf->st_size = iter->second->size();
      return 0;
    }
  }
  {
    auto iter = state.mutable_views.find(path);
    if (iter != state.mutable_views.end()) {
      stbuf->st_size = iter->second->length();
      return 0;
    }
  }

  {
    std::string path_string(path);
    if (!boost::algorithm::ends_with(path_string, ".arrow")) {
      return -ENOENT;
    }
    ObjectID target = InvalidObjectID();
    auto status = state.client->GetName(name_from_path(path), target);
    if (!status.ok()) {
      return -ENOENT;
    }
    bool exists = false;
    VINEYARD_CHECK_OK(state.client->Exists(target, exists));
    if (!exists) {
      return -ENOENT;
    }

    auto buffer = generate_fuse_view(state.client->GetObject(target));
    state.views.emplace(path, buffer);
    stbuf->st_size = buffer->size();
    return 0;
  }
}
#ifdef sdf
#define FUSE_CHECK_OK(status)                                            \
  do {                                                                   \
    auto _ret = (status);                                                \
    if (!_ret.ok()) {                                                    \
      VLOG(0) << "[error] Check failed: " << _ret.ToString() << " in \"" \
              << #status << "\""                                         \
              << ", in function " << __PRETTY_FUNCTION__ << ", file "    \
              << __FILE__ << ", line " << VINEYARD_TO_STRING(__LINE__)   \
              << std::endl;                                              \
      return -ECANCELED;                                                 \
    }                                                                   \
  }while (0)                                                            
// ECANCELED 125 Operation canceled

#define FUSE_ASSIGN_OR_RAISE_IMPL(result_name, lhs, rexpr) \
  auto&& result_name = (rexpr);                            \
  FUSE_CHECK_OK((result_name).status());                   \
  lhs = std::move(result_name).ValueUnsafe();

#define FUSE_ASSIGN_OR_RAISE(lhs, rexpr) \
  FUSE_ASSIGN_OR_RAISE_IMPL(             \
      ARROW_ASSIGN_OR_RAISE_NAME(_error_or_value, __COUNTER__), lhs, rexpr);
#endif
int fs::fuse_open(const char* path, struct fuse_file_info* fi) {
  VLOG(2) << "fuse: open " << path << " with mode " << fi->flags;
  if ((fi->flags & O_ACCMODE) != O_RDONLY) {
    return -EACCES;
  }
<<<<<<< HEAD
  std::lock_guard<std::mutex> guard(state.mtx_);

  std::string path_string(path);
  auto typeName = omp.get()->GetTypeName();
 

  VLOG(2)<<"initialized the variables";

  auto loc = state.views.find(target);
  std::shared_ptr<arrow::Buffer> buffer_;
  VLOG(1)<<"data type"<< typeName;
  if (loc == state.views.end()) {
      VLOG(2)<<"taregt ObjectID is not Found";

    if (typeName == "vineyard::Dataframe") {
      VLOG(1)<<"access a datarame";
      auto object = state.client->GetObject<vineyard::DataFrame>(target);
      if (object == nullptr) {
        return -ENOENT;
      }
      buffer_ = fuse::parquet_view(object);

      
    } else if (typeName == "vineyard::NumericArray<int64>"){
      VLOG(1)<<"access a numpy double array";
        auto arr = std::dynamic_pointer_cast<vineyard::NumericArray<int64_t>>(
      state.client->GetObject(target));
      buffer_ = fuse::arrow_ipc_view(arr);
    }
    // else if(typeName == "vineyard::NumericArray<double>") {
    //   VLOG(1)<<"access a numpy double array";
    //     auto arr = std::dynamic_pointer_cast<vineyard::NumericArray<double>>(
    //   state.client->GetObject(target));
    //   buffer_ = fuse::arrow_ipc_view(arr);
      
    // }
    else if(typeName == "vineyard::BooleanArray"){
      VLOG(1)<<"access a numpy boolean array";
        auto arr = std::dynamic_pointer_cast<vineyard::NumericArray<bool>>(
      state.client->GetObject(target));
      buffer_ = fuse::arrow_ipc_view(arr);
    }else if(typeName == "vineyard::BaseBinaryArray<arrow::LargeStringArray>"){
      VLOG(1)<<"access a string array";
        auto arr = std::dynamic_pointer_cast<vineyard::NumericArray<bool>>(
      state.client->GetObject(target));
      buffer_ = fuse::arrow_ipc_view(arr);
    }
    
    state.views[target] = buffer_;
  }else{
      VLOG(2)<<"taregt ObjectID is Found";

>>>>>>> dev_expr: learn fuse usage
  }

  // bypass kernel's page cache to avoid knowing the size in `getattr`.
  // see also:
  // https://stackoverflow.com/questions/46267972/fuse-avoid-calculating-size-in-getattr
  fi->direct_io = 1;
  return 0;
}

int fs::fuse_read(const char* path, char* buf, size_t size, off_t offset,
                  struct fuse_file_info* fi) {
  VLOG(2) << "fuse: read " << path << " from " << offset << ", expect " << size
          << " bytes";
  std::unordered_map<std::string,
                     std::shared_ptr<arrow::Buffer>>::const_iterator loc;
  {
    std::lock_guard<std::mutex> guard(state.mtx_);
    loc = state.views.find(path);
  }
  if (loc == state.views.end()) {
    return -ENOENT;
  }
  auto buffer = loc->second;
  if (offset >= buffer->size()) {
    return 0;
  } else {
    size_t slice = size;
    if (slice > static_cast<size_t>(buffer->size() - offset)) {
      slice = buffer->size() - offset;
    }
    memcpy(buf, buffer->data() + offset, slice);
    return slice;
  }
}

int fs::fuse_write(const char* path, const char* buf, size_t size, off_t offset,
                   struct fuse_file_info* fi) {
  VLOG(2) << "fuse: write " << path << " from " << offset << ", expect " << size
          << " bytes";

  auto loc = state.mutable_views.find(path);
  if (loc == state.mutable_views.end()) {
    return -ENOENT;
  }
  auto buffer = loc->second;
  if (static_cast<int64_t>(offset + size) >= buffer->capacity()) {
    VINEYARD_CHECK_OK(buffer->Reserve(offset + size));
  }
  memcpy(buffer->mutable_data() + offset, buf, size);
  buffer->UnsafeAdvance(size);
  return size;
}

int fs::fuse_statfs(const char* path, struct statvfs*) {
  VLOG(2) << "fuse: statfs " << path;
  return 0;
}

int fs::fuse_flush(const char* path, struct fuse_file_info*) {
  VLOG(2) << "fuse: flush " << path;
  return 0;
}

int fs::fuse_release(const char* path, struct fuse_file_info*) {
  VLOG(2) << "fuse: release " << path;

  {
      // TODO: ref count should be used
      //
      // auto loc = state.views.find(path);
      // if (loc != state.views.end()) {
      //   state.views.erase(loc);
      //   return 0;
      // }
  }

  {
    auto loc = state.mutable_views.find(path);
    if (loc != state.mutable_views.end()) {
      fuse::from_arrow_view(state.client.get(), loc->first, loc->second);
      loc->second->Reset();
      state.mutable_views.erase(loc);
      return 0;
    }
  }

  return -ENOENT;
}

int fs::fuse_getxattr(const char* path, const char* name, char*, size_t) {
  VLOG(2) << "fuse: getxattr " << path << ": name";
  return 0;
}

int fs::fuse_opendir(const char* path, struct fuse_file_info* info) {
  VLOG(2) << "fuse: opendir " << path;
  return 0;
}

int fs::fuse_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
                     off_t offset, struct fuse_file_info* fi,
                     enum fuse_readdir_flags flags) {
  VLOG(2) << "fuse: readdir " << path;

  if (strcmp(path, "/") != 0) {
    return -ENOENT;
  }

  filler(buf, ".", NULL, 0, fuse_fill_dir_flags::FUSE_FILL_DIR_PLUS);
  filler(buf, "..", NULL, 0, fuse_fill_dir_flags::FUSE_FILL_DIR_PLUS);

  std::unordered_map<ObjectID, json> metas{};
  // VINEYARD_CHECK_OK(state.client->ListData(
  //     "vineyard::DataFrame", false, std::numeric_limits<size_t>::max(),
  //     metas));
  VINEYARD_CHECK_OK(state.client->ListData(
      ".*", true, std::numeric_limits<size_t>::max(), metas));
  for (auto const& item : metas) {
    // std::string base = ObjectIDToString(item.first) + ".arrow";
    // filler(buf, base.c_str(), NULL, 0,
    // fuse_fill_dir_flags::FUSE_FILL_DIR_PLUS);
    if (item.second.contains("__name")) {
      std::string base = item.second["__name"].get<std::string>() + ".arrow";
      filler(buf, base.c_str(), NULL, 0,
             fuse_fill_dir_flags::FUSE_FILL_DIR_PLUS);
    }
  }
  return 0;
}

void* fs::fuse_init(struct fuse_conn_info* conn, struct fuse_config* cfg) {
  VLOG(2) << "fuse: initfs with vineyard socket " << state.vineyard_socket;

  state.client.reset(new vineyard::Client());
  state.client->Connect(state.vineyard_socket);

  fuse_apply_conn_info_opts(state.conn_opts, conn);
  conn->max_read = conn->max_readahead;

  cfg->kernel_cache = 0;
  return NULL;
}

void fs::fuse_destroy(void* private_data) {
  VLOG(2) << "fuse: destroy";

  state.views.clear();
  state.client->Disconnect();
}

int fs::fuse_access(const char* path, int mode) {
  VLOG(2) << "fuse: access " << path << " with mode " << mode;

  return mode;
}

int fs::fuse_create(const char* path, mode_t mode, struct fuse_file_info*) {
  VLOG(2) << "fuse: create " << path << " with mode " << mode;
  if (state.mutable_views.find(path) != state.mutable_views.end()) {
    LOG(ERROR) << "fuse: create: file already exists" << path;
    return EEXIST;
  }
  state.mutable_views.emplace(
      path, std::shared_ptr<arrow::BufferBuilder>(new arrow::BufferBuilder()));
  return 0;
}

}  // namespace fuse

}  // namespace vineyard
