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

option(BUILD_MESSAGES "Enable the use of outputting intermediate products, useful for debugging. Alwyas on with Debug builds")
if(BUILD_MESSAGES)
    set(BUILDINC " -DINCDEBUG")
else()
    set(BUILDINC "")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # conda-forge sundials only produces static libraries
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1 /permissive-)
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi -DINCDEBUG${BUILD_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 -DNDEBUG${BUILDINC}${BUILD_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/O2 /Zi -DINCDEBUG${BUILD_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-g -DINCDEBUG${BUILD_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG${BUILDINC}${BUILD_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DINCDEBUG${BUILD_FLAGS}")
endif()
# Prohibit in-source build
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source build prohibited.")
endif("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

option(BUILD_TESTS "Build ePhotosynthesis tests.")
option(TEST_COVERAGE "Include test coverage in the tests (turns BUILD_TESTS ON)")
option(WITH_ASAN "Enable Clang address sanitizer and undefined behavior sanitizer if available")
option(TEST_VALGRIND "Enable valgrind tests")
option(BUILD_DOCS "Create HTML and PDF API documentation for ePhotosynthesis (requires Doxygen)")
option(BUILD_PYTHON "Build python bindings for the library.")

if(NOT BUILD_PYTHON)
    set(BUILD_PYTHON OFF)
else()
    set(BUILD_PYTHON ON)
endif()

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

if(NOT WITH_ASAN)
    set(WITH_ASAN OFF)
else()
    set(WITH_ASAN ON)
endif()

if(BUILD_TESTS)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DTESTING")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "CMAKE_BUILD_TYPE not defined, defaulting to Release")
else()
    message(STATUS "Doing a ${CMAKE_BUILD_TYPE} build")
endif()

if(BUILD_DOCS)
    find_package(Doxygen)
    find_package(LATEX COMPONENTS PDFLATEX MAKEINDEX)
endif(BUILD_DOCS)

if(WITH_ASAN)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.8.0")
            message(FATAL_ERROR "GCC < 4.8 doesn't support the address sanitizer")
        else()
	    list(APPEND ASAN_COMPILE_FLAGS -fsanitize=address)
	    list(APPEND ASAN_LINK_FLAGS -fsanitize=address)
        endif()
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9.0")
            message(FATAL_ERROR "GCC < 4.9 doesn't support the undefined behavior sanitizer")
        else()
	    list(APPEND ASAN_COMPILE_FLAGS -fsanitize=undefined)
	    list(APPEND ASAN_LINK_FLAGS -fsanitize=undefined)
        endif()
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	list(APPEND ASAN_COMPILE_FLAGS -fsanitize=address)
	list(APPEND ASAN_LINK_FLAGS -fsanitize=address)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
	    list(APPEND ASAN_COMPILE_FLAGS -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error)
	    list(APPEND ASAN_LINK_FLAGS -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error)
        else()
	    list(APPEND ASAN_COMPILE_FLAGS -fsanitize=undefined)
	    list(APPEND ASAN_LINK_FLAGS -fsanitize=undefined)
        endif()
    else()
        message(FATAL_ERROR "CXX compiler ${CMAKE_CXX_COMPILER_ID} does not support address sanitizer or undefined behavior sanitizer")
    endif()
    if(ASAN_COMPILE_FLAGS)
        add_compile_options(${ASAN_COMPILE_FLAGS})
        add_link_options(${ASAN_LINK_FLAGS})
    endif()
endif()

if(NOT WIN32)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
        if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -fcolor-diagnostics")
        endif()
    endif(CCACHE_FOUND)
endif()


########################
# Library & Executable #
########################

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(APPEND SOURCES ${GENERATED_SOURCES})

add_library(EPhotosynthesis SHARED ${SOURCES})
add_executable(ePhoto bin/ePhotosynthesis.cpp)

find_package(Boost 1.36.0 REQUIRED COMPONENTS regex)
find_package(SUNDIALS REQUIRED)
if(NOT SUNDIALS_LIBRARIES)
    message(FATAL_ERROR "Failed to find sundials libraries")
endif()
message(STATUS "SUNDIALS_LIBRARIES = ${SUNDIALS_LIBRARIES}")
message(STATUS "SUNDIALS_INCLUDE_DIRS = ${SUNDIALS_INCLUDE_DIRS}")

if(WIN32)
    set_target_properties(
        EPhotosynthesis PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
    target_compile_options(
        EPhotosynthesis PUBLIC
	/Zc:preprocessor  # Required for more than 127 macro parameters
    )
endif()

target_link_libraries(
    EPhotosynthesis PUBLIC ${Boost_LIBRARIES} ${SUNDIALS_LIBRARIES}
)
target_include_directories(
    EPhotosynthesis PUBLIC ${Boost_INCLUDE_DIRS} ${SUNDIALS_INCLUDE_DIRS}
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/ePhotosynthesis>
)

target_link_libraries(ePhoto EPhotosynthesis)


###########################
# Generate exports header #
###########################

include(GenerateExportHeader)
generate_export_header(
    EPhotosynthesis
    BASE_NAME EPHOTO_API
    EXPORT_MACRO_NAME EPHOTO_API
    EXPORT_FILE_NAME ePhotosynthesis_export.h
    DEPRECATED_MACRO_NAME EPHOTO_API_DEPRECATED
    NO_DEPRECATED_MACRO_NAME EPHOTO_API_NO_DEPRECATED
)
target_include_directories(
    EPhotosynthesis PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)


###########
# Install #
###########

include(GNUInstallDirs)
cmake_path(
    APPEND CMAKE_INSTALL_INCLUDEDIR ePhotosynthesis
    OUTPUT_VARIABLE EPHOTOSYNTHESIS_INSTALL_INCLUDEDIR
)

install(
    TARGETS EPhotosynthesis ePhoto
    EXPORT ePhotosynthesisTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION EPHOTOSYNTHESIS_INSTALL_INCLUDEDIR
)
install(
    EXPORT ePhotosynthesisTargets
    FILE ePhotosynthesisTargets.cmake
    NAMESPACE ePhotosynthesis::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ePhotosynthesis
)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "ePhotosynthesisConfigVersion.cmake"
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
)
configure_file(
    cmake/ePhotosynthesisConfig.cmake.in
    ePhotosynthesisConfig.cmake @ONLY
)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/ePhotosynthesisConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ePhotosynthesisConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ePhotosynthesis
)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/ePhotosynthesis_export.h
    DESTINATION ${EPHOTOSYNTHESIS_INSTALL_INCLUDEDIR}
)
# install(
#      DIRECTORY param
#      DESTINATION ${EPHOTOSYNTHESIS_INSTALL_INCLUDEDIR}
# )

#################
# Documentation #
#################

if(BUILD_DOCS)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()
    message(STATUS "The following documentation can be built:")
    message(STATUS "   HTML...  Yes")
    set(EPHOTOROOT ${CMAKE_CURRENT_SOURCE_DIR})
    set(GENERATE_LATEX "NO")
    set(USE_PDFLATEX "NO")
    set(MAKEINDEX_CMD "")
    set(LATEX_EXE "")
    #if(LATEX_MAKEINDEX_FOUND)
    #    message(STATUS "   PDF...  Yes")
    #    set(GENERATE_LATEX "YES")
    #    set(LATEX_EXE ${LATEX_COMPILER})
    #    set(MAKEINDEX_CMD ${MAKEINDEX_COMPILER})
    #    if(LATEX_PDFLATEX_FOUND)
    #        set(USE_PDFLATEX "YES")
    #    endif(LATEX_PDFLATEX_FOUND)
    #else()
    #    message(STATUS "   PDF...  No  (LaTeX not found)")
    #endif()
    set(DOXYGEN_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/doc CACHE INTERNAL "")
    if (DOXYGEN_CHECK_MISSING)
        set(DOXYGEN_WARN_AS_ERROR YES)
    	set(DOXYGEN_EXTRACT_ALL NO)
    else()
        set(DOXYGEN_WARN_AS_ERROR NO)
    	set(DOXYGEN_EXTRACT_ALL YES)
    endif()
    set(doxyfile_in "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
    set(doxyfile "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile")
    configure_file(${doxyfile_in} ${doxyfile} @ONLY)
    #message(STATUS "Building documentation")
    message(STATUS "Documentation can be built by running make docs")
    file(MAKE_DIRECTORY ${DOXYGEN_BUILD_DIR})
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
    install(
        DIRECTORY ${DOXYGEN_BUILD_DIR}/
	DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/ephotosynthesis
	OPTIONAL
    )
endif()


#########
# Tests #
#########

function(add_ePhoto_tests)
    set(oneValueArgs PREFIX)
    set(multiValueArgs COMMAND_PREFIX)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(ePhoto_TEST_DIR ${CMAKE_CURRENT_BINARY_DIR})
    set(TEST_DATA_DIR ${PROJECT_SOURCE_DIR}/tests/data)
    set(ePhoto_TEST_DRIVERS trDynaPS DynaPS CM EPS)
    foreach(DRIVER RANGE 1 4)
      math(EXPR IDX ${DRIVER}-1)
      list(GET ePhoto_TEST_DRIVERS ${IDX} DRIVER_NAME)
      set(TEST_NAME ${ARGS_PREFIX}ePhoto_${DRIVER_NAME})
      if (DRIVER_NAME STREQUAL "DynaPS")
        # list(APPEND ADDED_ARGS --maxSubSteps 1000 --c3)
        continue()
      endif()
      set(CHECK_NAME ${TEST_NAME}_CHECK)
      set(EXPECTED_OUTPUT ${TEST_DATA_DIR}/ePhotoOutput_${DRIVER_NAME}.txt)
      set(ACTUAL_OUTPUT ${ePhoto_TEST_DIR}/output_${DRIVER_NAME}.data)
      list(APPEND ePhoto_TESTS ${TEST_NAME})
      list(APPEND ePhoto_TESTS_CHECK ${CHECK_NAME})
      list(APPEND ePhoto_TEST_EXPECTED ${EXPECTED_OUTPUT})
      list(APPEND ePhoto_TEST_ACTUAL ${ACTUAL_OUTPUT})
      add_test(
        NAME ${TEST_NAME} COMMAND ${ARGS_COMMAND_PREFIX}
        $<TARGET_FILE:ePhoto>
        -d ${DRIVER}
        --evn ${TEST_DATA_DIR}/InputEvn.txt
        --grn ${TEST_DATA_DIR}/InputGRNC.txt
        --enzyme ${TEST_DATA_DIR}/InputEnzyme.txt
        --atpcost ${TEST_DATA_DIR}/InputATPCost.txt
        --output ${ACTUAL_OUTPUT}
        ${ADDED_ARGS}
        WORKING_DIRECTORY ${ePhoto_TEST_DIR}
        COMMAND_EXPAND_LISTS
      )
      add_test(
        NAME ${CHECK_NAME} COMMAND
        ${CMAKE_COMMAND} -E compare_files ${EXPECTED_OUTPUT} ${ACTUAL_OUTPUT}
      )
      set_tests_properties(
        ${CHECK_NAME} PROPERTIES
        DEPENDS ${TEST_NAME}
      )
    endforeach()
    add_test(
      NAME ${ARGS_PREFIX}ePhoto_cleanup COMMAND
      ${CMAKE_COMMAND} -E remove ${ePhoto_TEST_ACTUAL}
    )
    list(JOIN ePhoto_TESTS_CHECK "\\;" ePhoto_TESTS_CHECK)
    set_tests_properties(
      ${ARGS_PREFIX}ePhoto_cleanup PROPERTIES
      DEPENDS ${ePhoto_TESTS_CHECK}
    )
endfunction()

if(BUILD_TESTS)
    message(STATUS "Building tests")
    enable_testing()
    find_package(GTest)
    if(GTEST_FOUND)
        include_directories(${GTEST_INCLUDE_DIRS})
        include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
    else()
        message(STATUS "GTest found... no")
        message(STATUS "   Using github GTest")
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
        set(CODE_COVERAGE_VERBOSE TRUE)
        set(COVERAGE_DIR ${CMAKE_BINARY_DIR}/coverage)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang|ARMClang")
            set(COVERAGE_LCOV_EXCLUDES */tests/* /usr/* */ssPS* */RedoxReg* */output/* */bin/* */cxxopts.hpp)
            include(CodeCoverage)
	    set(COVERAGE_COMPILER_FLAGS "${COVERAGE_COMPILER_FLAGS} -fno-inline")
	    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|ARMClang")
                set(CTEST_COVERAGE_COMMAND "llvm-cov")
                set(CTEST_COVERAGE_EXTRA_FLAGS "gcov")
		set(COVERAGE_COMPILER_FLAGS
	            "${COVERAGE_COMPILER_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
	    else()
                set(CTEST_COVERAGE_COMMAND "gcov")
                set(CTEST_COVERAGE_EXTRA_FLAGS "")
		set(COVERAGE_COMPILER_FLAGS
	            "${COVERAGE_COMPILER_FLAGS} -fno-inline-small-functions -fno-default-inline")
	    endif()
            append_coverage_compiler_flags()
  	    list(
	        APPEND GCOVR_ADDITIONAL_ARGS
		# --gcov-executable=\"${CTEST_COVERAGE_COMMAND} ${CTEST_COVERAGE_EXTRA_FLAGS}\"
		--coveralls=${COVERAGE_DIR}/coverage.info
	    )
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
    target_link_libraries(testRunner EPhotosynthesis)
    if(GTEST_FOUND)
        target_link_libraries(testRunner ${GTEST_BOTH_LIBRARIES})
    else()
        include(GoogleTest)
    endif()
    gtest_discover_tests(testRunner PROPERTIES DISCOVERY_TIMEOUT 1200)

    add_ePhoto_tests()

endif()

if(TEST_COVERAGE)
    # setup_target_for_coverage_lcov(
    #     NAME coverage
    #     EXECUTABLE testRunner
    #     DEPENDENCIES testRunner EPhotosynthesis
    # )
    setup_target_for_coverage_gcovr_html(
        NAME coverage
	EXECUTABLE testRunner
	DEPENDENCIES testRunner EPhotosynthesis
	EXCLUDE "tests/"
	BASE_DIRECTORY "${PROJECT_SOURCE_DIR}/src"
    )
endif()

if(TEST_VALGRIND)
    find_program(VALGRIND_FOUND valgrind)
    if(NOT VALGRIND_FOUND)
        message(FATAL_ERROR "valgrind could not be located")
    endif()
    add_ePhoto_tests(
        PREFIX valgrind_ COMMAND_PREFIX
        valgrind --suppressions=${CMAKE_CURRENT_SOURCE_DIR}/valgrind.supp --suppressions=${CMAKE_CURRENT_SOURCE_DIR}/valgrind-python.supp --leak-check=full --error-exitcode=1 --track-origins=yes --dsymutil=no --keep-debuginfo=yes --read-var-info=yes
    )
    # add_test(NAME valgrind_tests
    #          COMMAND valgrind --suppressions=${CMAKE_CURRENT_SOURCE_DIR}/valgrind.supp --suppressions=${CMAKE_CURRENT_SOURCE_DIR}/valgrind-python.supp --leak-check=full --error-exitcode=1 --track-origins=yes --dsymutil=no --keep-debuginfo=yes --read-var-info=yes ${CMAKE_CURRENT_BINARY_DIR}/testRunner --gtest_filter=-SIMD.*
    #          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()


##########
# Python #
##########

if(BUILD_PYTHON)
    find_package(PythonInterp 3)
    find_package(PythonLibs 3 REQUIRED)
    add_library(pyPhotosynthesis MODULE ${SOURCES})
    include_directories(${PYTHON_INCLUDE_DIRS})
    find_package(Boost 1.36.0 REQUIRED COMPONENTS regex python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR})
    target_link_libraries(pyPhotosynthesis boost_python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR})
    target_link_libraries(pyPhotosynthesis boost_regex)
    target_link_libraries(pyPhotosynthesis ${PYTHON_LIBRARIES})
    target_link_libraries(pyPhotosynthesis ${SUNDIALS_LIBRARIES})
    target_compile_options(pyPhotosynthesis PRIVATE -DBUILD_PYTHON)
    set_target_properties(pyPhotosynthesis PROPERTIES PREFIX "")
endif()
