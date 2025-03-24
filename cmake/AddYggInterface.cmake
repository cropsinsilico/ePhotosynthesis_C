function(get_yggdrasil_config OUTPUT_VARIABLE)
  include(SearchTools)
  find_package_python(REQUIRED COMPONENTS Interpreter)
  execute_process(
    COMMAND ${Python_EXECUTABLE} -c
    "from yggdrasil import config; print(config.usr_config_file)" install
    OUTPUT_VARIABLE ${OUTPUT_VARIABLE}
  )
  string(STRIP "${${OUTPUT_VARIABLE}}" ${OUTPUT_VARIABLE})
  set(${OUTPUT_VARIABLE} ${${OUTPUT_VARIABLE}} PARENT_SCOPE)
endfunction()

function(get_yggdrasil_version OUTPUT_VARIABLE)
  include(SearchTools)
  find_package_python(REQUIRED COMPONENTS Interpreter)
  get_yggdrasil_config(YGGDRASIL_CONFIG)
  set(YGGDRASIL_CONFIG_EXISTS)
  if(EXISTS ${YGGDRASIL_CONFIG})
    set(YGGDRASIL_CONFIG_EXISTS 1)
  endif()
  execute_process(
    COMMAND ${Python_EXECUTABLE} -m yggdrasil --version install
    OUTPUT_VARIABLE YGGDRASIL_VERSION
  )
  if((NOT YGGDRASIL_CONFIG_EXISTS) AND EXISTS ${YGGDRASIL_CONFIG})
    message(FATAL_ERROR "get_yggdrasil_version caused yggdrasil to be configured (${YGGDRASIL_CONFIG})")
  endif()
  string(STRIP "${YGGDRASIL_VERSION}" YGGDRASIL_VERSION)
  string(FIND "${YGGDRASIL_VERSION}" " " LEN_SPACE)
  if(NOT LEN_SPACE EQUAL -1)
    math(EXPR LEN_SPACE "${LEN_SPACE}+1")
    string(SUBSTRING "${YGGDRASIL_VERSION}" ${LEN_SPACE} -1 YGGDRASIL_VERSION)
  endif()
  string(FIND "${YGGDRASIL_VERSION}" "." LEN_MAJOR)
  string(SUBSTRING "${YGGDRASIL_VERSION}" 0 ${LEN_MAJOR} YGGDRASIL_VERSION_MAJOR)
  set(${OUTPUT_VARIABLE} ${YGGDRASIL_VERSION} PARENT_SCOPE)
  set(${OUTPUT_VARIABLE}_MAJOR ${YGGDRASIL_VERSION_MAJOR} PARENT_SCOPE)
endfunction()

function(get_yggdrasil_root OUTPUT_VARIABLE)
  include(SearchTools)
  find_package_python(REQUIRED COMPONENTS Interpreter)
  get_yggdrasil_config(YGGDRASIL_CONFIG)
  set(YGGDRASIL_CONFIG_EXISTS)
  if(EXISTS ${YGGDRASIL_CONFIG})
    set(YGGDRASIL_CONFIG_EXISTS 1)
  endif()
  execute_process(
    COMMAND ${Python_EXECUTABLE} -c
    "import yggdrasil; print(yggdrasil.__file__)" install
    OUTPUT_VARIABLE YGGDRASIL_INIT
  )
  if((NOT YGGDRASIL_CONFIG_EXISTS) AND EXISTS ${YGGDRASIL_CONFIG})
    message(FATAL_ERROR "get_yggdrasil_root caused yggdrasil to be configured (${YGGDRASIL_CONFIG})")
  endif()
  string(STRIP "${YGGDRASIL_INIT}" YGGDRASIL_INIT)
  cmake_path(GET YGGDRASIL_INIT PARENT_PATH ${OUTPUT_VARIABLE})
  set(${OUTPUT_VARIABLE} ${${OUTPUT_VARIABLE}} PARENT_SCOPE)
endfunction()

function(install_language_dependency NAME LANGUAGE)
  if(LANGUAGE STREQUAL "Julia")
    find_yggdrasil_dependency(Julia)
    if(NOT Julia_FOUND)
      message(STATUS "Julia could not be found so dependencies cannot be installed")
      return()
    endif()
    message(STATUS "Pkg.add(\"${NAME}\")")
    execute_process(
      COMMAND ${Julia_EXECUTABLE} -e "using Pkg; Pkg.add(\"${NAME}\")"
      COMMAND_ERROR_IS_FATAL ANY
    )
    if(NAME STREQUAL CxxWrap)
      execute_process(
        COMMAND ${Julia_EXECUTABLE} -e "using CxxWrap; print(CxxWrap.prefix_path())"
	COMMAND_ERROR_IS_FATAL ANY
	OUTPUT_VARIABLE CxxWrap_PREFIX
      )
      string(STRIP ${CxxWrap_PREFIX} CxxWrap_PREFIX)
      list(APPEND CMAKE_PREFIX_PATH ${CxxWrap_PREFIX})
      set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} PARENT_SCOPE)
    endif()
  else()
    message(FATAL_ERROR "Installation of ${LANGUAGE} dependencies not support (trying to install ${NAME}")
  endif()
endfunction()

function(find_yggdrasil_dependency NAME)
  include(SearchTools)
  set(options USING_PKGCONFIG FOR_PACKAGE_CONFIG VERBOSE)
  set(oneValueArgs LANGUAGE LANGUAGE_DEPENDENCY)
  set(multiValueArgs ADDITIONAL_PROPERTIES)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if(NOT ARGS_LANGUAGE_DEPENDENCY)
    set(ARGS_LANGUAGE_DEPENDENCY ${NAME})
  endif()
  if(NAME STREQUAL "rabbitmq-c")
    set(ARGS_USING_PKGCONFIG ON)
    list(APPEND ARGS_UNPARSED_ARGUMENTS HEADER amqp.h LIBNAMES rabbitmq)
  endif()
  if(NAME STREQUAL "Python3" OR NAME STREQUAL "Python")
    if(ARGS_FOR_PACKAGE_CONFIG)
      set(Python_PREFIX ${NAME})
      list(APPEND ARGS_ADDITIONAL_PROPERTIES Python_PREFIX Python_FOUND)
    endif()
    find_package_python()
  elseif(NAME STREQUAL "ZeroMQ")
    find_package_zmq()
  elseif(ARGS_USING_PKGCONFIG)
    find_package_pkgconfig(${NAME} ${ARGS_UNPARSED_ARGUMENTS})
    if (${NAME}_CONFIG)
      include(${${NAME}_CONFIG})
      if(NAME STREQUAL "rabbitmq-c" AND (NOT ARGS_FOR_PACKAGE_CONFIG))
        set(rabbitmq-c_LIBRARY rabbitmq::rabbitmq)
      endif()
    endif()
  else()
    find_package(${NAME} ${ARGS_UNPARSED_ARGUMENTS})
  endif()
  if(ARGS_LANGUAGE AND NOT ${name}_FOUND)
    install_language_dependency(
      ${ARGS_LANGUAGE_DEPENDENCY} ${ARGS_LANGUAGE}
    )
    if(ARGS_USING_PKGCONFIG)
      list(APPEND RECURSE_TOKENS USING_PKGCONFIG)
    endif()
    if(ARGS_FOR_PACKAGE_CONFIG)
      list(APPEND RECURSE_TOKENS FOR_PACKAGE_CONFIG)
    endif()
    if(ARGS_ADDITIONAL_PROPERTIES)
      list(APPEND RECURSE_TOKENS ADDITIONAL_PROPERTIES ${ARGS_ADDITIONAL_PROPERTIES})
    endif()
    find_yggdrasil_dependency(
      ${NAME} ${RECURSE_TOKENS}
      ${ARGS_UNPARSED_ARGUMENTS}
    )
  endif()
  propagate_cmake_library_variables("^${NAME}*" ${ARGS_ADDITIONAL_PROPERTIES})
  if(ARGS_VERBOSE)
    dump_cmake_variables(REGEX "${NAME}*" VERBOSE)
  endif()
endfunction()

function(add_yggdrasil_interface LANGUAGE)
  include(SearchTools)
  set(options DISABLE_BY_DEFAULT LANGUAGE_IS_DEPENDENCY)
  set(oneValueArgs DIRECTORY DEPENDENCY LANGUAGE_DEPENDENCY)
  set(multiValueArgs DEPENDENCY_PROPERTIES)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  set(DEFAULT_ENABLED ON)
  if (NOT ARGS_DIRECTORY)
    string(TOLOWER ${LANGUAGE} ARGS_DIRECTORY)
  endif()
  if (ARGS_DISABLE_BY_DEFAULT)
    set(DEFAULT_ENABLED OFF)
  endif()
  option(BUILD_${LANGUAGE}_LIBRARY "Build the Yggdrasil ${LANGUAGE} interface library" ${DEFAULT_ENABLED})
  option(YGG_BUILD_${LANGUAGE}_TESTS "Build ${LANGUAGE} tests" OFF)
  option(YGG_${LANGUAGE}_REQUIRED "Require that the ${LANGUAGE} interface is built" OFF)
  if(SKBUILD)
    if(LANGUAGE STREQUAL "Python")
      set(BUILD_${LANGUAGE}_LIBRARY ON)
    elseif(LANGUAGE STREQUAL "CXX" AND YGG_LINK_Python_TO_CXX)
      set(BUILD_${LANGUAGE}_LIBRARY ON)
      if(ALLOW_SKBUILD_NONPython)
        message(FATAL_ERROR "ALLOW_SKBUILD_NONPython is incompatible with YGG_LINK_Python_TO_CXX")
      endif()
    elseif(NOT ALLOW_SKBUILD_NONPython)
      set(BUILD_${LANGUAGE}_LIBRARY OFF)
    endif()
    set(YGG_BUILD_TESTS OFF)
    set(YGG_BUILD_THIRDPARTY_GTEST OFF)
    if(NOT LANGUAGE STREQUAL "Python")
      set(YGG_BUILD_${LANGUAGE}_TESTS OFF)
    endif()
  endif()
  if(LANGUAGE STREQUAL "CXX")
    set(BUILD_C_LIBRARY ${BUILD_${LANGUAGE}_LIBRARY} PARENT_SCOPE)
  endif()
  list(APPEND YGG_LANGUAGES_SUPPORTED ${LANGUAGE})
  if(BUILD_${LANGUAGE}_LIBRARY AND ARGS_LANGUAGE_IS_DEPENDENCY)
    find_yggdrasil_dependency(${LANGUAGE})
    if(NOT ${LANGUAGE}_FOUND)
      message(STATUS "${LANGUAGE} library not found, ${LANGUAGE} interface will not be built")
      set(BUILD_${LANGUAGE}_LIBRARY OFF)
    endif()
  endif()
  if(ARGS_DEPENDENCY AND BUILD_${LANGUAGE}_LIBRARY)
    find_yggdrasil_dependency(
      ${ARGS_DEPENDENCY} LANGUAGE ${LANGUAGE}
      ADDITIONAL_PROPERTIES ${ARGS_DEPENDENCY_PROPERTIES}
      LANGUAGE_DEPENDENCY ${ARGS_LANGUAGE_DEPENDENCY}
    )
    if(NOT ${ARGS_DEPENDENCY}_FOUND)
      message(STATUS "${LANGUAGE} dependency ${ARGS_DEPENDENCY} not found, ${LANGUAGE} interface will not be built")
      set(BUILD_${LANGUAGE}_LIBRARY OFF)
    endif()
  endif()
  if(BUILD_${LANGUAGE}_LIBRARY)
    add_subdirectory(${ARGS_DIRECTORY})
    list(APPEND YGG_LANGUAGES_AVAILABLE ${LANGUAGE})
  elseif(YGG_${LANGUAGE}_REQUIRED)
    message(FATAL_ERROR "${LANGUAGE} interface cannot be built")
  else()
    message(STATUS "${LANGUAGE} library skipped")
  endif()
  propagate_cmake_variables(
    YGG_LANGUAGES_SUPPORTED YGG_LANGUAGES_AVAILABLE
    YGG_INSTALL_TARGETS YGG_INSTALL_INCLUDES YGG_INSTALL_CONFIG
    BUILD_${LANGUAGE}_LIBRARY YGG_BUILD_${LANGUAGE}_TESTS
    YGG_${LANGUAGE}_REQUIRED YGG_TARGET_${LANGUAGE}
    YGG_BUILD_TESTS YGG_BUILD_THIRDPARTY_GTEST
  )
  if (YGG_Fortran_MOD_DIR)
    set(YGG_Fortran_MOD_DIR "${YGG_Fortran_MOD_DIR}" PARENT_SCOPE)
  endif()
endfunction()

function(add_yggdrasil_option NAME)
  include(SearchTools)
  set(options COMM DISABLE_BY_DEFAULT NO_DEPENDENCY)
  set(oneValueArgs DEPENDENCY DEFAULT FOUND_VAR)
  set(multiValueArgs SUPPORTED_OS LIBRARIES LIBRARY_DIRS
      INCLUDE_DIRS DEFINITIONS DEPENDENCY_PROPERTIES SEARCH_ARGS)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  set(DEFAULT_ENABLED ON)
  if (ARGS_DISABLE_BY_DEFAULT)
    set(DEFAULT_ENABLED OFF)
  endif()
  option(ENABLE_${NAME} "Enable ${NAME} support" ${DEFAULT_ENABLED})
  if(ENABLE_${NAME} AND ARGS_SUPPORTED_OS)
    set(ENABLE_${NAME} OFF)
    foreach(item ${ARGS_SUPPORTED_OS})
      if (${item})
        set(ENABLE_${NAME} ON)
      endif()
    endforeach()
    if(NOT ENABLE_${NAME})
      message(STATUS "${NAME} support not available on the current OS")
    endif()
  endif()
  if((NOT ARGS_DEPENDENCY) AND (NOT ARGS_NO_DEPENDENCY))
    set(ARGS_DEPENDENCY ${NAME})
  endif()
  if(NOT ARGS_FOUND_VAR)
    set(ARGS_FOUND_VAR ${ARGS_DEPENDENCY}_FOUND)
  endif()
  if(ENABLE_${NAME})
    if(ARGS_DEPENDENCY AND NOT ${ARGS_FOUND_VAR})
      find_yggdrasil_dependency(
        ${ARGS_DEPENDENCY}
	ADDITIONAL_PROPERTIES ${ARGS_DEPENDENCY_PROPERTIES}
	${ARGS_SEARCH_ARGS}
      )
      propagate_cmake_library_variables(
        "^${ARGS_DEPENDENCY}*" ${ARGS_DEPENDENCY_PROPERTIES}
      )
      if(${ARGS_FOUND_VAR})
        message(STATUS "${ARGS_DEPENDENCY} found")
        list(APPEND YGG_INSTALL_DEPS ${ARGS_DEPENDENCY})
	if(ARGS_DEFINITIONS)
	  list(APPEND YGG_INSTALL_DEFS ${ARGS_DEFINITIONS})
	endif()
	if(ARGS_LIBRARIES)
	  list(APPEND DEPS_LIBRARIES ${ARGS_LIBRARIES})
	endif()
	if(${ARGS_DEPENDENCY}_LIBRARY)
          list(APPEND DEPS_LIBRARIES ${${ARGS_DEPENDENCY}_LIBRARY})
	  if((NOT ${ARGS_DEPENDENCY}_LIBRARY_DIR) AND
	     (EXISTS ${${ARGS_DEPENDENCY}_LIBRARY}))
	    cmake_path(
	      REMOVE_FILENAME ${ARGS_DEPENDENCY}_LIBRARY
	      OUTPUT_VARIABLE ${ARGS_DEPENDENCY}_LIBRARY_DIR)
	  endif()
	endif()
	if(ARGS_LIBRARY_DIRS)
	  list(APPEND DEPS_LIBRARY_DIRS ${ARGS_LIBRARY_DIRS})
	endif()
	if(${ARGS_DEPENDENCY}_LIBRARY_DIR)
	  list(APPEND DEPS_LIBRARY_DIRS ${${ARGS_DEPENDENCY}_LIBRARY_DIR})
	endif()
	if(ARGS_INCLUDE_DIRS)
	  list(APPEND DEPS_INCLUDE_DIRS ${ARGS_INCLUDE_DIRS})
	endif()
	if(${ARGS_DEPENDENCY}_INCLUDE_DIR)
          list(APPEND DEPS_INCLUDE_DIRS ${${ARGS_DEPENDENCY}_INCLUDE_DIR})
	endif()
      else()
        message(STATUS "NO ${ARGS_DEPENDENCY}")
	set(ENABLE_${NAME} OFF)
      endif()
    endif()
    if(ENABLE_${NAME} AND ARGS_COMM)
      list(APPEND YGG_INSTALL_DEFS -D${NAME}INSTALLED)
      list(APPEND YGG_COMMS_AVAILABLE ${NAME})
    endif()
  else(ENABLE_${NAME})
    message(STATUS "No ${NAME} support will be available")
  endif(ENABLE_${NAME})
  list(APPEND YGG_OPTIONS_SUPPORTED ${NAME})
  if(ARGS_COMM)
    list(APPEND YGG_COMMS_SUPPORTED ${NAME})
  endif()
  if((NOT ENABLE_${NAME}) AND ARGS_COMM AND (NAME STREQUAL "${DEFAULT_COMM}"))
    unset(DEFAULT_COMM CACHE)
  endif()
  propagate_cmake_variables(
    ENABLE_${NAME} YGG_INSTALL_DEPS YGG_INSTALL_DEFS YGG_INSTALL_CONFIG
    DEPS_LIBRARIES DEPS_LIBRARY_DIRS DEPS_INCLUDE_DIRS YGG_COMMS_AVAILABLE
  )
endfunction()


function(get_rpath_directory RELATIVE_DIRECTORY OUTPUT_VARIABLE)
  set(options APPEND)
  set(oneValueArgs RELATIVE_TO)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if(ARGS_RELATIVE_TO)
    cmake_path(
      RELATIVE_PATH RELATIVE_DIRECTORY "${ARGS_RELATIVE_TO}"
      OUTPUT_VARIABLE RELATIVE_DIRECTORY
    )
  endif()
  if(APPLE)
    set(rpath "@loader_path/${RELATIVE_DIRECTORY}")
  else()
    set(rpath "$ORIGIN/${RELATIVE_DIRECTORY}")
  endif()
  if(ARGS_APPEND)
    list(APPEND ${OUTPUT_VARIABLE} ${rpath})
  else()
    set(${OUTPUT_VARIABLE} ${rpath})
  endif()
  set(${OUTPUT_VARIABLE} ${${OUTPUT_VARIABLE}} PARENT_SCOPE)
endfunction()


function(add_library_dirs_to_rpath TARGET)
  set(options VERBOSE)
  set(oneValueArgs DESTINATION)
  set(multiValueArgs ADDITIONAL_DIRECTORIES)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  get_target_property(YGG_RPATH ${TARGET} INSTALL_RPATH)
  if (ARGS_DESTINATION)
    get_rpath_directory(${ARGS_DESTINATION} YGG_RPATH APPEND)
  endif()
  if (DEPS_LIBRARY_DIRS)
    list(APPEND YGG_RPATH ${DEPS_LIBRARY_DIRS})
  endif()
  if (ARGS_ADDITIONAL_DIRECTORIES)
    list(APPEND YGG_RPATH ${ARGS_ADDITIONAL_DIRECTORIES})
  endif()
  # foreach(lib ${DEPS_LIBRARIES})
  #   if(EXISTS ${lib})
  #     cmake_path(REMOVE_FILENAME lib OUTPUT_VARIABLE dir)
  #     if (NOT dir IN_LIST YGG_RPATH)
  #       list(APPEND YGG_RPATH ${dir})
  #     endif()
  #   endif()
  # endforeach()
  if(ARGS_VERBOSE)
    message(STATUS "DEPS_LIBRARY_DIRS[${TARGET}] = ${DEPS_LIBRARY_DIRS}")
    message(STATUS "DEPS_LIBRARIES[${TARGET}] = ${DEPS_LIBRARIES}")
    message(STATUS "YGG_RPATH[${TARGET}] = ${YGG_RPATH}")
  endif()
  if (YGG_RPATH)
    if(APPLE)
      set_target_properties(${TARGET} PROPERTIES MACOSX_RPATH 1)
    endif()
    set_target_properties(
      ${TARGET} PROPERTIES
      INSTALL_RPATH "${YGG_RPATH}"
      INSTALL_RPATH_USE_LINK_PATH TRUE)
  endif()
endfunction()


function(set_ygg_test_paths TEST_NAME)
  include(TestTools)
  set(oneValueArgs OUTPUT_PROPERTIES)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if(ARGS_OUTPUT_PROPERTIES)
    list(APPEND ARGS_UNPARSED_ARGUMENTS OUTPUT_PROPERTIES ${ARGS_OUTPUT_PROPERTIES})
  endif()
  set_tests_runtime_paths(
    ${TEST_NAME}
    ${ARGS_UNPARSED_ARGUMENTS}
    PREPEND
    PATHS ${YGG_TEST_PATHS}
  )
  if(ARGS_OUTPUT_PROPERTIES)
    set(${ARGS_OUTPUT_PROPERTIES} "${${ARGS_OUTPUT_PROPERTIES}}" PARENT_SCOPE)
  endif()
endfunction()


function(set_ygg_test_env TEST_NAME)
  include(TestTools)
  set(oneValueArgs OUTPUT_PROPERTIES)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if(ARGS_OUTPUT_PROPERTIES)
    list(APPEND ARGS_UNPARSED_ARGUMENTS OUTPUT_PROPERTIES ${ARGS_OUTPUT_PROPERTIES})
  endif()
  set_ygg_test_paths(
    ${TEST_NAME}
    ${ARGS_UNPARSED_ARGUMENTS}
    ADDITIONAL_ENV_VARIABLES ${YGG_TEST_ENV}
  )
  if(ARGS_OUTPUT_PROPERTIES)
    set(${ARGS_OUTPUT_PROPERTIES} "${${ARGS_OUTPUT_PROPERTIES}}" PARENT_SCOPE)
  endif()
endfunction()

function(parse_yggdrasil_cmake_config)
  set(ikey)
  set(ityp)
  set(ides)
  foreach(cfg ${YGG_INSTALL_CONFIG})
    if(ikey AND ityp AND ides)
      set(${ikey} "${cfg}" CACHE ${ityp} "${ides}")
      set(ikey)
      set(ityp)
      set(ides)
    elseif(NOT ikey)
      set(ikey "${cfg}")
    elseif(NOT ityp)
      set(ityp "${cfg}")
    elseif(NOT ides)
      set(ides "${cfg}")
    endif()
  endforeach()
endfunction()

function(add_yggdrasil_cmake_config NAME)
  set(oneValueArgs VALUE TYPE DESCRIPTION)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if(ARGS_VALUE)
    set(${NAME} ${ARGS_VALUE})
  else()
    set(ARGS_VALUE ${${NAME}})
  endif()
  if(NOT ARGS_TYPE)
    set(ARGS_TYPE STRING)
  endif()
  if(NOT ARGS_DESCRIPTION)
    set(ARGS_DESCRIPTION "${NAME}")
  endif()
  set(${NAME} "${${NAME}}" CACHE ${ARGS_TYPE} "${ARGS_DESCRIPTION}")
  list(
    APPEND YGG_INSTALL_CONFIG
    "${NAME}" "${ARGS_TYPE}" "${ARGS_DESCRIPTION}" "${${NAME}}"
  )
  set(YGG_INSTALL_CONFIG "${YGG_INSTALL_CONFIG}" PARENT_SCOPE)
endfunction()


function(add_yggdrasil_interface_backwards TARGET TARGET_TYPE LANGUAGE)
  set(options DONT_CREATE_LIBRARY VERBOSE)
  set(multiValueArgs INCLUDE_DIRECTORIES INCLUDE_SUBDIRECTORIES
      SOURCES SOURCE_DIRECTORIES SOURCE_SUBDIRECTORIES SOURCE_EXTENSIONS
      HEADERS HEADER_DIRECTORIES HEADER_SUBDIRECTORIES HEADER_EXTENSIONS
      DEPENDENCIES PROPERTIES
      PRIVATE_LIBRARIES PUBLIC_LIBRARIES COMPILE_OPTIONS)
  set(oneValueArgs DIRECTORY YGGDRASIL_ROOT SOURCE_PREFIX
      TARGETS_LIST LANGUAGES_LIST HEADERS_LIST)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if(NOT ARGS_DIRECTORY)
    set(ARGS_DIRECTORY ${LANGUAGE})
  endif()
  if(NOT ARGS_YGGDRASIL_ROOT)
    get_yggdrasil_root(ARGS_YGGDRASIL_ROOT)
  endif()
  if(NOT ARGS_SOURCE_PREFIX)
    cmake_path(GET ARGS_YGGDRASIL_ROOT PARENT_PATH ARGS_SOURCE_PREFIX)
  endif()
  if(NOT ARGS_TARGETS_LIST)
    set(ARGS_TARGETS_LIST YGG_INSTALL_TARGETS)
  endif()
  if(NOT ARGS_LANGUAGES_LIST)
    set(ARGS_LANGUAGES_LIST YGG_LANGUAGES_AVAILABLE)
  endif()
  if(NOT ARGS_HEADERS_LIST)
    set(ARGS_HEADERS_LIST YGG_INSTALL_INCLUDES)
  endif()
  cmake_path(
    APPEND ARGS_YGGDRASIL_ROOT languages ${ARGS_DIRECTORY}
    OUTPUT_VARIABLE LANGUAGE_SDIR
  )
  set(${LANGUAGE}_SDIR ${LANGUAGE_SDIR} PARENT_SCOPE)

  # Include directories
  list(
    APPEND ARGS_INCLUDE_DIRECTORIES
    $<BUILD_INTERFACE:${LANGUAGE_SDIR}>
  )
  foreach(idir ${ARGS_INCLUDE_SUBDIRECTORIES})
    cmake_path(
      APPEND LANGUAGE_SDIR ${idir} OUTPUT_VARIABLE isrcdir
    )
    list(APPEND ARGS_INCLUDE_DIRECTORIES ${isrcdir})
  endforeach()

  # Locate header files
  if(NOT ARGS_HEADER_EXTENSIONS)
    if(LANGUAGE STREQUAL "C")
      set(ARGS_HEADER_EXTENSIONS .h .hpp)
    elseif(LANGUAGE STREQUAL "CXX")
      set(ARGS_HEADER_EXTENSIONS .h .hpp)
    elseif(LANGUAGE STREQUAL "Fortran")
      set(ARGS_HEADER_EXTENSIONS)
    else()
      message(FATAL_ERROR "Cannot determine header extensions for language \"${LANGUAGE}\"")
    endif()
  endif()
  list(APPEND ARGS_HEADER_DIRECTORIES ${LANGUAGE_SDIR})
  foreach(idir ${ARGS_HEADER_SUBDIRECTORIES})
    cmake_path(
      APPEND LANGUAGE_SDIR ${idir} OUTPUT_VARIABLE isrcdir
    )
    list(APPEND ARGS_HEADER_DIRECTORIES ${isrcdir})
  endforeach()
  set(HEADER_GLOB GLOB)
  if(NOT ARGS_HEADER_SUBDIRECTORIES)
    set(HEADER_GLOB GLOB_RECURSE)
  endif()
  foreach(idir ${ARGS_HEADER_DIRECTORIES})
    foreach(iext ${ARGS_HEADER_EXTENSIONS})
      cmake_path(APPEND idir "*${iext}" OUTPUT_VARIABLE pattern)
      file(${HEADER_GLOB} tmp "${pattern}")
      list(APPEND ARGS_HEADERS ${tmp})
    endforeach()
  endforeach()

  # Locate source files
  if(NOT ARGS_SOURCE_EXTENSIONS)
    set(ARGS_SOURCE_EXTENSIONS ${CMAKE_${LANGUAGE}_SOURCE_FILE_EXTENSIONS})
  endif()
  list(APPEND ARGS_SOURCE_DIRECTORIES ${LANGUAGE_SDIR})
  foreach(idir ${ARGS_SOURCE_SUBDIRECTORIES})
    cmake_path(
      APPEND LANGUAGE_SDIR ${idir} OUTPUT_VARIABLE isrcdir
    )
    list(APPEND ARGS_SOURCE_DIRECTORIES ${isrcdir})
  endforeach()
  set(SOURCE_GLOB GLOB)
  if(NOT ARGS_SOURCE_SUBDIRECTORIES)
    set(SOURCE_GLOB GLOB_RECURSE)
  endif()
  foreach(idir ${ARGS_SOURCE_DIRECTORIES})
    foreach(iext ${ARGS_SOURCE_EXTENSIONS})
      cmake_path(APPEND idir "*${iext}" OUTPUT_VARIABLE pattern)
      file(${SOURCE_GLOB} tmp "${pattern}")
      list(APPEND ARGS_SOURCES ${tmp})
    endforeach()
  endforeach()

  # Create library
  if(ARGS_VERBOSE)
    message(STATUS "HEADERS_${LANGUAGE} = ${ARGS_HEADERS}")
    message(STATUS "SOURCES_${LANGUAGE} = ${ARGS_SOURCES}")
  endif()
  set(HEADERS_${LANGUAGE} ${ARGS_HEADERS} PARENT_SCOPE)
  set(SOURCES_${LANGUAGE} ${ARGS_SOURCES} PARENT_SCOPE)
  add_library(${TARGET} ${TARGET_TYPE} ${ARGS_SOURCES})
  add_library(YggInterface::${LANGUAGE} ALIAS ${TARGET})

  if(ARGS_DEPENDENCIES)
    add_dependencies(${TARGET} ${ARGS_DEPENDENCIES})
  endif()
  if(ARGS_COMPILE_OPTIONS)
    target_compile_options(
      ${TARGET} PUBLIC ${ARGS_COMPILE_OPTIONS}
    )
  endif()
  if(ARGS_PRIVATE_LIBRARIES)
    target_link_libraries(
      ${TARGET} PRIVATE ${ARGS_PRIVATE_LIBRARIES}
    )
  endif()
  if(ARGS_PUBLIC_LIBRARIES)
    target_link_libraries(
      ${TARGET} PUBLIC ${ARGS_PUBLIC_LIBRARIES}
    )
  endif()
  if(ARGS_INCLUDE_DIRECTORIES)
    target_include_directories(
      ${TARGET} PUBLIC ${ARGS_INCLUDE_DIRECTORIES}
    )
  endif()
  if(ARGS_PROPERTIES)
    set_target_properties(
      ${TARGET} PROPERTIES ${ARGS_PROPERTIES}
    )
  endif()

  list(APPEND ${ARGS_TARGETS_LIST} ${TARGET})
  list(APPEND ${ARGS_LANGUAGES_LIST} ${LANGUAGE})
  list(APPEND ${ARGS_HEADERS_LIST} ${ARGS_HEADERS})
  set(${ARGS_TARGETS_LIST} ${${ARGS_TARGETS_LIST}} PARENT_SCOPE)
  set(${ARGS_LANGUAGES_LIST} ${${ARGS_LANGUAGES_LIST}} PARENT_SCOPE)
  set(${ARGS_HEADERS_LIST} ${${ARGS_HEADERS_LIST}} PARENT_SCOPE)

endfunction()

function(add_yggdrasil_interfaces_backwards)
  set(options DONT_INSTALL_CMAKE_PACKAGING DONT_INSTALL_HEADERS
      VERBOSE EXCLUDE_PYTHON)
  set(oneValueArgs VERSION YGGDRASIL_ROOT
      SOURCE_PREFIX INSTALL_PREFIX LIBRARY_BASE)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if(NOT ARGS_VERSION)
    get_yggdrasil_version(ARGS_VERSION)
  endif()
  if(NOT ARGS_YGGDRASIL_ROOT)
    get_yggdrasil_root(ARGS_YGGDRASIL_ROOT)
  endif()
  if(NOT ARGS_LIBRARY_BASE)
    set(ARGS_LIBRARY_BASE ygg)
  endif()
  if(NOT ARGS_SOURCE_PREFIX)
    cmake_path(GET ARGS_YGGDRASIL_ROOT PARENT_PATH ARGS_SOURCE_PREFIX)
  endif()

  set(YGGDRASIL_REL_DIR "..")
  set(
    INTERFACE_ARGS
    YGGDRASIL_ROOT ${ARGS_YGGDRASIL_ROOT}
    SOURCE_PREFIX ${ARGS_SOURCE_PREFIX}
  )
  if(ARGS_VERBOSE)
    list(APPEND INTERFACE_ARGS VERBOSE)
  endif()

  #########################################
  # DEPENDENCIES
  #########################################

  include(SearchTools)
  set(C_DEPENDENCIES)
  set(C_INCLUDE_DIRECTORIES)
  set(C_PRIVATE_LIBRARIES)
  set(C_PUBLIC_LIBRARIES)
  set(
    C_COMPILE_OPTIONS -DWITH_YGGDRASIL
    -DYGGVER_MAJOR=${ARGS_VERSION_MAJOR}
  )
  set(C_PROPERTIES)

  ##########
  # Python #
  ##########

  find_package_python(COMPONENTS Interpreter NumPy Development)
  if(Python_FOUND)
    list(APPEND C_DEPENDENCIES Python)
    if(ARGS_EXCLUDE_PYTHON)
      list(APPEND C_PUBLIC_LIBRARIES Python::NumPy)
    else()
      list(APPEND C_DEPENDENCIES Python)
      list(APPEND C_PUBLIC_LIBRARIES Python::Python Python::NumPy)
    endif()
  else()
    list(APPEND C_COMPILE_OPTIONS -DYGGDRASIL_DISABLE_PYTHON_C_API=1)
  endif()

  #######
  # IPC #
  #######

  set(ZMQ_REQUIRED)
  if(WIN32)
    set(ZMQ_REQUIRED REQUIRED)
  else()
    list(APPEND C_COMPILE_OPTIONS -DIPCINSTALLED)
  endif()

  ########
  # CZMQ #
  ########

  find_package_pkgconfig(czmq ${ZMQ_REQUIRED} LIBRARY_TYPE SHARED)
  if(czmq_FOUND)
    list(APPEND C_DEPENDENCIES czmq)
    if(TARGET czmq)
      message(STATUS "Using czmq target")
      list(APPEND C_PUBLIC_LIBRARIES czmq)
    else()
      list(APPEND C_PUBLIC_LIBRARIES ${czmq_LIBRARY})
      list(APPEND C_INCLUDE_DIRECTORIES ${czmq_INCLUDE_DIR})
    endif()
  endif()
  if(ARGS_VERBOSE)
    dump_cmake_variables(REGEX "^czmq*" VERBOSE)
  endif()

  ##########
  # ZeroMQ #
  ##########

  find_package_zmq(${ZMQ_REQUIRED})
  if(ZeroMQ_FOUND)
    list(APPEND C_DEPENDENCIES ZeroMQ)
    if(TARGET ZeroMQ)
      message(STATUS "Using ZeroMQ target")
      list(APPEND C_PUBLIC_LIBRARIES ZeroMQ)
    else()
      list(APPEND C_PUBLIC_LIBRARIES ${ZeroMQ_LIBRARY})
      list(APPEND C_INCLUDE_DIRECTORIES ${ZeroMQ_INCLUDE_DIR})
    endif()
    list(APPEND C_COMPILE_OPTIONS -DZMQINSTALLED -DZMQDEF)
  else()
    message(STATUS "ZeroMQ could not be located, defaulting to IPC")
    list(APPEND C_COMPILE_OPTIONS -DIPCDEF)
  endif()
  if(ARGS_VERBOSE)
    dump_cmake_variables(REGEX "^ZeroMQ*" VERBOSE)
    dump_cmake_variables(REGEX "^libzmq*" VERBOSE)
  endif()

  ##########
  # OpenMP #
  ##########

  find_package(OpenMP)
  if(OpenMP_C_FOUND)
    list(APPEND C_DEPENDENCIES OpenMP)
    list(APPEND C_PRIVATE_LIBRARIES OpenMP::OpenMP_C)
  endif()

  #############
  # Rapidjson #
  #############

  if (NOT DEFAULT_RAPIDJSON_INCLUDE_DIRS)
    cmake_path(APPEND ARGS_YGGDRASIL_ROOT rapidjson include
               OUTPUT_VARIABLE DEFAULT_RAPIDJSON_INCLUDE_DIRS)
  endif()
  set(RAPIDJSON_INCLUDE_DIRS "${DEFAULT_RAPIDJSON_INCLUDE_DIRS}" CACHE PATH "Path to the rapidjson include directory containing the headers that should be used")
  message(STATUS "RAPIDJSON_INCLUDE_DIRS = ${RAPIDJSON_INCLUDE_DIRS}")
  if (NOT EXISTS ${RAPIDJSON_INCLUDE_DIRS})
    message(FATAL_ERROR "RapidJSON sources not found: if you cloned "
                        " the git repository, you should initialize"
                        " the rapidjson submodule as explained in the"
                        " README.rst; in all other cases you may"
                        " want to report the issue.")
  endif()
  cmake_path(
    IS_PREFIX ARGS_YGGDRASIL_ROOT "${RAPIDJSON_INCLUDE_DIRS}"
    RJ_IN_YGGDRASIL
  )
  list(
    APPEND C_INCLUDE_DIRECTORIES
    $<BUILD_INTERFACE:${RAPIDJSON_INCLUDE_DIRS}>
  )
  if(NOT RJ_IN_YGGDRASIL)
    list(
      APPEND C_INCLUDE_DIRECTORIES
      $<INSTALL_INTERFACE:${RAPIDJSON_INCLUDE_DIRS}>
    )
  endif()


  #########################################
  # REGEX
  #########################################

  if(WIN32)
    set(REGEX_SUFFIX win32)
    cmake_path(
      APPEND ARGS_YGGDRASIL_ROOT languages C regex regex_${REGEX_SUFFIX}.cpp
      OUTPUT_VARIABLE REGEX_CPP
    )
  else()
    set(REGEX_SUFFIX posix)
    set(REGEX_CPP)
  endif()
  cmake_path(
    APPEND ARGS_YGGDRASIL_ROOT languages C regex regex_${REGEX_SUFFIX}.h
    OUTPUT_VARIABLE REGEX_H
  )


  #########################################
  # C INTERFACE
  #########################################

  if(ARGS_VERBOSE)
    message(STATUS "LIBRARY_BASE = ${ARGS_LIBRARY_BASE}")
    message(STATUS "C_DEPENDENCIES = ${C_DEPENDENCIES}")
    message(STATUS "C_INCLUDE_DIRECTORIES = ${C_INCLUDE_DIRECTORIES}")
    message(STATUS "C_PRIVATE_LIBRARIES = ${C_PRIVATE_LIBRARIES}")
    message(STATUS "C_PUBLIC_LIBRARIES = ${C_PUBLIC_LIBRARIES}")
    message(STATUS "C_COMPILE_OPTIONS = ${C_COMPILE_OPTIONS}")
    message(STATUS "C_PROPERTIES = ${C_PROPERTIES}")
  endif()

  set(C_LIBRARY_NAME ${ARGS_LIBRARY_BASE})
  add_yggdrasil_interface_backwards(
    ${C_LIBRARY_NAME} SHARED C
    SOURCE_SUBDIRECTORIES datatypes serialize communication
    INCLUDE_SUBDIRECTORIES regex communication  # Required until include path fixed in YggInterface.h & other headers
    INCLUDE_DIRECTORIES ${C_INCLUDE_DIRECTORIES}
    SOURCES ${REGEX_CPP}
    SOURCE_EXTENSIONS ".c" ".cpp"
    PRIVATE_LIBRARIES ${C_PRIVATE_LIBRARIES}
    PUBLIC_LIBRARIES ${C_PUBLIC_LIBRARIES}
    COMPILE_OPTIONS ${C_COMPILE_OPTIONS}
    PROPERTIES ${C_PROPERTIES}
    COMPILE_OPTIONS ${C_COMPILE_OPTIONS}
    ${INTERFACE_ARGS}
  )


  #########################################
  # CXX INTERFACE
  #########################################

  set(CXX_LIBRARY_NAME ${ARGS_LIBRARY_BASE}++)
  add_yggdrasil_interface_backwards(
    ${CXX_LIBRARY_NAME} SHARED CXX DIRECTORY CPP
    SOURCE_EXTENSIONS ".cpp"
    PUBLIC_LIBRARIES ${C_LIBRARY_NAME}
    ${INTERFACE_ARGS}
  )


  #########################################
  # Fortran INTERFACE
  #########################################

  # set(Fortran_LIBRARY_NAME f${ARGS_LIBRARY_BASE})
  # add_yggdrasil_interface_backwards(
  #   ${Fortran_LIBRARY_NAME} SHARED Fortran DIRECTORY fortran
  #   SOURCE_EXTENSIONS ".cpp" ".f90"
  #   PUBLIC_LIBRARIES ${C_LIBRARY_NAME}
  #   ${INTERFACE_ARGS}
  # )


  #########################################
  # Install
  #########################################

  include(GNUInstallDirs)
  include(InstallTools)
  
  if(NOT ARGS_INSTALL_PREFIX)
    set(ARGS_INSTALL_PREFIX ".")
  endif()
  if(ARGS_DONT_INSTALL_CMAKE_PACKAGING)
    list(APPEND ARGS_UNPARSED_ARGUMENTS DONT_INSTALL_CMAKE_PACKAGING)
  else()
    list(
      APPEND ARGS_UNPARSED_ARGUMENTS
      MODULES_INCLUDE "FindJulia.cmake"
    )
  endif()
  if(ARGS_DONT_INSTALL_HEADERS)
    list(APPEND ARGS_UNPARSED_ARGUMENTS DONT_INSTALL_HEADERS)
  endif()

  # Set variables required by YggInterfaceConfig.cmake.in
  set(YGG_INSTALL_DEPS ${C_DEPENDENCIES})
  set(YGG_TARGET_BASE ${ARGS_LIBRARY_BASE})

  complete_install(
    YggInterface
    TARGETS ${YGG_INSTALL_TARGETS}
    INSTALL_PREFIX ${ARGS_INSTALL_PREFIX}
    HEADERS ${YGG_INSTALL_INCLUDES}
    ${ARGS_UNPARSED_ARGUMENTS}
  )

  if(NOT ARGS_DONT_INSTALL_CMAKE_PACKAGING)
    install(
      DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/cmake/config
      DESTINATION ${ARGS_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/cmake/YggInterface
      FILES_MATCHING REGEX ".*\\.in$"
    )
  endif()

endfunction()

function(transfer_interface_properties SOURCE_TARGET)
  set(options SOURCE_INTERFACE DEST_INTERFACE VERBOSE
      LINK_SOURCE)
  set(oneValueArgs DEST_TARGET DEST_PREFIX)
  set(multiValueArgs PROPERTIES)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if(NOT ARGS_PROPERTIES)
    set(ARGS_PROPERTIES INCLUDE_DIRECTORIES
        COMPILE_OPTIONS COMPILE_DEFINITIONS
        LINK_OPTIONS LINK_LIBRARIES)
  endif()
  if(ARGS_UNPARSED_ARGUMENTS)
    foreach(prop ${ARGS_PROPERTIES})
      list(
        APPEND oneValueArgs EXCLUDE_REGEX_${prop} INCLUDE_REGEX_${prop}
        SOURCE_${prop} DEST_${prop} DESTLIST_${prop}
      )
    endforeach()
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if(NOT ARGS_PROPERTIES)
      set(ARGS_PROPERTIES INCLUDE_DIRECTORIES
          COMPILE_OPTIONS COMPILE_DEFINITIONS
          LINK_OPTIONS LINK_LIBRARIES)
    endif()
  endif()

  foreach(prop ${ARGS_PROPERTIES})
    set(srcprop ${prop})
    set(dstprop ${prop})
    set(dstproplist)
    if(ARGS_SOURCE_${prop})
      set(srcprop ${ARGS_SOURCE_${prop}})
    elseif(ARGS_SOURCE_INTERFACE)
      set(srcprop INTERFACE_${prop})
    endif()
    if(ARGS_DEST_${prop})
      set(dstprop ${ARGS_DEST_${prop}})
    elseif(ARGS_DEST_INTERFACE)
      set(dstprop INTERFACE_${prop})
    endif()
    if(ARGS_DESTLIST_${prop})
      set(dstproplist ${ARGS_DESTLIST_${prop}})
    elseif(ARGS_DEST_PREFIX)
      set(dstproplist ${ARGS_DEST_PREFIX}${prop})
    endif()
    set(DEST)
    if(ARGS_DEST_TARGET)
      set(DEST "${ARGS_DEST_TARGET} ${dstprop}")
    elseif(dstproplist)
      set(DEST "${dstproplist} ")
    endif()
    
    get_target_property(proplist ${SOURCE_TARGET} ${srcprop})
    if(proplist STREQUAL "proplist-NOTFOUND")
      set(proplist)
    endif()
    if(ARGS_VERBOSE)
      message(STATUS "TRANSFERING FROM ${SOURCE_TARGET} ${srcprop} = ${proplist} (INCLUDE=${ARGS_INCLUDE_REGEX_${prop}}, EXCLUDE=${ARGS_EXCLUDE_REGEX_${prop}})")
    endif()
    if(ARGS_INCLUDE_REGEX_${prop})
      list(FILTER proplist INCLUDE REGEX "${ARGS_INCLUDE_REGEX_${prop}}")
    endif()
    if(ARGS_EXCLUDE_REGEX_${prop})
      list(FILTER proplist EXCLUDE REGEX "${ARGS_EXCLUDE_REGEX_${prop}}")
    endif()
    if(ARGS_LINK_SOURCE AND (prop STREQUAL "LINK_LIBRARIES"))
      get_target_property(
        ARGS_LINK_SOURCE ${SOURCE_TARGET} IMPORTED_LOCATION
      )
      get_target_property(
        SOURCE_WHOLE_ARCHIVE ${SOURCE_TARGET} INTERFACE_WHOLE_ARCHIVE
      )
      if(SOURCE_WHOLE_ARCHIVE)
        add_whole_archive_flag(ARGS_LINK_SOURCE LIBRARY ${ARGS_LINK_SOURCE})
      endif()
      list(PREPEND proplist ${ARGS_LINK_SOURCE})
    endif()
    if(ARGS_VERBOSE)
      message(STATUS "TRANSFERED TO ${DEST} = ${proplist}")
    endif()
    if(dstproplist)
      list(APPEND ${dstproplist} ${proplist})
      set(${dstproplist} ${${dstproplist}} PARENT_SCOPE)
      if(ARGS_VERBOSE)
        message(STATUS "${dstproplist} = ${${dstproplist}}")
      endif()
    endif()
    if(ARGS_DEST_TARGET)
      set_property(
        TARGET ${ARGS_DEST_TARGET} APPEND PROPERTY
        ${dstprop} "${proplist}"
      )
    endif()
  endforeach()
endfunction()

# Adapted from
# https://github.com/headupinclouds/cmake_whole_archive/blob/master/README.md
# https://github.com/caffe2/caffe2/blob/f2cf8933fc8c04f149ea27f30381517df358c1b5/cmake/Utils.cmake#L173-L185
function(add_whole_archive_flag output_var)
  set(options APPEND)
  set(oneValueArgs LIBRARY)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  # TODO: LINK_LIBRARY requires CMake >= 3.24 and dosn't seem to work with
  #   an IMPORTED library
  # set(flags "$<LINK_LIBRARY:WHOLE_ARCHIVE,${ARGS_LIBRARY}>")
  if(ARGS_LIBRARY AND (TARGET ${ARGS_LIBRARY}))
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      set(flags -Wl,-force_load,$<TARGET_FILE:${ARGS_LIBRARY}>)
    elseif(MSVC)
      # In MSVC, we will add whole archive in default.
      set(flags -WHOLEARCHIVE:$<TARGET_FILE:${ARGS_LIBRARY}>)
    else()
      # Assume everything else is like gcc
      set(flags -Wl,--whole-archive $<TARGET_FILE:${ARGS_LIBRARY}> -Wl,--no-whole-archive)
    endif()
  elseif(ARGS_LIBRARY)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      set(flags -Wl,-force_load,${ARGS_LIBRARY})
    elseif(MSVC)
      # In MSVC, we will add whole archive in default.
      set(flags -WHOLEARCHIVE:${ARGS_LIBRARY})
    else()
      # Assume everything else is like gcc
      set(flags -Wl,--whole-archive ${ARGS_LIBRARY} -Wl,--no-whole-archive)
    endif()
  else()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      set(flags -Wl,-all_load)
    elseif(MSVC)
      # In MSVC, we will add whole archive in default.
      set(flags -WHOLEARCHIVE)
    else()
      # Assume everything else is like gcc
      set(flags -Wl,--whole-archive)
    endif()
  endif()
  if(ARGS_APPEND)
    list(APPEND ${output_var} ${flags})
  else()
    set(${output_var} ${flags})
  endif()
  set(${output_var} ${${output_var}} PARENT_SCOPE)
endfunction()

function(import_yggdrasil_interface_library LANGUAGE)
  # TODO: Allow indication of the toolchain being used
  set(options VERBOSE EXCLUDE_PYTHONLIB)
  set(oneValueArgs TARGET OUTPUT_VARIABLE)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  get_yggdrasil_flags(
    ${LANGUAGE} compiler YGG_CCFLAGS ${ARGS_UNPARSED_ARGUMENTS}
  )
  get_yggdrasil_flags(
    ${LANGUAGE} linker YGG_LDFLAGS ${ARGS_UNPARSED_ARGUMENTS}
  )
  find_yggdrasil_library(
    YGG_LDFLAGS ${YGG_LDFLAGS} COMPILE_MISSING
    LANGUAGE ${LANGUAGE} OUTPUT_VARIABLE LIBRARY_NAME
    ${ARGS_UNPARSED_ARGUMENTS}
  )
  if(NOT LIBRARY_NAME)
    message(FATAL_ERROR "Failed to locate or create the yggdrasil ${LANGUAGE} interface library")
  endif()
  # TODO: Handle .lib on windows which could be static or import
  string(REGEX MATCH "\.(so)|(dylib)|(dll)$" EXT_MATCH ${LIBRARY_NAME})
  if(EXT_MATCH)
    set(LIBRARY_TYPE SHARED)
  else()
    set(LIBRARY_TYPE STATIC)
  endif()
  select_flags(
    INCLUDE_DIRECTORIES FLAGS ${YGG_CCFLAGS}
    INCLUDE_PREFIXES I
    REMOVE_PREFIX
  )
  select_flags(
    COMPILE_DEFINITIONS FLAGS ${YGG_CCFLAGS}
    INCLUDE_PREFIXES D
    REMOVE_PREFIX
  )
  select_flags(
    LIBRARY_DIRECTORIES FLAGS ${YGG_LDFLAGS}
    INCLUDE_PREFIXES L LIBPATH
    REMOVE_PREFIX
  )
  select_flags(
    LINKED_LIBRARIES FLAGS ${YGG_LDFLAGS}
    INCLUDE_PREFIXES l
    REMOVE_PREFIX
  )
  set(LINK_LIBRARIES)
  foreach(lib ${LINKED_LIBRARIES})
    if((NOT lib MATCHES "^ygg")
       AND (NOT (ARGS_EXCLUDE_PYTHONLIB AND (lib MATCHES "^python"))))
      find_library(${lib} ${lib} HINTS ${LIBRARY_DIRECTORIES})
      if(${lib} STREQUAL "${lib}-NOTFOUND")
        message(WARNING "Could not find yggdrasil dependency \"${lib}\"")
      else()
        list(APPEND LINK_LIBRARIES ${${lib}})
      endif()
    endif()
  endforeach()
  set(WHOLE_ARCHIVE OFF)
  if((LIBRARY_TYPE STREQUAL "STATIC") AND (LANGUAGE STREQUAL "CXX"))
    set(WHOLE_ARCHIVE ON)
    # add_whole_archive_flag(LIBRARY_NAME LIBRARY ${LIBRARY_NAME})
  endif()
  if(NOT ARGS_TARGET)
    set(ARGS_TARGET YggInterface::${LANGUAGE})
  endif()
  if(ARGS_VERBOSE)
    message(STATUS "TARGET = ${ARGS_TARGET}")
    message(STATUS "YGG_CCFLAGS = ${YGG_CCFLAGS}")
    message(STATUS "YGG_LDFLAGS = ${YGG_LDFLAGS}")
    message(STATUS "LIBRARY_NAME = ${LIBRARY_NAME}")
    message(STATUS "LIBRARY_TYPE = ${LIBRARY_TYPE}")
    message(STATUS "INCLUDE_DIRECTORIES = ${INCLUDE_DIRECTORIES}")
    message(STATUS "COMPILE_DEFINITIONS = ${COMPILE_DEFINITIONS}")
    message(STATUS "LIBRARY_DIRECTORIES = ${LIBRARY_DIRECTORIES}")
    message(STATUS "LINKED_LIBRARIES = ${LINKED_LIBRARIES}")
    message(STATUS "LINK_LIBRARIES = ${LINK_LIBRARIES}")
    message(STATUS "LINK_OPTIONS = ${LINK_OPTIONS}")
  endif()
  add_library(${ARGS_TARGET} ${LIBRARY_TYPE} IMPORTED)
  set_target_properties(
    ${ARGS_TARGET} PROPERTIES
    IMPORTED_LOCATION ${LIBRARY_NAME}
  )
  if(WHOLE_ARCHIVE)
    set_property(
      TARGET ${ARGS_TARGET} PROPERTY
      INTERFACE_WHOLE_ARCHIVE ON
    )
  endif()
  if(INCLUDE_DIRECTORIES)
    set_property(
      TARGET ${ARGS_TARGET} APPEND PROPERTY
      INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIRECTORIES}"
    )
  endif()
  if(COMPILE_DEFINITIONS)
    set_property(
      TARGET ${ARGS_TARGET} APPEND PROPERTY
      INTERFACE_COMPILE_DEFINITIONS "${COMPILE_DEFINITIONS}"
    )
  endif()
  if(LINK_LIBRARIES)
    set_property(
      TARGET ${ARGS_TARGET} APPEND PROPERTY
      INTERFACE_LINK_LIBRARIES "${LINK_LIBRARIES}"
    )
  endif()
  if(LINK_OPTIONS)
    set_property(
      TARGET ${ARGS_TARGET} APPEND PROPERTY
      INTERFACE_LINK_OPTIONS "${LINK_OPTIONS}"
    )
  endif()
  if(ARGS_OUTPUT_VARIABLE)
    set(${ARGS_OUTPUT_VARIABLE} ${ARGS_TARGET} PARENT_SCOPE)
  endif()
endfunction()

function(remove_flag_prefix FLAG PREFIX OUTPUT_VARIABLE)
  string(REGEX REPLACE "^[\/\-]${PREFIX}" "" ${OUTPUT_VARIABLE} ${FLAG})
  set(${OUTPUT_VARIABLE} ${${OUTPUT_VARIABLE}} PARENT_SCOPE)
endfunction()

function(flag_matches FLAG PREFIX)
    set(options REMOVE_PREFIX)
    set(oneValueArgs OUTPUT_VARIABLE)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    string(REGEX MATCH "^[\/\-]${PREFIX}" RESULT ${FLAG})
    if(ARGS_OUTPUT_VARIABLE)
      if(ARGS_REMOVE_PREFIX AND RESULT)
        remove_flag_prefix(${FLAG} ${PREFIX} RESULT)
      endif()
      set(${ARGS_OUTPUT_VARIABLE} ${RESULT} PARENT_SCOPE)
    elseif(NOT RESULT)
      message(FATAL_ERROR "Failed to match \"{FLAG}\" against prefix \"{PREFIX}\"")
    endif()
endfunction()

function(find_flag OUTPUT_VARIABLE)
    set(options REMOVE_PREFIX)
    set(multiValueArgs FLAGS PREFIXES)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(FLAG_ARGS)
    if(ARGS_REMOVE_PREFIX)
      list(APPEND FLAG_ARGS REMOVE_PREFIX)
    endif()
    foreach(flag ${ARGS_FLAGS})
      set(iRESULT)
      flag_matches_any(${flag} PREFIXES ${ARGS_PREFIXES} OUTPUT_VARIABLE iRESULT ${FLAG_ARGS})
      if(iRESULT)
        if(ARGS_REMOVE_PREFIX)
          set(${OUTPUT_VARIABLE} ${iRESULT} PARENT_SCOPE)
        else()
          set(${OUTPUT_VARIABLE} ${flag} PARENT_SCOPE)
        endif()
        return()
      endif()
    endforeach()
    set(${OUTPUT_VARIABLE} "" PARENT_SCOPE)
endfunction()

function(flag_matches_any FLAG)
    set(options REMOVE_PREFIX)
    set(oneValueArgs OUTPUT_VARIABLE)
    set(multiValueArgs PREFIXES)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(FLAG_ARGS)
    if(ARGS_REMOVE_PREFIX)
      list(APPEND FLAG_ARGS REMOVE_PREFIX)
    endif()
    foreach(prefix ${ARGS_PREFIXES})
      set(iRESULT)
      flag_matches(${FLAG} ${prefix} OUTPUT_VARIABLE iRESULT ${FLAG_ARGS})
      if(iRESULT)
        if(ARGS_OUTPUT_VARIABLE)
          set(${ARGS_OUTPUT_VARIABLE} ${iRESULT} PARENT_SCOPE)
        endif()
        return()
      endif()
    endforeach()
    if (ARGS_OUTPUT_VARIABLE)
      set(${ARGS_OUTPUT_VARIABLE} 0 PARENT_SCOPE)
    else()
      message(FATAL_ERROR "Failed to match \"{FLAG}\" against prefixes {ARGS_PREFIXES}")
    endif()
endfunction()

function(select_flags OUTPUT_VARIABLE)
    set(options REMOVE_PREFIX)
    set(multiValueArgs FLAGS INCLUDE_PREFIXES EXCLUDE_PREFIXES)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(${OUTPUT_VARIABLE})
    set(FLAG_ARGS)
    if(ARGS_REMOVE_PREFIX)
      list(APPEND FLAG_ARGS REMOVE_PREFIX)
    endif()
    foreach(flag ${ARGS_FLAGS})
      set(iINCLUDE)
      set(iEXCLUDE)
      if(ARGS_INCLUDE_PREFIXES)
        flag_matches_any(${flag} PREFIXES ${ARGS_INCLUDE_PREFIXES} OUTPUT_VARIABLE iINCLUDE ${FLAG_ARGS})
      else()
        set(iINCLUDE 1)
      endif()
      if(ARGS_EXCLUDE_PREFIXES)
        flag_matches_any(${flag} PREFIXES ${ARGS_EXCLUDE_PREFIXES} OUTPUT_VARIABLE iEXCLUDE)
      else()
        set(iEXCLUDE 0)
      endif()
      if(iINCLUDE AND NOT iEXCLUDE)
        if(ARGS_INCLUDE_PREFIXES AND ARGS_REMOVE_PREFIX)
          list(APPEND ${OUTPUT_VARIABLE} ${iINCLUDE})
        else()
          list(APPEND ${OUTPUT_VARIABLE} ${flag})
        endif()
      endif()
    endforeach()
    set(${OUTPUT_VARIABLE} ${${OUTPUT_VARIABLE}} PARENT_SCOPE)
endfunction()

function(get_yggdrasil_flags LANGUAGE TOOL OUTPUT_VARIABLE)
    set(options WITH_ASAN)
    set(oneValueArgs TOOLNAME PYTHON)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(INFO_FLAGS --flags)
    if(ARGS_WITH_ASAN)
      list(APPEND INFO_FLAGS --with-asan)
    endif()
    if(ARGS_TOOLNAME)
      list(APPEND INFO_FLAGS --toolname ${ARGS_TOOLNAME})
    endif()
    if(NOT ARGS_PYTHON)
      set(ARGS_PYTHON python)
    endif()
    set(YGG_COMMAND ${ARGS_PYTHON} -m yggdrasil info
        ${TOOL} ${LANGUAGE} ${INFO_FLAGS})
    execute_process(
      COMMAND ${YGG_COMMAND}
      OUTPUT_VARIABLE ${OUTPUT_VARIABLE}
      ERROR_VARIABLE FLAGS_ERROR
      RESULT_VARIABLE RESULT
    )
    if(RESULT)
      message(FATAL_ERROR "WITH_YGGDRASIL specified, but command to get yggdrasil ${TOOL} flags failed:\n  COMMAND: ${YGG_COMMAND}\n  OUTPUT: ${${OUTPUT_VARIABLE}}\n  ERROR: ${FLAGS_ERROR}")
    endif()
    STRING(REPLACE " " ";" ${OUTPUT_VARIABLE} ${${OUTPUT_VARIABLE}})
    set(INCLUDE_PREFIXES)
    set(EXCLUDE_PREFIXES)
    if(TOOL STREQUAL "compiler")
      set(INCLUDE_PREFIXES D I)
    elseif(TOOL STREQUAL "linker")
      set(INCLUDE_PREFIXES L l LIBPATH)
      set(EXCLUDE_PREFIXES lstd)
    else()
      message(FATAL_ERROR "Invalid tool \"${TOOL}\"")
    endif()
    select_flags(
      ${OUTPUT_VARIABLE} FLAGS ${${OUTPUT_VARIABLE}}
      INCLUDE_PREFIXES ${INCLUDE_PREFIXES}
      EXCLUDE_PREFIXES ${EXCLUDE_PREFIXES}
    )
    set(${OUTPUT_VARIABLE} ${${OUTPUT_VARIABLE}} PARENT_SCOPE)
endfunction()

function(find_yggdrasil_library)
    set(options COMPILE_MISSING)
    set(multiValueArgs YGG_LDFLAGS)
    set(oneValueArgs OUTPUT_VARIABLE)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if(NOT ARGS_YGG_LDFLAGS)
      get_yggdrasil_flags(linker ARGS_YGG_LDFLAGS ${ARGS_UNPARSED_ARGUMENTS})
    endif()
    find_flag(YGG_LIBNAME FLAGS ${ARGS_YGG_LDFLAGS} PREFIXES lf?ygg)
    if(NOT YGG_LIBNAME)
      message(FATAL_ERROR "Failed to identify yggdrasil library from flags \"${ARGS_YGG_LDFLAGS}\"")
    endif()
    remove_flag_prefix(${YGG_LIBNAME} l YGG_LIBNAME)
    select_flags(
      LIBRARY_DIRS FLAGS ${ARGS_YGG_LDFLAGS}
      INCLUDE_PREFIXES L
      REMOVE_PREFIX
    )
    if(NOT LIBRARY_DIRS)
      message(FATAL_ERROR "Failed to identify library directories from flags \"${ARGS_YGG_LDFLAGS}\"")
    endif()
    find_library(${YGG_LIBNAME} ${YGG_LIBNAME} HINTS ${LIBRARY_DIRS})
    if(NOT ${YGG_LIBNAME} STREQUAL "${YGG_LIBNAME}-NOTFOUND")
      if(ARGS_OUTPUT_VARIABLE)
        set(${ARGS_OUTPUT_VARIABLE} ${${YGG_LIBNAME}} PARENT_SCOPE)
      endif()
      return()
    endif()
    # foreach(dir ${LIBRARY_DIRS})
    #   cmake_path(APPEND dir "*${YGG_LIBNAME}*" OUTPUT_VARIABLE ipath)
    #   file(GLOB MATCHES LIST_DIRECTORIES false "${ipath}" )
    #   if(MATCHES)
    #     list(LENGTH MATCHES LEN_MATCHES)
    #     if(LEN_MATCHES GREATER 1)
    #       message(FATAL_ERROR "More than one match to \"${ipath}\": \"${MATCHES}\"")
    #     endif()
    #     if(ARGS_OUTPUT_VARIABLE)
    #       set(${ARGS_OUTPUT_VARIABLE} ${MATCHES} PARENT_SCOPE)
    #     endif()
    #     return()
    #   endif()
    # endforeach()
    if(ARGS_COMPILE_MISSING)
      compile_yggdrasil(${ARGS_UNPARSED_ARGUMENTS})
      if(ARGS_OUTPUT_VARIABLE)
        list(APPEND ARGS_UNPARSED_ARGUMENTS OUTPUT_VARIABLE ${ARGS_OUTPUT_VARIABLE})
      endif()
      find_yggdrasil_library(
        YGG_LDFLAGS ${ARGS_YGG_LDFLAGS}
        ${ARGS_UNPARSED_ARGUMENTS}
      )
    elseif(ARGS_OUTPUT_VARIABLE)
      message(WARNING "Failed to locate or create any yggdrasil library matching \"${YGG_LIBNAME}\" in these directories: \"${LIBRARY_DIRS}\"")
      set(${ARGS_OUTPUT_VARIABLE})
    else()
      message(FATAL_ERROR "Failed to locate or create any yggdrasil library matching \"${YGG_LIBNAME}\" in these directories: \"${LIBRARY_DIRS}\"")
    endif()
    if(ARGS_OUTPUT_VARIABLE)
      set(${ARGS_OUTPUT_VARIABLE} ${${ARGS_OUTPUT_VARIABLE}} PARENT_SCOPE)
    endif()
endfunction()


function(compile_yggdrasil)
    set(options WITH_ASAN)
    set(oneValueArgs LANGUAGE TOOLNAME PYTHON)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(flags)
    if(ARGS_LANGUAGE)
      string(TOLOWER "${ARGS_LANGUAGE}" YGG_LANGUAGE)
      list(APPEND flags ${YGG_LANGUAGE})
    endif()
    if(ARGS_WITH_ASAN)
      list(APPEND flags --with-asan)
    endif()
    if(ARGS_TOOLNAME)
      list(APPEND flags --toolname ${ARGS_TOOLNAME})
    endif()
    if(NOT ARGS_PYTHON)
      set(ARGS_PYTHON python)
    endif()
    set(YGG_COMMAND ${ARGS_PYTHON} -m yggdrasil compile ${flags})
    execute_process(
      COMMAND ${YGG_COMMAND}
      RESULT_VARIABLE YGGCOMPILE_RESULT
    )
    if(YGGCOMPILE_RESULT)
      message(FATAL_ERROR "Error during compilation of yggdrasil interface")
    endif()
endfunction()

function(link_yggdrasil TARGET SCOPE LANGUAGE)
    # TODO: Allow indication of the toolchain being used
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    get_yggdrasil_flags(
      ${LANGUAGE} compiler YGG_CCFLAGS ${ARGS_UNPARSED_ARGUMENTS}
    )
    get_yggdrasil_flags(
      ${LANGUAGE} linker YGG_LDFLAGS ${ARGS_UNPARSED_ARGUMENTS}
    )
    find_yggdrasil_library(
      YGG_LDFLAGS ${YGG_LDFLAGS} COMPILE_MISSING
      LANGUAGE ${LANGUAGE}
      ${ARGS_UNPARSED_ARGUMENTS}
    )
    
    target_compile_options(
      ${TARGET} ${SCOPE} ${YGG_CCFLAGS}
    )
    target_link_options(
      ${TARGET} ${SCOPE} ${YGG_LDFLAGS}
    )
endfunction()

function(add_yggdrasil_test NAME YAML EXECUTABLE)
    set(oneValueArgs LIBRARY EXPECTED_OUTPUT ACTUAL_OUTPUT PRESERVE_TEST_OUTPUT)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    add_test(
      NAME ${NAME}
      COMMAND yggdrasil run ${YAML}
    )
    if(WIN32)
      set(PATH_SEP ";")
      set(LIB_PATH_VAR LIB)
    else()
      set(PATH_SEP ":")
      if(APPLE)
        set(LIB_PATH_VAR DYLD_LIBRARY_PATH)
      else()
        set(LIB_PATH_VAR LD_LIBRARY_PATH)
      endif()
    endif()
    set(PREV_PATH "$ENV{PATH}")
    if(PREV_PATH)
      set(PREV_PATH "${PREV_PATH}${PATH_SEP}")
    endif()
    set(TEST_ENV "PATH=${PREV_PATH}$<TARGET_FILE_DIR:${EXECUTABLE}>")
    if(ARGS_LIBRARY)
      set(PREV_LIB_PATH "$ENV{${LIB_PATH_VAR}}")
      set_tests_properties(
        ${NAME} PROPERTIES
        DEPENDS ${ARGS_LIBRARY}
      )
      if(PREV_LIB_PATH)
        set(PREV_LIB_PATH "${PREV_LIB_PATH}${PATH_SEP}")
      endif()
      set(TEST_ENV "${TEST_ENV};${LIB_PATH_VAR}=${PREV_LIB_PATH}$<TARGET_FILE_DIR:${ARGS_LIBRARY}>")
    endif()
    set_tests_properties(
      ${NAME} PROPERTIES
      DEPENDS ${EXECUTABLE}
      ENVIRONMENT "${TEST_ENV}"
    )
    if(ARGS_EXPECTED_OUTPUT AND ARGS_ACTUAL_OUTPUT)
      set(CHECK_TEST_ARGS)
      if(ARGS_PRESERVE_TEST_OUTPUT)
        list(APPEND CHECK_TEST_ARGS ARGS_PRESERVE_TEST_OUTPUT)
      endif()
      include(TestTools)
      check_test_output(
        ${NAME} ${ARGS_EXPECTED_OUTPUT} ${ARGS_ACTUAL_OUTPUT}
        ${CHECK_TEST_ARGS} FUZZY_COMPARE SEP "\t"
      )
    endif()
endfunction()
