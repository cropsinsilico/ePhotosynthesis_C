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
cmake_minimum_required(VERSION 3.5)

project(ePhotosynthesis LANGUAGES CXX)

set(PACKAGE_NAME "ePhotosynthesis")
set(PACKAGE_STRING "ePhotosynthesis 1.0.0")

set(PACKAGE_VERSION_MAJOR "1")
set(PACKAGE_VERSION_MINOR "0")
set(PACKAGE_VERSION_PATCH "0")
set(PACKAGE_VERSION_LABEL "")

if(PACKAGE_VERSION_LABEL)
  set(PACKAGE_VERSION "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}-${PACKAGE_VERSION_LABEL}")
else()
  set(PACKAGE_VERSION "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Prohibit in-source build
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source build prohibited.")
endif("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

find_package(Boost 1.36.0 REQUIRED)
include_directories(ePhotosynthesis ${Boost_INCLUDE_DIRS})

find_package(Sundials REQUIRED)
include_directories(ePhotosynthesis ${SUNDIALS_INCLUDE_DIRS})

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB SOURCES "source/*.cpp")
add_executable(ePhotosynthesis ${SOURCES})

target_link_libraries(ePhotosynthesis ${SUNDIALS_LIBRARIES})

install(TARGETS ePhotosynthesis DESTINATION bin)
