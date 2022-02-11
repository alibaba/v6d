// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.arrow;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXPointer;
import com.alibaba.fastffi.CXXValue;
import com.alibaba.fastffi.FFIExpr;
import com.alibaba.fastffi.FFIFactory;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFIGetter;
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

@FFITypeAlias("arrow::MonthDayNanoIntervalType")
@FFIGen
@CXXHead(
        system = "arrow/type.h"
)
public interface MonthDayNanoIntervalType extends IntervalType, FFIPointer {
    @CXXValue
    @FFITypeAlias("arrow::IntervalType::type")
    IntervalType.type interval_type();

    int bit_width();

    @CXXValue
    string ToString();

    @CXXValue
    string name();

    static MonthDayNanoIntervalType cast(final long __foreign_address) {
        try {
            Class<MonthDayNanoIntervalType> clz = (Class<MonthDayNanoIntervalType>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(MonthDayNanoIntervalType.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static MonthDayNanoIntervalType cast(final FFIPointer __foreign_pointer) {
        return MonthDayNanoIntervalType.cast(__foreign_pointer.getAddress());
    }

    static Factory getFactory() {
        return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(MonthDayNanoIntervalType.class, true));
    }

    static MonthDayNanoIntervalType create() {
        return MonthDayNanoIntervalType.getFactory().create();
    }

    @FFIFactory
    @CXXHead(
            system = "arrow/type.h"
    )
    interface Factory {
        MonthDayNanoIntervalType create();
    }

    @FFIGen
    @FFILibrary(
            value = "arrow::MonthDayNanoIntervalType",
            namespace = "arrow::MonthDayNanoIntervalType"
    )
    @CXXHead(
            system = "arrow/type.h"
    )
    interface Library {
        Library INSTANCE = FFITypeFactory.getLibrary(Library.class);

        CChar type_name();
    }

    @FFITypeAlias("arrow::MonthDayNanoIntervalType::PhysicalType")
    @FFIGen
    @CXXHead(
            system = "arrow/type.h"
    )
    interface PhysicalType extends CXXPointer {
        @FFIExpr("{0}")
        MonthDayNanoIntervalType get();

        @CXXValue
        @FFITypeAlias("arrow::IntervalType::type")
        IntervalType.type interval_type();

        int bit_width();

        @CXXValue
        string ToString();

        @CXXValue
        string name();

        static PhysicalType cast(final long __foreign_address) {
            try {
                Class<PhysicalType> clz = (Class<PhysicalType>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(PhysicalType.class, true));
                return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
            } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
                return null;
            }
        }

        static PhysicalType cast(final FFIPointer __foreign_pointer) {
            return PhysicalType.cast(__foreign_pointer.getAddress());
        }

        static Factory getFactory() {
            return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(PhysicalType.class, true));
        }

        static PhysicalType create() {
            return PhysicalType.getFactory().create();
        }

        @FFIFactory
        @CXXHead(
                system = "arrow/type.h"
        )
        interface Factory {
            PhysicalType create();
        }

        @FFIGen
        @FFILibrary(
                value = "arrow::MonthDayNanoIntervalType",
                namespace = "arrow::MonthDayNanoIntervalType"
        )
        @CXXHead(
                system = "arrow/type.h"
        )
        interface Library {
            Library INSTANCE = FFITypeFactory.getLibrary(Library.class);

            CChar type_name();
        }
    }

    @FFITypeAlias("arrow::MonthDayNanoIntervalType::MonthDayNanos")
    @FFIGen
    @CXXHead(
            system = "arrow/type.h"
    )
    interface MonthDayNanos extends FFIPointer {
        @FFIGetter
        int months();

        @FFIGetter
        int days();

        @FFIGetter
        long nanoseconds();

        static MonthDayNanos cast(final long __foreign_address) {
            try {
                Class<MonthDayNanos> clz = (Class<MonthDayNanos>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(MonthDayNanos.class, true));
                return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
            } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
                return null;
            }
        }

        static MonthDayNanos cast(final FFIPointer __foreign_pointer) {
            return MonthDayNanos.cast(__foreign_pointer.getAddress());
        }
    }

    @FFITypeAlias("arrow::MonthDayNanoIntervalType::c_type")
    @FFIGen
    @CXXHead(
            system = "arrow/type.h"
    )
    interface c_type extends CXXPointer {
        @FFIExpr("{0}")
        MonthDayNanos get();

        @FFIGetter
        int months();

        @FFIGetter
        int days();

        @FFIGetter
        long nanoseconds();

        static c_type cast(final long __foreign_address) {
            try {
                Class<c_type> clz = (Class<c_type>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(c_type.class, true));
                return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
            } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
                return null;
            }
        }

        static c_type cast(final FFIPointer __foreign_pointer) {
            return c_type.cast(__foreign_pointer.getAddress());
        }
    }
}
