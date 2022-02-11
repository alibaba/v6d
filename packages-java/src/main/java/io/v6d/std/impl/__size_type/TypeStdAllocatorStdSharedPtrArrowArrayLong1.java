// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.std.impl.__size_type;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXPointer;
import com.alibaba.fastffi.FFIExpr;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import io.v6d.std.impl.allocator.SizeTypeStdSharedPtrArrowArray;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("std::__size_type<std::allocator<std::shared_ptr<arrow::Array>>, long, 1>::type")
@FFIGen
@CXXHead(
        system = "__memory/allocator_traits.h"
)
public interface TypeStdAllocatorStdSharedPtrArrowArrayLong1 extends CXXPointer {
    @FFIExpr("{0}")
    SizeTypeStdSharedPtrArrowArray get();

    static TypeStdAllocatorStdSharedPtrArrowArrayLong1 cast(final long __foreign_address) {
        try {
            Class<TypeStdAllocatorStdSharedPtrArrowArrayLong1> clz = (Class<TypeStdAllocatorStdSharedPtrArrowArrayLong1>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(TypeStdAllocatorStdSharedPtrArrowArrayLong1.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static TypeStdAllocatorStdSharedPtrArrowArrayLong1 cast(final FFIPointer __foreign_pointer) {
        return TypeStdAllocatorStdSharedPtrArrowArrayLong1.cast(__foreign_pointer.getAddress());
    }
}
