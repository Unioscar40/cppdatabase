cmake_minimum_required(VERSION 3.10)
project(Isock)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/public
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/private
)

target_sources(${PROJECT_NAME} PRIVATE 
    src/socketfactory.cpp
    src/error.cpp
)

if (WIN32)
    target_sources(${PROJECT_NAME} PRIVATE src/windows/windowssocket.cpp)
    target_link_libraries(${PROJECT_NAME} PUBLIC ws2_32)
else()
    target_sources(${PROJECT_NAME} PRIVATE src/unix/unixsocket.cpp)
endif()