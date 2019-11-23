#!/usr/bin/env bash

# Exit on error
set -e
# Echo each command
set -x

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$CONDA_PREFIX ..
cmake --build . --target install
