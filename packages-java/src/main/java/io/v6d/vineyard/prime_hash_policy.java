// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.vineyard;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXReference;
import com.alibaba.fastffi.FFIFactory;
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

@FFITypeAlias("vineyard::prime_hash_policy")
@FFIGen
@CXXHead("basic/ds/hashmap.vineyard.h")
public interface prime_hash_policy extends FFIPointer {
    long index_for_hash(long hash);

    void set_prime(long prime);

    static prime_hash_policy cast(final long __foreign_address) {
        try {
            Class<prime_hash_policy> clz = (Class<prime_hash_policy>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(prime_hash_policy.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static prime_hash_policy cast(final FFIPointer __foreign_pointer) {
        return prime_hash_policy.cast(__foreign_pointer.getAddress());
    }

    static Factory getFactory() {
        return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(prime_hash_policy.class, true));
    }

    static prime_hash_policy create() {
        return prime_hash_policy.getFactory().create();
    }

    static prime_hash_policy create(@CXXReference prime_hash_policy rhs) {
        return prime_hash_policy.getFactory().create(rhs);
    }

    @FFIFactory
    @CXXHead("basic/ds/hashmap.vineyard.h")
    interface Factory {
        prime_hash_policy create();

        prime_hash_policy create(@CXXReference prime_hash_policy rhs);
    }
}
