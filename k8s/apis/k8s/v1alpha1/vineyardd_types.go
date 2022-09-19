/** Copyright 2020-2022 Alibaba Group Holding Limited.

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

// Package v1alpha1 contains API Schema definitions for the k8s v1alpha1 API group
package v1alpha1

import (
	appsv1 "k8s.io/api/apps/v1"
	corev1 "k8s.io/api/core/v1"
	metav1 "k8s.io/apimachinery/pkg/apis/meta/v1"
)

// MetricConfig holds all metrics configuration
type MetricConfig struct {
	// represent the metric's image
	// +kubebuilder:validation:Optional
	Image string `json:"image,omitempty"`
	// the policy about pulling image
	// +kubebuilder:validation:Optional
	ImagePullPolicy string `json:"imagePullPolicy,omitempty"`
}

// VineyarddConfig holds all command configuration about vineyardd
type VineyarddConfig struct {
	// path of etcd executable
	// +kubebuilder:validation:Optional
	EtcdCmd string `json:"etcdCmd,omitempty"`
	// endpoint of etcd
	// +kubebuilder:validation:Optional
	EtcdEndpoint string `json:"etcdEndpoint,omitempty"`
	// path prefix in etcd
	// +kubebuilder:validation:Optional
	EtcdPrefix string `json:"etcdPrefix,omitempty"`
	// enable the metrics in vineyardd
	// +kubebuilder:validation:Optional
	EnableMetrics bool `json:"enableMetrics,omitempty"`
	// whether to print metrics for prometheus
	// +kubebuilder:validation:Optional
	EnablePrometheus bool `json:"enablePrometheus,omitempty"`
	// ipc socket file location
	// +kubebuilder:validation:Optional
	Socket string `json:"socket,omitempty"`
	// the configuration of spilling
	// +kubebuilder:validation:Optional
	SpillConfig SpillConfig `json:"spillConfig,omitempty"`
	// memory threshold of streams (percentage of total memory)
	// +kubebuilder:validation:Optional
	StreamThreshold int64 `json:"streamThreshold,omitempty"`
	// shared memory size for vineyardd
	// +kubebuilder:validation:Optional
	SharedMemorySize string `json:"sharedMemorySize,omitempty"`
	// synchronize CRDs when persisting objects
	// +kubebuilder:validation:Optional
	SyncCRDs bool `json:"syncCRDs,omitempty"`
}

// SpillConfig holds all configuration about spilling
type SpillConfig struct {
	// the name of the spill config
	// +kubebuilder:validation:Optional
	Name string `json:"name,omitempty"`
	// the path of spilling
	// +kubebuilder:validation:Optional
	Path string `json:"path,omitempty"`
	// low watermark of spilling memory
	// +kubebuilder:validation:Optional
	SpillLowerRate string `json:"spillLowerRate,omitempty"`
	// high watermark of triggering spiling
	// +kubebuilder:validation:Optional
	SpillUpperRate string `json:"spillUpperRate,omitempty"`
	// the PersistentVolumeSpec of the spilling PV
	// +kubebuilder:validation:Optional
	PersistentVolumeSpec corev1.PersistentVolumeSpec `json:"persistentVolumeSpec,omitempty"`
	// the PersistentVolumeClaimSpec of the spill file
	// +kubebuilder:validation:Optional
	PersistentVolumeClaimSpec corev1.PersistentVolumeClaimSpec `json:"persistentVolumeClaimSpec,omitempty"`
}

// ServiceConfig holds all service configuration about vineyardd
type ServiceConfig struct {
	// service type
	// +kubebuilder:validation:Required
	Type string `json:"type,omitempty"`
	// service port
	// +kubebuilder:validation:Required
	Port int `json:"port,omitempty"`
}

// Etcd holds all configuration about Etcd
type Etcd struct {
	// Etcd instances
	// +kubebuilder:validation:Optional
	Instances int `json:"instances,omitempty"`
}

// VineyarddSpec holds all configuration about vineyardd
type VineyarddSpec struct {
	// represent the vineyardd's image
	// +kubebuilder:validation:Required
	Image string `json:"image"`
	// the policy about pulling image
	// +kubebuilder:validation:Optional
	ImagePullPolicy string `json:"imagePullPolicy,omitempty"`
	// the version of vineyardd
	// +kubebuilder:validation:Required
	Version string `json:"version"`
	// the replicas of vineyardd
	// +kubebuilder:validation:Required
	Replicas int `json:"replicas"`
	// metric configuration
	// +kubebuilder:validation:Optional
	Metric MetricConfig `json:"metric,omitempty"`
	// vinyardd configuration
	// +kubebuilder:validation:Optional
	Config VineyarddConfig `json:"config,omitempty"`
	// environment configuration
	// +kubebuilder:validation:Optional
	Env []corev1.EnvVar `json:"env,omitempty"`
	// vineyardd's service
	// +kubebuilder:validation:Optional
	Service ServiceConfig `json:"service,omitempty"`
	// Etcd describe the etcd instances
	// +kubebuilder:validation:Optional
	Etcd Etcd `json:"etcd,omitempty"`
}

// VineyarddStatus defines the observed state of Vineyardd
type VineyarddStatus struct {
	// Total replicas of running vineyardd.
	Running int32 `json:"running,omitempty"`
	// Total replicas of required vineyardd.
	Required int32 `json:"required,omitempty"`
	// Represents the vineyardd deployment's current state.
	Conditions []appsv1.DeploymentCondition `json:"conditions,omitempty"`
}

// +kubebuilder:object:root=true
// +kubebuilder:subresource:status
// +kubebuilder:printcolumn:name="Running",type=string,JSONPath=`.status.running`
// +kubebuilder:printcolumn:name="Required",type=string,JSONPath=`.status.required`
// +genclient

// Vineyardd is the Schema for the vineyardds API
type Vineyardd struct {
	metav1.TypeMeta   `json:",inline"`
	metav1.ObjectMeta `json:"metadata,omitempty"`

	Spec   VineyarddSpec   `json:"spec,omitempty"`
	Status VineyarddStatus `json:"status,omitempty"`
}

// +kubebuilder:object:root=true

// VineyarddList contains a list of Vineyardd
type VineyarddList struct {
	metav1.TypeMeta `json:",inline"`
	metav1.ListMeta `json:"metadata,omitempty"`
	Items           []Vineyardd `json:"items"`
}

func init() {
	SchemeBuilder.Register(&Vineyardd{}, &VineyarddList{})
}
