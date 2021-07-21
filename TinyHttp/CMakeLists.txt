cmake_minimum_required(VERSION 3.6)
project(TinyHttp)

include_directories(${INC_DIR}/workflow)

add_subdirectory(kernel)
add_subdirectory(server)

