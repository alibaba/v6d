// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.arrow;

import com.alibaba.fastffi.CXXEnum;
import com.alibaba.fastffi.CXXEnumMap;
import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFIGetter;
import com.alibaba.fastffi.FFILibrary;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import com.alibaba.fastffi.FFITypeRefiner;
import io.v6d.std.CInt;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("arrow::IntervalType")
@FFIGen
@CXXHead(
        system = "arrow/type.h"
)
public interface IntervalType extends TemporalType, ParametricType, FFIPointer {
    static IntervalType cast(final long __foreign_address) {
        try {
            Class<IntervalType> clz = (Class<IntervalType>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(IntervalType.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static IntervalType cast(final FFIPointer __foreign_pointer) {
        return IntervalType.cast(__foreign_pointer.getAddress());
    }

    @FFITypeAlias("arrow::IntervalType::type")
    @FFITypeRefiner("io.v6d.arrow.IntervalType.type.get")
    @CXXHead(
            system = "arrow/type.h"
    )
    enum type implements CXXEnum {
        MONTHS(Library.INSTANCE.MONTHS()),

        DAY_TIME(Library.INSTANCE.DAY_TIME()),

        MONTH_DAY_NANO(Library.INSTANCE.MONTH_DAY_NANO());

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
                value = "arrow::IntervalType::type",
                namespace = "arrow::IntervalType::type"
        )
        @CXXHead(
                system = "arrow/type.h"
        )
        public interface Library {
            Library INSTANCE = FFITypeFactory.getLibrary(Library.class);

            @FFIGetter
            int MONTHS();

            @FFIGetter
            int DAY_TIME();

            @FFIGetter
            int MONTH_DAY_NANO();
        }
    }
}
