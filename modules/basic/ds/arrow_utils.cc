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

#include "basic/ds/arrow_utils.h"

#include <algorithm>
#include <map>
#include <unordered_map>

#include "arrow/api.h"
#include "arrow/compute/api.h"
#include "arrow/io/api.h"
#include "arrow/ipc/api.h"

#include "boost/algorithm/string.hpp"

#include "common/util/typename.h"

namespace vineyard {

std::shared_ptr<arrow::Table> ConcatenateTables(
    std::vector<std::shared_ptr<arrow::Table>>& tables) {
  if (tables.size() == 1) {
    return tables[0];
  }
  auto col_names = tables[0]->ColumnNames();
  for (size_t i = 1; i < tables.size(); ++i) {
    CHECK_ARROW_ERROR_AND_ASSIGN(tables[i],
                                 tables[i]->RenameColumns(col_names));
  }
  std::shared_ptr<arrow::Table> table;
  CHECK_ARROW_ERROR_AND_ASSIGN(table, arrow::ConcatenateTables(tables));
  return table;
}

std::shared_ptr<arrow::DataType> FromAnyType(AnyType type) {
  switch (type) {
  case AnyType::Int32:
    return arrow::int32();
  case AnyType::UInt32:
    return arrow::uint32();
  case AnyType::Int64:
    return arrow::int64();
  case AnyType::UInt64:
    return arrow::uint64();
  case AnyType::Float:
    return arrow::float32();
  case AnyType::Double:
    return arrow::float64();
  default:
    return arrow::null();
  }
}

Status GetRecordBatchStreamSize(const arrow::RecordBatch& batch, size_t* size) {
  // emulates the behavior of Write without actually writing
  arrow::io::MockOutputStream dst;

  std::shared_ptr<arrow::ipc::RecordBatchWriter> writer;
#if defined(ARROW_VERSION) && ARROW_VERSION < 2000000
  RETURN_ON_ARROW_ERROR_AND_ASSIGN(
      writer, arrow::ipc::NewStreamWriter(&dst, batch.schema()));
#else
  RETURN_ON_ARROW_ERROR_AND_ASSIGN(
      writer, arrow::ipc::MakeStreamWriter(&dst, batch.schema()));
#endif
  RETURN_ON_ARROW_ERROR(writer->WriteRecordBatch(batch));
  RETURN_ON_ARROW_ERROR(writer->Close());
  *size = dst.GetExtentBytesWritten();
  return Status::OK();
}

Status SerializeRecordBatch(std::shared_ptr<arrow::RecordBatch>& batch,
                            std::shared_ptr<arrow::Buffer>* buffer) {
  std::shared_ptr<arrow::io::BufferOutputStream> out_stream;
  RETURN_ON_ARROW_ERROR_AND_ASSIGN(out_stream,
                                   arrow::io::BufferOutputStream::Create(1024));
#if defined(ARROW_VERSION) && ARROW_VERSION < 9000000
  RETURN_ON_ARROW_ERROR(arrow::ipc::WriteRecordBatchStream(
      {batch}, arrow::ipc::IpcOptions::Defaults(), out_stream.get()));
#else
  RETURN_ON_ARROW_ERROR(arrow::ipc::WriteRecordBatchStream(
      {batch}, arrow::ipc::IpcWriteOptions::Defaults(), out_stream.get()));
#endif
  RETURN_ON_ARROW_ERROR_AND_ASSIGN(*buffer, out_stream->Finish());
  return Status::OK();
}

Status DeserializeRecordBatch(std::shared_ptr<arrow::Buffer>& buffer,
                              std::shared_ptr<arrow::RecordBatch>* batch) {
  if (buffer == nullptr || buffer->size() == 0) {
    return Status::Invalid(
        "Unable to deserialize to recordbatch: buffer is empty");
  }
  arrow::io::BufferReader reader(buffer);
  std::shared_ptr<arrow::RecordBatchReader> batch_reader;
  RETURN_ON_ARROW_ERROR_AND_ASSIGN(
      batch_reader, arrow::ipc::RecordBatchStreamReader::Open(&reader));
  RETURN_ON_ARROW_ERROR(batch_reader->ReadNext(batch));
  return Status::OK();
}

Status SerializeRecordBatchesToAllocatedBuffer(
    const std::vector<std::shared_ptr<arrow::RecordBatch>>& batches,
    std::shared_ptr<arrow::Buffer>* buffer) {
  arrow::io::FixedSizeBufferWriter stream(*buffer);
#if defined(ARROW_VERSION) && ARROW_VERSION < 9000000
  RETURN_ON_ARROW_ERROR(arrow::ipc::WriteRecordBatchStream(
      batches, arrow::ipc::IpcOptions::Defaults(), &stream));
#else
  RETURN_ON_ARROW_ERROR(arrow::ipc::WriteRecordBatchStream(
      batches, arrow::ipc::IpcWriteOptions::Defaults(), &stream));
#endif
  return Status::OK();
}

Status SerializeRecordBatches(
    const std::vector<std::shared_ptr<arrow::RecordBatch>>& batches,
    std::shared_ptr<arrow::Buffer>* buffer) {
  std::shared_ptr<arrow::io::BufferOutputStream> out_stream;
  RETURN_ON_ARROW_ERROR_AND_ASSIGN(out_stream,
                                   arrow::io::BufferOutputStream::Create(1024));
#if defined(ARROW_VERSION) && ARROW_VERSION < 9000000
  RETURN_ON_ARROW_ERROR(arrow::ipc::WriteRecordBatchStream(
      batches, arrow::ipc::IpcOptions::Defaults(), out_stream.get()));
#else
  RETURN_ON_ARROW_ERROR(arrow::ipc::WriteRecordBatchStream(
      batches, arrow::ipc::IpcWriteOptions::Defaults(), out_stream.get()));
#endif
  RETURN_ON_ARROW_ERROR_AND_ASSIGN(*buffer, out_stream->Finish());
  return Status::OK();
}

Status DeserializeRecordBatches(
    const std::shared_ptr<arrow::Buffer>& buffer,
    std::vector<std::shared_ptr<arrow::RecordBatch>>* batches) {
  arrow::io::BufferReader reader(buffer);
  std::shared_ptr<arrow::RecordBatchReader> batch_reader;
  RETURN_ON_ARROW_ERROR_AND_ASSIGN(
      batch_reader, arrow::ipc::RecordBatchStreamReader::Open(&reader));
  RETURN_ON_ARROW_ERROR(batch_reader->ReadAll(batches));
  return Status::OK();
}

Status RecordBatchesToTable(
    const std::vector<std::shared_ptr<arrow::RecordBatch>>& batches,
    std::shared_ptr<arrow::Table>* table) {
  RETURN_ON_ARROW_ERROR_AND_ASSIGN(*table,
                                   arrow::Table::FromRecordBatches(batches));
  return Status::OK();
}

Status CombineRecordBatches(
    const std::vector<std::shared_ptr<arrow::RecordBatch>>& batches,
    std::shared_ptr<arrow::RecordBatch>* batch) {
  std::shared_ptr<arrow::Table> table, combined_table;
  RETURN_ON_ERROR(RecordBatchesToTable(batches, &table));
  RETURN_ON_ARROW_ERROR_AND_ASSIGN(
      combined_table, table->CombineChunks(arrow::default_memory_pool()));
  arrow::TableBatchReader tbreader(*combined_table);
  RETURN_ON_ARROW_ERROR(tbreader.ReadNext(batch));
  std::shared_ptr<arrow::RecordBatch> test_batch;
  RETURN_ON_ARROW_ERROR(tbreader.ReadNext(&test_batch));
  RETURN_ON_ASSERT(test_batch == nullptr);
  return Status::OK();
}

Status TableToRecordBatches(
    std::shared_ptr<arrow::Table> table,
    std::vector<std::shared_ptr<arrow::RecordBatch>>* batches) {
  arrow::TableBatchReader tbr(*table);
  RETURN_ON_ARROW_ERROR(tbr.ReadAll(batches));
  return Status::OK();
}

Status SerializeTableToAllocatedBuffer(std::shared_ptr<arrow::Table> table,
                                       std::shared_ptr<arrow::Buffer>* buffer) {
  std::vector<std::shared_ptr<arrow::RecordBatch>> batches;
  RETURN_ON_ERROR(TableToRecordBatches(table, &batches));
  RETURN_ON_ERROR(SerializeRecordBatchesToAllocatedBuffer(batches, buffer));
  return Status::OK();
}

Status SerializeTable(std::shared_ptr<arrow::Table> table,
                      std::shared_ptr<arrow::Buffer>* buffer) {
  std::vector<std::shared_ptr<arrow::RecordBatch>> batches;
  RETURN_ON_ERROR(TableToRecordBatches(table, &batches));
  RETURN_ON_ERROR(SerializeRecordBatches(batches, buffer));
  return Status::OK();
}

Status DeserializeTable(std::shared_ptr<arrow::Buffer> buffer,
                        std::shared_ptr<arrow::Table>* table) {
  arrow::io::BufferReader reader(buffer);
  std::shared_ptr<arrow::RecordBatchReader> batch_reader;
  RETURN_ON_ARROW_ERROR_AND_ASSIGN(
      batch_reader, arrow::ipc::RecordBatchStreamReader::Open(&reader));
  RETURN_ON_ARROW_ERROR(batch_reader->ReadAll(table));
  return Status::OK();
}

Status EmptyTableBuilder::Build(const std::shared_ptr<arrow::Schema>& schema,
                                std::shared_ptr<arrow::Table>& table) {
  std::vector<std::shared_ptr<arrow::ChunkedArray>> columns;

  for (int i = 0; i < schema->num_fields(); i++) {
    std::shared_ptr<arrow::Array> dummy;
    auto type = schema->field(i)->type();

    if (type == arrow::boolean()) {
      arrow::BooleanBuilder builder;
      RETURN_ON_ARROW_ERROR(builder.Finish(&dummy));
    } else if (type == arrow::uint64()) {
      arrow::UInt64Builder builder;
      RETURN_ON_ARROW_ERROR(builder.Finish(&dummy));
    } else if (type == arrow::int64()) {
      arrow::Int64Builder builder;
      RETURN_ON_ARROW_ERROR(builder.Finish(&dummy));
    } else if (type == arrow::uint32()) {
      arrow::UInt32Builder builder;
      RETURN_ON_ARROW_ERROR(builder.Finish(&dummy));
    } else if (type == arrow::int32()) {
      arrow::Int32Builder builder;
      RETURN_ON_ARROW_ERROR(builder.Finish(&dummy));
    } else if (type == arrow::float32()) {
      arrow::FloatBuilder builder;
      RETURN_ON_ARROW_ERROR(builder.Finish(&dummy));
    } else if (type == arrow::float64()) {
      arrow::DoubleBuilder builder;
      RETURN_ON_ARROW_ERROR(builder.Finish(&dummy));
    } else if (type == arrow::utf8()) {
      arrow::StringBuilder builder;
      RETURN_ON_ARROW_ERROR(builder.Finish(&dummy));
    } else if (type == arrow::large_utf8()) {
      arrow::LargeStringBuilder builder;
      RETURN_ON_ARROW_ERROR(builder.Finish(&dummy));
    } else if (type == arrow::list(arrow::uint64())) {
      auto builder = std::make_shared<arrow::UInt64Builder>();
      arrow::ListBuilder list_builder(arrow::default_memory_pool(), builder);
      RETURN_ON_ARROW_ERROR(list_builder.Finish(&dummy));
    } else if (type == arrow::list(arrow::int64())) {
      auto builder = std::make_shared<arrow::Int64Builder>();
      arrow::ListBuilder list_builder(arrow::default_memory_pool(), builder);
      RETURN_ON_ARROW_ERROR(list_builder.Finish(&dummy));
    } else if (type == arrow::list(arrow::uint32())) {
      auto builder = std::make_shared<arrow::UInt32Builder>();
      arrow::ListBuilder list_builder(arrow::default_memory_pool(), builder);
      RETURN_ON_ARROW_ERROR(list_builder.Finish(&dummy));
    } else if (type == arrow::list(arrow::int32())) {
      auto builder = std::make_shared<arrow::Int32Builder>();
      arrow::ListBuilder list_builder(arrow::default_memory_pool(), builder);
      RETURN_ON_ARROW_ERROR(list_builder.Finish(&dummy));
    } else if (type == arrow::list(arrow::float64())) {
      auto builder = std::make_shared<arrow::DoubleBuilder>();
      arrow::ListBuilder list_builder(arrow::default_memory_pool(), builder);
      RETURN_ON_ARROW_ERROR(list_builder.Finish(&dummy));
    } else if (type == arrow::list(arrow::int64())) {
      auto builder = std::make_shared<arrow::FloatBuilder>();
      arrow::ListBuilder list_builder(arrow::default_memory_pool(), builder);
      RETURN_ON_ARROW_ERROR(list_builder.Finish(&dummy));
    } else if (type == arrow::null()) {
      arrow::NullBuilder builder;
      RETURN_ON_ARROW_ERROR(builder.Finish(&dummy));
    } else {
      return Status::NotImplemented("Unsupported type: " + type->ToString());
    }
    columns.push_back(std::make_shared<arrow::ChunkedArray>(dummy));
  }
  table = arrow::Table::Make(schema, columns);
  return Status::OK();
}

std::shared_ptr<arrow::Schema> EmptyTableBuilder::EmptySchema() {
#if defined(ARROW_VERSION) && ARROW_VERSION >= 4000000
  return std::shared_ptr<arrow::Schema>(
      new arrow::Schema({}, arrow::Endianness::Native));
#else
  return std::shared_ptr<arrow::Schema>(new arrow::Schema({}));
#endif
}

std::shared_ptr<arrow::ArrayData> CopyArrayData(
    std::shared_ptr<arrow::ArrayData> const& array) {
  if (array == nullptr) {
    return array;
  }
  std::vector<std::shared_ptr<arrow::Buffer>> buffers;
  for (auto const& buffer : array->buffers) {
    if (buffer == nullptr || buffer->size() == 0) {
      buffers.push_back(buffer);
    } else {
      std::shared_ptr<arrow::Buffer> buf;
      CHECK_ARROW_ERROR_AND_ASSIGN(buf, buffer->CopySlice(0, buffer->size()));
      buffers.push_back(buf);
    }
  }
  std::vector<std::shared_ptr<arrow::ArrayData>> child_data;
  for (auto const& child : array->child_data) {
    child_data.push_back(CopyArrayData(child));
  }
  return arrow::ArrayData::Make(array->type, array->length, buffers, child_data,
                                array->null_count, array->offset);
}

std::shared_ptr<arrow::RecordBatch> CopyRecordBatch(
    std::shared_ptr<arrow::RecordBatch> const& batch) {
  if (batch == nullptr) {
    return nullptr;
  }
  arrow::ArrayDataVector columns_data;
  for (auto const& column : batch->column_data()) {
    columns_data.push_back(CopyArrayData(column));
  }
  return arrow::RecordBatch::Make(batch->schema(), batch->num_rows(),
                                  columns_data);
}

std::shared_ptr<arrow::RecordBatch> AddMetadataToRecordBatch(
    std::shared_ptr<arrow::RecordBatch> const& batch,
    std::map<std::string, std::string> const& meta) {
  if (batch == nullptr || meta.empty()) {
    return batch;
  }
  std::shared_ptr<arrow::KeyValueMetadata> metadata;
  if (batch->schema()->metadata() != nullptr) {
    metadata = batch->schema()->metadata()->Copy();
  } else {
    metadata.reset(new arrow::KeyValueMetadata());
  }

  for (auto const& kv : meta) {
    CHECK_ARROW_ERROR(metadata->Set(kv.first, kv.second));
  }
  return batch->ReplaceSchemaMetadata(metadata);
}

std::shared_ptr<arrow::RecordBatch> AddMetadataToRecordBatch(
    std::shared_ptr<arrow::RecordBatch> const& batch,
    std::unordered_map<std::string, std::string> const& meta) {
  if (batch == nullptr || meta.empty()) {
    return batch;
  }
  std::shared_ptr<arrow::KeyValueMetadata> metadata;
  if (batch->schema()->metadata() != nullptr) {
    metadata = batch->schema()->metadata()->Copy();
  } else {
    metadata.reset(new arrow::KeyValueMetadata());
  }

  for (auto const& kv : meta) {
    CHECK_ARROW_ERROR(metadata->Set(kv.first, kv.second));
  }
  return batch->ReplaceSchemaMetadata(metadata);
}

std::shared_ptr<arrow::DataType> type_name_to_arrow_type(
    const std::string& name) {
  if (name == "bool") {
    return vineyard::ConvertToArrowType<bool>::TypeValue();
  } else if (name == "int8_t" || name == "int8" || name == "byte") {
    return vineyard::ConvertToArrowType<int8_t>::TypeValue();
  } else if (name == "uint8_t" || name == "uint8" || name == "char") {
    return vineyard::ConvertToArrowType<uint8_t>::TypeValue();
  } else if (name == "int16_t" || name == "int16" || name == "half") {
    return vineyard::ConvertToArrowType<int16_t>::TypeValue();
  } else if (name == "uint16_t" || name == "uint16") {
    return vineyard::ConvertToArrowType<uint16_t>::TypeValue();
  } else if (name == "int32_t" || name == "int32" || name == "int") {
    return vineyard::ConvertToArrowType<int32_t>::TypeValue();
  } else if (name == "uint32_t" || name == "uint32") {
    return vineyard::ConvertToArrowType<uint32_t>::TypeValue();
  } else if (name == "int64_t" || name == "int64" || name == "long") {
    return vineyard::ConvertToArrowType<int64_t>::TypeValue();
  } else if (name == "uint64_t" || name == "uint64") {
    return vineyard::ConvertToArrowType<uint64_t>::TypeValue();
  } else if (name == "float") {
    return vineyard::ConvertToArrowType<float>::TypeValue();
  } else if (name == "double") {
    return vineyard::ConvertToArrowType<double>::TypeValue();
  } else if (name == "string" || name == "std::string" || name == "str" ||
             name == "std::__1::string" || name == "std::__cxx11::string") {
    return vineyard::ConvertToArrowType<std::string>::TypeValue();
  } else if (name.substr(0, std::string("list<item: ").length()) ==
             std::string("list<item: ")) {
    std::string inner_type_name =
        name.substr(std::string("list<item: ").length(),
                    name.length() - std::string("list<item: ").length() - 1);
    return arrow::list(type_name_to_arrow_type(inner_type_name));
  } else if (name.substr(0, std::string("large_list<item: ").length()) ==
             std::string("large_list<item: ")) {
    std::string inner_type_name = name.substr(
        std::string("large_list<item: ").length(),
        name.length() - std::string("large_list<item: ").length() - 1);
    return arrow::large_list(type_name_to_arrow_type(inner_type_name));
  } else if (name.substr(0, std::string("fixed_size_list<item: ").length()) ==
             std::string("fixed_size_list<item: ")) {
    auto pos = name.find_first_of('[');
    std::string inner_type_name =
        name.substr(std::string("fixed_size_list<item: ").length(),
                    pos - std::string("fixed_size_list<item: ").length() - 1);
    auto size = std::stoi(name.substr(pos + 1, name.length() - pos - 2));
    return arrow::fixed_size_list(type_name_to_arrow_type(inner_type_name),
                                  size);
  } else if (name == "null" || name == "NULL") {
    return arrow::null();
  } else {
    LOG(ERROR) << "Unsupported data type: '" << name << "'";
    return arrow::null();
  }
}

std::string type_name_from_arrow_type(
    std::shared_ptr<arrow::DataType> const& type) {
  if (arrow::null()->Equals(type)) {
    return "null";
  } else if (vineyard::ConvertToArrowType<bool>::TypeValue()->Equals(type)) {
    return type_name<bool>();
  } else if (vineyard::ConvertToArrowType<int8_t>::TypeValue()->Equals(type)) {
    return type_name<int8_t>();
  } else if (vineyard::ConvertToArrowType<uint8_t>::TypeValue()->Equals(type)) {
    return type_name<uint8_t>();
  } else if (vineyard::ConvertToArrowType<int16_t>::TypeValue()->Equals(type)) {
    return type_name<int16_t>();
  } else if (vineyard::ConvertToArrowType<uint16_t>::TypeValue()->Equals(
                 type)) {
    return type_name<uint16_t>();
  } else if (vineyard::ConvertToArrowType<int32_t>::TypeValue()->Equals(type)) {
    return type_name<int32_t>();
  } else if (vineyard::ConvertToArrowType<uint32_t>::TypeValue()->Equals(
                 type)) {
    return type_name<uint32_t>();
  } else if (vineyard::ConvertToArrowType<int64_t>::TypeValue()->Equals(type)) {
    return type_name<int64_t>();
  } else if (vineyard::ConvertToArrowType<uint64_t>::TypeValue()->Equals(
                 type)) {
    return type_name<uint64_t>();
  } else if (vineyard::ConvertToArrowType<float>::TypeValue()->Equals(type)) {
    return type_name<float>();
  } else if (vineyard::ConvertToArrowType<double>::TypeValue()->Equals(type)) {
    return type_name<double>();
  } else if (vineyard::ConvertToArrowType<std::string>::TypeValue()->Equals(
                 type)) {
    return type_name<std::string>();
  } else if (type != nullptr && type->id() == arrow::Type::LIST) {
    auto list_type = std::static_pointer_cast<arrow::ListType>(type);
    return "list<item: " + type_name_from_arrow_type(list_type->value_type()) +
           ">";
  } else if (type != nullptr && type->id() == arrow::Type::LARGE_LIST) {
    auto list_type = std::static_pointer_cast<arrow::LargeListType>(type);
    return "large_list<item: " +
           type_name_from_arrow_type(list_type->value_type()) + ">";
  } else if (type != nullptr && type->id() == arrow::Type::FIXED_SIZE_LIST) {
    auto list_type = std::static_pointer_cast<arrow::FixedSizeListType>(type);
    return "fixed_size_list<item: " +
           type_name_from_arrow_type(list_type->value_type()) + ">[" +
           std::to_string(list_type->list_size()) + "]";
  } else {
    LOG(ERROR) << "Unsupported arrow type '" << type->ToString()
               << "', type id: " << type->id();
    return "undefined";
  }
}

const void* get_arrow_array_data(std::shared_ptr<arrow::Array> const& array) {
  if (array->type()->Equals(arrow::int8())) {
    return reinterpret_cast<const void*>(
        std::dynamic_pointer_cast<arrow::Int8Array>(array)->raw_values());
  } else if (array->type()->Equals(arrow::uint8())) {
    return reinterpret_cast<const void*>(
        std::dynamic_pointer_cast<arrow::UInt8Array>(array)->raw_values());
  } else if (array->type()->Equals(arrow::int16())) {
    return reinterpret_cast<const void*>(
        std::dynamic_pointer_cast<arrow::Int16Array>(array)->raw_values());
  } else if (array->type()->Equals(arrow::uint16())) {
    return reinterpret_cast<const void*>(
        std::dynamic_pointer_cast<arrow::UInt16Array>(array)->raw_values());
  } else if (array->type()->Equals(arrow::int32())) {
    return reinterpret_cast<const void*>(
        std::dynamic_pointer_cast<arrow::Int32Array>(array)->raw_values());
  } else if (array->type()->Equals(arrow::uint32())) {
    return reinterpret_cast<const void*>(
        std::dynamic_pointer_cast<arrow::UInt32Array>(array)->raw_values());
  } else if (array->type()->Equals(arrow::int64())) {
    return reinterpret_cast<const void*>(
        std::dynamic_pointer_cast<arrow::Int64Array>(array)->raw_values());
  } else if (array->type()->Equals(arrow::uint64())) {
    return reinterpret_cast<const void*>(
        std::dynamic_pointer_cast<arrow::UInt64Array>(array)->raw_values());
  } else if (array->type()->Equals(arrow::float32())) {
    return reinterpret_cast<const void*>(
        std::dynamic_pointer_cast<arrow::FloatArray>(array)->raw_values());
  } else if (array->type()->Equals(arrow::float64())) {
    return reinterpret_cast<const void*>(
        std::dynamic_pointer_cast<arrow::DoubleArray>(array)->raw_values());
  } else if (array->type()->Equals(arrow::utf8())) {
    return reinterpret_cast<const void*>(
        std::dynamic_pointer_cast<arrow::StringArray>(array).get());
  } else if (array->type()->Equals(arrow::large_utf8())) {
    return reinterpret_cast<const void*>(
        std::dynamic_pointer_cast<arrow::LargeStringArray>(array).get());
  } else if (array->type()->id() == arrow::Type::LIST) {
    return reinterpret_cast<const void*>(
        std::dynamic_pointer_cast<arrow::ListArray>(array).get());
  } else if (array->type()->id() == arrow::Type::LARGE_LIST) {
    return reinterpret_cast<const void*>(
        std::dynamic_pointer_cast<arrow::LargeListArray>(array).get());
  } else if (array->type()->id() == arrow::Type::FIXED_SIZE_LIST) {
    return reinterpret_cast<const void*>(
        std::dynamic_pointer_cast<arrow::FixedSizeListArray>(array).get());
  } else if (array->type()->Equals(arrow::null())) {
    return reinterpret_cast<const void*>(
        std::dynamic_pointer_cast<arrow::NullArray>(array).get());
  } else {
    LOG(FATAL) << "Array type - " << array->type()->ToString()
               << " is not supported yet...";
    return NULL;
  }
}

Status TypeLoosen(const std::vector<std::shared_ptr<arrow::Schema>>& schemas,
                  std::shared_ptr<arrow::Schema>& schema) {
  int field_num = -1;
  std::shared_ptr<arrow::KeyValueMetadata> metadata(
      new arrow::KeyValueMetadata());
  for (const auto& schema : schemas) {
    if (schema != nullptr) {
      RETURN_ON_ASSERT(
          field_num == -1 || field_num == schema->num_fields(),
          "Inconsistent field number in those schemas that will be unified");
      field_num = schema->num_fields();
      if (schema->metadata() != nullptr) {
        std::unordered_map<std::string, std::string> metakv;
        schema->metadata()->ToUnorderedMap(&metakv);
        for (auto const& kv : metakv) {
          metadata->Append(kv.first, kv.second);
        }
      }
    }
  }
  RETURN_ON_ASSERT(field_num > 0,
                   "Empty table list cannot be used for normalizing schema");

  // Perform type lossen.
  // Date32 -> int32
  // Timestamp -> int64 -> double -> utf8   binary (not supported)

  // Timestamp value are stored as as number of seconds, milliseconds,
  // microseconds or nanoseconds since UNIX epoch.
  // CSV reader can only produce timestamp in seconds.
  std::vector<std::vector<std::shared_ptr<arrow::Field>>> fields(field_num);
  for (int i = 0; i < field_num; ++i) {
    for (const auto& schema : schemas) {
      if (schema != nullptr) {
        fields[i].push_back(schema->field(i));
      }
    }
  }
  std::vector<std::shared_ptr<arrow::Field>> lossen_fields(field_num);

  for (int i = 0; i < field_num; ++i) {
    lossen_fields[i] = fields[i][0];
    auto res = fields[i][0]->type();
    if (res == arrow::null()) {
      continue;
    }
    if (res->Equals(arrow::boolean())) {
      res = arrow::int32();
    }
    if (res->Equals(arrow::date32())) {
      res = arrow::int32();
    }
    if (res->Equals(arrow::date64())) {
      res = arrow::int64();
    }
    if (res->id() == arrow::Type::TIMESTAMP) {
      res = arrow::int64();
    }
    if (res->Equals(arrow::int64())) {
      for (size_t j = 1; j < fields[i].size(); ++j) {
        if (fields[i][j]->type()->Equals(arrow::float64())) {
          res = arrow::float64();
        }
      }
    }
    if (res->Equals(arrow::float64())) {
      for (size_t j = 1; j < fields[i].size(); ++j) {
        if (fields[i][j]->type()->Equals(arrow::utf8())) {
          res = arrow::utf8();
        }
      }
    }
    if (res->Equals(arrow::utf8())) {
      res = arrow::large_utf8();
    }
    lossen_fields[i] = lossen_fields[i]->WithType(res);
  }
  schema = std::make_shared<arrow::Schema>(lossen_fields, metadata);
  return Status::OK();
}

Status CastStringToBigString(const std::shared_ptr<arrow::Array>& in,
                             const std::shared_ptr<arrow::DataType>& to_type,
                             std::shared_ptr<arrow::Array>& out) {
  auto array_data = in->data()->Copy();
  auto offset = array_data->buffers[1];
  using from_offset_type = typename arrow::StringArray::offset_type;
  using to_string_offset_type = typename arrow::LargeStringArray::offset_type;
  auto raw_value_offsets_ =
      offset == NULLPTR
          ? NULLPTR
          : reinterpret_cast<const from_offset_type*>(offset->data());
  std::vector<to_string_offset_type> to_offset(offset->size() /
                                               sizeof(from_offset_type));
  for (size_t i = 0; i < to_offset.size(); ++i) {
    to_offset[i] = raw_value_offsets_[i];
  }
  std::shared_ptr<arrow::Buffer> buffer;
  arrow::TypedBufferBuilder<to_string_offset_type> buffer_builder;
  RETURN_ON_ARROW_ERROR(
      buffer_builder.Append(to_offset.data(), to_offset.size()));
  RETURN_ON_ARROW_ERROR(buffer_builder.Finish(&buffer));
  array_data->type = to_type;
  array_data->buffers[1] = buffer;
  out = arrow::MakeArray(array_data);
  RETURN_ON_ARROW_ERROR(out->ValidateFull());
  return Status::OK();
}

Status CastNullToOthers(const std::shared_ptr<arrow::Array>& in,
                        const std::shared_ptr<arrow::DataType>& to_type,
                        std::shared_ptr<arrow::Array>& out) {
  std::unique_ptr<arrow::ArrayBuilder> builder;
  RETURN_ON_ARROW_ERROR(
      arrow::MakeBuilder(arrow::default_memory_pool(), to_type, &builder));
  RETURN_ON_ARROW_ERROR(builder->AppendNulls(in->length()));
  RETURN_ON_ARROW_ERROR(builder->Finish(&out));
  RETURN_ON_ARROW_ERROR(out->ValidateFull());
  return Status::OK();
}

Status GeneralCast(const std::shared_ptr<arrow::Array>& in,
                   const std::shared_ptr<arrow::DataType>& to_type,
                   std::shared_ptr<arrow::Array>& out) {
#if defined(ARROW_VERSION) && ARROW_VERSION < 1000000
  arrow::compute::FunctionContext ctx;
  ARROW_OK_OR_RAISE(arrow::compute::Cast(&ctx, *in, to_type, {}, &out));
#else
  CHECK_ARROW_ERROR_AND_ASSIGN(out, arrow::compute::Cast(*in, to_type));
#endif
  return Status::OK();
}

Status CastTableToSchema(const std::shared_ptr<arrow::Table>& table,
                         const std::shared_ptr<arrow::Schema>& schema,
                         std::shared_ptr<arrow::Table>& out) {
  if (table->schema()->Equals(schema)) {
    out = table;
    return Status::OK();
  }

  RETURN_ON_ASSERT(
      table->num_columns() == schema->num_fields(),
      "The schema of original table and expected schema is not consistent");
  std::vector<std::shared_ptr<arrow::ChunkedArray>> new_columns;
  for (int64_t i = 0; i < table->num_columns(); ++i) {
    auto col = table->column(i);
    if (table->field(i)->type()->Equals(schema->field(i)->type())) {
      new_columns.push_back(col);
      continue;
    }
    auto from_type = table->field(i)->type();
    auto to_type = schema->field(i)->type();
    std::vector<std::shared_ptr<arrow::Array>> chunks;
    for (int64_t j = 0; j < col->num_chunks(); ++j) {
      auto array = col->chunk(j);
      std::shared_ptr<arrow::Array> out;
      if (arrow::compute::CanCast(*from_type, *to_type)) {
        RETURN_ON_ERROR(GeneralCast(array, to_type, out));
        chunks.push_back(out);
      } else if (from_type->Equals(arrow::utf8()) &&
                 to_type->Equals(arrow::large_utf8())) {
        RETURN_ON_ERROR(CastStringToBigString(array, to_type, out));
        chunks.push_back(out);
      } else if (from_type->Equals(arrow::null())) {
        RETURN_ON_ERROR(CastNullToOthers(array, to_type, out));
        chunks.push_back(out);
      } else {
        return Status::Invalid(
            "Unsupported cast: To type: " + to_type->ToString() +
            " vs. origin type: " + from_type->ToString());
      }
    }
    new_columns.push_back(
        std::make_shared<arrow::ChunkedArray>(chunks, to_type));
  }
  out = arrow::Table::Make(schema, new_columns);
  return Status::OK();
}

inline bool IsDataTypeConsilidatable(std::shared_ptr<arrow::DataType> type) {
  if (type == nullptr) {
    return false;
  }
  switch (type->id()) {
  case arrow::Type::INT8:
  case arrow::Type::INT16:
  case arrow::Type::INT32:
  case arrow::Type::INT64:
  case arrow::Type::UINT8:
  case arrow::Type::UINT16:
  case arrow::Type::UINT32:
  case arrow::Type::UINT64:
  case arrow::Type::FLOAT:
  case arrow::Type::DOUBLE: {
    return true;
  }
  default: {
    return false;
  }
  }
}

template <typename T>
inline void AssignArrayWithStride(std::shared_ptr<arrow::Buffer> array,
                                  std::shared_ptr<arrow::Buffer> target,
                                  int64_t length, int64_t stride,
                                  int64_t offset) {
  auto array_data = reinterpret_cast<const T*>(array->data());
  auto target_data = reinterpret_cast<T*>(target->mutable_data());
  for (int64_t i = 0; i < length; ++i) {
    target_data[i * stride + offset] = array_data[i];
  }
}

inline void AssignArrayWithStrideUntyped(std::shared_ptr<arrow::Array> array,
                                         std::shared_ptr<arrow::Buffer> target,
                                         int64_t length, int64_t stride,
                                         int64_t offset) {
  if (array->length() == 0) {
    return;
  }
  switch (array->type()->id()) {
  case arrow::Type::INT8: {
    AssignArrayWithStride<int8_t>(array->data()->buffers[1], target, length,
                                  stride, offset);
    return;
  }
  case arrow::Type::INT16: {
    AssignArrayWithStride<int16_t>(array->data()->buffers[1], target, length,
                                   stride, offset);
    return;
  }
  case arrow::Type::INT32: {
    AssignArrayWithStride<int32_t>(array->data()->buffers[1], target, length,
                                   stride, offset);
    return;
  }
  case arrow::Type::INT64: {
    AssignArrayWithStride<int64_t>(array->data()->buffers[1], target, length,
                                   stride, offset);
    return;
  }
  case arrow::Type::UINT8: {
    AssignArrayWithStride<uint8_t>(array->data()->buffers[1], target, length,
                                   stride, offset);
    return;
  }
  case arrow::Type::UINT16: {
    AssignArrayWithStride<uint16_t>(array->data()->buffers[1], target, length,
                                    stride, offset);
    return;
  }
  case arrow::Type::UINT32: {
    AssignArrayWithStride<uint32_t>(array->data()->buffers[1], target, length,
                                    stride, offset);
    return;
  }
  case arrow::Type::UINT64: {
    AssignArrayWithStride<uint64_t>(array->data()->buffers[1], target, length,
                                    stride, offset);
    return;
  }
  case arrow::Type::FLOAT: {
    AssignArrayWithStride<float>(array->data()->buffers[1], target, length,
                                 stride, offset);
    return;
  }
  case arrow::Type::DOUBLE: {
    AssignArrayWithStride<double>(array->data()->buffers[1], target, length,
                                  stride, offset);
    return;
  }
  default: {
  }
  }
}

Status ConsolidateColumns(
    const std::vector<std::shared_ptr<arrow::Array>>& columns,
    std::shared_ptr<arrow::Array>& out) {
  if (columns.size() == 0) {
    return Status::Invalid("No columns to consolidate");
  }
  // check the types of columns that will be consolidated
  std::shared_ptr<arrow::DataType> dtype = nullptr;
  for (auto const& column : columns) {
    auto column_dtype = column->type();
    if (!IsDataTypeConsilidatable(column_dtype)) {
      return Status::Invalid("column type '" + column->type()->ToString() +
                             "' is not a numeric type");
    }
    if (dtype == nullptr || dtype->Equals(column_dtype)) {
      dtype = column_dtype;
    } else {
      return Status::Invalid(
          "cannot consolidate columns"
          "', column type '" +
          column->type()->ToString() +
          "' has different type with other columns");
    }
  }

  // consolidate columns into one

  // build the data buffer
  std::shared_ptr<arrow::DataType> list_array_dtype =
      arrow::fixed_size_list(dtype, columns.size());

  std::shared_ptr<arrow::Buffer> data_buffer;
  CHECK_ARROW_ERROR_AND_ASSIGN(
      data_buffer,
      arrow::AllocateBuffer(
          columns[0]->length() * columns.size() *
          static_cast<arrow::FixedWidthType*>(dtype.get())->bit_width() / 8));

  for (size_t index = 0; index < columns.size(); ++index) {
    auto array = columns[index];
    AssignArrayWithStrideUntyped(array, data_buffer, array->length(),
                                 columns.size(), index);
  }

  // build the list array
  out = std::make_shared<arrow::FixedSizeListArray>(
      list_array_dtype, columns[0]->length(),
      std::make_shared<arrow::PrimitiveArray>(
          dtype, columns[0]->length() * columns.size(), data_buffer));
  return Status::OK();
}

Status ConsolidateColumns(
    const std::vector<std::shared_ptr<arrow::ChunkedArray>>& columns,
    std::shared_ptr<arrow::ChunkedArray>& out) {
  std::vector<std::shared_ptr<arrow::Array>> array_chunks;

  for (int64_t i = 0; i < columns[0]->num_chunks(); ++i) {
    std::vector<std::shared_ptr<arrow::Array>> columns_chunk;
    for (auto const& column : columns) {
      columns_chunk.push_back(column->chunk(i));
    }
    std::shared_ptr<arrow::Array> array_chunk;
    RETURN_ON_ERROR(ConsolidateColumns(columns_chunk, array_chunk));
    array_chunks.push_back(array_chunk);
  }

  CHECK_ARROW_ERROR_AND_ASSIGN(out, arrow::ChunkedArray::Make(array_chunks));
  return Status::OK();
}

/**
 * @brief Consolidate columns in an arrow table into one column
 * (FixedSizeListArray).
 *
 * Note that the bitmap in the given columns will be discard.
 *
 * @param table
 */
Status ConsolidateColumns(const std::shared_ptr<arrow::Table>& table,
                          std::vector<std::string> const& column_names,
                          std::string const& consolidated_column_name,
                          std::shared_ptr<arrow::Table>& out) {
  // check the types of columns that will be consolidated
  std::string column_names_joined = boost::algorithm::join(column_names, ",");
  auto schema = table->schema();
  std::shared_ptr<arrow::DataType> dtype = nullptr;
  std::vector<int> column_indexes;
  std::vector<std::shared_ptr<arrow::ChunkedArray>> columns;
  for (auto const& column_name : column_names) {
    auto column_index = schema->GetFieldIndex(column_name);
    if (column_index == -1) {
      return Status::Invalid("column name '" + column_name +
                             "' doesn't exist in the table");
    }
    auto column_dtype = schema->field(column_index)->type();
    if (!IsDataTypeConsilidatable(column_dtype)) {
      return Status::Invalid("column '" + column_name +
                             "' is not a numeric type");
    }
    if (dtype == nullptr || dtype->Equals(column_dtype)) {
      dtype = column_dtype;
      column_indexes.push_back(column_index);
      columns.push_back(table->column(column_index));
    } else {
      return Status::Invalid("cannot consolidate columns '" +
                             column_names_joined + "', column name '" +
                             column_name +
                             "' has different type with other columns");
    }
  }

  // consolidate columns into one
  std::shared_ptr<arrow::DataType> list_array_dtype =
      arrow::fixed_size_list(dtype, columns.size());
  std::shared_ptr<arrow::ChunkedArray> consolidated_array;
  RETURN_ON_ERROR(ConsolidateColumns(columns, consolidated_array));

  // replace those columns with the consolidated column
  std::vector<int> sorted_column_indexes(column_indexes);
  std::sort(sorted_column_indexes.begin(), sorted_column_indexes.end());
  std::shared_ptr<arrow::Table> result = table;
  for (size_t index = 0; index < sorted_column_indexes.size(); ++index) {
    CHECK_ARROW_ERROR_AND_ASSIGN(
        result,
        result->RemoveColumn(
            sorted_column_indexes[sorted_column_indexes.size() - 1 - index]));
  }
  std::shared_ptr<arrow::Field> field;
  if (consolidated_column_name.empty()) {
    field = arrow::field(column_names_joined, list_array_dtype);
  } else {
    field = arrow::field(consolidated_column_name, list_array_dtype);
  }
  CHECK_ARROW_ERROR_AND_ASSIGN(
      out, result->AddColumn(result->num_columns(), field, consolidated_array));
  return Status::OK();
}

Status ConsolidateColumns(const std::shared_ptr<arrow::Table>& table,
                          std::shared_ptr<arrow::Table>& out) {
  if (table == nullptr || table->schema() == nullptr ||
      table->schema()->metadata() == nullptr) {
    out = table;
    return Status::OK();
  }
  auto metadata = table->schema()->metadata();
  auto consolidate_columns_index = metadata->FindKey("consolidate");
  if (consolidate_columns_index == -1) {
    out = table;
    return Status::OK();
  }
  auto consolidate_columns = metadata->value(consolidate_columns_index);
  if (consolidate_columns.empty()) {
    out = table;
    return Status::OK();
  }

  std::vector<std::string> consolidate_columns_vec;
  boost::algorithm::split(consolidate_columns_vec, consolidate_columns,
                          boost::is_any_of(",;"));
  return ConsolidateColumns(table, consolidate_columns_vec, "", out);
}

}  // namespace vineyard
