#!/bin/sh

export LD_LIBRARY_DIR=$(dirname $0)
$LD_LIBRARY_DIR/haproxy.exe "$@"

