#!/bin/bash

set -x

SRC_DIR=../src/idris2
BUILD_DIR=$(pwd)

idris2 --output-dir $BUILD_DIR --build-dir $BUILD_DIR --mkdoc $SRC_DIR/RCPR.ipkg
