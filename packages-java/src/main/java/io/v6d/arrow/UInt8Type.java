// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.arrow;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFILibrary;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import io.v6d.std.CChar;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("arrow::UInt8Type")
@FFIGen
@CXXHead(
        system = "arrow/type.h"
)
public interface UInt8Type extends FFIPointer {
    static UInt8Type cast(final long __foreign_address) {
        try {
            Class<UInt8Type> clz = (Class<UInt8Type>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(UInt8Type.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static UInt8Type cast(final FFIPointer __foreign_pointer) {
        return UInt8Type.cast(__foreign_pointer.getAddress());
    }

    @FFIGen
    @FFILibrary(
            value = "arrow::UInt8Type",
            namespace = "arrow::UInt8Type"
    )
    @CXXHead(
            system = "arrow/type.h"
    )
    interface Library {
        Library INSTANCE = FFITypeFactory.getLibrary(Library.class);

        CChar type_name();
    }
}
