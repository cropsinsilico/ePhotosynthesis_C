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
  -v, --verbose          Record output values for all steps (this can
                         significantly slow the program).
  -e, --evn arg          The file (including path) containing environmental
                         parameters (default: InputEvn.txt)
  -a, --atpcost arg      The file (including path) containing the ATP cost
                         (default: InputATPCost.txt)
  -n, --enzyme arg       The file (including path) containing enzyme
                         activities like InputEnzyme.txt (default: "")
  -g, --grn arg          The file (including path) containing protein ratios
                         for relevant genes like InputGRNC.txt (default: "")
  -b, --begintime arg    The starting time for the calculations. (default:
                         0.0)
  -s, --stoptime arg     The time to stop calculations. (default: 5000.0)
  -z, --stepsize arg     The step size to use in the calculations. (default:
                         1.0)
  -m, --maxSubSteps arg  The maximum number of iterations at each time step.
                         (default: 750)
  -d, --driver arg       The driver to use. Choices are:                     
                           1 - trDynaPS: PS, PR, FI, BF, SUCS, RuACT,        
                                 XanCycle, RROEA                             
                           2 - DynaPS: PS, PR, FI, BF, SUCS, XanCycle        
                           3 - CM: PS, PR, SUCS                              
                           4 - EPS: PS, PR, FI, BF, SUCS                    
                         (default: 1)
  -c, --c3               Use the C3 model, automatically set to true for EPS
                         driver
  -t, --abstol arg       Absolute tolerance for calculations (default: 1e-5)
  -r, --reltol arg       Relative tolerance for calculations (default: 1e-4)
  -T, --Tp arg           Input Temperature (default: 0.0)
  -o, --options arg      Name of a text file which specifies any of the above
                         options. Command line arguments have priority.
                         (default: "")
  -h, --help             Produce help message
      --debug arg        Debug level (default: 0)
      --debugDelta       Debug deltas
      --debugInternal    Debug internals
```

#### Modules

- PS: Calvin cycle, starch synthesis, triose phosphate export
- PR: Photorespiration
- FI: Light energy absorption, transfer, primary charge separation, electro transfer around PSII
- BF: Electron transfer from reduced platquinone until the generation of ATP and NADPH including io transfer through thylakoid membrane and ATP synthesis process
- SUCS: 
- RuACT: Rubisco activation
- XanCycle: Xanthophylls cycle
- RedoxReg: Redox regulation of enzyme activities (EPS + RuAct + RA)
- RROEA: Redox regulation of enzyme activities?
- RA: ? (EPS + RuAct)

#### Module Combinations
- CM: PS + PR + SUCS
- EPS: PS + PR + FI + BF + SUCS
- DynaPS: EPS + XanCycle
- trDynaPS: DynaPS + RuACT + RROEA
