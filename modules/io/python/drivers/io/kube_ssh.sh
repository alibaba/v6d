#!/bin/bash

POD_NAME=$1
shift

if [[ "$POD_NAME" =~ ":" ]]; then
    KUBE_NS=${POD_NAME%%:*}
    KUBE_POD_CONTAINER_NAME=${POD_NAME#*:}
    if [[ "$KUBE_POD_CONTAINER_NAME" =~ ":" ]]; then
        KUBE_POD_NAME=${KUBE_POD_CONTAINER_NAME%%:*}
        KUBE_CONTAINER_NAME=${KUBE_POD_CONTAINER_NAME#*:}
        kubectl -n ${KUBE_NS} exec ${KUBE_POD_NAME} -c ${KUBE_CONTAINER_NAME} -- /bin/bash -lc "
            cat /etc/hosts > /dev/null || true && \
            source /etc/profile || true && \
            source /etc/bash.bashrc || true && \
            source ~/.profile || true && \
            source ~/.bashrc || true && \
            shopt -s huponexit 2>/dev/null || true && \
            $*"
    else
        kubectl -n ${KUBE_NS} exec ${KUBE_POD_CONTAINER_NAME} -c engine -- /bin/bash -lc "
            cat /etc/hosts > /dev/null || true && \
            source /etc/profile || true && \
            source /etc/bash.bashrc || true && \
            source ~/.profile || true && \
            source ~/.bashrc || true && \
            shopt -s huponexit 2>/dev/null || true && \
            $*"
    fi
else
    kubectl exec ${POD_NAME} -c engine -- /bin/bash -lc "
        cat /etc/hosts > /dev/null || true && \
        source /etc/profile || true && \
        source /etc/bash.bashrc || true && \
        source ~/.profile || true && \
        source ~/.bashrc || true && \
        shopt -s huponexit 2>/dev/null || true && \
        $*"
fi
