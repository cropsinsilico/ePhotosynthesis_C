# ePhotosynthesis

This is a C++ port of the ePhotosynthesis Matlab model code. The code is built with
*CMake*.

The Matlab origin code is tagged [C++ conversion](https://github.com/cropsinsilico/ePhotosynthesis/releases/tag/1.0.0). The scripts
directory contains the python script used for the initial conversion and the README file details the overall conversion process.
The script is provided for convenience and is not used in the build process.

### Prerequisites:
All of the packages required to build & run the library, tests, & documentation are included in the `environment.yml` file that can be installed via conda. However, you should be able to build using your favorite package manager.
- [CMake](https://cmake.org/) (3.5 or newer) - required to build
- [Boost](https://www.boost.org/) (1.36 or newer) - only the algorithm headers are needed
- [Sundials>=6.7.0](https://computing.llnl.gov/projects/sundials) - Only the following modules are used:
  - CVode
  - Kinsol
- [Doxygen](https://www.doxygen.nl/index.html) - (optional) needed to build the documentation
- [LaTeX](https://www.latex-project.org/) - (optional) needed to build pdf formatted documentation
- [GoogleTest](https://google.github.io/googletest/) - (optional) needed to build the tests
- [LCOV](https://wiki.documentfoundation.org/Development/Lcov) - (optional) needed to get test coverage

### Building
This can be built using a conda environment. This way, the cmake should be able to find the boost and sundial automatically. Of course, be sure to activate the related env the next time after login.

1. Initialize a new conda env so that the package path is on your local drive with r/w permissons. You can use the `environment.yml` provided to create a conda environment with the required packages via `conda env create -f environment.yml`. If you use this method you can skip to building ePhotosynthesis.
1. Install both boost and sundial through conda.
1. Follow the following steps to build ePhotosynthesis via cmake (call make or nmake). The build must be in a non-source directory.

```
mkdir build
cd build
cmake ..
cmake --build .
cmake --install . (if desired)
```

### Documentation
The documentation is not automatically built. To build the docs run the following from the build directory (requires Doxygen)
```
cmake .. -DBUILD_DOCS:BOOL=ON
cmake --build . --target docs
```

This will build the documentation and put the resulting files in the doc directory.

### Tests
The tests are not automatically built. To build and run the tests, can be run the following from the build directory (requires GoogleTest)
```
cmake .. -DBUILD_TESTS:BOOL=ON
cmake --build .
ctest
```

### Command line interface

The ePhotosynthesis executable is named `ePhoto` and takes the following arguments:
```
 -r,--record    Record the output values of all intermediate steps (this can substantially increase the runtime of the code)
 -e,--evn       The InputEvn.txt file, including path
 -a,--atpcost   The InputATPCost.txt file, including path
 -b,--begintime The starting time for the calculations (default is 0)
 -s,--stoptime  The ending time for the calculations (default is 5000)
 -z,--stepsize  The step size for the calculations (default is 1)
```
