cmake_minimum_required(VERSION 3.6)
project(kernel)

set(SRC
        Communicator.cpp
        )

add_library(${PROJECT_NAME} OBJECT ${SRC})
