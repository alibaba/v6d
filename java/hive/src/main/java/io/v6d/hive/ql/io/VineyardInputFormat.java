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
package io.v6d.hive.ql.io;

import io.v6d.core.client.IPCClient;
import io.v6d.core.client.ds.ObjectFactory;
import io.v6d.core.common.util.ObjectID;
import io.v6d.modules.basic.arrow.Arrow;
import io.v6d.modules.basic.arrow.Table;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.hadoop.hive.ql.exec.Utilities;
import org.apache.hadoop.hive.ql.exec.vector.*;
import org.apache.hadoop.hive.ql.io.HiveInputFormat;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapred.FileInputFormat;
import org.apache.hadoop.mapred.InputSplit;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.RecordReader;
import org.apache.hadoop.mapred.Reporter;

import org.apache.arrow.vector.*;
import org.apache.arrow.vector.VectorSchemaRoot;
import org.apache.arrow.vector.types.pojo.*;
import org.apache.hadoop.fs.Path;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class VineyardInputFormat extends HiveInputFormat<NullWritable, VectorizedRowBatch> implements VectorizedInputFormatInterface {

    @Override
    public RecordReader<NullWritable, VectorizedRowBatch>
    getRecordReader(InputSplit genericSplit, JobConf job, Reporter reporter)
            throws IOException {
        reporter.setStatus(genericSplit.toString());
        return new VineyardBatchRecordReader(job, (VineyardSplit) genericSplit);
    }

    @Override
    public VectorizedSupport.Support[] getSupportedFeatures() {
        return new VectorizedSupport.Support[] {VectorizedSupport.Support.DECIMAL_64};
    }

    @Override
    public InputSplit[] getSplits(JobConf job, int numSplits) throws IOException {
        List<InputSplit> splits = new ArrayList<InputSplit>();
        Path path = FileInputFormat.getInputPaths(job)[0];
        // fill splits
        VineyardSplit vineyardSplit = new VineyardSplit(path, 0, 0, job);
        splits.add(vineyardSplit);
        return splits.toArray(new VineyardSplit[splits.size()]);
    }
}

class VineyardBatchRecordReader implements RecordReader<NullWritable, VectorizedRowBatch> {
    private static Logger logger = LoggerFactory.getLogger(VineyardBatchRecordReader.class);

    // vineyard field
    private IPCClient client;
    private String tableName;
    private Boolean tableNameValid = false;
    private VectorSchemaRoot vectorSchemaRoot;
    private VectorizedRowBatchCtx ctx;
    private Table table;
    private int recordBatchIndex = 0;
    private int recordBatchInnerIndex = 0;

    VineyardBatchRecordReader(JobConf job, VineyardSplit split) {
        String path = split.getPath().toString();
        int index = path.lastIndexOf("/");
        tableName = path.substring(index + 1);
        tableNameValid = true;

        // connect to vineyard
        if (client == null) {
            // TBD: get vineyard socket path from table properties
            try {
                client = new IPCClient("/tmp/vineyard/vineyard.sock");
            } catch (Exception e) {
                System.out.println("connect to vineyard failed!");
                System.out.println(e.getMessage());
            }
        }
        if (client == null || !client.connected()) {
            System.out.println("connected to vineyard failed!");
            return;
        } else {
            System.out.println("connected to vineyard succeed!");
            System.out.println("Hello, vineyard!");
        }

        Arrow.instantiate();
        ctx = Utilities.getVectorizedRowBatchCtx(job);
    }

    @Override
    public void close() throws IOException {
        if(client != null && client.connected()) {
            client.disconnect();
            System.out.println("Bye, vineyard!");
        }
    }

    @Override
    public NullWritable createKey() {
        return null;
    }

    @Override
    public VectorizedRowBatch createValue() {
        return ctx.createVectorizedRowBatch();
    }

    @Override
    public long getPos() throws IOException {
        return 0;
    }

    @Override
    public float getProgress() throws IOException {
        return 0;
    }

    /**
     * Copy data from the Arrow RecordBatch to the VectorizedRowBatch.
     *
     * @param recordBatch the Arrow RecordBatch to copy from
     * @param batch the VectorizedRowBatch to copy to
     */
    private void arrowToVectorizedRowBatch(VectorizedRowBatch batch) {
        // batch.numCols = recordBatch.getFieldVectors().size();
        int remaining = vectorSchemaRoot.getRowCount() - recordBatchInnerIndex;

        /*
         * When we use SQL with condition to query data, the recordBatchSize may be large than VectorizedRowBatch.DEFAULT_SIZE, 
         * which will cause ArrayIndexOutOfBoundsException. Because hive use a temporary array to store the data of
         * VectorizedRowBatch when process condition query, and the size of the array is VectorizedRowBatch.DEFAULT_SIZE
         * (VectorFilterOperator.java: 102).
         * So we need to limit the recordBatchSize to VectorizedRowBatch.DEFAULT_SIZE if the recordBatchSize is larger than
         * VectorizedRowBatch.DEFAULT_SIZE.
         */
        int recordBatchSize = remaining <= VectorizedRowBatch.DEFAULT_SIZE ? remaining : VectorizedRowBatch.DEFAULT_SIZE;
        batch.size = recordBatchSize;

        for (int i = 0; i < vectorSchemaRoot.getSchema().getFields().size(); i++) {
            Field field = vectorSchemaRoot.getSchema().getFields().get(i);
            if (field.getType().equals(Arrow.Type.Boolean)) {
                LongColumnVector vector = new LongColumnVector(batch.size);
                BitVector bitVector = (BitVector) vectorSchemaRoot.getFieldVectors().get(i);
                for (int k = 0; k < batch.size; k++) {
                    vector.vector[k] = bitVector.get(k + recordBatchInnerIndex);
                }
                batch.cols[i] = vector;
            } else if (field.getType().equals(Arrow.Type.Int)) {
                LongColumnVector vector = new LongColumnVector(batch.size);
                IntVector intVector = (IntVector) vectorSchemaRoot.getFieldVectors().get(i);
                for (int k = 0; k < batch.size; k++) {
                    vector.vector[k] = intVector.get(k + recordBatchInnerIndex);
                }
                batch.cols[i] = vector;
            } else if (field.getType().equals(Arrow.Type.Int64)) {
                LongColumnVector vector = new LongColumnVector(batch.size);
                BigIntVector bigIntVector = (BigIntVector) vectorSchemaRoot.getFieldVectors().get(i);
                for (int k = 0; k < batch.size; k++) {
                    vector.vector[k] = bigIntVector.get(k + recordBatchInnerIndex);
                }
                batch.cols[i] = vector;
            } else if (field.getType().equals(Arrow.Type.Float)) {
                DoubleColumnVector vector = new DoubleColumnVector(batch.size);
                Float4Vector float4Vector = (Float4Vector) vectorSchemaRoot.getFieldVectors().get(i);
                for (int k = 0; k < batch.size; k++) {
                    vector.vector[k] = float4Vector.get(k + recordBatchInnerIndex);
                }
                batch.cols[i] = vector;
            } else if (field.getType().equals(Arrow.Type.Double)) {
                DoubleColumnVector vector = new DoubleColumnVector(batch.size);
                Float8Vector float8Vector = (Float8Vector) vectorSchemaRoot.getFieldVectors().get(i);
                for (int k = 0; k < batch.size; k++) {
                    vector.vector[k] = float8Vector.get(k + recordBatchInnerIndex);
                }
                batch.cols[i] = vector;
            } else if (field.getType().equals(Arrow.Type.LargeVarChar)) {
                BytesColumnVector vector = new BytesColumnVector(batch.size);
                LargeVarCharVector largeVarCharVector = (LargeVarCharVector) vectorSchemaRoot.getFieldVectors().get(i);
                for (int k = 0; k < batch.size; k++) {
                    vector.isNull[k] = false;
                    vector.setRef(k, largeVarCharVector.get(k + recordBatchInnerIndex), 0, largeVarCharVector.get(k + recordBatchInnerIndex).length);
                }
                batch.cols[i] = vector;
            } else if (field.getType().equals(Arrow.Type.VarChar)) {
                BytesColumnVector vector = new BytesColumnVector(batch.size);
                vector.init();
                VarCharVector varCharVector = (VarCharVector) vectorSchemaRoot.getFieldVectors().get(i);
                for (int k = 0; k < batch.size; k++) {
                    vector.isNull[k] = false;
                    vector.setRef(k, varCharVector.get(k + recordBatchInnerIndex), 0, varCharVector.get(k + recordBatchInnerIndex).length);
                }
                batch.cols[i] = vector;
            } else {
                System.out.println("array builder for type " + field.getType() + " is not supported");
                throw new UnsupportedOperationException("array builder for type " + field.getType() + " is not supported");
            }
        }
        recordBatchInnerIndex += recordBatchSize;
        if (recordBatchInnerIndex >= vectorSchemaRoot.getRowCount()) {
            recordBatchInnerIndex = 0;
            recordBatchIndex++;
            vectorSchemaRoot = null;
        }
    }

    @Override
    public boolean next(NullWritable key, VectorizedRowBatch value) throws IOException {
        if (tableNameValid) {
            if (table == null) {
                try {
                    ObjectID objectID = client.getName(tableName, false);
                    table = (Table) ObjectFactory.getFactory().resolve(client.getMetaData(objectID));
                } catch (Exception e) {
                    System.out.println("Get objectID failed.");
                    return false;
                }
            }
            if (recordBatchIndex >= table.getBatches().size()) {
                return false;
            }
            if (vectorSchemaRoot == null) {
                vectorSchemaRoot = table.getArrowBatch(recordBatchIndex);
            }
            if (vectorSchemaRoot == null) {
                System.out.println("Get vector schema root failed.");
                return false;
            }

            this.arrowToVectorizedRowBatch(value);
            return true;
        }
        return false;
    }
}
