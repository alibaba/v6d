// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.arrow.impl.numericarray;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXPointer;
import com.alibaba.fastffi.FFIExpr;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import io.v6d.arrow.Int16Type;
import io.v6d.arrow.NumericArray;
import io.v6d.arrow.stl.ArrayIterator;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("arrow::NumericArray<arrow::Int16Type>::IteratorType")
@FFIGen
@CXXHead(
        system = "arrow/array/array_primitive.h"
)
public interface IteratorTypeArrowInt16Type extends CXXPointer {
    @FFIExpr("{0}")
    ArrayIterator<NumericArray<Int16Type>> get();

    static IteratorTypeArrowInt16Type cast(final long __foreign_address) {
        try {
            Class<IteratorTypeArrowInt16Type> clz = (Class<IteratorTypeArrowInt16Type>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(IteratorTypeArrowInt16Type.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static IteratorTypeArrowInt16Type cast(final FFIPointer __foreign_pointer) {
        return IteratorTypeArrowInt16Type.cast(__foreign_pointer.getAddress());
    }
}
