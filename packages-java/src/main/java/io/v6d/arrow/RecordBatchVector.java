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
import com.alibaba.fastffi.FFINameAlias;
import com.alibaba.fastffi.FFIPointer;
import com.alibaba.fastffi.FFITypeAlias;
import com.alibaba.fastffi.FFITypeFactory;
import io.v6d.std.impl.vector.AllocatorTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch;
import io.v6d.std.impl.vector.ConstIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch;
import io.v6d.std.impl.vector.ConstReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch;
import io.v6d.std.impl.vector.ConstReverseIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch;
import io.v6d.std.impl.vector.IteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch;
import io.v6d.std.impl.vector.ReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch;
import io.v6d.std.impl.vector.ReverseIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch;
import io.v6d.std.impl.vector.ValueTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch;
import io.v6d.std.initializer_list;
import io.v6d.std.shared_ptr;
import io.v6d.std.vector;
import java.lang.Class;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.Long;
import java.lang.NoSuchMethodException;
import java.lang.reflect.InvocationTargetException;

@FFITypeAlias("arrow::RecordBatchVector")
@FFIGen
@CXXHead(
        system = "arrow/type_fwd.h"
)
public interface RecordBatchVector extends CXXPointer {
    @FFIExpr("{0}")
    vector<shared_ptr<RecordBatch>> get();

    void assign(long __n,
            @CXXValue ConstReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __u);

    void assign(
            @CXXValue @FFITypeAlias("std::initializer_list<std::vector<std::shared_ptr<arrow::RecordBatch>, std::allocator<std::shared_ptr<arrow::RecordBatch>>>::value_type>") initializer_list<ValueTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch> __il);

    @CXXValue
    AllocatorTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch get_allocator(
            );

    @CXXValue
    IteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch begin();

    @CXXValue
    @FFINameAlias("begin")
    ConstIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch begin_1();

    @CXXValue
    IteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch end();

    @CXXValue
    @FFINameAlias("end")
    ConstIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch end_1();

    @CXXValue
    ReverseIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch rbegin();

    @CXXValue
    @FFINameAlias("rbegin")
    ConstReverseIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch rbegin_1(
            );

    @CXXValue
    ReverseIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch rend();

    @CXXValue
    @FFINameAlias("rend")
    ConstReverseIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch rend_1(
            );

    @CXXValue
    ConstIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch cbegin();

    @CXXValue
    ConstIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch cend();

    @CXXValue
    ConstReverseIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch crbegin(
            );

    @CXXValue
    ConstReverseIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch crend(
            );

    long size();

    long capacity();

    boolean empty();

    long max_size();

    void reserve(long __n);

    void shrink_to_fit();

    @CXXValue
    ReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch at(long __n);

    @CXXValue
    @FFINameAlias("at")
    ConstReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch at_1(
            long __n);

    @CXXValue
    ReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch front();

    @CXXValue
    @FFINameAlias("front")
    ConstReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch front_1();

    @CXXValue
    ReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch back();

    @CXXValue
    @FFINameAlias("back")
    ConstReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch back_1();

    ValueTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch data();

    @FFINameAlias("data")
    ValueTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch data_1();

    void push_back(
            @CXXValue ConstReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __x);

    void pop_back();

    @CXXValue
    IteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch insert(
            @CXXValue ConstIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __position,
            @CXXValue ConstReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __x);

    @CXXValue
    IteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch insert(
            @CXXValue ConstIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __position,
            long __n,
            @CXXValue ConstReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __x);

    @CXXValue
    IteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch insert(
            @CXXValue ConstIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __position,
            @CXXValue @FFITypeAlias("std::initializer_list<std::vector<std::shared_ptr<arrow::RecordBatch>, std::allocator<std::shared_ptr<arrow::RecordBatch>>>::value_type>") initializer_list<ValueTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch> __il);

    @CXXValue
    IteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch erase(
            @CXXValue ConstIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __position);

    @CXXValue
    IteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch erase(
            @CXXValue ConstIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __first,
            @CXXValue ConstIteratorStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __last);

    void clear();

    void resize(long __sz);

    void resize(long __sz,
            @CXXValue ConstReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __x);

    boolean __invariants();

    static RecordBatchVector cast(final long __foreign_address) {
        try {
            Class<RecordBatchVector> clz = (Class<RecordBatchVector>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(RecordBatchVector.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static RecordBatchVector cast(final FFIPointer __foreign_pointer) {
        return RecordBatchVector.cast(__foreign_pointer.getAddress());
    }

    static Factory getFactory() {
        return FFITypeFactory.getFactory(FFITypeFactory.getFFITypeName(RecordBatchVector.class, true));
    }

    static RecordBatchVector create() {
        return RecordBatchVector.getFactory().create();
    }

    static RecordBatchVector create(
            @CXXReference AllocatorTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __a) {
        return RecordBatchVector.getFactory().create(__a);
    }

    static RecordBatchVector create(long __n) {
        return RecordBatchVector.getFactory().create(__n);
    }

    static RecordBatchVector create(long __n,
            @CXXReference AllocatorTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __a) {
        return RecordBatchVector.getFactory().create(__n, __a);
    }

    static RecordBatchVector create(long __n,
            @CXXReference ValueTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __x) {
        return RecordBatchVector.getFactory().create(__n, __x);
    }

    static RecordBatchVector create(long __n,
            @CXXReference ValueTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __x,
            @CXXReference AllocatorTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __a) {
        return RecordBatchVector.getFactory().create(__n, __x, __a);
    }

    static RecordBatchVector create(
            @CXXValue @FFITypeAlias("std::initializer_list<std::vector<std::shared_ptr<arrow::RecordBatch>, std::allocator<std::shared_ptr<arrow::RecordBatch>>>::value_type>") initializer_list<ValueTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch> __il) {
        return RecordBatchVector.getFactory().create(__il);
    }

    static RecordBatchVector create(
            @CXXValue @FFITypeAlias("std::initializer_list<std::vector<std::shared_ptr<arrow::RecordBatch>, std::allocator<std::shared_ptr<arrow::RecordBatch>>>::value_type>") initializer_list<ValueTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch> __il,
            @CXXReference AllocatorTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __a) {
        return RecordBatchVector.getFactory().create(__il, __a);
    }

    @FFIFactory
    @CXXHead(
            system = "arrow/type_fwd.h"
    )
    interface Factory {
        RecordBatchVector create();

        RecordBatchVector create(
                @CXXReference AllocatorTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __a);

        RecordBatchVector create(long __n);

        RecordBatchVector create(long __n,
                @CXXReference AllocatorTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __a);

        RecordBatchVector create(long __n,
                @CXXReference ValueTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __x);

        RecordBatchVector create(long __n,
                @CXXReference ValueTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __x,
                @CXXReference AllocatorTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __a);

        RecordBatchVector create(
                @CXXValue @FFITypeAlias("std::initializer_list<std::vector<std::shared_ptr<arrow::RecordBatch>, std::allocator<std::shared_ptr<arrow::RecordBatch>>>::value_type>") initializer_list<ValueTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch> __il);

        RecordBatchVector create(
                @CXXValue @FFITypeAlias("std::initializer_list<std::vector<std::shared_ptr<arrow::RecordBatch>, std::allocator<std::shared_ptr<arrow::RecordBatch>>>::value_type>") initializer_list<ValueTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch> __il,
                @CXXReference AllocatorTypeStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch __a);
    }
}
