#pragma once

#include <queue>

template<typename T>
class CStack{
public:
    CStack();
    ~CStack();

public:
    void push(const T& node);
    void pop();
    bool empty();

private:
    std::queue<T> pushQueue;
    std::queue<T> popQueue;
};

template<typename T>
CStack<T>::CStack()
{

}

template<typename T>
CStack<T>::~CStack()
{

}

template<typename T>
void CStack<T>::push(const T& node)
{
    pushQueue.push(node);
}

template<typename T>
void CStack<T>::pop()
{
    if(!pushQueue.empty() && popQueue.empty())
    {
        while(pushQueue.size() > 1)
        {
            T& data = pushQueue.front();
            popQueue.push(data);
            pushQueue.pop();
        }
    }

    pushQueue.pop();

}

template<typename T>
bool CStack<T>::empty()
{
    if(pushQueue.empty() && popQueue.empty())
    {
        return true;
    }

    return false;
}
