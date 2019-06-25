cmake_minimum_required(VERSION 3.7)
project(libgraph)

set(CMAKE_C_STANDARD 99)

INCLUDE_DIRECTORIES(contrib/*)

set(SOURCE_FILES graph.c graph.h errors.h)
add_library(libgraph.a ${SOURCE_FILES})

ENABLE_TESTING()
ADD_SUBDIRECTORY( tests )