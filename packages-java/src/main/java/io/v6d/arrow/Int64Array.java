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
import io.v6d.arrow.impl.numericarray.IteratorTypeArrowInt64Type;
import io.v6d.arrow.impl.numericarray.ValueTypeArrowInt64Type;
import io.v6d.std.shared_ptr;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("arrow::Int64Array")
@FFIGen
@CXXHead("arrow/array/array_primitive.h")
public interface Int64Array extends CXXPointer {
    @FFIExpr("{0}")
    NumericArray<Int64Type> get();

    ValueTypeArrowInt64Type raw_values();

    long Value(long i);

    long GetView(long i);

    @CXXValue
    IteratorTypeArrowInt64Type begin();

    @CXXValue
    IteratorTypeArrowInt64Type end();

    static Int64Array cast(final long __foreign_address) {
        try {
            Class<Int64Array> clz = (Class<Int64Array>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(Int64Array.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static Int64Array cast(final FFIPointer __foreign_pointer) {
        return Int64Array.cast(__foreign_pointer.getAddress());
    }

    static Factory getFactory() {
        return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(Int64Array.class, true));
    }

    static Int64Array create(
            @CXXReference @FFITypeAlias("const std::shared_ptr<arrow::ArrayData>") shared_ptr<ArrayData> data) {
        return Int64Array.getFactory().create(data);
    }

    @FFIFactory
    @CXXHead("arrow/array/array_primitive.h")
    interface Factory {
        Int64Array create(
                @CXXReference @FFITypeAlias("const std::shared_ptr<arrow::ArrayData>") shared_ptr<ArrayData> data);
    }
}
