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
import io.v6d.std.reverse_iterator;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("std::vector<std::shared_ptr<arrow::Field>, std::allocator<std::shared_ptr<arrow::Field>>>::reverse_iterator")
@FFIGen
@CXXHead(
        system = "vector"
)
public interface ReverseIteratorStdSharedPtrArrowFieldStdAllocatorStdSharedPtrArrowField extends CXXPointer {
    @FFIExpr("{0}")
    reverse_iterator<IteratorStdSharedPtrArrowFieldStdAllocatorStdSharedPtrArrowField> get();

    static ReverseIteratorStdSharedPtrArrowFieldStdAllocatorStdSharedPtrArrowField cast(
            final long __foreign_address) {
        try {
            Class<ReverseIteratorStdSharedPtrArrowFieldStdAllocatorStdSharedPtrArrowField> clz = (Class<ReverseIteratorStdSharedPtrArrowFieldStdAllocatorStdSharedPtrArrowField>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(ReverseIteratorStdSharedPtrArrowFieldStdAllocatorStdSharedPtrArrowField.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static ReverseIteratorStdSharedPtrArrowFieldStdAllocatorStdSharedPtrArrowField cast(
            final FFIPointer __foreign_pointer) {
        return ReverseIteratorStdSharedPtrArrowFieldStdAllocatorStdSharedPtrArrowField.cast(__foreign_pointer.getAddress());
    }
}
