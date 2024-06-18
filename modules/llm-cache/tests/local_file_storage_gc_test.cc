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

#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

#include "common/util/logging.h"
#include "common/util/status.h"
#include "gulrak/filesystem.hpp"
#include "llm-cache/storage/local_file_storage.h"
#include "llm-cache/thread_group.h"

using namespace vineyard;  // NOLINT(build/namespaces)

std::mutex createFileMutex;
std::string kTempDir = "/tmp/llm_cache/__gc/";

void CreateFile(std::string path, std::shared_ptr<LocalFileStorage> storage) {
  std::shared_ptr<FileDescriptor> fd = storage->CreateFileDescriptor();
  VINEYARD_CHECK_OK(storage->Open(path, fd, FileOperationType::WRITE));

  std::shared_ptr<LocalFileDescriptor> lfd =
      std::static_pointer_cast<LocalFileDescriptor>(fd);
  std::string content = "Hello, world!";
  ssize_t ret = write(lfd->fd, content.c_str(), content.size());
  if (ret == -1) {
    LOG(ERROR) << "Failed to write file: " << path;
    VINEYARD_ASSERT(false);
  }
  close(lfd->fd);
  std::lock_guard<std::mutex> lock(createFileMutex);
  storage->GetGCList().push_back(path);
}

void CheckFilesExist(std::vector<std::string>::iterator begin,
                     std::vector<std::string>::iterator end) {
  ghc::filesystem::path path;
  for (auto it = begin; it != end; ++it) {
    path = *it;
    VINEYARD_ASSERT_VERBOSE(ghc::filesystem::exists(path),
                            "File: " + path.string() + " should exist!");
  }
}

void CheckFilesNotExist(std::string dir) {
  for (auto it = ghc::filesystem::recursive_directory_iterator(dir);
       it != ghc::filesystem::recursive_directory_iterator(); ++it) {
    VINEYARD_ASSERT_VERBOSE(
        !ghc::filesystem::is_regular_file(*it),
        "File: " + it->path().string() + " should not exist!");
  }
}

std::vector<std::string> GeneratePath(std::string parentDir, int num) {
  std::vector<std::string> paths;
  for (int i = 0; i < num; i++) {
    paths.push_back(parentDir + std::to_string(i));
  }
  return paths;
}

void TestLocalGC() {
  LOG(INFO) << "Test Local GC";
  std::vector<std::string> paths = GeneratePath(kTempDir, 20);
  std::shared_ptr<LocalFileStorage> storage =
      std::make_shared<LocalFileStorage>(10, 10, 1, 5, 5, kTempDir, 1, 6, false,
                                         300, 300);
  VINEYARD_CHECK_OK(storage->Mkdir(kTempDir));

  auto fn = [&paths, storage](int i) -> Status {
    CreateFile(paths[i], storage);
    return Status::OK();
  };

  parallel::ThreadGroup tg(std::min(
      paths.size(), static_cast<size_t>(std::thread::hardware_concurrency())));
  for (size_t i = 0; i < paths.size(); i++) {
    tg.AddTask(fn, i);
  }

  for (auto& s : tg.TakeResults()) {
    VINEYARD_CHECK_OK(s);
  }
  VINEYARD_CHECK_OK(storage->Init());

  CheckFilesExist(paths.begin(), paths.end());
  sleep(2);
  CheckFilesExist(paths.begin(), paths.end());
  sleep(2);
  CheckFilesExist(paths.begin(), paths.end());
  sleep(5);
  CheckFilesNotExist(kTempDir);
  storage->CloseCache();
  storage->StopGlobalGCThread();
  LOG(INFO) << "Local GC test pass!";
}

void TestGlobalGC() {
  LOG(INFO) << "TestGlobalGC";
  std::vector<std::string> paths = GeneratePath(kTempDir, 20);
  std::shared_ptr<LocalFileStorage> storage =
      std::make_shared<LocalFileStorage>(10, 10, 1, 5, 5, kTempDir, 100, 600,
                                         true, 1, 6);
  VINEYARD_CHECK_OK(storage->Mkdir(kTempDir));

  auto fn = [&paths, storage](int i) -> Status {
    CreateFile(paths[i], storage);
    return Status::OK();
  };

  parallel::ThreadGroup tg(std::min(
      paths.size(), static_cast<size_t>(std::thread::hardware_concurrency())));
  for (size_t i = 0; i < paths.size(); i++) {
    tg.AddTask(fn, i);
  }

  for (auto& s : tg.TakeResults()) {
    VINEYARD_CHECK_OK(s);
  }
  VINEYARD_CHECK_OK(storage->Init());

  CheckFilesExist(paths.begin(), paths.end());
  sleep(2);
  CheckFilesExist(paths.begin(), paths.end());
  sleep(2);
  CheckFilesExist(paths.begin(), paths.end());
  sleep(5);
  CheckFilesNotExist(kTempDir);
  storage->CloseCache();
  storage->StopGlobalGCThread();
  LOG(INFO) << "Global GC test pass!";
}

void TestLocalAndGlobalGC() {
  LOG(INFO) << "TestLocalAndGlobalGC";
  std::vector<std::string> paths = GeneratePath(kTempDir, 40);
  std::shared_ptr<LocalFileStorage> storage =
      std::make_shared<LocalFileStorage>(10, 10, 1, 5, 5, kTempDir, 1, 6, true,
                                         1, 10);
  VINEYARD_CHECK_OK(storage->Mkdir(kTempDir));

  auto fn = [&paths, storage](int i) -> Status {
    CreateFile(paths[i], storage);
    return Status::OK();
  };

  parallel::ThreadGroup tg(std::min(
      paths.size(), static_cast<size_t>(std::thread::hardware_concurrency())));
  for (size_t i = 0; i < paths.size() / 2; i++) {
    tg.AddTask(fn, i);
  }

  for (auto& s : tg.TakeResults()) {
    VINEYARD_CHECK_OK(s);
  }
  sleep(2);

  for (size_t i = paths.size() / 2; i < paths.size(); i++) {
    tg.AddTask(fn, i);
  }

  for (auto& s : tg.TakeResults()) {
    VINEYARD_CHECK_OK(s);
  }

  VINEYARD_CHECK_OK(storage->Init());

  CheckFilesExist(paths.begin(), paths.end());
  sleep(2);
  CheckFilesExist(paths.begin(), paths.end());
  sleep(4);
  CheckFilesExist(paths.begin() + paths.size() / 2, paths.end());
  sleep(4);
  CheckFilesNotExist(kTempDir);
  storage->CloseCache();
  storage->StopGlobalGCThread();
  LOG(INFO) << "Local and Global GC test pass!";
}

void TestCleanAllFiles() {
  LOG(INFO) << "TestCleanAllFiles";
  std::vector<std::string> paths = GeneratePath(kTempDir, 20);
  std::shared_ptr<LocalFileStorage> storage =
      std::make_shared<LocalFileStorage>(10, 10, 1, 5, 5, kTempDir, 1, 6, true,
                                         1, 10);
  VINEYARD_CHECK_OK(storage->Mkdir(kTempDir));

  auto fn = [&paths, storage](int i) -> Status {
    CreateFile(paths[i], storage);
    return Status::OK();
  };

  parallel::ThreadGroup tg(std::min(
      paths.size(), static_cast<size_t>(std::thread::hardware_concurrency())));
  for (size_t i = 0; i < paths.size(); i++) {
    tg.AddTask(fn, i);
  }

  for (auto& s : tg.TakeResults()) {
    VINEYARD_CHECK_OK(s);
  }

  storage = std::make_shared<LocalFileStorage>(10, 10, 1, 5, 5, kTempDir, 1, 1,
                                               true, 1, 1);

  VINEYARD_CHECK_OK(storage->Init());
  sleep(2);
  CheckFilesNotExist(kTempDir);
  storage->CloseCache();
  storage->StopGlobalGCThread();
  LOG(INFO) << "Clean all files test pass!";
}

int main() {
  // create file
  std::string parentDir = kTempDir;

  TestLocalGC();

  TestGlobalGC();

  TestLocalAndGlobalGC();

  TestCleanAllFiles();

  LOG(INFO) << "Pass all tests!";
}
