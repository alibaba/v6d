// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.std.impl.allocator;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXPointer;
import com.alibaba.fastffi.FFIExpr;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import io.v6d.arrow.Scalar;
import io.v6d.std.shared_ptr;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("std::allocator<std::shared_ptr<arrow::Scalar>>::pointer")
@FFIGen
@CXXHead(
        system = "__memory/allocator.h"
)
public interface PointerStdSharedPtrArrowScalar extends CXXPointer {
    @FFIExpr("(*{0})")
    shared_ptr<Scalar> get();

    @FFIExpr("*{0} = (std::allocator<std::shared_ptr<arrow::Scalar>>::pointer){1}")
    void set(shared_ptr<Scalar> __value);

    static PointerStdSharedPtrArrowScalar cast(final long __foreign_address) {
        try {
            Class<PointerStdSharedPtrArrowScalar> clz = (Class<PointerStdSharedPtrArrowScalar>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(PointerStdSharedPtrArrowScalar.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static PointerStdSharedPtrArrowScalar cast(final FFIPointer __foreign_pointer) {
        return PointerStdSharedPtrArrowScalar.cast(__foreign_pointer.getAddress());
    }
}
