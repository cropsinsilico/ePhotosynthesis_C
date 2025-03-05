#!/bin/sh
set -euo pipefail

# Using cmake
mkdir conda_build
cd conda_build
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=$PREFIX -D CMAKE_VERBOSE_MAKEFILE:BOOL=ON -D BUILD_PYTHON:BOOL=ON ..

# Build all, run tests, then install
cmake --build . --config Release
ctest -V
cmake --install .

# Make all, run tests, then install
# make all VERBOSE=1
# ctest -V
# make install
