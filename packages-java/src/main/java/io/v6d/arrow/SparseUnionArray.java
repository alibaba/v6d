// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.arrow;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXPointer;
import com.alibaba.fastffi.CXXReference;
import com.alibaba.fastffi.CXXValue;
import com.alibaba.fastffi.FFIExpr;
import com.alibaba.fastffi.FFIFactory;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFILibrary;
import com.alibaba.fastffi.FFINameAlias;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import io.v6d.std.CChar;
import io.v6d.std.CSignedChar;
import io.v6d.std.shared_ptr;
import io.v6d.std.string;
import io.v6d.std.vector;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("arrow::SparseUnionArray")
@FFIGen
@CXXHead(
        system = "arrow/array/array_nested.h"
)
public interface SparseUnionArray extends UnionArray, FFIPointer {
    @FFINameAlias("union_type")
    SparseUnionType union_type_1();

    static SparseUnionArray cast(final long __foreign_address) {
        try {
            Class<SparseUnionArray> clz = (Class<SparseUnionArray>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(SparseUnionArray.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static SparseUnionArray cast(final FFIPointer __foreign_pointer) {
        return SparseUnionArray.cast(__foreign_pointer.getAddress());
    }

    static Factory getFactory() {
        return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(SparseUnionArray.class, true));
    }

    static SparseUnionArray create(
            @CXXValue @FFITypeAlias("std::shared_ptr<arrow::ArrayData>") shared_ptr<ArrayData> data) {
        return SparseUnionArray.getFactory().create(data);
    }

    static SparseUnionArray create(
            @CXXValue @FFITypeAlias("std::shared_ptr<arrow::DataType>") shared_ptr<DataType> type,
            long length, @CXXValue ArrayVector children,
            @CXXValue @FFITypeAlias("std::shared_ptr<arrow::Buffer>") shared_ptr<Buffer> type_ids,
            long offset) {
        return SparseUnionArray.getFactory().create(type, length, children, type_ids, offset);
    }

    @FFIFactory
    @CXXHead(
            system = "arrow/array/array_nested.h"
    )
    interface Factory {
        SparseUnionArray create(
                @CXXValue @FFITypeAlias("std::shared_ptr<arrow::ArrayData>") shared_ptr<ArrayData> data);

        SparseUnionArray create(
                @CXXValue @FFITypeAlias("std::shared_ptr<arrow::DataType>") shared_ptr<DataType> type,
                long length, @CXXValue ArrayVector children,
                @CXXValue @FFITypeAlias("std::shared_ptr<arrow::Buffer>") shared_ptr<Buffer> type_ids,
                long offset);
    }

    @FFIGen
    @FFILibrary(
            value = "arrow::SparseUnionArray",
            namespace = "arrow::SparseUnionArray"
    )
    @CXXHead(
            system = "arrow/array/array_nested.h"
    )
    interface Library {
        Library INSTANCE = FFITypeFactory.getLibrary(Library.class);

        @CXXValue
        @FFITypeAlias("arrow::Result<std::shared_ptr<arrow::Array>>")
        Result<shared_ptr<Array>> Make(@CXXReference Array type_ids, @CXXValue ArrayVector children,
                @CXXValue @FFITypeAlias("std::vector<signed char>") vector<CSignedChar> type_codes);

        @CXXValue
        @FFITypeAlias("arrow::Result<std::shared_ptr<arrow::Array>>")
        Result<shared_ptr<Array>> Make(@CXXReference Array type_ids, @CXXValue ArrayVector children,
                @CXXValue @FFITypeAlias("std::vector<std::string>") vector<string> field_names,
                @CXXValue @FFITypeAlias("std::vector<signed char>") vector<CSignedChar> type_codes);
    }

    @FFITypeAlias("arrow::SparseUnionArray::TypeClass")
    @FFIGen
    @CXXHead(
            system = "arrow/array/array_nested.h"
    )
    interface TypeClass extends CXXPointer {
        @FFIExpr("{0}")
        SparseUnionType get();

        @CXXValue
        string name();

        static TypeClass cast(final long __foreign_address) {
            try {
                Class<TypeClass> clz = (Class<TypeClass>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(TypeClass.class, true));
                return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
            } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
                return null;
            }
        }

        static TypeClass cast(final FFIPointer __foreign_pointer) {
            return TypeClass.cast(__foreign_pointer.getAddress());
        }

        static Factory getFactory() {
            return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(TypeClass.class, true));
        }

        static TypeClass create(
                @CXXValue @FFITypeAlias("std::vector<std::shared_ptr<arrow::Field>>") vector<shared_ptr<Field>> fields,
                @CXXValue @FFITypeAlias("std::vector<signed char>") vector<CSignedChar> type_codes) {
            return TypeClass.getFactory().create(fields, type_codes);
        }

        @FFIFactory
        @CXXHead(
                system = "arrow/array/array_nested.h"
        )
        interface Factory {
            TypeClass create(
                    @CXXValue @FFITypeAlias("std::vector<std::shared_ptr<arrow::Field>>") vector<shared_ptr<Field>> fields,
                    @CXXValue @FFITypeAlias("std::vector<signed char>") vector<CSignedChar> type_codes);
        }

        @FFIGen
        @FFILibrary(
                value = "arrow::SparseUnionType",
                namespace = "arrow::SparseUnionType"
        )
        @CXXHead(
                system = "arrow/array/array_nested.h"
        )
        interface Library {
            Library INSTANCE = FFITypeFactory.getLibrary(Library.class);

            CChar type_name();

            @CXXValue
            @FFITypeAlias("arrow::Result<std::shared_ptr<arrow::DataType>>")
            Result<shared_ptr<DataType>> Make(
                    @CXXValue @FFITypeAlias("std::vector<std::shared_ptr<arrow::Field>>") vector<shared_ptr<Field>> fields,
                    @CXXValue @FFITypeAlias("std::vector<signed char>") vector<CSignedChar> type_codes);
        }
    }
}
