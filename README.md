# ePhotosynthesis

This is a C++ port of the ePhotosynthesis Matlab model code. The code is built with
*CMake*.

### Prerequisites:
- [Boost](https://www.boost.org/) (1.36 or newer) - only the algorithm headers are needed
- [Sundials](https://computing.llnl.gov/projects/sundials) - Only the following modules are used:
  - CVode
  - Kinsol

The build must be in a non-source directory.
```
mkdir build
cd build
cmake ..
make
make install (if desired)
```
