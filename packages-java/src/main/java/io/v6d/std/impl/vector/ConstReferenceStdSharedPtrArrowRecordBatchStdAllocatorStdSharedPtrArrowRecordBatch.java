// Code generated by alibaba/fastFFI. DO NOT EDIT.
//
package io.v6d.std.impl.vector;

import com.alibaba.fastffi.CXXHead;
import com.alibaba.fastffi.CXXPointer;
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

@FFITypeAlias("std::vector<std::shared_ptr<arrow::RecordBatch>, std::allocator<std::shared_ptr<arrow::RecordBatch>>>::const_reference")
@FFIGen
@CXXHead(
        system = "vector"
)
public interface ConstReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch extends CXXPointer {
    static ConstReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch cast(
            final long __foreign_address) {
        try {
            Class<ConstReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch> clz = (Class<ConstReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch>) FFITypeFactory.getType(FFITypeFactory.getFFITypeName(ConstReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch.class, true));
            return clz.getConstructor(Long.TYPE).newInstance(__foreign_address);
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            return null;
        }
    }

    static ConstReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch cast(
            final FFIPointer __foreign_pointer) {
        return ConstReferenceStdSharedPtrArrowRecordBatchStdAllocatorStdSharedPtrArrowRecordBatch.cast(__foreign_pointer.getAddress());
    }
}
