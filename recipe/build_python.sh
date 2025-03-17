#!/bin/sh
set -euo pipefail

"${PYTHON}" -m pip install . --no-deps --ignore-installed --no-cache-dir -vvv

# # Using cmake
# mkdir conda_build
# cd conda_build
# cmake ${CMAKE_ARGS} \
#       -D CMAKE_BUILD_TYPE=Release \
#       -D CMAKE_INSTALL_PREFIX=$PREFIX \
#       -D CMAKE_VERBOSE_MAKEFILE:BOOL=ON \
#       -D BUILD_CXX:BOOL=OFF \
#       -D BUILD_PYTHON:BOOL=ON \
#       -D WITH_YGGDRASIL:BOOL=ON ..

# # Build all, run tests, then install
# cmake --build . --config Release
# ctest -V
# cmake --install .
