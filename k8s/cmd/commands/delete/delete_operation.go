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
package delete

import (
	"context"

	"github.com/spf13/cobra"

	apierrors "k8s.io/apimachinery/pkg/api/errors"
	"k8s.io/apimachinery/pkg/types"

	vineyardV1alpha1 "github.com/v6d-io/v6d/k8s/apis/k8s/v1alpha1"
	"github.com/v6d-io/v6d/k8s/cmd/commands/flags"
	"github.com/v6d-io/v6d/k8s/cmd/commands/util"
)

// deleteOperationCmd deletes the specific operation
var deleteOperationCmd = &cobra.Command{
	Use:   "operation",
	Short: "Delete the operation on kubernetes",
	Long: `Delete the operation on kubernetes. 
For example:

# delete the operation named "assembly-test" in the "vineyard-system" namespace
vineyardctl delete operation --name assembly-test`,
	Run: func(cmd *cobra.Command, args []string) {
		util.AssertNoArgs(cmd, args)

		client := util.KubernetesClient()

		operation := &vineyardV1alpha1.Operation{}
		if err := client.Get(context.Background(),
			types.NamespacedName{Name: flags.OperationName,
				Namespace: flags.GetDefaultVineyardNamespace()},
			operation); err != nil && !apierrors.IsNotFound(err) {
			util.ErrLogger.Fatal("failed to get operation: ", err)
		}

		if err := client.Delete(context.Background(), operation); err != nil {
			util.ErrLogger.Fatal("failed to delete operation: ", err)
		}

		util.InfoLogger.Println("Operation is deleted.")
	},
}

func NewDeleteOperationCmd() *cobra.Command {
	return deleteOperationCmd
}

func init() {
	flags.ApplyOperationName(deleteOperationCmd)
}