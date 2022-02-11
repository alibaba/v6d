// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.std.impl.vector;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXPointer;
import com.alibaba.fastffi.FFIExpr;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import io.v6d.std.__wrap_iter;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("std::vector<std::shared_ptr<arrow::Field>, std::allocator<std::shared_ptr<arrow::Field>>>::const_iterator")
@FFIGen
@CXXHead(
        system = "vector"
)
public interface ConstIteratorStdSharedPtrArrowFieldStdAllocatorStdSharedPtrArrowField extends CXXPointer {
    @FFIExpr("{0}")
    __wrap_iter<ConstPointerStdSharedPtrArrowFieldStdAllocatorStdSharedPtrArrowField> get();

    static ConstIteratorStdSharedPtrArrowFieldStdAllocatorStdSharedPtrArrowField cast(
            final long __foreign_address) {
        try {
            Class<ConstIteratorStdSharedPtrArrowFieldStdAllocatorStdSharedPtrArrowField> clz = (Class<ConstIteratorStdSharedPtrArrowFieldStdAllocatorStdSharedPtrArrowField>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(ConstIteratorStdSharedPtrArrowFieldStdAllocatorStdSharedPtrArrowField.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static ConstIteratorStdSharedPtrArrowFieldStdAllocatorStdSharedPtrArrowField cast(
            final FFIPointer __foreign_pointer) {
        return ConstIteratorStdSharedPtrArrowFieldStdAllocatorStdSharedPtrArrowField.cast(__foreign_pointer.getAddress());
    }
}
