#pragma once
#include <stack>

//template

template<typename T>
class CQueue{
public:
    CQueue();
    ~CQueue();

public:
    void push(const T& node);
    void pop();
    bool empty();

private:
    std::stack<T> pushStack;
    std::stack<T> popStack;
};

// Queue FIFO
// Stack FILO

template<typename T>
CQueue<T>::CQueue()
{

}

template<typename T>
CQueue<T>::~CQueue()
{

}

template<typename T>
void CQueue<T>::push(const T& node)
{
    pushStack.push(node);
}

template<typename T>
void CQueue<T>::pop()
{
    if(popStack.empty() && !pushStack.empty())
    {
        while(!pushStack.empty())
        {
            T& data = pushStack.top();
            popStack.push(data);
            pushStack.pop();
        }
    }

    popStack.pop();
}

template<typename T>
bool CQueue<T>::empty()
{
    if(pushStack.empty() && popStack.empty())
    {
        return true;
    }

    return false;
}
