cmake_minimum_required(VERSION 3.6)
project(factory)

set(SRC
        THTaskFactory.cpp
        )

add_library(${PROJECT_NAME} OBJECT ${SRC})
