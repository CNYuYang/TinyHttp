cmake_minimum_required(VERSION 3.6)
project(manager)

set(SRC
        THGlobal.cpp
        )

add_library(${PROJECT_NAME} OBJECT ${SRC})
