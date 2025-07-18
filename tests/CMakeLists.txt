cmake_minimum_required(VERSION 3.10)
project(UnitTests)

add_executable(${PROJECT_NAME}
    socket_tests.cpp
    error_tests.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    GTest::gtest_main
    Isock
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})