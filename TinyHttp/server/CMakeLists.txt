cmake_minimum_required(VERSION 3.6)
project(server)

set(SRC
        THServer.cpp
        )

add_library(${PROJECT_NAME} OBJECT ${SRC})