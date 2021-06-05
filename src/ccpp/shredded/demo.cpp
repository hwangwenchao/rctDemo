#include "ncStack.h"
#include "ncQueue.h"
#include "ncShreddedCommon.h"
#include <iostream>
#include <vector>

template<typename T>
bool CheckEqual(const T& left, const T& right)
{
    if(left == right)
    {
        std::cout<<"successfully delete node~"<<std::endl;
        return true;
    }
    else
    {
        std::cout<<"Failed to delete node!"<<std::endl;
        return false;
    }

    return false;
}

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
    CStack<int> stack;
    stack.push(3);
    stack.push(5232);

    stack.pop();
    bool isEmpty = stack.empty();
    std::cout<<"delRes:"<<isEmpty<<std::endl;

    return 0;
}