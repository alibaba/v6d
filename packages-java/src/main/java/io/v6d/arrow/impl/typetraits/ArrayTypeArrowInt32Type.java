// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.arrow.impl.typetraits;

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
import io.v6d.arrow.ArrayData;
import io.v6d.arrow.Int32Array;
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

@FFITypeAlias("arrow::TypeTraits<arrow::Int32Type>::ArrayType")
@FFIGen
@CXXHead(
        system = "arrow/type_traits.h"
)
public interface ArrayTypeArrowInt32Type extends CXXPointer {
    @FFIExpr("{0}")
    Int32Array get();

    ValueTypeArrowInt32Type raw_values();

    int Value(long i);

    int GetView(long i);

    @CXXValue
    IteratorTypeArrowInt32Type begin();

    @CXXValue
    IteratorTypeArrowInt32Type end();

    static ArrayTypeArrowInt32Type cast(final long __foreign_address) {
        try {
            Class<ArrayTypeArrowInt32Type> clz = (Class<ArrayTypeArrowInt32Type>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(ArrayTypeArrowInt32Type.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static ArrayTypeArrowInt32Type cast(final FFIPointer __foreign_pointer) {
        return ArrayTypeArrowInt32Type.cast(__foreign_pointer.getAddress());
    }

    static Factory getFactory() {
        return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(ArrayTypeArrowInt32Type.class, true));
    }

    static ArrayTypeArrowInt32Type create(
            @CXXReference @FFITypeAlias("const std::shared_ptr<arrow::ArrayData>") shared_ptr<ArrayData> data) {
        return ArrayTypeArrowInt32Type.getFactory().create(data);
    }

    @FFIFactory
    @CXXHead(
            system = "arrow/type_traits.h"
    )
    interface Factory {
        ArrayTypeArrowInt32Type create(
                @CXXReference @FFITypeAlias("const std::shared_ptr<arrow::ArrayData>") shared_ptr<ArrayData> data);
    }
}
