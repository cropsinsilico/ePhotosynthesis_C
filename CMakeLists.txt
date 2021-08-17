# **********************************************************************************************************************************************
# *   Copyright (2020)   Douglas N Friedel
# *
# * University of Illinois at Urbana Champaign
# * National Center for Supercomputing Applications
# * Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.
# *
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

cmake_minimum_required(VERSION 3.5)

project(ePhotosynthesisProject LANGUAGES CXX)

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
set(CMAKE_CXX_FLAGS_DEBUG "-g ${BUILD_FLAGS}")
#set(CMAKE_CXX_FLAGS_DEBUG "-g -DINCDEBUG${BUILD_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG${BUILD_FLAGS}")
# Prohibit in-source build
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source build prohibited.")
endif("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

option(BUILD_TESTS "Build ePhotosynthesis tests.")
option(TEST_COVERAGE "Include test coverage in the tests (turns BUILD_TESTS ON)")
option(BUILD_DOCS "Create HTML and PDF API documentation for ePhotosynthesis (requires Doxygen)")

if(NOT BUILD_TESTS)
    set(BUILD_TESTS OFF)
else()
    set(BUILD_TESTS ON)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(NOT TEST_COVERAGE)
    set(TEST_COVERAGE OFF)
else()
    set(TEST_COVERAGE ON)
    set(BUILD_TESTS ON)
    set(CMAKE_BUILD_TYPE Debug)
endif(NOT TEST_COVERAGE)

if(BUILD_TESTS)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DTESTING")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message("CMAKE_BUILD_TYPE not defined, defaulting to Release")
else()
    message("Doing a ${CMAKE_BUILD_TYPE} build")
endif()

if(BUILD_DOCS)
    find_package(Doxygen)
    find_package(LATEX COMPONENTS PDFLATEX MAKEINDEX)
endif(BUILD_DOCS)

find_package(Boost 1.36.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Sundials REQUIRED)
include_directories(${SUNDIALS_INCLUDE_DIRS})

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(EPhotosynthesis SHARED ${SOURCES})
add_executable(ePhotosynthesis bin/ePhotosynthesis.cpp)

target_link_libraries(EPhotosynthesis ${SUNDIALS_LIBRARIES})
target_link_libraries(EPhotosynthesis boost_regex)
target_link_libraries(ePhotosynthesis EPhotosynthesis)

install(TARGETS EPhotosynthesis DESTINATION lib)
install(TARGETS ePhotosynthesis DESTINATION bin)

if(BUILD_DOCS)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()
    message("The following documentation can be built:")
    message("   HTML...  Yes")
    set(EPHOTOROOT ${CMAKE_CURRENT_SOURCE_DIR})
    set(GENERATE_LATEX "NO")
    set(USE_PDFLATEX "NO")
    set(MAKEINDEX_CMD "")
    set(LATEX_EXE "")
    if(LATEX_MAKEINDEX_FOUND)
        message("   PDF...  Yes")
        set(GENERATE_LATEX "YES")
        set(LATEX_EXE ${LATEX_COMPILER})
        set(MAKEINDEX_CMD ${MAKEINDEX_COMPILER})
        if(LATEX_PDFLATEX_FOUND)
            set(USE_PDFLATEX "YES")
        endif(LATEX_PDFLATEX_FOUND)
    else()
        message("   PDF...  No  (LaTeX not found)")
    endif()

    set(doxyfile_in "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
    set(doxyfile "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile")
    configure_file(${doxyfile_in} ${doxyfile} @ONLY)
    #message("Building documentation")
    message("Documentation can be built by running make docs")
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/doc)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()

if(BUILD_TESTS)
    enable_testing()
    find_package(GTest)
    if(GTEST_FOUND)
        include_directories(${GTEST_INCLUDE_DIRS})
    else()
        message("GTest found... no")
        message("   Using github GTest")
        include(FetchContent)
        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
        )
        FetchContent_MakeAvailable(googletest)
    endif()
    include_directories("${PROJECT_SOURCE_DIR}/tests/include")
    file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
    if(TEST_COVERAGE)
        if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
            set(COVERAGE_LCOV_EXCLUDES */tests/* /usr/* */ssPS* */RedoxReg* */output/* */bin/* */cxxopts.hpp)
            include(CodeCoverage)
            append_coverage_compiler_flags()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
        else()
            set(TEST_COVERAGE OFF)
        endif()
    endif()

    set(TEST_LIST "")
    foreach(TEST_PATH ${TEST_SOURCES})
        list(APPEND TEST_LIST ${TEST_PATH})
    endforeach()
    add_executable(testRunner ${TEST_LIST})
    target_link_libraries(testRunner gtest_main)
    target_link_libraries(testRunner EPhotosynthesis)
    if(GTEST_FOUND)
        target_link_libraries(testRunner ${GTEST_BOTH_LIBRARIES})
    else()
        include(GoogleTest)
    endif()
    gtest_discover_tests(testRunner)
endif()

if(TEST_COVERAGE)
    setup_target_for_coverage_lcov(
        NAME coverage
        EXECUTABLE testRunner
        DEPENDENCIES testRunner EPhotosynthesis)
endif()
