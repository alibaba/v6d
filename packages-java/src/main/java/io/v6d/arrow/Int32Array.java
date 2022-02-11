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
import io.v6d.arrow.impl.numericarray.IteratorTypeArrowInt32Type;
import io.v6d.arrow.impl.numericarray.ValueTypeArrowInt32Type;
import io.v6d.std.shared_ptr;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("arrow::Int32Array")
@FFIGen
@CXXHead("arrow/array/array_primitive.h")
public interface Int32Array extends CXXPointer {
    @FFIExpr("{0}")
    NumericArray<Int32Type> get();

    ValueTypeArrowInt32Type raw_values();

    int Value(long i);

    int GetView(long i);

    @CXXValue
    IteratorTypeArrowInt32Type begin();

    @CXXValue
    IteratorTypeArrowInt32Type end();

    static Int32Array cast(final long __foreign_address) {
        try {
            Class<Int32Array> clz = (Class<Int32Array>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(Int32Array.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static Int32Array cast(final FFIPointer __foreign_pointer) {
        return Int32Array.cast(__foreign_pointer.getAddress());
    }

    static Factory getFactory() {
        return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(Int32Array.class, true));
    }

    static Int32Array create(
            @CXXReference @FFITypeAlias("const std::shared_ptr<arrow::ArrayData>") shared_ptr<ArrayData> data) {
        return Int32Array.getFactory().create(data);
    }

    @FFIFactory
    @CXXHead("arrow/array/array_primitive.h")
    interface Factory {
        Int32Array create(
                @CXXReference @FFITypeAlias("const std::shared_ptr<arrow::ArrayData>") shared_ptr<ArrayData> data);
    }
}
