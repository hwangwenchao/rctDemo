#pragma once

#include <vector>

const int STEP_LENGTH = 3;

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

int Median3(std::vector<int> &arr, int start, int end)
{
    int mid = (start+end)/2;
    if(arr[start] > arr[mid])
    {
        std::swap(arr[start], arr[mid]);
    }
    if(arr[start] > arr[end])
    {
        std::swap(arr[start], arr[end]);
    }
    if(arr[mid] > arr[end])
    {
        std::swap(arr[mid], arr[end]);
    }

    std::swap(arr[mid], arr[end-1]);

    return arr[end-1];
}

void QuicklyEx(std::vector<int>& arr, int start, int end)
{
    if(start+STEP_LENGTH <= end)
    {
        int pivot = Median3(arr, start, end);
        int i = start;
        int j = end - 1;
        for(;;)
        {
            while(arr[++j] < pivot)
                ;
            while(arr[++i] > pivot)
                ;
            if(i<j)
            {
                std::swap(arr[i], arr[j]);
            }
            else
                break;
        }
        std::swap(arr[i], arr[j-1]);

        QuicklyEx(arr, start, i-1);
        QuicklyEx(arr, i+1, end);
    }
}