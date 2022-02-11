// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.arrow;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXReference;
import com.alibaba.fastffi.CXXValue;
import com.alibaba.fastffi.FFIFactory;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFIGetter;
import com.alibaba.fastffi.FFILibrary;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import io.v6d.std.CLongLong;
import io.v6d.std.atomic;
import io.v6d.std.shared_ptr;
import io.v6d.std.vector;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("arrow::ArrayData")
@FFIGen
@CXXHead(
        system = "arrow/array/data.h"
)
@CXXHead("arrow/type.h")
public interface ArrayData extends FFIPointer {
    @FFIGetter
    @CXXReference
    @FFITypeAlias("std::shared_ptr<arrow::DataType>")
    shared_ptr<DataType> type();

    @FFIGetter
    long length();

    @FFIGetter
    @CXXReference
    @FFITypeAlias("std::atomic<long long>")
    atomic<CLongLong> null_count();

    @FFIGetter
    long offset();

    @FFIGetter
    @CXXReference
    @FFITypeAlias("std::vector<std::shared_ptr<arrow::Buffer>>")
    vector<shared_ptr<Buffer>> buffers();

    @FFIGetter
    @CXXReference
    @FFITypeAlias("std::vector<std::shared_ptr<arrow::ArrayData>>")
    vector<shared_ptr<ArrayData>> child_data();

    @FFIGetter
    @CXXReference
    @FFITypeAlias("std::shared_ptr<arrow::ArrayData>")
    shared_ptr<ArrayData> dictionary();

    @CXXValue
    @FFITypeAlias("std::shared_ptr<arrow::ArrayData>")
    shared_ptr<ArrayData> Copy();

    @CXXValue
    @FFITypeAlias("std::shared_ptr<arrow::ArrayData>")
    shared_ptr<ArrayData> Slice(long offset, long length);

    @CXXValue
    @FFITypeAlias("arrow::Result<std::shared_ptr<arrow::ArrayData>>")
    Result<shared_ptr<ArrayData>> SliceSafe(long offset, long length);

    void SetNullCount(long v);

    long GetNullCount();

    boolean MayHaveNulls();

    static ArrayData cast(final long __foreign_address) {
        try {
            Class<ArrayData> clz = (Class<ArrayData>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(ArrayData.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static ArrayData cast(final FFIPointer __foreign_pointer) {
        return ArrayData.cast(__foreign_pointer.getAddress());
    }

    static Factory getFactory() {
        return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(ArrayData.class, true));
    }

    static ArrayData create(
            @CXXValue @FFITypeAlias("std::shared_ptr<arrow::DataType>") shared_ptr<DataType> type,
            long length, long null_count, long offset) {
        return ArrayData.getFactory().create(type, length, null_count, offset);
    }

    static ArrayData create(
            @CXXValue @FFITypeAlias("std::shared_ptr<arrow::DataType>") shared_ptr<DataType> type,
            long length,
            @CXXValue @FFITypeAlias("std::vector<std::shared_ptr<arrow::Buffer>>") vector<shared_ptr<Buffer>> buffers,
            long null_count, long offset) {
        return ArrayData.getFactory().create(type, length, buffers, null_count, offset);
    }

    static ArrayData create(
            @CXXValue @FFITypeAlias("std::shared_ptr<arrow::DataType>") shared_ptr<DataType> type,
            long length,
            @CXXValue @FFITypeAlias("std::vector<std::shared_ptr<arrow::Buffer>>") vector<shared_ptr<Buffer>> buffers,
            @CXXValue @FFITypeAlias("std::vector<std::shared_ptr<arrow::ArrayData>>") vector<shared_ptr<ArrayData>> child_data,
            long null_count, long offset) {
        return ArrayData.getFactory().create(type, length, buffers, child_data, null_count, offset);
    }

    static ArrayData create(@CXXReference ArrayData other) {
        return ArrayData.getFactory().create(other);
    }

    @FFIFactory
    @CXXHead(
            system = "arrow/array/data.h"
    )
    @CXXHead("arrow/type.h")
    interface Factory {
        ArrayData create(
                @CXXValue @FFITypeAlias("std::shared_ptr<arrow::DataType>") shared_ptr<DataType> type,
                long length, long null_count, long offset);

        ArrayData create(
                @CXXValue @FFITypeAlias("std::shared_ptr<arrow::DataType>") shared_ptr<DataType> type,
                long length,
                @CXXValue @FFITypeAlias("std::vector<std::shared_ptr<arrow::Buffer>>") vector<shared_ptr<Buffer>> buffers,
                long null_count, long offset);

        ArrayData create(
                @CXXValue @FFITypeAlias("std::shared_ptr<arrow::DataType>") shared_ptr<DataType> type,
                long length,
                @CXXValue @FFITypeAlias("std::vector<std::shared_ptr<arrow::Buffer>>") vector<shared_ptr<Buffer>> buffers,
                @CXXValue @FFITypeAlias("std::vector<std::shared_ptr<arrow::ArrayData>>") vector<shared_ptr<ArrayData>> child_data,
                long null_count, long offset);

        ArrayData create(@CXXReference ArrayData other);
    }

    @FFIGen
    @FFILibrary(
            value = "arrow::ArrayData",
            namespace = "arrow::ArrayData"
    )
    @CXXHead(
            system = "arrow/array/data.h"
    )
    @CXXHead("arrow/type.h")
    interface Library {
        Library INSTANCE = FFITypeFactory.getLibrary(Library.class);

        @CXXValue
        @FFITypeAlias("std::shared_ptr<arrow::ArrayData>")
        shared_ptr<ArrayData> Make(
                @CXXValue @FFITypeAlias("std::shared_ptr<arrow::DataType>") shared_ptr<DataType> type,
                long length,
                @CXXValue @FFITypeAlias("std::vector<std::shared_ptr<arrow::Buffer>>") vector<shared_ptr<Buffer>> buffers,
                long null_count, long offset);

        @CXXValue
        @FFITypeAlias("std::shared_ptr<arrow::ArrayData>")
        shared_ptr<ArrayData> Make(
                @CXXValue @FFITypeAlias("std::shared_ptr<arrow::DataType>") shared_ptr<DataType> type,
                long length,
                @CXXValue @FFITypeAlias("std::vector<std::shared_ptr<arrow::Buffer>>") vector<shared_ptr<Buffer>> buffers,
                @CXXValue @FFITypeAlias("std::vector<std::shared_ptr<arrow::ArrayData>>") vector<shared_ptr<ArrayData>> child_data,
                long null_count, long offset);

        @CXXValue
        @FFITypeAlias("std::shared_ptr<arrow::ArrayData>")
        shared_ptr<ArrayData> Make(
                @CXXValue @FFITypeAlias("std::shared_ptr<arrow::DataType>") shared_ptr<DataType> type,
                long length,
                @CXXValue @FFITypeAlias("std::vector<std::shared_ptr<arrow::Buffer>>") vector<shared_ptr<Buffer>> buffers,
                @CXXValue @FFITypeAlias("std::vector<std::shared_ptr<arrow::ArrayData>>") vector<shared_ptr<ArrayData>> child_data,
                @CXXValue @FFITypeAlias("std::shared_ptr<arrow::ArrayData>") shared_ptr<ArrayData> dictionary,
                long null_count, long offset);

        @CXXValue
        @FFITypeAlias("std::shared_ptr<arrow::ArrayData>")
        shared_ptr<ArrayData> Make(
                @CXXValue @FFITypeAlias("std::shared_ptr<arrow::DataType>") shared_ptr<DataType> type,
                long length, long null_count, long offset);
    }
}
