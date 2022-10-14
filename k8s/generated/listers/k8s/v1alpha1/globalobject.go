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
// Code generated by lister-gen. DO NOT EDIT.

package v1alpha1

import (
	v1alpha1 "github.com/v6d-io/v6d/k8s/apis/k8s/v1alpha1"
	"k8s.io/apimachinery/pkg/api/errors"
	"k8s.io/apimachinery/pkg/labels"
	"k8s.io/client-go/tools/cache"
)

// GlobalObjectLister helps list GlobalObjects.
// All objects returned here must be treated as read-only.
type GlobalObjectLister interface {
	// List lists all GlobalObjects in the indexer.
	// Objects returned here must be treated as read-only.
	List(selector labels.Selector) (ret []*v1alpha1.GlobalObject, err error)
	// GlobalObjects returns an object that can list and get GlobalObjects.
	GlobalObjects(namespace string) GlobalObjectNamespaceLister
	GlobalObjectListerExpansion
}

// globalObjectLister implements the GlobalObjectLister interface.
type globalObjectLister struct {
	indexer cache.Indexer
}

// NewGlobalObjectLister returns a new GlobalObjectLister.
func NewGlobalObjectLister(indexer cache.Indexer) GlobalObjectLister {
	return &globalObjectLister{indexer: indexer}
}

// List lists all GlobalObjects in the indexer.
func (s *globalObjectLister) List(selector labels.Selector) (ret []*v1alpha1.GlobalObject, err error) {
	err = cache.ListAll(s.indexer, selector, func(m interface{}) {
		ret = append(ret, m.(*v1alpha1.GlobalObject))
	})
	return ret, err
}

// GlobalObjects returns an object that can list and get GlobalObjects.
func (s *globalObjectLister) GlobalObjects(namespace string) GlobalObjectNamespaceLister {
	return globalObjectNamespaceLister{indexer: s.indexer, namespace: namespace}
}

// GlobalObjectNamespaceLister helps list and get GlobalObjects.
// All objects returned here must be treated as read-only.
type GlobalObjectNamespaceLister interface {
	// List lists all GlobalObjects in the indexer for a given namespace.
	// Objects returned here must be treated as read-only.
	List(selector labels.Selector) (ret []*v1alpha1.GlobalObject, err error)
	// Get retrieves the GlobalObject from the indexer for a given namespace and name.
	// Objects returned here must be treated as read-only.
	Get(name string) (*v1alpha1.GlobalObject, error)
	GlobalObjectNamespaceListerExpansion
}

// globalObjectNamespaceLister implements the GlobalObjectNamespaceLister
// interface.
type globalObjectNamespaceLister struct {
	indexer   cache.Indexer
	namespace string
}

// List lists all GlobalObjects in the indexer for a given namespace.
func (s globalObjectNamespaceLister) List(selector labels.Selector) (ret []*v1alpha1.GlobalObject, err error) {
	err = cache.ListAllByNamespace(s.indexer, s.namespace, selector, func(m interface{}) {
		ret = append(ret, m.(*v1alpha1.GlobalObject))
	})
	return ret, err
}

// Get retrieves the GlobalObject from the indexer for a given namespace and name.
func (s globalObjectNamespaceLister) Get(name string) (*v1alpha1.GlobalObject, error) {
	obj, exists, err := s.indexer.GetByKey(s.namespace + "/" + name)
	if err != nil {
		return nil, err
	}
	if !exists {
		return nil, errors.NewNotFound(v1alpha1.Resource("globalobject"), name)
	}
	return obj.(*v1alpha1.GlobalObject), nil
}