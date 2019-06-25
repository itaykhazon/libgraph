cmake_minimum_required(VERSION 3.7)
project(libgraph)

set(CMAKE_C_STANDARD 99)

INCLUDE_DIRECTORIES(..)
LINK_DIRECTORIES(..)

ADD_EXECUTABLE( test_sanity sanity.c tests.h)
TARGET_LINK_LIBRARIES( test_sanity libgraph.a )
ADD_TEST(test_sanity test_sanity)