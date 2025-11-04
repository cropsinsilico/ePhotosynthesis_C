#!/bin/sh
set -euo pipefail

# Using cmake
if [ ! -d conda_build ]; then
    mkdir conda_build
fi
cd conda_build
# -D CMAKE_BUILD_TYPE=Release \
# -D CMAKE_INSTALL_PREFIX=$PREFIX \
cmake ${CMAKE_ARGS} \
    -D CMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -D WITH_YGGDRASIL:BOOL=ON ..

# Build all, run tests, then install
cmake --build . --config Release
ctest -V
cmake --install .

# Make all, run tests, then install
# make all VERBOSE=1
# ctest -V
# make install

cd ..
