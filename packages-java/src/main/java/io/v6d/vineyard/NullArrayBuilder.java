// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.vineyard;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXReference;
import com.alibaba.fastffi.CXXValue;
import com.alibaba.fastffi.FFIFactory;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFINameAlias;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import io.v6d.arrow.NullArray;
import io.v6d.std.shared_ptr;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("vineyard::NullArrayBuilder")
@FFIGen
@CXXHead("basic/ds/arrow.h")
public interface NullArrayBuilder extends NullArrayBaseBuilder, FFIPointer {
    @CXXValue
    @FFITypeAlias("std::shared_ptr<arrow::NullArray>")
    shared_ptr<NullArray> GetArray();

    @CXXValue
    @FFINameAlias("Build")
    Status Build_1(@CXXReference Client client);

    static NullArrayBuilder cast(final long __foreign_address) {
        try {
            Class<NullArrayBuilder> clz = (Class<NullArrayBuilder>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(NullArrayBuilder.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static NullArrayBuilder cast(final FFIPointer __foreign_pointer) {
        return NullArrayBuilder.cast(__foreign_pointer.getAddress());
    }

    static Factory getFactory() {
        return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(NullArrayBuilder.class, true));
    }

    static NullArrayBuilder create(@CXXReference Client client,
            @CXXValue @FFITypeAlias("std::shared_ptr<arrow::NullArray>") shared_ptr<NullArray> array) {
        return NullArrayBuilder.getFactory().create(client, array);
    }

    @FFIFactory
    @CXXHead("basic/ds/arrow.h")
    interface Factory {
        NullArrayBuilder create(@CXXReference Client client,
                @CXXValue @FFITypeAlias("std::shared_ptr<arrow::NullArray>") shared_ptr<NullArray> array);
    }
}
