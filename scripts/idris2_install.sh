#!/bin/bash

set -x

SRC_DIR=../src/idris2
BUILD_DIR=$(pwd)

idris2 --build-dir $BUILD_DIR --install $SRC_DIR/RCPR.ipkg
