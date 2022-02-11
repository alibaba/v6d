// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.arrow;

import com.alibaba.fastffi.CXXEnum;
import com.alibaba.fastffi.CXXEnumMap;
import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXReference;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFIGetter;
import com.alibaba.fastffi.FFILibrary;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import com.alibaba.fastffi.FFITypeRefiner;
import io.v6d.std.CInt;
import io.v6d.std.vector;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("arrow::TimeUnit")
@FFIGen
@CXXHead(
        system = "arrow/type_fwd.h"
)
public interface TimeUnit extends FFIPointer {
    static TimeUnit cast(final long __foreign_address) {
        try {
            Class<TimeUnit> clz = (Class<TimeUnit>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(TimeUnit.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static TimeUnit cast(final FFIPointer __foreign_pointer) {
        return TimeUnit.cast(__foreign_pointer.getAddress());
    }

    @FFIGen
    @FFILibrary(
            value = "arrow::TimeUnit",
            namespace = "arrow::TimeUnit"
    )
    @CXXHead(
            system = "arrow/type_fwd.h"
    )
    interface Library {
        Library INSTANCE = FFITypeFactory.getLibrary(Library.class);

        @CXXReference
        @FFITypeAlias("const std::vector<arrow::TimeUnit::type>")
        vector<type> values();
    }

    @FFITypeAlias("arrow::TimeUnit::type")
    @FFITypeRefiner("io.v6d.arrow.TimeUnit.type.get")
    @CXXHead(
            system = "arrow/type_fwd.h"
    )
    enum type implements CXXEnum {
        SECOND(Library.INSTANCE.SECOND()),

        MILLI(Library.INSTANCE.MILLI()),

        MICRO(Library.INSTANCE.MICRO()),

        NANO(Library.INSTANCE.NANO());

        private static final CXXEnumMap<type> $map = new CXXEnumMap<>(values());

        int $value;

        type(int value) {
            $value = value;
        }

        type(CInt value) {
            $value = value.get();
        }

        public static type get(int value) {
            return $map.get(value);
        }

        public static type get(CInt value) {
            return $map.get(value.get());
        }

        public int getValue() {
            return $value;
        }

        public static type cast(final long __foreign_address) {
            try {
                Class<type> clz = (Class<type>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(type.class, true));
                return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
            } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
                return null;
            }
        }

        public static type cast(final FFIPointer __foreign_pointer) {
            return type.cast(__foreign_pointer.getAddress());
        }

        @FFIGen
        @FFILibrary(
                value = "arrow::TimeUnit::type",
                namespace = "arrow::TimeUnit::type"
        )
        @CXXHead(
                system = "arrow/type_fwd.h"
        )
        public interface Library {
            Library INSTANCE = FFITypeFactory.getLibrary(Library.class);

            @FFIGetter
            int SECOND();

            @FFIGetter
            int MILLI();

            @FFIGetter
            int MICRO();

            @FFIGetter
            int NANO();
        }
    }
}
