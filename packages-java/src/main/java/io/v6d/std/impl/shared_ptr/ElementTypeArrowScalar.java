// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.std.impl.shared_ptr;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXPointer;
import com.alibaba.fastffi.CXXReference;
import com.alibaba.fastffi.CXXValue;
import com.alibaba.fastffi.FFIExpr;
import com.alibaba.fastffi.FFIFactory;
import com.alibaba.fastffi.FFIGen;
import com.alibaba.fastffi.FFIGetter;
import com.alibaba.fastffi.FFILibrary;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import io.v6d.arrow.DataType;
import io.v6d.arrow.EqualOptions;
import io.v6d.arrow.Result;
import io.v6d.arrow.Scalar;
import io.v6d.arrow.Status;
import io.v6d.std.shared_ptr;
import io.v6d.std.string;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("std::shared_ptr<arrow::Scalar>::element_type")
@FFIGen
@CXXHead(
        system = "__memory/shared_ptr.h"
)
public interface ElementTypeArrowScalar extends CXXPointer {
    @FFIExpr("{0}")
    Scalar get();

    @FFIGetter
    @CXXReference
    @FFITypeAlias("std::shared_ptr<arrow::DataType>")
    shared_ptr<DataType> type();

    @FFIGetter
    boolean is_valid();

    boolean Equals(@CXXReference Scalar other, @CXXReference EqualOptions options);

    boolean ApproxEquals(@CXXReference Scalar other, @CXXReference EqualOptions options);

    long hash();

    @CXXValue
    string ToString();

    @CXXValue
    Status Validate();

    @CXXValue
    Status ValidateFull();

    @CXXValue
    @FFITypeAlias("arrow::Result<std::shared_ptr<arrow::Scalar>>")
    Result<shared_ptr<Scalar>> CastTo(
            @CXXValue @FFITypeAlias("std::shared_ptr<arrow::DataType>") shared_ptr<DataType> to);

    static ElementTypeArrowScalar cast(final long __foreign_address) {
        try {
            Class<ElementTypeArrowScalar> clz = (Class<ElementTypeArrowScalar>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(ElementTypeArrowScalar.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static ElementTypeArrowScalar cast(final FFIPointer __foreign_pointer) {
        return ElementTypeArrowScalar.cast(__foreign_pointer.getAddress());
    }

    static Factory getFactory() {
        return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(ElementTypeArrowScalar.class, true));
    }

    static ElementTypeArrowScalar create(
            @CXXValue @FFITypeAlias("std::shared_ptr<arrow::DataType>") shared_ptr<DataType> type) {
        return ElementTypeArrowScalar.getFactory().create(type);
    }

    @FFIFactory
    @CXXHead(
            system = "__memory/shared_ptr.h"
    )
    interface Factory {
        ElementTypeArrowScalar create(
                @CXXValue @FFITypeAlias("std::shared_ptr<arrow::DataType>") shared_ptr<DataType> type);
    }

    @FFIGen
    @FFILibrary(
            value = "arrow::Scalar",
            namespace = "arrow::Scalar"
    )
    @CXXHead(
            system = "__memory/shared_ptr.h"
    )
    interface Library {
        Library INSTANCE = FFITypeFactory.getLibrary(Library.class);
    }
}
