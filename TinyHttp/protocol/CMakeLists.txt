cmake_minimum_required(VERSION 3.6)
project(protocol)

set(SRC
        HttpMessage.cpp
        http_parser.c
        )

add_library(${PROJECT_NAME} OBJECT ${SRC})
