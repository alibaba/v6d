// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.arrow;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXPointer;
import com.alibaba.fastffi.CXXReference;
import com.alibaba.fastffi.CXXValue;
import com.alibaba.fastffi.FFIExpr;
import com.alibaba.fastffi.FFIFactory;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import io.v6d.arrow.impl.numericarray.IteratorTypeArrowFloatType;
import io.v6d.arrow.impl.numericarray.ValueTypeArrowFloatType;
import io.v6d.std.shared_ptr;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("arrow::FloatArray")
@FFIGen
@CXXHead("arrow/array/array_primitive.h")
public interface FloatArray extends CXXPointer {
    @FFIExpr("{0}")
    NumericArray<FloatType> get();

    ValueTypeArrowFloatType raw_values();

    float Value(long i);

    float GetView(long i);

    @CXXValue
    IteratorTypeArrowFloatType begin();

    @CXXValue
    IteratorTypeArrowFloatType end();

    static FloatArray cast(final long __foreign_address) {
        try {
            Class<FloatArray> clz = (Class<FloatArray>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(FloatArray.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static FloatArray cast(final FFIPointer __foreign_pointer) {
        return FloatArray.cast(__foreign_pointer.getAddress());
    }

    static Factory getFactory() {
        return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(FloatArray.class, true));
    }

    static FloatArray create(
            @CXXReference @FFITypeAlias("const std::shared_ptr<arrow::ArrayData>") shared_ptr<ArrayData> data) {
        return FloatArray.getFactory().create(data);
    }

    @FFIFactory
    @CXXHead("arrow/array/array_primitive.h")
    interface Factory {
        FloatArray create(
                @CXXReference @FFITypeAlias("const std::shared_ptr<arrow::ArrayData>") shared_ptr<ArrayData> data);
    }
}
