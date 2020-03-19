//
// Created by User on 25/06/2019.
//

#ifndef LIBGRAPH_TESTS_H
#define LIBGRAPH_TESTS_H

#include <stdio.h>

#define SUITE_INIT( suite_name ) \
    {  \
    fprintf(stdout, "Starting suite %s\n",                              \
            #suite_name                                                 \
    );                                                                  \
    int curr_test = 0, curr_failed = 0;

#define SUITE_END( suite_name ) \
    if( !( curr_failed == 0 ) )                                     \
  {                                                                 \
    fprintf(stdout, "Suite %s [%d/%d] - FAILED!\n",\
            #suite_name,                                             \
            curr_test-curr_failed, curr_test                                  \
    );                                                              \
  } else {                                                             \
    fprintf(stdout, "Suite %s [%d/%d] - PASSED!\n",\
            #suite_name,                                             \
            curr_test, curr_test                                  \
    );                                                              \
  }                                                                 \
}

#define ASSERT_TEST( func_name )                                    \
do {\
    curr_test++;                                                    \
    fprintf(stdout, "[%02d] - testing %s...",                       \
            curr_test,                                               \
            #func_name                                              \
    );                                                              \
  if( ( (func_name)() ) )                                              \
  {                                                                 \
    fprintf(stdout, "[OK]\n");                                        \
  } else {                                                          \
    curr_failed++;                                                  \
    fprintf(stdout, "[Failed]\n");                                   \
  }                                                                 \
} while(0)

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
