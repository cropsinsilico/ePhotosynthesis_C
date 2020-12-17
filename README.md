# ePhotosynthesis

This is a C++ port of the ePhotosynthesis Matlab model code. The code is built with
*CMake*.

The Matlab origin code is tagged [C++ conversion](https://github.com/cropsinsilico/ePhotosynthesis/releases/tag/1.0.0). The scripts
directory contains the python script used for the initial conversion and the README file details the overall conversion process.
The script is provided for convenience and is not used in the build process.

### Prerequisites:
- [Boost](https://www.boost.org/) (1.36 or newer) - only the algorithm headers are needed
- [Sundials](https://computing.llnl.gov/projects/sundials) - Only the following modules are used:
  - CVode
  - Kinsol
- [Doxygen](https://www.doxygen.nl/index.html) - (optional) needed to build the documentation
- [LaTeX](https://www.latex-project.org/) - (optional) needed to build pdf formatted documentation

### Building
The build must be in a non-source directory.
```
mkdir build
cd build
cmake ..
make
make install (if desired)
```

### Documentation
The documentation is not automatically built. To build the docs run the following from the build directory
```
cmake .. -DBUILD_DOCS=true
make docs
```

This will build the documentation and put the resulting files in the doc directory.

The ePhotosynthesis executable takes the following arguments:
```
 -r,--record    Record the output values of all intermediate steps (this can substantially increase the runtime of the code)
 -e,--evn       The InputEvn.txt file, including path
 -a,--atpcost   The InputATPCost.txt file, including path
 -b,--begintime The starting time for the calculations (default is 0)
 -s,--stoptime  The ending time for the calculations (default is 5000)
 -z,--stepsize  The step size for the calculations (default is 1)
```