cmake_minimum_required(VERSION 3.6)
project(kernel)

set(SRC
        Communicator.cpp
        CommScheduler.cpp
        mpoller.c
        msgqueue.c
        poller.c
        rbtree.c
        thrdpool.c
        )

add_library(${PROJECT_NAME} OBJECT ${SRC})
