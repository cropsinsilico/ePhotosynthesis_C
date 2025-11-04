#!/bin/sh
set -euo pipefail

if [ ! -d conda_build ]; then
    mkdir conda_build
fi
cd conda_build
cmake ${CMAKE_ARGS} \
      -G "Ninja" \
      -D CMAKE_VERBOSE_MAKEFILE:BOOL=ON \
      -D WITH_YGGDRASIL:BOOL=ON \
      ..

# Build all, run tests, then install
cmake --build . --config Release
ctest -V

cmake --install .

cd ..
