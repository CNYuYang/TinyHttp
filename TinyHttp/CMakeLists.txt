cmake_minimum_required(VERSION 3.6)
project(TinyHttp)


if (NOT CMAKE_SYSTEM_NAME STREQUAL "Android")
    find_package(OpenSSL REQUIRED)
endif ()
# 引入头文件


include_directories(${OPENSSL_INCLUDE_DIR} ${INC_DIR}/TinyHttp)

# 添加文件夹

add_subdirectory(kernel)
add_subdirectory(protocol)
add_subdirectory(factory)
add_subdirectory(server)
add_subdirectory(manager)

# 编译生成静态链接库

add_library(
        ${PROJECT_NAME} STATIC
        $<TARGET_OBJECTS:kernel>
        $<TARGET_OBJECTS:protocol>
        $<TARGET_OBJECTS:factory>
        $<TARGET_OBJECTS:server>
        $<TARGET_OBJECTS:manager>
)