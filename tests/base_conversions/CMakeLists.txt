cmake_minimum_required(VERSION 3.13...3.25)

set(TESTNAME base_conversions_test)

add_executable(${TESTNAME} ${TESTNAME}.cc)

target_link_libraries(${TESTNAME}
    PRIVATE
        gtest_main
        base_conversions
)

gtest_discover_tests(${TESTNAME}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
)

set_target_properties(${TESTNAME}
    PROPERTIES FOLDER tests
)
