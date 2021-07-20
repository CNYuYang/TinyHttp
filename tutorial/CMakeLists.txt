cmake_minimum_required(VERSION 3.19)
project(Tutorial)

include_directories(${WORKFLOW_INCLUDE_DIR})
link_directories(${WORKFLOW_LIB_DIR})

add_executable(Tutorial main.cpp)