// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.arrow.impl.numericbuilder;

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
import io.v6d.arrow.impl.numericarray.IteratorTypeArrowUInt8Type;
import io.v6d.arrow.impl.numericarray.ValueTypeArrowUInt8Type;
import io.v6d.std.shared_ptr;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("arrow::NumericBuilder<arrow::UInt8Type>::ArrayType")
@FFIGen
@CXXHead(
        system = "arrow/array/builder_primitive.h"
)
public interface ArrayTypeArrowUInt8Type extends CXXPointer {
    @FFIExpr("{0}")
    io.v6d.arrow.impl.typetraits.ArrayTypeArrowUInt8Type get();

    ValueTypeArrowUInt8Type raw_values();

    char Value(long i);

    char GetView(long i);

    @CXXValue
    IteratorTypeArrowUInt8Type begin();

    @CXXValue
    IteratorTypeArrowUInt8Type end();

    static ArrayTypeArrowUInt8Type cast(final long __foreign_address) {
        try {
            Class<ArrayTypeArrowUInt8Type> clz = (Class<ArrayTypeArrowUInt8Type>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(ArrayTypeArrowUInt8Type.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static ArrayTypeArrowUInt8Type cast(final FFIPointer __foreign_pointer) {
        return ArrayTypeArrowUInt8Type.cast(__foreign_pointer.getAddress());
    }

    static Factory getFactory() {
        return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(ArrayTypeArrowUInt8Type.class, true));
    }

    static ArrayTypeArrowUInt8Type create(
            @CXXReference @FFITypeAlias("const std::shared_ptr<arrow::ArrayData>") shared_ptr<ArrayData> data) {
        return ArrayTypeArrowUInt8Type.getFactory().create(data);
    }

    @FFIFactory
    @CXXHead(
            system = "arrow/array/builder_primitive.h"
    )
    interface Factory {
        ArrayTypeArrowUInt8Type create(
                @CXXReference @FFITypeAlias("const std::shared_ptr<arrow::ArrayData>") shared_ptr<ArrayData> data);
    }
}
