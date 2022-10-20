#!/bin/bash

set -x

SRC_DIR=../src/idris2
BUILD_DIR=$(pwd)

idris2 --output-dir $BUILD_DIR --build-dir $BUILD_DIR --build $SRC_DIR/RCPR.ipkg

for n in $(find $SRC_DIR/Example -type f -name "*.idr"); do
    idris2 --output-dir $BUILD_DIR --source-dir $SRC_DIR \
        --build-dir $BUILD_DIR -c $n
done
