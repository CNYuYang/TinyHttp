cmake_minimum_required(VERSION 3.6)
project(Tutorial)

set(INC_DIR ${PROJECT_SOURCE_DIR}/../_include CACHE PATH "TinyHttp inc")
set(LIB_DIR ${PROJECT_SOURCE_DIR}/../_lib CACHE PATH "TinyHttp lib")

include_directories(${INC_DIR}/TinyHttp)
link_directories(${LIB_DIR})

add_executable(Tutorial main.cpp)