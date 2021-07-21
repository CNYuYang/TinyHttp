cmake_minimum_required(VERSION 3.6)
project(TinyHttp)

include_directories(${INC_DIR}/TinyHttp)

add_subdirectory(kernel)
add_subdirectory(protocol)
add_subdirectory(factory)
add_subdirectory(server)

add_library(
        ${PROJECT_NAME} STATIC
        $<TARGET_OBJECTS:kernel>
        $<TARGET_OBJECTS:protocol>
        $<TARGET_OBJECTS:factory>
        $<TARGET_OBJECTS:server>
)