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

import java.io.IOException;
import java.util.Properties;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hive.ql.exec.FileSinkOperator;
import org.apache.hadoop.hive.ql.io.HiveOutputFormat;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.RecordWriter;
import org.apache.hadoop.util.Progressable;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class VineyardOutputFormat<K extends WritableComparable, V extends Writable>
        implements HiveOutputFormat<K, V> {
    private static Logger logger = LoggerFactory.getLogger(VineyardOutputFormat.class);

    @Override
    public FileSinkOperator.RecordWriter getHiveRecordWriter(
            JobConf jc,
            Path finalOutPath,
            Class<? extends Writable> valueClass,
            boolean isCompressed,
            Properties tableProperties,
            Progressable progress)
            throws IOException {
        return null;
    }

    @Override
    public RecordWriter<K, V> getRecordWriter(
            FileSystem fileSystem, JobConf jobConf, String s, Progressable progressable)
            throws IOException {
        return null;
    }

    @Override
    public void checkOutputSpecs(FileSystem fileSystem, JobConf jobConf) throws IOException {}
}
