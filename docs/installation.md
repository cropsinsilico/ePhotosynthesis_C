Installation   {#install}
============

## Downloading ##

The ePhotosynthesis code base can be downloaded from github: [github.com/cropsinsilico/ePhotosynthesis_C](https://github.com/cropsinsilico/ePhotosynthesis_C)

    git clone https://github.com/cropsinsilico/ePhotosynthesis_C

@section prereq Prerequisites

The following packages are required to build the ePhotosynthesis code:
- C++ Compiler ([gcc](https://gcc.gnu.org/), [clang](https://clang.llvm.org/), etc.)
- [CMake](https://clang.llvm.org/) version 3.5 or newer
- [Boost](https://www.boost.org/) version 1.36.0 or newer
- [Sundials](https://computing.llnl.gov/projects/sundials)
- [Doxygen](https://www.doxygen.nl/index.html) only required for building documentation

@section building Building

ePhotosynthesis uses CMake to configure the build and make to do the compilation and installation. To do a basic build run the following:

    cd ePhotosynthesis_C
    mkdir build
    cd build
    cmake ..
    make
    make install (if desired)

To build the documentation (assuming you are in the build directory):

    cmake .. -DBUILD_DOCS=ON
    make docs
    
This will place the documentation in the doc directory.

@subsection cliargs Command line options 

The build can be asked to do specific things, or look for packages in non-standard locations by passing arguments on the command line to CMake.In addition to the standard CMake command line options the following options are also available.

| Argument | Values | Description |
| -------- | ------ | ----------- |
| <b><center>Build Types</center></b> |||
| CMAKE_BUILD_TYPE | Release, Debug | What type of build to do. \b Release will build a compact version that is best for production. \b Debug will build a larger output which includes debugging info and the ability to print out the intermediate data. Default is \b Release. |
| BUILD_DOCS | ON, OFF | Whether to enable the building of the documentation. This requires Doxygen and other packages depending on the output type. Default is \b OFF. |
| BUILD_LIBRARY | ON, OFF | Whether to build the code as a library (\b ON) or as a binary/executable (\b OFF). Default is \b OFF. |
| <b><center>Boost</center></b> |||
| Boost_DIR | | The directory containing a CMake configuration file for Boost.
| Boost_INCLUDE_DIR | | Path to the Boost include files. Usually automatically found by CMake.
| <b><center>Sundials</center></b> |||
| SUNTYPES_INCLUDE_DIR | | Path to the respective include files. Usually automatically found by CMake. |
| SUNLINSOL_INCLUDE_DIR | | ^ |
| SUNMATH_INCLUDE_DIR | | ^ |
| SUNMATRIX_INCLUDE_DIR | | ^ |
| NVECTOR_INCLUDE_DIR | | ^ |
| KINSOL_INCLUDE_DIR | | ^ |
| CVODE_DIRECT_INCLUDE_DIR | | ^ |
| CVODE_INCLUDE_DIR | | ^ |
| CVODE_LIBRARY | | The file name of the CVODE library (full path). Usually automatically found by CMake. |
| KINSOL_LIBRARY | | The file name of the KINSOL library (full path). Usually automatically found by CMake. |
| NVECSERIAL_LIBRARY | | The file name of the NVECSERIAL library (full path). Usually automatically found by CMake. |
| <b><center>Doxygen</center></b> |||
| DOXYGEN_EXECUTABLE | | Doxygen documentation generation tool (full path). |
| DOXYGEN_DOT_EXECUTABLE | | Dot tool for use with Doxygen (full path). |
| DVIPDF_CONVERTER | | File name of the dvipdf program (full path). Usually automatically found by CMake. |
| DVIPS_CONVERTER | | File name of the dvips program (full path). Usually automatically found by CMake. |
| HTLATEX_COMPILER | | File name of the htlatex program (full path). Usually automatically found by CMake. |
| LATEX2HTML_CONVERTER | | File name of the latex2html program (full path). Usually automatically found by CMake. |
| LATEX_COMPILER | | File name of the latex program (full path). Usually automatically found by CMake. |
| MAKEINDEX_COMPILER | | File name of the makeindex program (full path). Usually automatically found by CMake. |
| PDFLATEX_COMPILER | | File name of the pdflatex program (full path). Usually automatically found by CMake. |
| PDFTOPS_CONVERTER | | File name of the pdf2ps program (full path). Usually automatically found by CMake. |
| PS2PDF_CONVERTER | | File name of the ps2pdf program (full path). Usually automatically found by CMake. |
| XELATEX_COMPILER | | File name of the xelatex program (full path). Usually automatically found by CMake. |
| XINDY_COMPILER | | File name of the xindy program (full path). Usually automatically found by CMake. |
