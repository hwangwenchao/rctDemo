
#ifndef __NC_SHREDDEDCOMMON_H__
#define __NC_SHREDDEDCOMMON_H__

#ifdef __WINDOWS__
#program onece
#endif

#include <iostream>
#include <vector>

#define NC_TRACE_ENTRY { \
    std::cout<<__PRETTY_FUNCTION__<<std::endl;\
}

#define NC_TRACE_EXIT { \
    std::cout<<__PRETTY_FUNCTION__<<std::endl;\
}


struct Box
{
    int m_minX;
    int m_minY;
    int m_maxX;
    int m_maxY;
    Box()
    : m_minX(0)
    , m_minY(0)
    , m_maxX(0)
    , m_maxY(0)
    {
    }
};

std::vector<Box> GetBindingBox(int m, int n, int pic[][9]);


#endif
