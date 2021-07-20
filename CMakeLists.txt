cmake_minimum_required(VERSION 3.6)

project(TinyHttp)

if (MINGW)
    set(CMAKE_C_COMPILER C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/bin/gcc.exe)
    set(CMAKE_CXX_COMPILER C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/bin/g++.exe)
endif ()

set(CMAKE_CXX_STANDARD 11)

set(INC_DIR ${PROJECT_SOURCE_DIR}/_include CACHE PATH "TinyHttp inc")
set(LIB_DIR ${PROJECT_SOURCE_DIR}/_lib CACHE PATH "TinyHttp lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})

add_custom_target(
        LINK_HEADERS ALL
        COMMENT "link headers..."
)

INCLUDE(CMakeLists_Headers.txt)
macro(makeLink src dest target)
    add_custom_command(
            TARGET ${target} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${src} ${dest}
            DEPENDS ${dest}
    )
endmacro()

add_custom_command(
        TARGET LINK_HEADERS PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${INC_DIR}/${PROJECT_NAME}
)

foreach (header_file ${INCLUDE_HEADERS} ${INCLUDE_KERNEL_HEADERS})
    string(REPLACE "/" ";" arr ${header_file})
    list(GET arr -1 file_name)
    makeLink(${PROJECT_SOURCE_DIR}/${header_file} ${INC_DIR}/${PROJECT_NAME}/${file_name} LINK_HEADERS)
    message(${PROJECT_SOURCE_DIR}/${header_file})
    message(${INC_DIR}/${PROJECT_NAME}/${file_name})
endforeach ()

add_subdirectory(TinyHttp)