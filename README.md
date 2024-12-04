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
This can be built using a conda environment. This way, the cmake should be able to find the boost and sundials dependencies automatically. When using conda to manage the dependencies:

1. Be sure to activate the related env the next time after login.
1. Rebuild fresh (after removing the existing build files) after new versions of the dependencies are installed to the environment.

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
  ePhoto [OPTION...]

  -v, --verbose            Record output values for all steps (this can
                           significantly slow the program).
  -e, --evn arg            The file (including path) containing environmental
                           parameters (default: InputEvn.txt)
  -a, --atpcost arg        The file (including path) containing the ATP cost
                           (default: InputATPCost.txt)
  -n, --enzyme arg         The file (including path) containing enzyme
                           activities like InputEnzyme.txt (default: "")
  -g, --grn arg            The file (including path) containing protein
                           ratios for relevant genes like InputGRNC.txt (default:
                           "")
  -b, --begintime arg      The starting time for the calculations. (default:
                           0.0)
  -s, --stoptime arg       The time to stop calculations. (default: 5000.0)
  -z, --stepsize arg       The step size to use in the calculations.
                           (default: 1.0)
  -m, --maxSubSteps arg    The maximum number of iterations at each time
                           step. (default: 750)
  -d, --driver arg         The driver to use. Choices are:
                           1 - trDynaPS (default): PS, PR, FI, BF, SUCS,
                                  RuACT, XanCycle, RROEA
                           2 - DynaPS: PS, PR, FI, BF, SUCS, XanCycle
                           3 - CM: PS, PR, SUCS
                           4 - EPS: PS, PR, FI, BF, SUCS
  -c, --c3                 Use the C3 model, automatically set to true for
                           EPS driver
      --rubiscomethod arg  The method to use for rubisco calculations.
                           Choices are:
                           1 - (default) Use enzyme concentration for
                               calculation
                           2 - Use the michaelis menton and enzyme
                               concentration together for calculation
  -t, --abstol arg         Absolute tolerance for calculations (default:
                           1e-5)
  -r, --reltol arg         Relative tolerance for calculations (default:
                           1e-4)
  -T, --Tp arg             Input Temperature (default: 0.0)
  -o, --options arg        Name of a text file which specifies any of the
                           above options. Command line arguments have priority.
                           (default: "")
  -x, --output arg         Name the the text file that outputs should be
                           saved to. (default: output.data)
  -h, --help               Produce help message
      --debug arg          Debug level (default: 0)
      --debugDelta         Debug deltas
      --debugInternal      Debug internals

```