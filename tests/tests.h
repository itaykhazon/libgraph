//
// Created by User on 25/06/2019.
//

#ifndef LIBGRAPH_TESTS_H
#define LIBGRAPH_TESTS_H

#include <stdio.h>

#define ASSERT_TRUE( condition )                                    \
do {                                                                \
  if( !( condition ) )                                              \
  {                                                                 \
    fprintf(stderr, "%s:%d@%s - ASSERT_TRUE FAILED - %s is false\n",\
            __FILE__,                                               \
            __LINE__,                                               \
            __FUNCTION__,                                           \
            #condition                                              \
    );                                                              \
    return false;                                                   \
  }                                                                 \
} while(0)


#define ASSERT_EQUAL( x, y)                                         \
do {                                                                \
  if( ( x ) != ( y ) )                                              \
  {                                                                 \
    fprintf(stderr, "%s:%d@%s - ASSERT_EQUAL FAILED - %s != %s\n",  \
            __FILE__,                                               \
            __LINE__,                                               \
            __FUNCTION__,                                           \
            #x,                                                     \
            #y                                                      \
    );                                                              \
    return false;                                                   \
  }                                                                 \
} while(0)

#endif //LIBGRAPH_TESTS_H
