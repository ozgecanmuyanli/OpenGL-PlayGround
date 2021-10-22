cmake_minimum_required(VERSION 3.11)

project(OpenGL-Playground)

set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES   FALSE)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES FALSE)

add_subdirectory(Dependencies)
add_subdirectory(Source)

