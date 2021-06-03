#include "ncSorts.h"
#include <stdbool.h>

void SwapEx(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void BubbleSort(int A[], int length)
{
    NC_TRACE_ENTRY
    int i;
    int j;
    bool flag = false;
    for(i=0; i<length; i++)
    {
        for(j=i+1; j<length; j++)
        {
            if(A[i] > A[j])
            {
                flag = true;
                SwapEx(&A[i], &A[j]);
            }
            if(!flag)
            {
                break;
            }
        }
    }

    NC_TRACE_EXIT
}
/*
void ShellSort(int A[], int length)
{
    NC_TRACE_ENTRY

    int d = length/2;
    int tmp;
    while(d>0)
    {
        int i;
        for(i=1; i>d; i++)
        {
            int j;
            for(j=i+d; j<length; j=j+d)
            {
                int k;
                tmp = A[j];
                k = j-d;
                while(k>0 && A[k]>tmp)
                {
                    A[k+d] = A[k];
                    k = k-d;
                }
                A[k+d] = tmp;
            }
        }
        d = d/2;
    }

    NC_TRACE_EXIT
}
*/

void ShellSort(int A[], int length)
{
    int i; //原数组索引
    int j; //增量序列索引
    int incerStep;
    int tmp;
    for(incerStep = length/2; incerStep>0; incerStep /=2)
    {
        for(i = incerStep; i<length; i++)
        {
            tmp = A[i];
            for(j=i; j>=incerStep; j-=incerStep)
            {
                if(tmp<A[j-incerStep])
                {
                    A[j] = A[j-incerStep];
                }
                else{
                    break;
                }
                A[j] = tmp;
            }
        }
    }
}

void InsertionSort(int A[], int length)
{
    NC_TRACE_ENTRY
    int i;//数组的索引|从第二个元素开始，默认第一个有序
    int tmp;
    int j; //待排序临时数组索引

    for(i = 1; i < length; i++){
        tmp = A[i];
        // 内层排序
        for(j = i; j>0 && A[j-1] > tmp; j--){
            A[j] = A[j-1];
        }
        A[j] = tmp;
    }
    NC_TRACE_EXIT
}


void HalfInsertionSort(int A[], int length)
{
    NC_TRACE_ENTRY

    int i;
    int tmp;
    int j;

    for(i=1; i< length; i++){
        if(A[i] < A[i-1]){
            tmp = A[i];
            int left = 0;
            int right = i-1;
            while (left <= right)
            {
                int mid = (right-left);
                if(A[mid] > tmp) // 待插入位置为当前key右侧
                {
                    right = mid - 1;
                }
                else
                {
                    left = mid + 1;
                }
            }
            for (j=i-1; j >= right+1; j--)
            {
                A[j+1] = A[j];
            }

            A[right+1] = tmp;
        }
        else
        {
            // do nothing
        }
    }

    NC_TRACE_EXIT
}

void DispalyEx(int A[], int length)
{
    int i;
    for(i=0; i< length; i++){
        printf("%d\n", A[i]);
    }
}

int Median3(int A[], int left, int right)
{
    NC_TRACE_ENTRY
    int center = (left+right)/2;
    if(A[left] > A[center])
    {
        SwapEx(&A[left], &A[center]);
    }
    if(A[left] > A[right])
    {
        SwapEx(&A[left], &A[right]);
    }
    if(A[center] > A[right])
    {
        SwapEx(&A[center], &A[right]);
    }

    SwapEx(&A[center], &A[right-1]);

    return A[right-1];

    NC_TRACE_EXIT
}


void QSort(int A[], int left, int right)
{

    NC_TRACE_ENTRY
    int i; // 待排左数组|枢纽元左边
    int j; // 待排右数组|枢纽元右边
    int pivot;

    if(left + CUTOFF_RANGE <= right)
    {
        pivot = Median3(A, left, right);
        i = left;
        j = right - 1;

        for(;;)
        {
            while(A[++i] < pivot)
                ;
            while (A[--j] > pivot)
                ;
            if(i<j)
            {
                SwapEx(&A[i], &A[j]);
            }
            else
            {
                break;
            }
        }

        SwapEx(&A[i], &A[right-1]);

        QSort(A, left, i-1);
        QSort(A, i+1, right);
    }
    else
    {
        InsertionSort(A+left, right-left+1);
    }

    NC_TRACE_EXIT
}

void QuickSort(int A[], int length)
{
    NC_TRACE_ENTRY
    QSort(A, 0, length-1);
    NC_TRACE_EXIT
}