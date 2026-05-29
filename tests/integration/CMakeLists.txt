include(GoogleTest)

file(GLOB INTEGRATION_TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp")

foreach(TEST_SOURCE ${INTEGRATION_TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    set(TARGET_NAME "integration_${TEST_NAME}")
    add_executable(${TARGET_NAME} ${TEST_SOURCE})
    target_link_libraries(${TARGET_NAME} PRIVATE GTest::gtest_main decompiler_lib)
    set_project_warnings(${TARGET_NAME})
    gtest_discover_tests(${TARGET_NAME} PROPERTIES LABELS "integration")
endforeach()
