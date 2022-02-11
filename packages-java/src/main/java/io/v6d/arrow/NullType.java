// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.arrow;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXValue;
import com.alibaba.fastffi.FFIFactory;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFILibrary;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import io.v6d.std.CChar;
import io.v6d.std.string;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("arrow::NullType")
@FFIGen
@CXXHead(
        system = "arrow/type.h"
)
public interface NullType extends DataType, FFIPointer {
    @CXXValue
    string ToString();

    @CXXValue
    DataTypeLayout layout();

    @CXXValue
    string name();

    static NullType cast(final long __foreign_address) {
        try {
            Class<NullType> clz = (Class<NullType>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(NullType.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static NullType cast(final FFIPointer __foreign_pointer) {
        return NullType.cast(__foreign_pointer.getAddress());
    }

    static Factory getFactory() {
        return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(NullType.class, true));
    }

    static NullType create() {
        return NullType.getFactory().create();
    }

    @FFIFactory
    @CXXHead(
            system = "arrow/type.h"
    )
    interface Factory {
        NullType create();
    }

    @FFIGen
    @FFILibrary(
            value = "arrow::NullType",
            namespace = "arrow::NullType"
    )
    @CXXHead(
            system = "arrow/type.h"
    )
    interface Library {
        Library INSTANCE = FFITypeFactory.getLibrary(Library.class);

        CChar type_name();
    }
}
