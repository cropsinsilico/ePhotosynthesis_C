# **********************************************************************************************************************************************
# *   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
# *
# * CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
# * China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
# * University of Illinois at Urbana Champaign
# * Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.
# *
# * Converted from Matlab to C++ by Douglas N. Friedel, National Center for Supercomputing Applications (2020)
# *
# *   This file is part of e-photosynthesis.
# *
# *    e-photosynthesis is free software; you can redistribute it and/or modify
# *    it under the terms of the GNU General Public License as published by
# *    the Free Software Foundation;
# *
# *    e-photosynthesis is distributed in the hope that it will be useful,
# *    but WITHOUT ANY WARRANTY; without even the implied warranty of
# *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# *    GNU General Public License for more details.
# *
# *    You should have received a copy of the GNU General Public License (GPL)
# *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
# *
# **********************************************************************************************************************************************
#
# Check for the needed Sundials components.
# Since only parts of the Sundials package are needed, only specific files are looked for

include(FindPackageHandleStandardArgs)

if(DEFINED ENV{SUNDIALS_INCLUDE_DIR})
    set(SUNDIALS_INCLUDE_DIR "$ENV{SUNDIALS_INCLUDE_DIR}")
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

if(DEFINED ENV{SUNDIALS_DIR})
    set(SUNLINSOL_LIBRARY_DIR "$ENV{SUNDIALS_DIR}")
endif()

set(NVECSERIAL_LIB_NAMES sundials_nvecserial SUNDIALS_NVECSERIAL)
find_library(NVECSERIAL_LIBRARY
    NAMES ${NVECSERIAL_LIB_NAMES}
    PATHS ${SUNLINSOL_LIBRARY_DIR}
    )
set(KINSOL_LIB_NAMES sundials_kinsol SUNDIALS_KINSOL)
find_library(KINSOL_LIBRARY
    NAMES ${KINSOL_LIB_NAMES}
    PATHS ${SUNLINSOL_LIBRARY_DIR}
    )
set(CVODE_LIB_NAMES sundials_cvode SUNDIALS_CVODE)
find_library(CVODE_LIBRARY
    NAMES ${CVODE_LIB_NAMES}
    PATHS ${SUNLINSOL_LIBRARY_DIR}
    )
find_package_handle_standard_args(Sundials DEFAULT_MSG
    SUNMATH_INCLUDE_DIR
    SUNMATRIX_INCLUDE_DIR
    SUNTYPES_INCLUDE_DIR
    SUNLINSOL_INCLUDE_DIR
    CVODE_INCLUDE_DIR
    CVODE_DIRECT_INCLUDE_DIR
    NVECTOR_INCLUDE_DIR
    KINSOL_INCLUDE_DIR
    NVECSERIAL_LIBRARY
    KINSOL_LIBRARY
    CVODE_LIBRARY
    )

if(Sundials_FOUND)
    set(SUNDIALS_INCLUDE_DIRS ${SUNMATH_INCLUDE_DIR}
        ${SUNMATRIX_INCLUDE_DIR}
        ${SUNTYPES_INCLUDE_DIR}
        ${SUNLINSOL_INCLUDE_DIR}
        ${CVODE_INCLUDE_DIR}
        ${CVODE_DIRECT_INCLUDE_DIR}
        ${NVECTOR_INCLUDE_DIR}
        ${KINSOL_INCLUDE_DIR})
    set(SUNDIALS_LIBRARIES ${NVECSERIAL_LIBRARY} ${KINSOL_LIBRARY} ${CVODE_LIBRARY})

    MARK_AS_ADVANCED(
        SUNMATH_INCLUDE_DIR
        SUNMATRIX_INCLUDE_DIR
        SUNTYPES_INCLUDE_DIR
        SUNLINSOL_INCLUDE_DIR
        CVODE_INCLUDE_DIR
        CVODE_DIRECT_INCLUDE_DIR
        NVECTOR_INCLUDE_DIR
        KINSOL_INCLUDE_DIR
        NVECSERIAL_LIBRARY
        KINSOL_LIBRARY
        CVODE_LIBRARY
        )
    message(STATUS "Searching for Sundials libraries... ok")
else()
    message(FATAL_ERROR "Searching for Sundials libraries... failed")
endif()

