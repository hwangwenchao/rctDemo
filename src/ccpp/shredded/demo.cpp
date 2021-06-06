#include "ncFibonacci.h"
#include "ncStack.h"
#include "ncQueue.h"
#include "ncShreddedCommon.h"
#include <iostream>
#include <vector>

#ifndef __WINDOWS__
#include <time.h>
#include <chrono>
#endif

#ifndef __WINDOWS__
int64_t GetCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1000 + tv.tv_sec/1000;
}

int64_t GetTimeStamp()
{
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());//获取当前时间点
    std::time_t timestamp =  tp.time_since_epoch().count(); //计算距离1970-1-1,00:00的时间长度

    return timestamp;
}
#endif

#ifdef __WINDOWS__
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
    std::cout<<"------------start--------------"<<std::endl;

    /*
    int pic[4][9] = {
        {0, 0, 0, 1, 1, 1, 0, 1, 1},
        {0, 0, 1, 1, 1, 0, 0, 0, 1},
        {0, 0, 1, 0, 0, 1, 0, 0, 1},
        {0, 0, 1, 0, 0, 0, 1, 1, 1},
    };

    std::vector<Box> boxes = GetBindingBox(4, 9, pic);
    for(size_t i = 0; i< boxes.size(); i++){
        std::cout<<"(("<<boxes.at(i).m_minX<<","<<boxes.at(i).m_minY<<"),";
        std::cout<<"("<<boxes.at(i).m_maxX<<", "<<boxes.at(i).m_maxY<<"))";
    }
    */

    //implement the queue with tow stack;
    // CQueue<int> queue;
    // queue.push(3);
    // queue.push(5232);

    // queue.pop();

    // CheckEqual(3, testNode);

    // implement the stach with two queue
    // CStack<int> stack;
    // stack.push(3);
    // stack.push(5232);

    // stack.pop();
    // bool isEmpty = stack.empty();
    // std::cout<<"delRes:"<<isEmpty<<std::endl;

    // Recursive
    long long resFibonacci = GetFibonacciSequence(50);
    #ifndef __WINDOWS__
    long long startTime = GetCurrentTime();
    std::cout<<"startTime:"<<startTime<<std::endl;
    #endif
    std::cout<<"resFibonacci:"<<resFibonacci<<std::endl;

    return 0;
}