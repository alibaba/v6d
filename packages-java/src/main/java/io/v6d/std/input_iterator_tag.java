// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.std;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("std::input_iterator_tag")
@FFIGen
@CXXHead(
        system = "__iterator/iterator_traits.h"
)
public interface input_iterator_tag extends FFIPointer {
    static input_iterator_tag cast(final long __foreign_address) {
        try {
            Class<input_iterator_tag> clz = (Class<input_iterator_tag>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(input_iterator_tag.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static input_iterator_tag cast(final FFIPointer __foreign_pointer) {
        return input_iterator_tag.cast(__foreign_pointer.getAddress());
    }
}
