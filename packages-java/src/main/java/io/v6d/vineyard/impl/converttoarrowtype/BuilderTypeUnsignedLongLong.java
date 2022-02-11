// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.vineyard.impl.converttoarrowtype;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXPointer;
import com.alibaba.fastffi.CXXReference;
import com.alibaba.fastffi.CXXValue;
import com.alibaba.fastffi.FFIConst;
import com.alibaba.fastffi.FFIExpr;
import com.alibaba.fastffi.FFIFactory;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import io.v6d.arrow.ArrayData;
import io.v6d.arrow.DataType;
import io.v6d.arrow.MemoryPool;
import io.v6d.arrow.Status;
import io.v6d.arrow.UInt64Builder;
import io.v6d.arrow.impl.numericbuilder.ArrayTypeArrowUInt64Type;
import io.v6d.arrow.impl.numericbuilder.ValueTypeArrowUInt64Type;
import io.v6d.std.CBool;
import io.v6d.std.CUnsignedChar;
import io.v6d.std.shared_ptr;
import io.v6d.std.vector;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("vineyard::ConvertToArrowType<unsigned long long>::BuilderType")
@FFIGen
@CXXHead("basic/ds/arrow_utils.h")
public interface BuilderTypeUnsignedLongLong extends CXXPointer {
    @FFIExpr("{0}")
    UInt64Builder get();

    @CXXValue
    Status Append(@FFIConst long val);

    @CXXValue
    Status AppendNulls(long length);

    @CXXValue
    Status AppendNull();

    @CXXValue
    Status AppendEmptyValue();

    @CXXValue
    Status AppendEmptyValues(long length);

    long GetValue(long index);

    void Reset();

    @CXXValue
    Status Resize(long capacity);

    @CXXValue
    Status AppendValues(ValueTypeArrowUInt64Type values, long length, CUnsignedChar valid_bytes);

    @CXXValue
    Status AppendValues(ValueTypeArrowUInt64Type values, long length, CUnsignedChar bitmap,
            long bitmap_offset);

    @CXXValue
    Status AppendValues(ValueTypeArrowUInt64Type values, long length,
            @CXXReference @FFITypeAlias("const std::vector<bool>") vector<CBool> is_valid);

    @CXXValue
    Status AppendValues(
            @CXXReference @FFITypeAlias("const std::vector<arrow::NumericBuilder<arrow::UInt64Type>::value_type>") vector<ValueTypeArrowUInt64Type> values,
            @CXXReference @FFITypeAlias("const std::vector<bool>") vector<CBool> is_valid);

    @CXXValue
    Status AppendValues(
            @CXXReference @FFITypeAlias("const std::vector<arrow::NumericBuilder<arrow::UInt64Type>::value_type>") vector<ValueTypeArrowUInt64Type> values);

    @CXXValue
    Status FinishInternal(
            @FFITypeAlias("std::shared_ptr<arrow::ArrayData>") shared_ptr<ArrayData> out);

    @CXXValue
    Status Finish(
            @FFITypeAlias("std::shared_ptr<arrow::NumericBuilder<arrow::UInt64Type>::ArrayType>") shared_ptr<ArrayTypeArrowUInt64Type> out);

    @CXXValue
    Status AppendArraySlice(@CXXReference ArrayData array, long offset, long length);

    void UnsafeAppend(@FFIConst long val);

    void UnsafeAppendNull();

    @CXXValue
    @FFITypeAlias("std::shared_ptr<arrow::DataType>")
    shared_ptr<DataType> type();

    static BuilderTypeUnsignedLongLong cast(final long __foreign_address) {
        try {
            Class<BuilderTypeUnsignedLongLong> clz = (Class<BuilderTypeUnsignedLongLong>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(BuilderTypeUnsignedLongLong.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static BuilderTypeUnsignedLongLong cast(final FFIPointer __foreign_pointer) {
        return BuilderTypeUnsignedLongLong.cast(__foreign_pointer.getAddress());
    }

    static Factory getFactory() {
        return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(BuilderTypeUnsignedLongLong.class, true));
    }

    static BuilderTypeUnsignedLongLong create(
            @CXXReference @FFITypeAlias("const std::shared_ptr<arrow::DataType>") shared_ptr<DataType> type,
            MemoryPool pool) {
        return BuilderTypeUnsignedLongLong.getFactory().create(type, pool);
    }

    @FFIFactory
    @CXXHead("basic/ds/arrow_utils.h")
    interface Factory {
        BuilderTypeUnsignedLongLong create(
                @CXXReference @FFITypeAlias("const std::shared_ptr<arrow::DataType>") shared_ptr<DataType> type,
                MemoryPool pool);
    }
}
