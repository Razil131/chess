cmake_minimum_required(VERSION 3.10)
project(Chess)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


file(GLOB_RECURSE SOURCES 
    src/*.cpp
    src/figures/*.cpp
)

add_executable(chess ${SOURCES})
