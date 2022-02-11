// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.arrow.stl;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXPointer;
import com.alibaba.fastffi.CXXReference;
import com.alibaba.fastffi.FFIExpr;
import com.alibaba.fastffi.FFIFactory;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import io.v6d.std.random_access_iterator_tag;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("arrow::stl::ArrayIterator")
@FFIGen
@CXXHead(
        system = "arrow/stl_iterator.h"
)
public interface ArrayIterator<ArrayType> extends FFIPointer {
    long index();

    static <ArrayType> ArrayIterator<ArrayType> cast(Class<ArrayType> __arraytype,
            final long __foreign_address) {
        try {
            Class<ArrayIterator<ArrayType>> clz = (Class<ArrayIterator<ArrayType>>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(FFITypeFactory.makeParameterizedType(ArrayIterator.class, __arraytype), true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static <ArrayType> ArrayIterator<ArrayType> cast(Class<ArrayType> __arraytype,
            final FFIPointer __foreign_pointer) {
        return ArrayIterator.cast(__arraytype, __foreign_pointer.getAddress());
    }

    static <ArrayType> Factory<ArrayType> getFactory(Class<ArrayType> __arraytype) {
        return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(FFITypeFactory.makeParameterizedType(ArrayIterator.class, __arraytype), true));
    }

    static <ArrayType> ArrayIterator<ArrayType> create(Class<ArrayType> __arraytype) {
        return ArrayIterator.getFactory(__arraytype).create();
    }

    static <ArrayType> ArrayIterator<ArrayType> create(Class<ArrayType> __arraytype,
            @CXXReference ArrayType array, long index) {
        return ArrayIterator.getFactory(__arraytype).create(array, index);
    }

    @FFIFactory
    @CXXHead(
            system = "arrow/stl_iterator.h"
    )
    interface Factory<ArrayType> {
        ArrayIterator<ArrayType> create();

        ArrayIterator<ArrayType> create(@CXXReference ArrayType array, long index);
    }

    @FFITypeAlias("arrow::stl::ArrayIterator<%s>::iterator_category")
    @FFIGen
    @CXXHead(
            system = "arrow/stl_iterator.h"
    )
    interface iterator_category<ArrayType> extends CXXPointer {
        @FFIExpr("{0}")
        random_access_iterator_tag get();

        static <ArrayType> iterator_category<ArrayType> cast(Class<ArrayType> __arraytype,
                final long __foreign_address) {
            try {
                Class<iterator_category<ArrayType>> clz = (Class<iterator_category<ArrayType>>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(FFITypeFactory.makeParameterizedType(iterator_category.class, __arraytype), true));
                return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
            } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
                return null;
            }
        }

        static <ArrayType> iterator_category<ArrayType> cast(Class<ArrayType> __arraytype,
                final FFIPointer __foreign_pointer) {
            return iterator_category.cast(__arraytype, __foreign_pointer.getAddress());
        }
    }

    @FFITypeAlias("arrow::stl::ArrayIterator<%s>::value_type")
    @FFIGen
    @CXXHead(
            system = "arrow/stl_iterator.h"
    )
    interface value_type<ArrayType> extends CXXPointer {
        static <ArrayType> value_type<ArrayType> cast(Class<ArrayType> __arraytype,
                final long __foreign_address) {
            try {
                Class<value_type<ArrayType>> clz = (Class<value_type<ArrayType>>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(FFITypeFactory.makeParameterizedType(value_type.class, __arraytype), true));
                return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
            } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
                return null;
            }
        }

        static <ArrayType> value_type<ArrayType> cast(Class<ArrayType> __arraytype,
                final FFIPointer __foreign_pointer) {
            return value_type.cast(__arraytype, __foreign_pointer.getAddress());
        }
    }

    @FFITypeAlias("arrow::stl::ArrayIterator<%s>::reference")
    @FFIGen
    @CXXHead(
            system = "arrow/stl_iterator.h"
    )
    interface reference<ArrayType> extends CXXPointer {
        @FFIExpr("(*{0})")
        value_type<ArrayType> get();

        @FFIExpr("*{0} = (arrow::stl::ArrayIterator<%s>::reference){1}")
        void set(value_type<ArrayType> __value);

        static <ArrayType> reference<ArrayType> cast(Class<ArrayType> __arraytype,
                final long __foreign_address) {
            try {
                Class<reference<ArrayType>> clz = (Class<reference<ArrayType>>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(FFITypeFactory.makeParameterizedType(reference.class, __arraytype), true));
                return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
            } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
                return null;
            }
        }

        static <ArrayType> reference<ArrayType> cast(Class<ArrayType> __arraytype,
                final FFIPointer __foreign_pointer) {
            return reference.cast(__arraytype, __foreign_pointer.getAddress());
        }
    }

    @FFITypeAlias("arrow::stl::ArrayIterator<%s>::difference_type")
    @FFIGen
    @CXXHead(
            system = "arrow/stl_iterator.h"
    )
    interface difference_type<ArrayType> extends CXXPointer {
        @FFIExpr("(*{0})")
        long get();

        @FFIExpr("*{0} = (arrow::stl::ArrayIterator<%s>::difference_type){1}")
        void set(long __value);

        static <ArrayType> difference_type<ArrayType> cast(Class<ArrayType> __arraytype,
                final long __foreign_address) {
            try {
                Class<difference_type<ArrayType>> clz = (Class<difference_type<ArrayType>>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(FFITypeFactory.makeParameterizedType(difference_type.class, __arraytype), true));
                return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
            } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
                return null;
            }
        }

        static <ArrayType> difference_type<ArrayType> cast(Class<ArrayType> __arraytype,
                final FFIPointer __foreign_pointer) {
            return difference_type.cast(__arraytype, __foreign_pointer.getAddress());
        }

        static <ArrayType> Factory<ArrayType> getFactory(Class<ArrayType> __arraytype) {
            return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(FFITypeFactory.makeParameterizedType(difference_type.class, __arraytype), true));
        }

        static <ArrayType> difference_type<ArrayType> create(Class<ArrayType> __arraytype) {
            return difference_type.getFactory(__arraytype).create();
        }

        static <ArrayType> difference_type<ArrayType> create(Class<ArrayType> __arraytype,
                long __value) {
            return difference_type.getFactory(__arraytype).create(__value);
        }

        @FFIFactory
        @CXXHead(
                system = "arrow/stl_iterator.h"
        )
        interface Factory<ArrayType> {
            difference_type<ArrayType> create();

            difference_type<ArrayType> create(long __value);
        }
    }

    @FFITypeAlias("arrow::stl::ArrayIterator<%s>::pointer")
    @FFIGen
    @CXXHead(
            system = "arrow/stl_iterator.h"
    )
    interface pointer<ArrayType> extends CXXPointer {
        @FFIExpr("(*{0})")
        value_type<ArrayType> get();

        @FFIExpr("*{0} = (arrow::stl::ArrayIterator<%s>::pointer){1}")
        void set(value_type<ArrayType> __value);

        static <ArrayType> pointer<ArrayType> cast(Class<ArrayType> __arraytype,
                final long __foreign_address) {
            try {
                Class<pointer<ArrayType>> clz = (Class<pointer<ArrayType>>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(FFITypeFactory.makeParameterizedType(pointer.class, __arraytype), true));
                return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
            } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
                return null;
            }
        }

        static <ArrayType> pointer<ArrayType> cast(Class<ArrayType> __arraytype,
                final FFIPointer __foreign_pointer) {
            return pointer.cast(__arraytype, __foreign_pointer.getAddress());
        }
    }
}
