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
import io.v6d.arrow.UInt64Array;
import io.v6d.arrow.impl.numericarray.IteratorTypeArrowUInt64Type;
import io.v6d.arrow.impl.numericarray.ValueTypeArrowUInt64Type;
import io.v6d.std.shared_ptr;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("arrow::TypeTraits<arrow::UInt64Type>::ArrayType")
@FFIGen
@CXXHead(
        system = "arrow/type_traits.h"
)
public interface ArrayTypeArrowUInt64Type extends CXXPointer {
    @FFIExpr("{0}")
    UInt64Array get();

    ValueTypeArrowUInt64Type raw_values();

    long Value(long i);

    long GetView(long i);

    @CXXValue
    IteratorTypeArrowUInt64Type begin();

    @CXXValue
    IteratorTypeArrowUInt64Type end();

    static ArrayTypeArrowUInt64Type cast(final long __foreign_address) {
        try {
            Class<ArrayTypeArrowUInt64Type> clz = (Class<ArrayTypeArrowUInt64Type>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(ArrayTypeArrowUInt64Type.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static ArrayTypeArrowUInt64Type cast(final FFIPointer __foreign_pointer) {
        return ArrayTypeArrowUInt64Type.cast(__foreign_pointer.getAddress());
    }

    static Factory getFactory() {
        return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(ArrayTypeArrowUInt64Type.class, true));
    }

    static ArrayTypeArrowUInt64Type create(
            @CXXReference @FFITypeAlias("const std::shared_ptr<arrow::ArrayData>") shared_ptr<ArrayData> data) {
        return ArrayTypeArrowUInt64Type.getFactory().create(data);
    }

    @FFIFactory
    @CXXHead(
            system = "arrow/type_traits.h"
    )
    interface Factory {
        ArrayTypeArrowUInt64Type create(
                @CXXReference @FFITypeAlias("const std::shared_ptr<arrow::ArrayData>") shared_ptr<ArrayData> data);
    }
}
