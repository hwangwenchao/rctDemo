#ifndef __COMMON_DEFINE_H__
#define __COMMON_DEFINE_H__
#include <stdio.h>

typedef int ElementType;

#define NC_TRACE_ENTRY { \
    printf("%s() start\n", __FUNCTION__);\
}

#define NC_TRACE_EXIT { \
    printf("%s() end\n", __FUNCTION__);\
}

#endif