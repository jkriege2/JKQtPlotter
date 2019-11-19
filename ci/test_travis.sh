#!/usr/bin/env bash

# Exit on error
set -e
# Echo each command
set -x

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$CONDA_PREFIX ..
cmake --build . --target install

if [[ "${TRAVIS_OS_NAME}" != "osx" ]]; then
    cd ../ci/test_project
    cmake -DCMAKE_PREFIX_PATH=$CONDA_PREFIX .
    cmake --build .
    ldd test_project | grep "not found"
fi
