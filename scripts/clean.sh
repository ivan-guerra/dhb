#!/bin/bash

source config.sh

# Remove the binary directory.
if [ -d $DHB_BIN_DIR ]
then
    echo "removing '$DHB_BIN_DIR'"
    rm -r $DHB_BIN_DIR
fi

# Remove the CMake build directory.
if [ -d $DHB_BUILD_DIR ]
then
    echo "removing '$DHB_BUILD_DIR'"
    rm -r $DHB_BUILD_DIR
fi
