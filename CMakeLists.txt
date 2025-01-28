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

cmake_minimum_required(VERSION 3.10)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

project(ePhotosynthesisProject LANGUAGES C CXX)

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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
    set(EPHOTO_USE_SCOPED_ENUM ON)
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-g -DINCDEBUG${BUILD_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG${BUILDINC}${BUILD_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DINCDEBUG${BUILD_FLAGS}")
endif()
# Prohibit in-source build
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source build prohibited.\nCMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}\nCMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
endif("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
if(NOT CONDA_PREFIX)
  cmake_path(SET CONDA_PREFIX "$ENV{CONDA_PREFIX}")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

option(BUILD_TESTS "Build ePhotosynthesis tests.")
option(TEST_COVERAGE "Include test coverage in the tests (turns BUILD_TESTS ON)")
option(PRESERVE_TEST_OUTPUT "Don't clean up test output after the test runs")
option(WITH_ASAN "Enable Clang address sanitizer and undefined behavior sanitizer if available")
option(TEST_VALGRIND "Enable valgrind tests")
option(BUILD_DOCS "Create HTML and PDF API documentation for ePhotosynthesis (requires Doxygen)")
option(BUILD_PYTHON "Build python bindings for the library.")
option(EPHOTO_USE_SCOPED_ENUM "Build using a scoped enumerator for the ValueSet base class (enabled by default when building with MSVC)")
option(MAKE_EQUIVALENT_TO_MATLAB "Build with settings that make this model equivalent to the Matlab C3 model")

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

if(NOT EPHOTO_USE_SCOPED_ENUM)
    set(EPHOTO_USE_SCOPED_ENUM OFF)
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

if(MAKE_EQUIVALENT_TO_MATLAB)
    add_compile_options(-DMAKE_EQUIVALENT_TO_MATLAB=1)
endif()
if(EPHOTO_USE_SCOPED_ENUM)
    add_compile_options(-DEPHOTO_USE_SCOPED_ENUM=1)
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

set(EXPORT_HEADER ePhotosynthesis_export.h)
set(EXPORT_MACRO EPHOTO_API)

#########################
# Regenerate enum files #
#########################

set(ENUM_TARGET generate_enum)
set(ENUM_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/utils/generate_enum.py)
set(ENUM_UTILS ${CMAKE_CURRENT_SOURCE_DIR}/include/enums/enums_utils.hpp)
set(ENUM_GLOBAL_H ${CMAKE_CURRENT_SOURCE_DIR}/include/enums/enums.hpp)
set(ENUM_GLOBAL ${CMAKE_CURRENT_SOURCE_DIR}/src/enums/enums.cpp)
set(ENUM_HELPER ${CMAKE_CURRENT_SOURCE_DIR}/include/enums/enums_helpers.hpp)
set(UTILS_DST_H ${CMAKE_CURRENT_SOURCE_DIR}/include/enums/enums_utils.hpp)
file(GLOB ENUM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/param/*.txt")
list(APPEND ENUM_SOURCES ${ENUM_SCRIPT})
set(ENUM_BYPRODUCTS)
set(ENUM_OUTPUT ${ENUM_GLOBAL_H} ${ENUM_HELPER} ${ENUM_GLOBAL})
set(GENERATED_SOURCES ${ENUM_GLOBAL})
set(ENUM_COMMAND
    python ${ENUM_SCRIPT}
    "${CMAKE_CURRENT_SOURCE_DIR}/param"
    --ePhotosynthesis
    --api-macro-c=${EXPORT_MACRO}
    --include-file-c-source=${UTILS_DST_H}
    --include-file-global-source=${UTILS_DST_H}
    --include-file-helper=${CMAKE_CURRENT_SOURCE_DIR}/include/${EXPORT_HEADER}
    --dst-global=${ENUM_GLOBAL_H}
    --dst-global-source=${ENUM_GLOBAL}
    --root-include-dir=${CMAKE_CURRENT_SOURCE_DIR}/include
    --overwrite
)

# The last suffix must have a complete set of modules for the global
# enum to be generated correctly
foreach(fsuffix COND POOL KE MOD RC VARS VEL)
  set(ENUM_DST ${CMAKE_CURRENT_SOURCE_DIR}/src/enums/enums_${fsuffix}.cpp)
  set(ENUM_DST_H ${CMAKE_CURRENT_SOURCE_DIR}/include/enums/enums_${fsuffix}.hpp)
  list(APPEND ENUM_OUTPUT ${ENUM_DST} ${ENUM_DST_H})
  list(APPEND GENERATED_SOURCES ${ENUM_DST})
endforeach()
if(ENUM_TARGET)
  add_custom_command(
    COMMAND ${ENUM_COMMAND}
    OUTPUT ${ENUM_GLOBAL_H}
    BYPRODUCTS ${ENUM_OUTPUT}
    DEPENDS ${ENUM_SOURCES}
  )
  add_custom_target(
    ${ENUM_TARGET}
    DEPENDS ${ENUM_GLOBAL_H}
  )
else()
  add_custom_command(
    COMMAND ${ENUM_COMMAND}
    OUTPUT ${ENUM_OUTPUT}
    DEPENDS ${ENUM_SOURCES}
  )
endif()


########################
# Library & Executable #
########################

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(APPEND SOURCES ${GENERATED_SOURCES})

add_library(EPhotosynthesis SHARED ${SOURCES})
if(ENUM_TARGET)
  add_dependencies(EPhotosynthesis ${ENUM_TARGET})
endif()
add_executable(ePhoto bin/ePhotosynthesis.cpp)

find_package(Boost 1.36.0 REQUIRED COMPONENTS regex)
# Find MPI
find_package(MPI COMPONENTS C CXX)

find_package(SUNDIALS REQUIRED CONFIG COMPONENTS kinsol cvode)
if (NOT SUNDIALS_LIBRARIES)
  set(SUNDIALS_LIBRARIES SUNDIALS::kinsol SUNDIALS::cvode)
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

target_link_libraries(ePhoto PRIVATE EPhotosynthesis)


###########################
# Generate exports header #
###########################

include(GenerateExportHeader)
generate_export_header(
    EPhotosynthesis
    BASE_NAME ${EXPORT_MACRO}
    EXPORT_MACRO_NAME ${EXPORT_MACRO}
    EXPORT_FILE_NAME ${EXPORT_HEADER}
    DEPRECATED_MACRO_NAME ${EXPORT_MACRO}_DEPRECATED
    NO_DEPRECATED_MACRO_NAME ${EXPORT_MACRO}_NO_DEPRECATED
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
    COMPONENT CXX
)
install(
    EXPORT ePhotosynthesisTargets
    FILE ePhotosynthesisTargets.cmake
    NAMESPACE ePhotosynthesis::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ePhotosynthesis
    COMPONENT CXX
)
install(
    DIRECTORY include/
    DESTINATION ${EPHOTOSYNTHESIS_INSTALL_INCLUDEDIR}
    COMPONENT CXX
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
    COMPONENT CXX
)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${EXPORT_HEADER}
    DESTINATION ${EPHOTOSYNTHESIS_INSTALL_INCLUDEDIR}
    COMPONENT CXX
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
        COMPONENT docs
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
      set(SHOW_NAME ${TEST_NAME}_SHOW)
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
        NAME ${SHOW_NAME}_EXPECTED COMMAND
        ${CMAKE_COMMAND} -E cat ${EXPECTED_OUTPUT}
      )
      set_tests_properties(
        ${SHOW_NAME}_EXPECTED PROPERTIES
        DEPENDS ${TEST_NAME}
      )
      add_test(
        NAME ${SHOW_NAME}_ACTUAL COMMAND
        ${CMAKE_COMMAND} -E cat ${ACTUAL_OUTPUT}
      )
      set_tests_properties(
        ${SHOW_NAME}_ACTUAL PROPERTIES
        DEPENDS ${TEST_NAME}
      )
      add_test(
        NAME ${CHECK_NAME} COMMAND
        ${CMAKE_COMMAND} -E compare_files ${EXPECTED_OUTPUT} ${ACTUAL_OUTPUT}
      )
      set_tests_properties(
        ${CHECK_NAME} PROPERTIES
        DEPENDS "${TEST_NAME};${SHOW_NAME}_EXPECTED;${SHOW_NAME}_ACTUAL"
      )
    endforeach()
    if(NOT PRESERVE_TEST_OUTPUT)
      add_test(
        NAME ${ARGS_PREFIX}ePhoto_cleanup COMMAND
        ${CMAKE_COMMAND} -E remove ${ePhoto_TEST_ACTUAL}
      )
      list(JOIN ePhoto_TESTS_CHECK "\\;" ePhoto_TESTS_CHECK)
      set_tests_properties(
        ${ARGS_PREFIX}ePhoto_cleanup PROPERTIES
        DEPENDS ${ePhoto_TESTS_CHECK}
      )
    endif()
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
    set(PYTHON_PACKAGE_NAME ePhotosynthesis)
    set(PYTHON_LIBRARY_NAME pyPhotosynthesis)
    message(STATUS "CONDA_PREFIX = ${CONDA_PREFIX}")
    if(NOT Python_ROOT_DIR AND CONDA_PREFIX)
      set(Python_ROOT_DIR ${CONDA_PREFIX})
    endif()
    if(Python_ROOT_DIR AND NOT Python_FIND_STRATEGY)
      set(Python_FIND_STRATEGY LOCATION)
    endif()
    if (NOT DEFINED Python_FIND_REGISTRY)
      set(Python_FIND_REGISTRY "LAST")
    endif()
    if (NOT DEFINED Python_FIND_FRAMEWORK)
      set(Python_FIND_FRAMEWORK "LAST")
    endif()
    find_package(
        Python REQUIRED COMPONENTS
        Interpreter Development.Module
    )
    find_package(
        Boost 1.36.0 REQUIRED COMPONENTS
        regex python${Python_VERSION_MAJOR}${Python_VERSION_MINOR}
    )
    add_library(${PYTHON_LIBRARY_NAME} MODULE ${SOURCES})
    if(ENUM_TARGET)
        add_dependencies(${PYTHON_LIBRARY_NAME} ${ENUM_TARGET})
    endif()
    target_link_libraries(
        ${PYTHON_LIBRARY_NAME}
        ${Boost_LIBRARIES}
        Python::Module
        ${SUNDIALS_LIBRARIES}
    )
    target_include_directories(
        ${PYTHON_LIBRARY_NAME} PUBLIC
        ${Boost_INCLUDE_DIRS}
        ${Python_INCLUDE_DIRS}
        ${SUNDIALS_INCLUDE_DIRS}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include/ePhotosynthesis>
    )
    target_compile_options(
        ${PYTHON_LIBRARY_NAME} PRIVATE -DBUILD_PYTHON
        -DPYTHON_LIBRARY_NAME=${PYTHON_LIBRARY_NAME}
    )
    set_target_properties(${PYTHON_LIBRARY_NAME} PROPERTIES PREFIX "")

    # Tests (requires the Python package actually be installed)
    add_test(NAME python_tests
             COMMAND pytest -svx ${PROJECT_SOURCE_DIR}/tests/python)

    # Installation
    # set(PYTHON_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
    cmake_path(
        APPEND CMAKE_CURRENT_BINARY_DIR ${PYTHON_PACKAGE_NAME}
        OUTPUT_VARIABLE PYTHON_BUILD_DIR
    )
    cmake_path(
        APPEND Python_SITEARCH ${PYTHON_PACKAGE_NAME}
        OUTPUT_VARIABLE PYTHON_INSTALL_DIR
    )
    message(STATUS "Python_SITEARCH = ${Python_SITEARCH}")
    message(STATUS "PYTHON_PACKAGE_NAME = ${PYTHON_PACKAGE_NAME}")
    message(STATUS "PYTHON_INSTALL_DIR = ${PYTHON_INSTALL_DIR}")
    message(STATUS "PYTHON_BUILD_DIR = ${PYTHON_BUILD_DIR}")
    install(
        TARGETS ${PYTHON_LIBRARY_NAME}
        DESTINATION "${PYTHON_INSTALL_DIR}"
        COMPONENT Python
    )
    set_target_properties(
        ${PYTHON_LIBRARY_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PYTHON_BUILD_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PYTHON_BUILD_DIR}
    )
    configure_file(
        cmake/__init__.py.in
        ${PYTHON_BUILD_DIR}/__init__.py @ONLY
    )
    install(
        FILES ${PYTHON_BUILD_DIR}/__init__.py
        DESTINATION "${PYTHON_INSTALL_DIR}"
        COMPONENT Python
    )
endif()
