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
- [Sundials>=5.7.0](https://computing.llnl.gov/projects/sundials) - Only the following modules are used:
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
The documentation is not automatically built. To build the docs run the following from the build directory
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

  -v, --verbose                 Record output values for all steps (this can
                                significantly slow the program).
  -e, --evn arg                 The file (including path) containing
                                environmental parameters (default: InputEvn.txt)
  -a, --atpcost arg             The file (including path) containing the ATP
                                cost (default: InputATPCost.txt)
  -n, --enzyme arg              The file (including path) containing enzyme
                                activities like InputEnzyme.txt (default: "")
  -g, --grn arg                 The file (including path) containing protein
                                ratios for relevant genes like InputGRNC.txt
                                (default: "")
  -b, --begintime arg           The starting time for the calculations.
                                (default: 0.0)
  -s, --stoptime arg            The time to stop calculations. (default:
                                5000.0)
  -z, --stepsize arg            The step size to use in the calculations.
                                (default: 1.0)
  -m, --maxSubSteps arg         The maximum number of iterations at each time
                                step. (default: 750)
  -d, --driver arg              The driver to use. Choices are:
                                1 - trDynaPS (default): PS, PR, FI, BF, SUCS,
                                       RuACT, XanCycle, RROEA
                                2 - DynaPS: PS, PR, FI, BF, SUCS, XanCycle
                                3 - CM: PS, PR, SUCS
                                4 - EPS: PS, PR, FI, BF, SUCS
  -c, --c3                      Use the C3 model, automatically set to true
                                for EPS driver. If false, the Farquar model is
                                used instead
      --rubiscomethod arg       The method to use for rubisco calculations.
                                Choices are:
                                1 - (default) Use enzyme concentration for
                                    calculation
                                2 - Use the michaelis menton and enzyme
                                    concentration together for calculation
  -t, --abstol arg              Absolute tolerance for calculations (default:
                                1e-5)
  -r, --reltol arg              Relative tolerance for calculations (default:
                                1e-4)
  -T, --Tp arg                  Input Temperature (default: 0.0)
  -o, --options arg             Name of a text file which specifies any of
                                the above options. Command line arguments have
                                priority. (default: "")
  -x, --output arg              Name the the text file that outputs should be
                                saved to. (default: output.data)
  -h, --help                    Produce help message
      --debug arg               Debug level (default: 0)
      --debugDelta              Debug deltas
      --debugInternal           Debug internals
      --outputParam             Output the initial and final parameter values
      --outputParamBase arg     Base name for files that initial and final
                                parameter values should be output to (default:
                                "")
      --BF_COND_constants arg   File containing initial conditions tracked by
                                the BF module (default: "")
      --BF_POOL_constants arg   File containing pool constants that control
                                the the BF module (default: "")
      --BF_KE_constants arg     File containing equilibrium constants that
                                control the the BF module (default: "")
      --BF_MOD_constants arg    File containing top level constants that
                                control the the BF module (default: "")
      --BF_RC_constants arg     File containing rate constants that control
                                the the BF module (default: "")
      --BF_VARS_constants arg   File containing top level variables the BF
                                module (default: "")
      --CM_COND_constants arg   File containing initial conditions tracked by
                                the CM module (default: "")
      --CM_POOL_constants arg   File containing pool constants that control
                                the the CM module (default: "")
      --CM_KE_constants arg     File containing equilibrium constants that
                                control the the CM module (default: "")
      --CM_MOD_constants arg    File containing top level constants that
                                control the the CM module (default: "")
      --CM_RC_constants arg     File containing rate constants that control
                                the the CM module (default: "")
      --CM_VARS_constants arg   File containing top level variables the CM
                                module (default: "")
      --DynaPS_COND_constants arg
                                File containing initial conditions tracked by
                                the DynaPS module (default: "")
      --DynaPS_POOL_constants arg
                                File containing pool constants that control
                                the the DynaPS module (default: "")
      --DynaPS_KE_constants arg
                                File containing equilibrium constants that
                                control the the DynaPS module (default: "")
      --DynaPS_MOD_constants arg
                                File containing top level constants that
                                control the the DynaPS module (default: "")
      --DynaPS_RC_constants arg
                                File containing rate constants that control
                                the the DynaPS module (default: "")
      --DynaPS_VARS_constants arg
                                File containing top level variables the
                                DynaPS module (default: "")
      --EPS_COND_constants arg  File containing initial conditions tracked by
                                the EPS module (default: "")
      --EPS_POOL_constants arg  File containing pool constants that control
                                the the EPS module (default: "")
      --EPS_KE_constants arg    File containing equilibrium constants that
                                control the the EPS module (default: "")
      --EPS_MOD_constants arg   File containing top level constants that
                                control the the EPS module (default: "")
      --EPS_RC_constants arg    File containing rate constants that control
                                the the EPS module (default: "")
      --EPS_VARS_constants arg  File containing top level variables the EPS
                                module (default: "")
      --FIBF_COND_constants arg
                                File containing initial conditions tracked by
                                the FIBF module (default: "")
      --FIBF_POOL_constants arg
                                File containing pool constants that control
                                the the FIBF module (default: "")
      --FIBF_KE_constants arg   File containing equilibrium constants that
                                control the the FIBF module (default: "")
      --FIBF_MOD_constants arg  File containing top level constants that
                                control the the FIBF module (default: "")
      --FIBF_RC_constants arg   File containing rate constants that control
                                the the FIBF module (default: "")
      --FIBF_VARS_constants arg
                                File containing top level variables the FIBF
                                module (default: "")
      --FI_COND_constants arg   File containing initial conditions tracked by
                                the FI module (default: "")
      --FI_POOL_constants arg   File containing pool constants that control
                                the the FI module (default: "")
      --FI_KE_constants arg     File containing equilibrium constants that
                                control the the FI module (default: "")
      --FI_MOD_constants arg    File containing top level constants that
                                control the the FI module (default: "")
      --FI_RC_constants arg     File containing rate constants that control
                                the the FI module (default: "")
      --FI_VARS_constants arg   File containing top level variables the FI
                                module (default: "")
      --PR_COND_constants arg   File containing initial conditions tracked by
                                the PR module (default: "")
      --PR_POOL_constants arg   File containing pool constants that control
                                the the PR module (default: "")
      --PR_KE_constants arg     File containing equilibrium constants that
                                control the the PR module (default: "")
      --PR_MOD_constants arg    File containing top level constants that
                                control the the PR module (default: "")
      --PR_RC_constants arg     File containing rate constants that control
                                the the PR module (default: "")
      --PR_VARS_constants arg   File containing top level variables the PR
                                module (default: "")
      --PS_COND_constants arg   File containing initial conditions tracked by
                                the PS module (default: "")
      --PS_POOL_constants arg   File containing pool constants that control
                                the the PS module (default: "")
      --PS_KE_constants arg     File containing equilibrium constants that
                                control the the PS module (default: "")
      --PS_MOD_constants arg    File containing top level constants that
                                control the the PS module (default: "")
      --PS_RC_constants arg     File containing rate constants that control
                                the the PS module (default: "")
      --PS_VARS_constants arg   File containing top level variables the PS
                                module (default: "")
      --PS_PR_COND_constants arg
                                File containing initial conditions tracked by
                                the PS_PR module (default: "")
      --PS_PR_POOL_constants arg
                                File containing pool constants that control
                                the the PS_PR module (default: "")
      --PS_PR_KE_constants arg  File containing equilibrium constants that
                                control the the PS_PR module (default: "")
      --PS_PR_MOD_constants arg
                                File containing top level constants that
                                control the the PS_PR module (default: "")
      --PS_PR_RC_constants arg  File containing rate constants that control
                                the the PS_PR module (default: "")
      --PS_PR_VARS_constants arg
                                File containing top level variables the PS_PR
                                module (default: "")
      --RA_COND_constants arg   File containing initial conditions tracked by
                                the RA module (default: "")
      --RA_POOL_constants arg   File containing pool constants that control
                                the the RA module (default: "")
      --RA_KE_constants arg     File containing equilibrium constants that
                                control the the RA module (default: "")
      --RA_MOD_constants arg    File containing top level constants that
                                control the the RA module (default: "")
      --RA_RC_constants arg     File containing rate constants that control
                                the the RA module (default: "")
      --RA_VARS_constants arg   File containing top level variables the RA
                                module (default: "")
      --RROEA_COND_constants arg
                                File containing initial conditions tracked by
                                the RROEA module (default: "")
      --RROEA_POOL_constants arg
                                File containing pool constants that control
                                the the RROEA module (default: "")
      --RROEA_KE_constants arg  File containing equilibrium constants that
                                control the the RROEA module (default: "")
      --RROEA_MOD_constants arg
                                File containing top level constants that
                                control the the RROEA module (default: "")
      --RROEA_RC_constants arg  File containing rate constants that control
                                the the RROEA module (default: "")
      --RROEA_VARS_constants arg
                                File containing top level variables the RROEA
                                module (default: "")
      --RedoxReg_COND_constants arg
                                File containing initial conditions tracked by
                                the RedoxReg module (default: "")
      --RedoxReg_POOL_constants arg
                                File containing pool constants that control
                                the the RedoxReg module (default: "")
      --RedoxReg_KE_constants arg
                                File containing equilibrium constants that
                                control the the RedoxReg module (default: "")
      --RedoxReg_MOD_constants arg
                                File containing top level constants that
                                control the the RedoxReg module (default: "")
      --RedoxReg_RC_constants arg
                                File containing rate constants that control
                                the the RedoxReg module (default: "")
      --RedoxReg_VARS_constants arg
                                File containing top level variables the
                                RedoxReg module (default: "")
      --RuACT_COND_constants arg
                                File containing initial conditions tracked by
                                the RuACT module (default: "")
      --RuACT_POOL_constants arg
                                File containing pool constants that control
                                the the RuACT module (default: "")
      --RuACT_KE_constants arg  File containing equilibrium constants that
                                control the the RuACT module (default: "")
      --RuACT_MOD_constants arg
                                File containing top level constants that
                                control the the RuACT module (default: "")
      --RuACT_RC_constants arg  File containing rate constants that control
                                the the RuACT module (default: "")
      --RuACT_VARS_constants arg
                                File containing top level variables the RuACT
                                module (default: "")
      --SUCS_COND_constants arg
                                File containing initial conditions tracked by
                                the SUCS module (default: "")
      --SUCS_POOL_constants arg
                                File containing pool constants that control
                                the the SUCS module (default: "")
      --SUCS_KE_constants arg   File containing equilibrium constants that
                                control the the SUCS module (default: "")
      --SUCS_MOD_constants arg  File containing top level constants that
                                control the the SUCS module (default: "")
      --SUCS_RC_constants arg   File containing rate constants that control
                                the the SUCS module (default: "")
      --SUCS_VARS_constants arg
                                File containing top level variables the SUCS
                                module (default: "")
      --XanCycle_COND_constants arg
                                File containing initial conditions tracked by
                                the XanCycle module (default: "")
      --XanCycle_POOL_constants arg
                                File containing pool constants that control
                                the the XanCycle module (default: "")
      --XanCycle_KE_constants arg
                                File containing equilibrium constants that
                                control the the XanCycle module (default: "")
      --XanCycle_MOD_constants arg
                                File containing top level constants that
                                control the the XanCycle module (default: "")
      --XanCycle_RC_constants arg
                                File containing rate constants that control
                                the the XanCycle module (default: "")
      --XanCycle_VARS_constants arg
                                File containing top level variables the
                                XanCycle module (default: "")
      --trDynaPS_COND_constants arg
                                File containing initial conditions tracked by
                                the trDynaPS module (default: "")
      --trDynaPS_POOL_constants arg
                                File containing pool constants that control
                                the the trDynaPS module (default: "")
      --trDynaPS_KE_constants arg
                                File containing equilibrium constants that
                                control the the trDynaPS module (default: "")
      --trDynaPS_MOD_constants arg
                                File containing top level constants that
                                control the the trDynaPS module (default: "")
      --trDynaPS_RC_constants arg
                                File containing rate constants that control
                                the the trDynaPS module (default: "")
      --trDynaPS_VARS_constants arg
                                File containing top level variables the
                                trDynaPS module (default: "")
      --ALL_COND_constants arg  File containing initial conditions tracked by
                                shared by all modules (default: "")
      --ALL_POOL_constants arg  File containing pool constants that control
                                the shared by all modules (default: "")
      --ALL_KE_constants arg    File containing equilibrium constants that
                                control the shared by all modules (default: "")
      --ALL_MOD_constants arg   File containing top level constants that
                                control the shared by all modules (default: "")
      --ALL_RC_constants arg    File containing rate constants that control
                                the shared by all modules (default: "")
      --ALL_VARS_constants arg  File containing top level variables shared by
                                all modules (default: "")

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

### Running with Yggdrasil

The model can be run as part of an [yggdrasil](https://github.com/cropsinsilico/yggdrasil) integration. The steps to running ePhotosynthesis via yggdrasil can be found below

#### Install yggdrasil in development mode

Yggdrasil is under active development on the topic/cache branch to add features for calling ePhotosynthesis from python as a function so it is recommended to install this version of yggdrasil from source. A full description of development installation can be found [here](https://cropsinsilico.github.io/yggdrasil/development/general.html#dev-env-rst), but a simplified form is found below. It is recommended that you install mamba for faster dependency solutions. Directions for doing so can be found [here](https://mamba.readthedocs.io/en/latest/installation/mamba-installation.html). If you do not install mamba, replace any occurance of 'mamba' with 'conda' in the directions below:

```
conda activate [your env name here]
git clone --recurse-submodules git@github.com:cropsinsilico/yggdrasil.git
cd yggdrasil
git checkout topic/cache
git submodule init  # required because the main branch does not have the python-rapidjson submodule
git submodule update
python utils/manage_requirements.py install mamba --for-development
pip install .
cd ../  # To ensure the installed yggdrasil is used
yggconfig
yggcompile cpp
```

#### Run via yggdrasil

Then you should be able to run the ePhotosynthesis model from the command line via

```
yggrun ePhoto.yml
```

The inputs to the model are controlled in the ePhoto.yml file on the line that begins `args: [./ePhoto ...`. Much of the model's behavior can be controlled via these arguments or via the files passed as arguments. The location of the output file is determined by the value in the `outputs` section of ePhoto.yml. The default is currently `output_ygg_single.data`. If you make changes to the ePhotosynthesis source code, you should pass the `--overwrite` flag to `yggrun` the next time you call it so that ePhotosynthesis will be rebuilt.

### Running interactively in Python with Yggdrasil

If yggdrasil is installed, it can be used to load the ePhotosynthesis model as a function into Python for interactive use. The currently available inputs are CO2, PAR, ATPCost, GRNC, & SucPath. If any of the input files are modified (e.g. ePhoto.yml, InputEnzyme.txt), you will need to reload the model as a function by calling the reload method.
Make sure to call ePhoto.stop() before closing the Python process or it could leave the model running in the background.
```
from yggdrasil import import_as_function
ePhoto = import_as_function("ePhoto.yml")
ePhoto(CO2=400, PAR=339.575, ATPCost=0, GRNC=1, SucPath=False)
ePhoto(CO2=410, PAR=339.575, ATPCost=0, GRNC=1, SucPath=False)
# Changes made to input file
ePhoto.reload()
ePhoto(CO2=400, PAR=339.575, ATPCost=0, GRNC=1, SucPath=False)
ePhoto.stop()
```
