#ifndef __COMMON_DEFINE_H__
#define __COMMON_DEFINE_H__
#include <stdio.h>

typedef int ElementType;

#define CUTOFF_RANGE 3
#define SPACE_SIZE 10

#define NC_TRACE_ENTRY { \
    printf("%s() start\n", __FUNCTION__);\
}

#define NC_TRACE_EXIT { \
    printf("%s() end\n", __FUNCTION__);\
}

#endif