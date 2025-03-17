function(create_yggdrasil_interface_library LANGUAGE)
  # TODO: Allow indication of the toolchain being used
  set(oneValueArgs OUTPUT_VARIABLE)
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
    COMPILE_DEFINNITIONS FLAGS ${YGG_CCFLAGS}
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
    if(NOT lib MATCHES "^ygg")
      find_library(${lib} ${lib} HINTS ${LIBRARY_DIRECTORIES})
      if(${lib} STREQUAL "${lib}-NOTFOUND")
        message(WARNING "Could not find yggdrasil dependency \"${lib}\"")
      else()
        list(APPEND LINK_LIBRARIES ${${lib}})
      endif()
    endif()
  endforeach()
  message(STATUS "LIBRARY_NAME = ${LIBRARY_NAME}")
  message(STATUS "LIBRARY_TYPE = ${LIBRARY_TYPE}")
  message(STATUS "INCLUDE_DIRECTORIES = ${INCLUDE_DIRECTORIES}")
  message(STATUS "COMPILE_DEFINNITIONS = ${COMPILE_DEFINNITIONS}")
  message(STATUS "LIBRARY_DIRECTORIES = ${LIBRARY_DIRECTORIES}")
  message(STATUS "LINKED_LIBRARIES = ${LINKED_LIBRARIES}")
  message(STATUS "LINK_LIBRARIES = ${LINK_LIBRARIES}")
  set(TARGET YggInterface::${LANGUAGE})
  add_library(${TARGET} ${LIBRARY_TYPE} IMPORTED)
  set_target_properties(
    ${TARGET} PROPERTIES
    IMPORTED_LOCATION ${LIBRARY_NAME}
  )
  if(INCLUDE_DIRECTORIES)
    set_property(
      TARGET ${TARGET} APPEND PROPERTY
      INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIRECTORIES}"
    )
  endif()
  if(COMPILE_DEFINNITIONS)
    set_property(
      TARGET ${TARGET} APPEND PROPERTY
      INTERFACE_COMPILE_DEFINITIONS "${COMPILE_DEFINNITIONS}"
    )
  endif()
  if(LINK_LIBRARIES)
    set_property(
      TARGET ${TARGET} APPEND PROPERTY
      INTERFACE_LINK_LIBRARIES "${LINK_LIBRARIES}"
    )
  endif()
  if(ARGS_OUTPUT_VARIABLE)
    set(${ARGS_OUTPUT_VARIABLE} ${TARGET} PARENT_SCOPE)
  endif()
endfunction()

function(remove_flag_prefix FLAG PREFIX OUTPUT_VARIABLE)
  string(REGEX REPLACE "^[\/\-]${PREFIX}" "" ${OUTPUT_VARIABLE} ${FLAG})
  set(${OUTPUT_VARIABLE} ${${OUTPUT_VARIABLE}} PARENT_SCOPE)
  # message(STATUS "REMOVE_PREFIX ${FLAG} -> ${${OUTPUT_VARIABLE}}")
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
      # message(STATUS "FLAG=${flag}, iINCLUDE=${iINCLUDE}, iEXCLUDE=${iEXCLUDE}")
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
    set(oneValueArgs TOOLNAME)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(INFO_FLAGS --flags)
    if(ARGS_WITH_ASAN)
      list(APPEND INFO_FLAGS --with-asan)
    endif()
    if(ARGS_TOOLNAME)
      list(APPEND INFO_FLAGS --toolname ${ARGS_TOOLNAME})
    endif()
    execute_process(
      COMMAND yggdrasil info ${TOOL} ${LANGUAGE} ${INFO_FLAGS}
      OUTPUT_VARIABLE ${OUTPUT_VARIABLE}
      ERROR_VARIABLE FLAGS_ERROR
      RESULT_VARIABLE RESULT
    )
    if(RESULT)
      message(FATAL_ERROR "WITH_YGGDRASIL specified, but command to get yggdrasil ${TOOL} flags failed: ${FLAGS_ERROR}")
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
    set(oneValueArgs LANGUAGE TOOLNAME)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(flags)
    if(ARGS_LANGUAGE)
      list(APPEND flags ${ARGS_LANGUAGE})
    endif()
    if(ARGS_WITH_ASAN)
      list(APPEND flags --with-asan)
    endif()
    if(ARGS_TOOLNAME)
      list(APPEND flags --toolname ${ARGS_TOOLNAME})
    endif()
    execute_process(
      COMMAND yggdrasil compile ${flags}
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
    
    message(STATUS "YGG_CCFLAGS = ${YGG_CCFLAGS}")
    target_compile_options(
      ${TARGET} ${SCOPE} ${YGG_CCFLAGS}
    )
    message(STATUS "YGG_LDFLAGS = ${YGG_LDFLAGS}")
    target_link_options(
      ${TARGET} ${SCOPE} ${YGG_LDFLAGS}
    )
    # message(FATAL_ERROR "CHECK RESULTS")
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
