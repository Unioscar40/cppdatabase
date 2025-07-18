cmake_minimum_required(VERSION 3.15)
project(SocketWrapperProject)

# GoogleTest vía FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

# Tests
enable_testing()

add_subdirectory(ISock)
add_subdirectory(tests)
#add_subdirectory(cppdatabase)