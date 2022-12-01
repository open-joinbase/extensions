cmake_minimum_required(VERSION 3.0.0)
project(testudm VERSION 0.1.0)

set(CMAKE_BUILD_TYPE Release)

add_library(testudm SHARED main.c)
