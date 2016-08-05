FILE(GLOB TESTS *_test.cpp)

include(CTest)

include_directories(../include)

foreach(TEST ${TESTS})

    get_filename_component(TEST_NAME ${TEST} NAME_WE)

    add_executable(${TEST_NAME} ${TEST})
    target_link_libraries(${TEST_NAME} ${GMP_LIBRARIES})

    add_test(${TEST_NAME} ${TEST_NAME})

endforeach()
