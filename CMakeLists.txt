cmake_minimum_required(VERSION 3.6)
project(TinyHttp)

set(CMAKE_CXX_STANDARD 11)

set(INC_DIR ${PROJECT_SOURCE_DIR}/_include CACHE PATH "TinyHttp inc")
set(LIB_DIR ${PROJECT_SOURCE_DIR}/_lib CACHE PATH "TinyHttp lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})

# 引入头文件存放信息
INCLUDE(CMakeLists_Headers.txt)

# 创建头文件输出文件夹

add_custom_target(
        LINK_HEADERS ALL
        COMMENT "link headers..."
)

add_custom_command(
        TARGET LINK_HEADERS PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${INC_DIR}/${PROJECT_NAME}
)
# 复制头文件

macro(makeLink src dest target)
    add_custom_command(
            TARGET ${target} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${src} ${dest}
            DEPENDS ${dest}
    )
endmacro()

foreach (header_file ${INCLUDE_HEADERS} ${INCLUDE_KERNEL_HEADERS})
    string(REPLACE "/" ";" arr ${header_file})
    list(GET arr -1 file_name)
    makeLink(${PROJECT_SOURCE_DIR}/${header_file} ${INC_DIR}/${PROJECT_NAME}/${file_name} LINK_HEADERS)
endforeach ()

add_subdirectory(TinyHttp)