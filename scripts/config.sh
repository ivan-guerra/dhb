#!/bin/bash

CWD=$(pwd)

# Root directory.
DHB_PROJECT_PATH=$(dirname ${CWD})

# Scripts directory.
DHB_SCRIPTS_PATH="${DHB_PROJECT_PATH}/scripts"

# Binary directory.
DHB_BIN_DIR="${DHB_PROJECT_PATH}/bin"

# CMake build files and cache.
DHB_BUILD_DIR="${DHB_PROJECT_PATH}/build"
