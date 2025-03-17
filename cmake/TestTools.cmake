
function(check_test_output TEST_NAME EXPECTED_OUTPUT ACTUAL_OUTPUT)
    set(options PRESERVE_TEST_OUTPUT FUZZY_COMPARE)
    set(oneValueArgs SHOW_NAME CHECK_NAME ABSTOL RELTOL SEP)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if(ARGS_SHOW_NAME)
      set(SHOW_NAME ${ARGS_SHOW_NAME})
    else()
      set(SHOW_NAME ${TEST_NAME}_SHOW)
    endif()
    if(ARGS_CHECK_NAME)
      set(CHECK_NAME ${ARGS_CHECK_NAME})
    else()
      set(CHECK_NAME ${TEST_NAME}_CHECK)
    endif()
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
    if(ARGS_FUZZY_COMPARE)
      set(ARGS_DEVTASKS)
      if(ARGS_ABSTOL)
        list(APPEND ARGS_DEVTASKS --abstol ${ARGS_ABSTOL})
      endif()
      if(ARGS_RELTOL)
        list(APPEND ARGS_DEVTASKS --reltol ${ARGS_RELTOL})
      endif()
      if(ARGS_SEP)
        list(APPEND ARGS_DEVTASKS --sep ${ARGS_SEP})
      endif()
      cmake_path(
        APPEND PROJECT_SOURCE_DIR
        scripts devtasks.py
        OUTPUT_VARIABLE DEVTASKS_COMMAND
      )
      add_test(
        NAME ${CHECK_NAME} COMMAND
        python ${DEVTASKS_COMMAND} compare-files
        ${ACTUAL_OUTPUT} --expected ${EXPECTED_OUTPUT}
        ${ARGS_DEVTASKS}
      )
    else()
      add_test(
        NAME ${CHECK_NAME} COMMAND
        ${CMAKE_COMMAND} -E compare_files ${EXPECTED_OUTPUT} ${ACTUAL_OUTPUT}
      )
    endif()
    set_tests_properties(
      ${CHECK_NAME} PROPERTIES
      DEPENDS "${TEST_NAME};${SHOW_NAME}_EXPECTED;${SHOW_NAME}_ACTUAL"
    )
    if(NOT ARGS_PRESERVE_TEST_OUTPUT)
      add_test(
        NAME ${TEST_NAME}_CLEANUP COMMAND
        ${CMAKE_COMMAND} -E remove ${ACTUAL_OUTPUT}
      )
      set_tests_properties(
        ${TEST_NAME}_CLEANUP PROPERTIES
        DEPENDS ${CHECK_NAME}
      )
    endif()
endfunction()