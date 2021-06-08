#pragma once

#include <vector>

void Bubble(std::vector<int> &arr)
{
    unsigned length = arr.size();
    for(size_t i=0; i<length; i++)
    {
        for(size_t j=1; j < length-i; j++)
        {
            if(arr[j] < arr[j-1])
            {
                std::swap(arr[j-1], arr[j]);
            }
        }
    }
}

void Quickly(std::vector<int>& arr, int start, int end)
{
    if(start>=end)
    {
        return;
    }

    int i = start;
    int j = end;
    int pivot = arr[start];
    while(start<end)
    {
        while(arr[i] <= pivot && i<j)
        {
            i++;
        }
        while(arr[j] >= pivot && i<j)
        {
            j--;
        }
        if(i<j)
        {
            std::swap(arr[i], arr[j]);
        }
    }

    arr[start] = arr[i];
    arr[i] = pivot;

    Quickly(arr, start, i-1);
    Quickly(arr, i+1, end);

}