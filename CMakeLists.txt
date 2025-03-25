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

cmake_minimum_required(VERSION 3.15)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

project(ePhotosynthesisProject LANGUAGES C CXX)

set(PACKAGE_NAME "ePhotosynthesis")
set(PACKAGE_STRING "ePhotosynthesis 1.0.0")

set(PACKAGE_VERSION_MAJOR "1")
set(PACKAGE_VERSION_MINOR "0")
set(PACKAGE_VERSION_PATCH "1")
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

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

include(SearchTools)
find_package_python(REQUIRED COMPONENTS Interpreter VERBOSE)

option(BUILD_CXX "Build CXX library & executable." ON)
option(BUILD_TESTS "Build ePhotosynthesis tests.")
option(TEST_COVERAGE "Include test coverage in the tests (turns BUILD_TESTS ON)")
option(PRESERVE_TEST_OUTPUT "Don't clean up test output after the test runs")
option(WITH_ASAN "Enable Clang address sanitizer and undefined behavior sanitizer if available")
option(TEST_VALGRIND "Enable valgrind tests")
option(BUILD_DOCS "Create HTML and PDF API documentation for ePhotosynthesis (requires Doxygen)")
option(BUILD_PYTHON "Build python bindings for the library.")
option(EPHOTO_USE_SCOPED_ENUM "Build using a scoped enumerator for the ValueSet base class (enabled by default when building with MSVC)")
option(MAKE_EQUIVALENT_TO_MATLAB "Build with settings that make this model equivalent to the Matlab C3 model")
option(WITH_YGGDRASIL "Compile with yggdrasil enabled")
option(INSTALL_PREFIX_PYTHON "Directory where the Python package should be installed. Can be absolute or relative to CMAKE_INSTALL_PREFIX." ".")
option(BUILD_WITH_YGGINTERFACE "If WITH_YGGDRASIL specified, build against Yggdrasil sources directly instead of linking")

if(SKBUILD)
    set(BUILD_CXX OFF)
    set(BUILD_PYTHON ON)
    set(BUILD_TESTS OFF)
    set(BUILD_DOCS OFF)
    if(NOT SKBUILD STREQUAL "2")
        message(FATAL_ERROR "Python interface must be built using scikit-build-core")
    endif()
endif()

if(NOT BUILD_CXX)
    set(BUILD_CXX OFF)
else()
    set(BUILD_CXX ON)
endif()

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
set(ENUM_STAMP ${CMAKE_CURRENT_BINARY_DIR}/enum_created.txt)
set(UTILS_DST_H ${CMAKE_CURRENT_SOURCE_DIR}/include/enums/enums_utils.hpp)
file(GLOB ENUM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/param/*.txt")
list(APPEND ENUM_SOURCES ${ENUM_SCRIPT})
set(ENUM_BYPRODUCTS)
set(ENUM_OUTPUT ${ENUM_GLOBAL_H} ${ENUM_HELPER} ${ENUM_GLOBAL})
set(GENERATED_SOURCES ${ENUM_GLOBAL})
set(ENUM_COMMAND
    ${Python_EXECUTABLE} ${ENUM_SCRIPT}
    "${CMAKE_CURRENT_SOURCE_DIR}/param"
    --ePhotosynthesis
    --api-macro-c=${EXPORT_MACRO}
    --include-file-c-source=${UTILS_DST_H}
    --include-file-global-source=${UTILS_DST_H}
    --include-file-helper=${CMAKE_CURRENT_SOURCE_DIR}/include/definitions.hpp
    --dst-global=${ENUM_GLOBAL_H}
    --dst-global-source=${ENUM_GLOBAL}
    --root-include-dir-source=${CMAKE_CURRENT_SOURCE_DIR}/include
    --root-include-dir-header=${CMAKE_CURRENT_SOURCE_DIR}/include/enums
    --timestamp-file=${ENUM_STAMP}
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
    OUTPUT ${ENUM_STAMP}
    # OUTPUT ${ENUM_GLOBAL_H}
    # BYPRODUCTS ${ENUM_OUTPUT}
    DEPENDS ${ENUM_SOURCES}
  )
  add_custom_target(
    ${ENUM_TARGET}
    DEPENDS ${ENUM_STAMP}
    # DEPENDS ${ENUM_GLOBAL_H}
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

file(GLOB_RECURSE ePhoto_SOURCES "src/*.cpp")
list(APPEND ePhoto_SOURCES ${GENERATED_SOURCES})

set(ePhoto_DEPENDENCIES)
set(ePhoto_COMPILE_OPTIONS)
set(ePhoto_COMPILE_DEFINITIONS)
set(ePhoto_PRIVATE_LIBRARIES)
set(ePhoto_PRIVATE_LIBRARIES_C)
set(ePhoto_PRIVATE_LIBRARIES_Python)
set(ePhoto_PUBLIC_LIBRARIES)
set(ePhoto_INCLUDE_DIRECTORIES)
set(ePhoto_PROPERTIES)
set(ePhoto_RPATHS)

if(ENUM_TARGET)
  list(APPEND ePhoto_DEPENDENCIES ${ENUM_TARGET})
endif()

# Find MPI
find_package(MPI COMPONENTS C CXX)

find_package(SUNDIALS REQUIRED CONFIG COMPONENTS kinsol cvode)
if (NOT SUNDIALS_LIBRARIES)
  set(SUNDIALS_LIBRARIES SUNDIALS::kinsol SUNDIALS::cvode)
endif()
message(STATUS "SUNDIALS_LIBRARIES = ${SUNDIALS_LIBRARIES}")
message(STATUS "SUNDIALS_INCLUDE_DIRS = ${SUNDIALS_INCLUDE_DIRS}")

if(WIN32)
    # list(APPEND ePhoto_PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    # Required for more than 127 macro parameters
    list(APPEND ePhoto_COMPILE_OPTIONS /Zc:preprocessor)
endif()

if(WITH_YGGDRASIL)
    # The next release of yggdrasil will install a proper cmake target,
    # but until then, if the build environment is isolated, the yggdrasil
    # interface library needs to be built and installed alongside the
    # ePhotosynthesis Python wrappers
    find_package(YggInterface COMPONENTS CXX)
    if(YggInterface_FOUND)
        set(BUILD_WITH_YGGINTERFACE OFF)
    else()
        include(AddYggInterface)
        set(YGGDRASIL_ARGS)
        set(SKBUILD_ISOLATED)
        if(SKBUILD)
          get_yggdrasil_root(YGGDRASIL_ROOT)
          cmake_path(
            IS_PREFIX Python_SITEARCH "${YGGDRASIL_ROOT}"
            yggdrasil_in_sitearch
          )
          if(NOT yggdrasil_in_sitearch)
            set(SKBUILD_ISOLATED ON)
          endif()
        endif()
        if(SKBUILD_ISOLATED)
          set(BUILD_WITH_YGGINTERFACE ON)
        endif()
        if(BUILD_WITH_YGGINTERFACE)
          message(STATUS "Building with YggInterface source files")
          set(YGGDRASIL_PYTHON_PREFIX YggInterface)
          add_yggdrasil_interfaces_backwards(
            INSTALL_PREFIX ${YGGDRASIL_PYTHON_PREFIX}
            INSTALL_LIBDIR ${YGGDRASIL_PYTHON_PREFIX}
            INSTALL_BINDIR ${YGGDRASIL_PYTHON_PREFIX}
            YGGDRASIL_ROOT ${YGGDRASIL_ROOT}
            LIBRARY_BASE yggPhoto
            DONT_INSTALL_CMAKE_PACKAGING
            DONT_INSTALL_HEADERS
            DONT_INSTALL_TARGETS
            ${YGGDRASIL_ARGS}
          )
          transfer_interface_properties(
            yggPhoto DEST_PREFIX ePhoto_
            EXCLUDE_REGEX_LINK_LIBRARIES "Python::Python"
            DESTLIST_LINK_LIBRARIES ePhoto_PRIVATE_LIBRARIES_Python
            PROPERTIES SOURCES INCLUDE_DIRECTORIES COMPILE_OPTIONS
            LINK_LIBRARIES
          )
          transfer_interface_properties(
            yggPhoto++ DEST_PREFIX ePhoto_
            PROPERTIES SOURCES INCLUDE_DIRECTORIES COMPILE_OPTIONS
          )
          # TODO: Try building YggInterface::CXX in ePhotosynthesis?
        else()
          message(STATUS "Linking to YggInterface library")
          # TODO: Handle case where yggdrasil already installed &
          #   interface library is a shared library.
          #   Add rpath to location of shared library?
          # set(YGGDRASIL_PYTHON_PREFIX "..")
          # cmake_path(
          #   APPEND YGGDRASIL_PYTHON_PREFIX yggdrasil C
          # )
          # get_rpath_directory(${YGGDRASIL_PYTHON_PREFIX} ePhoto_RPATHS APPEND)
          if(WITH_ASAN)
            list(APPEND YGGDRASIL_ARGS WITH_ASAN)
          endif()
          import_yggdrasil_interface_library(
            CXX PYTHON ${Python_EXECUTABLE} VERBOSE
            ${YGGDRASIL_ARGS}
          )
        endif()
    endif()
    if(NOT BUILD_WITH_YGGINTERFACE)
      if(BUILD_CXX)
        get_target_property(YggInterface_TYPE YggInterface::CXX TYPE)
        if(YggInterface_TYPE STREQUAL "STATIC_LIBRARY")
          transfer_interface_properties(
            YggInterface::CXX SOURCE_INTERFACE LINK_SOURCE
            DEST_PREFIX ePhoto_
            DESTLIST_LINK_LIBRARIES ePhoto_PRIVATE_LIBRARIES_C
            VERBOSE
          )
        else()
          list(APPEND ePhoto_PRIVATE_LIBRARIES_C YggInterface::CXX)
        endif()
      endif()
      if(BUILD_PYTHON)
        transfer_interface_properties(
          YggInterface::CXX SOURCE_INTERFACE LINK_SOURCE
          DEST_PREFIX ePhoto_
          EXCLUDE_REGEX_LINK_LIBRARIES ".*python.*"
          DESTLIST_LINK_LIBRARIES ePhoto_PRIVATE_LIBRARIES_Python
          VERBOSE
        )
      endif()
    endif()
endif()

list(APPEND ePhoto_PUBLIC_LIBRARIES ${SUNDIALS_LIBRARIES})
list(
    APPEND ePhoto_INCLUDE_DIRECTORIES
    ${SUNDIALS_INCLUDE_DIRS}
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>  # Export header
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/ePhotosynthesis>
)


###########################
# EPhotosynthesis Library #
###########################

set(CXX_LIBRARY_NAME EPhotosynthesis)
if(BUILD_CXX)
  add_library(${CXX_LIBRARY_NAME} SHARED ${ePhoto_SOURCES})
  if(ePhoto_DEPENDENCIES)
    add_dependencies(${CXX_LIBRARY_NAME} ${ePhoto_DEPENDENCIES})
  endif()
  if(ePhoto_COMPILE_OPTIONS)
    target_compile_options(
      ${CXX_LIBRARY_NAME} PUBLIC ${ePhoto_COMPILE_OPTIONS}
    )
  endif()
  if(ePhoto_COMPILE_DEFINITIONS)
    target_compile_definitions(
      ${CXX_LIBRARY_NAME} PUBLIC ${ePhoto_COMPILE_DEFINITIONS}
    )
  endif()
  message(STATUS "ePhoto_PRIVATE_LIBRARIES_C = ${ePhoto_PRIVATE_LIBRARIES_C}")
  if(ePhoto_PRIVATE_LIBRARIES OR ePhoto_PRIVATE_LIBRARIES_C)
    target_link_libraries(
      ${CXX_LIBRARY_NAME} PRIVATE ${ePhoto_PRIVATE_LIBRARIES}
      ${ePhoto_PRIVATE_LIBRARIES_C}
    )
  endif()
  if(ePhoto_PUBLIC_LIBRARIES)
    target_link_libraries(
      ${CXX_LIBRARY_NAME} PUBLIC ${ePhoto_PUBLIC_LIBRARIES}
    )
  endif()
  if(ePhoto_INCLUDE_DIRECTORIES)
    target_include_directories(
      ${CXX_LIBRARY_NAME} PUBLIC ${ePhoto_INCLUDE_DIRECTORIES}
    )
  endif()
  if(ePhoto_PROPERTIES)
    set_target_properties(
      ${CXX_LIBRARY_NAME} PROPERTIES ${ePhoto_PROPERTIES}
    )
  endif()
  if(ePhoto_RPATHS)
    add_library_dirs_to_rpath(
      ${CXX_LIBRARY_NAME} ADDITIONAL_DIRECTORIES ${ePhoto_RPATHS}
    )
  endif()

  add_executable(ePhoto bin/ePhotosynthesis.cpp)
  target_link_libraries(ePhoto PRIVATE ${CXX_LIBRARY_NAME})
  
endif()




##########
# Python #
##########

if(BUILD_PYTHON)
    set(PYTHON_PACKAGE_NAME ePhotosynthesis)
    set(PYTHON_LIBRARY_NAME pyPhotosynthesis)
    message(STATUS "PYTHON_PACKAGE_NAME = ${PYTHON_PACKAGE_NAME}")
    find_package(
        Python REQUIRED COMPONENTS
        Interpreter Development.Module
    )
    message(STATUS "Python_VERSION_MAJOR = ${Python_VERSION_MAJOR}")
    message(STATUS "Python_VERSION_MINOR = ${Python_VERSION_MINOR}")
    find_package(
        Boost 1.36.0 REQUIRED COMPONENTS
        python${Python_VERSION_MAJOR}${Python_VERSION_MINOR}
    )
    # Python_add_library(${PYTHON_LIBRARY_NAME} MODULE ${ePhoto_SOURCES})
    add_library(${PYTHON_LIBRARY_NAME} MODULE ${ePhoto_SOURCES})
    if(ePhoto_DEPENDENCIES)
        add_dependencies(${PYTHON_LIBRARY_NAME} ${ePhoto_DEPENDENCIES})
    endif()
    target_link_libraries(
        ${PYTHON_LIBRARY_NAME} PRIVATE
        ${ePhoto_PRIVATE_LIBRARIES}
        ${ePhoto_PUBLIC_LIBRARIES}
        ${ePhoto_PRIVATE_LIBRARIES_Python}
        ${Boost_LIBRARIES}
        Python::Module
    )
    target_include_directories(
        ${PYTHON_LIBRARY_NAME} PUBLIC
        ${ePhoto_INCLUDE_DIRECTORIES}
        ${Boost_INCLUDE_DIRS}
        ${Python_INCLUDE_DIRS}
    )
    target_compile_options(
        ${PYTHON_LIBRARY_NAME} PRIVATE -DBUILD_PYTHON
        -DPYTHON_LIBRARY_NAME=${PYTHON_LIBRARY_NAME}
        ${ePhoto_COMPILE_OPTIONS}
    )
    if(ePhoto_COMPILE_DEFINITIONS)
      target_compile_definitions(
        ${PYTHON_LIBRARY_NAME} PUBLIC ${ePhoto_COMPILE_DEFINITIONS}
      )
    endif()
    set_target_properties(
        ${PYTHON_LIBRARY_NAME} PROPERTIES
        PREFIX ""
        DEFINE_SYMBOL "${CXX_LIBRARY_NAME}_EXPORTS"
        ${ePhoto_PROPERTIES}
    )
    if(ePhoto_RPATHS)
      add_library_dirs_to_rpath(
        ${PYTHON_LIBRARY_NAME} ADDITIONAL_DIRECTORIES ${ePhoto_RPATHS}
      )
    endif()
    cmake_path(
        APPEND CMAKE_CURRENT_BINARY_DIR ${PYTHON_PACKAGE_NAME}
        OUTPUT_VARIABLE PYTHON_BUILD_DIR
    )
    message(STATUS "PYTHON_BUILD_DIR = ${PYTHON_BUILD_DIR}")
    set_target_properties(
        ${PYTHON_LIBRARY_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PYTHON_BUILD_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PYTHON_BUILD_DIR}
    )
    configure_file(
        cmake/__init__.py.in
        ${PYTHON_BUILD_DIR}/__init__.py @ONLY
    )
endif()


###########################
# Generate exports header #
###########################

include(GenerateExportHeader)
if(BUILD_CXX)
    set(EXPORT_LIBRARY ${CXX_LIBRARY_NAME})
elseif(BUILD_PYTHON)
    set(EXPORT_LIBRARY ${PYTHON_LIBRARY_NAME})
endif()
if(EXPORT_LIBRARY)
    generate_export_header(
        ${EXPORT_LIBRARY}
        BASE_NAME ${EXPORT_MACRO}
        EXPORT_MACRO_NAME ${EXPORT_MACRO}
        EXPORT_FILE_NAME ${EXPORT_HEADER}
        DEPRECATED_MACRO_NAME ${EXPORT_MACRO}_DEPRECATED
        NO_DEPRECATED_MACRO_NAME ${EXPORT_MACRO}_NO_DEPRECATED
    )
endif()


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
endif()


###########
# Install #
###########

include(GNUInstallDirs)
include(InstallTools)
cmake_path(
    APPEND CMAKE_INSTALL_INCLUDEDIR ePhotosynthesis
    OUTPUT_VARIABLE EPHOTOSYNTHESIS_INSTALL_INCLUDEDIR
)

if(BUILD_CXX)
  complete_install(
    ePhotosynthesis
    TARGETS ${CXX_LIBRARY_NAME} ePhoto
    COMPONENT CXX
    VERSION ${PACKAGE_VERSION}
    HEADER_DIR include/
    MODULES_INCLUDE cmake/FindSUNDIALS.cmake
  )
endif()

if(BUILD_PYTHON)
    if(SKBUILD)
        set(PYTHON_INSTALL_DIR ".")
    elseif(INSTALL_PREFIX_PYTHON)
        cmake_path(
          APPEND INSTALL_PREFIX_PYTHON ${PYTHON_PACKAGE_NAME}
          OUTPUT_VARIABLE PYTHON_INSTALL_DIR
        )
    else()
        set(PYTHON_INSTALL_DIR ${PYTHON_PACKAGE_NAME})
    endif()
    message(STATUS "PYTHON_INSTALL_DIR = ${PYTHON_INSTALL_DIR}")
    message(STATUS "Python_SITEARCH = ${Python_SITEARCH}")
    install(
        TARGETS ${PYTHON_LIBRARY_NAME}
        DESTINATION "${PYTHON_INSTALL_DIR}"
        COMPONENT Python
    )
    install(
        FILES ${PYTHON_BUILD_DIR}/__init__.py
        DESTINATION "${PYTHON_INSTALL_DIR}"
        COMPONENT Python
    )
endif()

if(BUILD_DOCS)
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

include(TestTools)
set(CHECK_TEST_ARGS)
if(PRESERVE_TEST_OUTPUT)
    list(APPEND CHECK_TEST_ARGS PRESERVE_TEST_OUTPUT)
endif()
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
      # if ((DRIVER_NAME STREQUAL "EPS") AND
      #     (SUNDIALS_VERSION VERSION_LESS "6.7.0"))
      #   set(EXPECTED_OUTPUT ${TEST_DATA_DIR}/ePhotoOutput_${DRIVER_NAME}_SUNDIALS_LT_6p7.txt)
      # else()
      set(EXPECTED_OUTPUT ${TEST_DATA_DIR}/ePhotoOutput_${DRIVER_NAME}.txt)
      # endif()
      set(ACTUAL_OUTPUT ${ePhoto_TEST_DIR}/output_${DRIVER_NAME}.data)
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
      check_test_output(
        ${TEST_NAME} ${EXPECTED_OUTPUT} ${ACTUAL_OUTPUT}
        ${CHECK_TEST_ARGS}
      )
    endforeach()
endfunction()

if(BUILD_TESTS AND BUILD_CXX)
    message(STATUS "Building CXX tests")
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
                --gcov-ignore-parse-errors=suspicious_hits.warn_once_per_file
	    )
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
        else()
            set(TEST_COVERAGE OFF)
        endif()
    endif()

    set(TEST_LIST "")
    foreach(TEST_PATH ${TEST_SOURCES})
        if(NOT TEST_PATH MATCHES ".*external.*")
            message(STATUS "TEST_PATH = ${TEST_PATH}")
            list(APPEND TEST_LIST ${TEST_PATH})
        endif()
    endforeach()
    # Build executable directly from compiled object files so that
    # non-exported symbols can still be tested by unit tests
    add_executable(testRunner ${TEST_LIST} $<TARGET_OBJECTS:${CXX_LIBRARY_NAME}>)
    target_compile_options(
      testRunner PUBLIC ${ePhoto_COMPILE_OPTIONS}
    )
    # target_link_options(
    #   testRunner PUBLIC $<TARGET_PROPERTY:${CXX_LIBRARY_NAME},LINK_OPTIONS>
    # )
    target_include_directories(
      testRunner PUBLIC ${ePhoto_INCLUDE_DIRECTORIES}
    )
    target_link_libraries(
      testRunner ${ePhoto_PUBLIC_LIBRARIES} ${ePhoto_PRIVATE_LIBRARIES}
      ${ePhoto_PRIVATE_LIBRARIES_C}
    )
    if(GTEST_FOUND)
        target_link_libraries(testRunner ${GTEST_BOTH_LIBRARIES})
    else()
        include(GoogleTest)
    endif()
    gtest_discover_tests(testRunner PROPERTIES DISCOVERY_TIMEOUT 1200)

    add_ePhoto_tests()

    if(WITH_YGGDRASIL)
        add_yggdrasil_test(
          yggrun_isolated
          ${PROJECT_SOURCE_DIR}/ePhotosynthesis_C_prebuilt.yaml
          ePhoto LIBRARY ${CXX_LIBRARY_NAME}
          EXPECTED_OUTPUT ${PROJECT_SOURCE_DIR}/tests/data/yggrunOutput.txt
          ACTUAL_OUTPUT ${PROJECT_SOURCE_DIR}/output_ygg_multiple_prebuilt.data
          ${CHECK_TEST_ARGS}
        )
    endif()

  if(TEST_COVERAGE)
      # setup_target_for_coverage_lcov(
      #     NAME coverage
      #     EXECUTABLE testRunner
      #     DEPENDENCIES testRunner ${CXX_LIBRARY_NAME}
      # )
      setup_target_for_coverage_gcovr_html(
          NAME coverage
	  EXECUTABLE testRunner
	  DEPENDENCIES testRunner ${CXX_LIBRARY_NAME}
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

  ################
  # Python Tests #
  ################
  if(BUILD_PYTHON)
      # Requires the Python package be installed
      add_test(NAME python_tests
               COMMAND pytest -svx ${PROJECT_SOURCE_DIR}/tests/python)
  endif()

endif()
