# This module is adapted from that in CADET (`<https://github.com/modsim/CADET)>`_):



# Find SUNDIALS, the SUite of Nonlinear and DIfferential/ALgebraic equation Solvers.
#
# The module will optionally accept the COMPONENTS argument. If no COMPONENTS
# are specified, then the find module will default to find all the SUNDIALS
# libraries. If one or more COMPONENTS are specified, the module will attempt to
# find the specified components.
#
# Valid components are
#   * core
#   * cvode
#   * cvodes
#   * ida
#   * idas
#   * kinsol
#   * sunlinsolklu
#   * sunlinsoldense
#   * sunlinsolspbcgs
#   * sunlinsollapackdense
#   * sunmatrixsparse
#   * sunmatrixdense
#   * sunmatrixband
#   * nvecserial
#   * nvecopenmp
#   * nvecpthreads
#
#
# On UNIX systems, this module will read the variable SUNDIALS_PREFER_STATIC_LIBRARIES
# to determine whether or not to prefer a static link to a dynamic link for SUNDIALS
# and all of it's dependencies.  To use this feature, make sure that the
# SUNDIALS_PREFER_STATIC_LIBRARIES variable is set before the call to find_package.
#
# To provide the module with a hint about where to find your SUNDIALS installation,
# you can set the environment variable SUNDIALS_ROOT. The FindSUNDIALS module will
# then look in this path when searching for SUNDIALS paths and libraries.
#
# This module will define the following variables:
#  SUNDIALS_FOUND - true if SUNDIALS was found on the system
#  SUNDIALS_INCLUDE_DIRS - Location of the SUNDIALS includes
#  SUNDIALS_LIBRARIES - Required libraries for all requested components
#  SUNDIALS_VERSION_MAJOR - Major version
#  SUNDIALS_VERSION_MINOR - Minro version
#  SUNDIALS_VERSION_PATCH - Patch level
#  SUNDIALS_VERSION - Full version string
#
# This module exports the target SUNDIALS::<component> if it was found.


find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(PKGCONFIG_SUNDIALS QUIET sundials)
else()
    set(PKGCONFIG_SUNDIALS_INCLUDE_DIRS "")
endif()


# find the SUNDIALS include directories
find_path(SUNDIALS_INCLUDE_DIR sundials_types.h
    PATHS
        ${SUNDIALS_USER_PATHS}
        ${SUNDIALS_ROOT}
        ${SUNDIALS_ROOT_DIR}
        ${PKGCONFIG_SUNDIALS_INCLUDE_DIRS}
    ENV
        SUNDIALS_ROOT
        SUNDIALS_ROOT_DIR
    PATH_SUFFIXES
        include
        include/sundials
)
cmake_path(GET SUNDIALS_INCLUDE_DIR PARENT_PATH SUNDIALS_INCLUDE_DIR_PARENT)
set(SUNDIALS_INCLUDE_DIRS
    "${SUNDIALS_INCLUDE_DIR_PARENT}"
    "${SUNDIALS_INCLUDE_DIR}"
)

# extract version
if (SUNDIALS_INCLUDE_DIR)
    file(READ "${SUNDIALS_INCLUDE_DIR}/sundials_config.h" _SUNDIALS_VERSION_FILE)

    string(FIND "${_SUNDIALS_VERSION_FILE}" "SUNDIALS_PACKAGE_VERSION" index)
    if ("${index}" LESS 0)
        # Handle versions from 3.0.0 onwards (including)
        string(REGEX REPLACE ".*#define SUNDIALS_VERSION \"([0-9]+)\\.([0-9]+)\\.([0-9]+)\".*" "\\1" SUNDIALS_VERSION_MAJOR "${_SUNDIALS_VERSION_FILE}")
        string(REGEX REPLACE ".*#define SUNDIALS_VERSION \"([0-9]+)\\.([0-9]+)\\.([0-9]+)\".*" "\\2" SUNDIALS_VERSION_MINOR "${_SUNDIALS_VERSION_FILE}")
        string(REGEX REPLACE ".*#define SUNDIALS_VERSION \"([0-9]+)\\.([0-9]+)\\.([0-9]+)\".*" "\\3" SUNDIALS_VERSION_PATCH "${_SUNDIALS_VERSION_FILE}")
        set(SUNDIALS_VERSION "${SUNDIALS_VERSION_MAJOR}.${SUNDIALS_VERSION_MINOR}.${SUNDIALS_VERSION_PATCH}")
    else()
        # Handle versions up to 2.7.0 (including)
        string(REGEX REPLACE ".*#define SUNDIALS_PACKAGE_VERSION \"([0-9]+)\\.([0-9]+)\\.([0-9]+)\".*" "\\1" SUNDIALS_VERSION_MAJOR "${_SUNDIALS_VERSION_FILE}")
        string(REGEX REPLACE ".*#define SUNDIALS_PACKAGE_VERSION \"([0-9]+)\\.([0-9]+)\\.([0-9]+)\".*" "\\2" SUNDIALS_VERSION_MINOR "${_SUNDIALS_VERSION_FILE}")
        string(REGEX REPLACE ".*#define SUNDIALS_PACKAGE_VERSION \"([0-9]+)\\.([0-9]+)\\.([0-9]+)\".*" "\\3" SUNDIALS_VERSION_PATCH "${_SUNDIALS_VERSION_FILE}")
        set(SUNDIALS_VERSION "${SUNDIALS_VERSION_MAJOR}.${SUNDIALS_VERSION_MINOR}.${SUNDIALS_VERSION_PATCH}")
    endif()
    find_path(SUNMATH_INCLUDE_DIR "sundials/sundials_math.h"
        HINTS ${SUNDIALS_INCLUDE_DIR}
        )
    find_path(SUNMATRIX_INCLUDE_DIR "sunmatrix/sunmatrix_dense.h"
        HINTS ${SUNDIALS_INCLUDE_DIR}
        )
    find_path(SUNTYPES_INCLUDE_DIR "sundials/sundials_types.h"
        HINTS ${SUNDIALS_INCLUDE_DIR}
        )
    find_path(SUNLINSOL_INCLUDE_DIR "sunlinsol/sunlinsol_dense.h"
        HINTS ${SUNDIALS_INCLUDE_DIR}
        )
    find_path(CVODE_INCLUDE_DIR "cvode/cvode.h"
        HINTS ${SUNDIALS_INCLUDE_DIR}
        )
    find_path(CVODE_DIRECT_INCLUDE_DIR "cvode/cvode_direct.h"
        HINTS ${SUNDIALS_INCLUDE_DIR}
        )
    find_path(NVECTOR_INCLUDE_DIR "nvector/nvector_serial.h"
        HINTS ${SUNDIALS_INCLUDE_DIR}
        )
    find_path(KINSOL_INCLUDE_DIR "kinsol/kinsol.h"
        HINTS ${SUNDIALS_INCLUDE_DIR}
        )
    foreach(x SUNMATH_INCLUDE_DIR
            SUNMATRIX_INCLUDE_DIR
	    SUNTYPES_INCLUDE_DIR
	    SUNLINSOL_INCLUDE_DIR
	    CVODE_INCLUDE_DIR
	    CVODE_DIRECT_INCLUDE_DIR
	    NVECTOR_INCLUDE_DIR
	    KINSOL_INCLUDE_DIR)
      if(NOT ${${x}} STREQUAL "${x}-NOTFOUND")
        list(APPEND SUNDIALS_INCLUDE_DIRS ${${x}})
      endif()
    endforeach()
endif()



# List of the valid SUNDIALS components
set(SUNDIALS_VALID_COMPONENTS
    cvode
    cvodes
    ida
    idas
    kinsol
    sunlinsolklu
    sunlinsoldense
    sunlinsolspbcgs
    sunlinsollapackdense
    sunmatrixsparse
    sunmatrixdense
    sunmatrixband
    nvecserial
    nvecopenmp
    nvecpthreads
)
if(SUNDIALS_VERSION VERSION_GREATER_EQUAL "7.0.0")
    list(APPEND SUNDIALS_VALID_COMPONENTS core)
else()
    list(APPEND SUNDIALS_VALID_COMPONENTS generic)
endif()

if (NOT SUNDIALS_FIND_COMPONENTS)
    set(SUNDIALS_WANT_COMPONENTS ${SUNDIALS_VALID_COMPONENTS})
else()
    # add the extra specified components, ensuring that they are valid.
    foreach(_COMPONENT ${SUNDIALS_FIND_COMPONENTS})
        string(TOLOWER ${_COMPONENT} _COMPONENT_LOWER)
        string(FIND _COMPONENT_LOWER "sundials_" IDX_PREFIX)
        if(IDX_PREFIX EQUAL 0)
          string(SUBSTRING _COMPONENT_LOWER 9 -1 _COMPONENT_LOWER)
        endif()
        list(FIND SUNDIALS_VALID_COMPONENTS ${_COMPONENT_LOWER} COMPONENT_LOCATION)
        if (${COMPONENT_LOCATION} EQUAL -1)
            message(FATAL_ERROR "\"${_COMPONENT_LOWER}\" is not a valid SUNDIALS component.")
        else()
            list(APPEND SUNDIALS_WANT_COMPONENTS ${_COMPONENT_LOWER})
        endif()
    endforeach()
endif()



# find the SUNDIALS libraries
foreach(LIB ${SUNDIALS_WANT_COMPONENTS})
    set(PREFIXED_LIB "sundials_${LIB}")
    if (UNIX AND SUNDIALS_PREFER_STATIC_LIBRARIES)
        # According to bug 1643 on the CMake bug tracker, this is the
        # preferred method for searching for a static library.
        # See http://www.cmake.org/Bug/view.php?id=1643.  We search
        # first for the full static library name, but fall back to a
        # generic search on the name if the static search fails.
        set(THIS_LIBRARY_SEARCH lib${PREFIXED_LIB}.a ${PREFIXED_LIB})
    else()
        set(THIS_LIBRARY_SEARCH ${PREFIXED_LIB})
    endif()

    find_library(SUNDIALS_${LIB}_LIBRARY
        NAMES ${THIS_LIBRARY_SEARCH}
        PATHS
            ${SUNDIALS_USER_PATHS}
            ${SUNDIALS_ROOT}
            ${SUNDIALS_ROOT_DIR}
            ${PKGCONFIG_SUNDIALS_INCLUDE_DIRS}
            ${SUNDIALS_INCLUDE_DIR}/..
        ENV
            SUNDIALS_ROOT
            SUNDIALS_ROOT_DIR
        PATH_SUFFIXES
            lib
            Lib
    )

    set(SUNDIALS_${LIB}_FOUND FALSE)
    if (SUNDIALS_${LIB}_LIBRARY)
        list(APPEND SUNDIALS_COMPONENTS SUNDIALS::${LIB})
        list(APPEND SUNDIALS_LIBRARIES ${SUNDIALS_${LIB}_LIBRARY})
        set(SUNDIALS_${LIB}_FOUND TRUE)
    endif()
    mark_as_advanced(SUNDIALS_${LIB}_LIBRARY)
endforeach()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SUNDIALS
    REQUIRED_VARS SUNDIALS_LIBRARIES SUNDIALS_INCLUDE_DIRS
    VERSION_VAR SUNDIALS_VERSION
    HANDLE_COMPONENTS
)

include(FeatureSummary)
set_package_properties(SUNDIALS PROPERTIES
    URL "https://computation.llnl.gov/projects/sundials"
    DESCRIPTION "SUNDIALS Suite of nonlinear and differential/algebraic equation solvers"
)

if (SUNDIALS_FOUND)
    foreach(LIB ${SUNDIALS_WANT_COMPONENTS})
        if (SUNDIALS_${LIB}_LIBRARY AND NOT TARGET SUNDIALS::${LIB})
            add_library(SUNDIALS::${LIB} UNKNOWN IMPORTED)
            set_target_properties(SUNDIALS::${LIB} PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${SUNDIALS_INCLUDE_DIRS}"
                IMPORTED_LOCATION ${SUNDIALS_${LIB}_LIBRARY}
            )
        endif()
    endforeach()
endif()


mark_as_advanced(
    SUNDIALS_LIBRARIES
    SUNDIALS_INCLUDE_DIR
    SUNDIALS_INCLUDE_DIRS
    SUNDIALS_COMPONENTS
)