cmake_minimum_required(VERSION 3.26)
project(practical1)

set(CMAKE_CXX_STANDARD 17)

add_executable(practical1 main.cpp
        application.cpp
        application.h
        internalrepres.cpp
        internalrepres.h)
