/*
* Copyright 2020-2023 Alibaba Group Holding Limited.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
package create

import (
	"github.com/spf13/cobra"

	metav1 "k8s.io/apimachinery/pkg/apis/meta/v1"

	"github.com/v6d-io/v6d/k8s/apis/k8s/v1alpha1"
	"github.com/v6d-io/v6d/k8s/cmd/commands/flags"
	"github.com/v6d-io/v6d/k8s/cmd/commands/util"
	"github.com/v6d-io/v6d/k8s/controllers/k8s"
)

// createRecoverCmd creates the recover job of vineyard cluster on kubernetes
var createRecoverCmd = &cobra.Command{
	Use:   "recover",
	Short: "Recover the current vineyard cluster on kubernetes",
	Long: `Recover the current vineyard cluster on kubernetes. You could recover all objects from
a backup of vineyard cluster. Usually, the recover job should be created in the same namespace of
the backup job.

For example:

# create a recover job for a backup job in the same namespace
vineyardctl create recover --backup-name vineyardd-sample -n vineyard-system`,
	Run: func(cmd *cobra.Command, args []string) {
		util.AssertNoArgs(cmd, args)
		client := util.KubernetesClient()

		recover, err := buildRecoverJob()
		if err != nil {
			util.ErrLogger.Fatalf("failed to build recover job: %+v", err)
		}

		if err := util.Create(client, recover, func(*v1alpha1.Recover) bool {
			return recover.Status.State != k8s.SucceedState
		}); err != nil {
			util.ErrLogger.Fatalf("failed to create and wait recover job: %+v", err)
		}
		util.InfoLogger.Println("Backup Job is ready.")
	},
}

func NewCreateRecoverCmd() *cobra.Command {
	return createRecoverCmd
}

func init() {
	flags.ApplyRecoverOpts(createRecoverCmd)
}

func buildRecoverJob() (*v1alpha1.Recover, error) {
	namespace := flags.GetDefaultVineyardNamespace()
	recover := &v1alpha1.Recover{
		ObjectMeta: metav1.ObjectMeta{
			Name:      flags.RecoverName,
			Namespace: namespace,
		},
		Spec: v1alpha1.RecoverSpec{
			BackupName:      flags.BackupName,
			BackupNamespace: namespace,
		},
	}
	return recover, nil
}