/** Copyright 2020-2023 Alibaba Group Holding Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package io.v6d.modules.basic.arrow;

import com.google.common.base.Objects;
import io.v6d.core.client.ds.Object;
import io.v6d.core.client.ds.ObjectFactory;
import io.v6d.core.client.ds.ObjectMeta;
import java.util.Arrays;
import java.util.List;
import org.apache.arrow.memory.ArrowBuf;
import org.apache.arrow.vector.BigIntVector;
import org.apache.arrow.vector.FieldVector;
import org.apache.arrow.vector.ipc.message.ArrowFieldNode;

public class Int64Array extends Array {
    private BigIntVector array;

    public static void instantiate() {
        ObjectFactory.getFactory()
                .register("vineyard::NumericArray<int64>", new Int64ArrayResolver());
        ObjectFactory.getFactory()
                .register("vineyard::NumericArray<uint64>", new Int64ArrayResolver());
    }

    public Int64Array(final ObjectMeta meta, List<ArrowBuf> buffers, long length, int nullCount) {
        super(meta);
        this.array = new BigIntVector("", Arrow.default_allocator);
        this.array.loadFieldBuffers(new ArrowFieldNode(length, nullCount), buffers);
    }

    public double get(int index) {
        return this.array.get(index);
    }

    @Override
    public FieldVector getArray() {
        return this.array;
    }

    @Override
    public boolean equals(java.lang.Object o) {
        if (this == o) {
            return true;
        }
        if (o == null || getClass() != o.getClass()) {
            return false;
        }
        Int64Array that = (Int64Array) o;
        return Objects.equal(array, that.array);
    }

    @Override
    public int hashCode() {
        return Objects.hashCode(array);
    }
}

class Int64ArrayResolver extends ObjectFactory.Resolver {
    @Override
    public Object resolve(final ObjectMeta meta) {
        Buffer buffer =
                (Buffer) ObjectFactory.getFactory().resolve(meta.getMemberMeta("data_buffer_"));
        Buffer validityBuffer =
                (Buffer) ObjectFactory.getFactory().resolve(meta.getMemberMeta("validity_buffer_"));
        int nullCount = meta.getIntValue("null_count_");
        int length = meta.getIntValue("length_");
        return new Int64Array(
                meta,
                Arrays.asList(validityBuffer.getBuffer(), buffer.getBuffer()),
                length,
                nullCount);
    }
}
