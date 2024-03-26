# ePhotosynthesis

This is a C++ port of the ePhotosynthesis Matlab model code. The code is built with
*CMake*.

The Matlab origin code is tagged [C++ conversion](https://github.com/cropsinsilico/ePhotosynthesis/releases/tag/1.0.0). The scripts
directory contains the python script used for the initial conversion and the README file details the overall conversion process.
The script is provided for convenience and is not used in the build process.

### Prerequisites:
- [Boost](https://www.boost.org/) (1.36 or newer) - only the algorithm headers are needed
- [Sundials>=6.7.0](https://computing.llnl.gov/projects/sundials) - Only the following modules are used:
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
