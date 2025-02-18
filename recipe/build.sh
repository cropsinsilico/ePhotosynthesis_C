#!/bin/sh
set -euo pipefail

# if [[ `uname` == Darwin ]]; then
#     export LDFLAGS="-Wl,-rpath,$PREFIX/lib $LDFLAGS"
# fi

# Using cmake
mkdir conda_build
cd conda_build
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=$PREFIX ..

# Make all, run tests, then install
make all VERBOSE=1
ctest -V
make install
