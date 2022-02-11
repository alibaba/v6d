// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.vineyard;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXPointer;
import com.alibaba.fastffi.CXXReference;
import com.alibaba.fastffi.CXXValue;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import io.v6d.std.enable_shared_from_this;
import io.v6d.std.shared_ptr;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("vineyard::Object")
@FFIGen
@CXXHead("client/ds/i_object.h")
public interface Object extends ObjectBase, enable_shared_from_this<Object>, CXXPointer {
    long id();

    @CXXReference
    ObjectMeta meta();

    long nbytes();

    void Construct(@CXXReference ObjectMeta meta);

    void PostConstruct(@CXXReference ObjectMeta meta);

    @CXXValue
    Status Build(@CXXReference Client client);

    @CXXValue
    @FFITypeAlias("std::shared_ptr<vineyard::Object>")
    shared_ptr<Object> _Seal(@CXXReference Client client);

    @CXXValue
    Status Persist(@CXXReference ClientBase client);

    boolean IsLocal();

    boolean IsPersist();

    boolean IsGlobal();

    static Object cast(final long __foreign_address) {
        try {
            Class<Object> clz = (Class<Object>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(Object.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static Object cast(final FFIPointer __foreign_pointer) {
        return Object.cast(__foreign_pointer.getAddress());
    }
}
