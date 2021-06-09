#ifndef __SORT_ALGORITHM_H__
#define __SORT_ALGORITHM_H__
#include "ncCommonDef.h"
#include <stdio.h>

void SwapEx(int* a, int* b);
void BubbleSort(int A[], int length);
void QuickSort(int A[], int length);
void BubbleSort(int A[], int length);
void ShellSort(int A[], int length);
void InsertionSort(int A[], int length);
void HalfInsertionSort(int A[], int length);
void DispalyEx(int A[], int length);
int Median3(int A[], int left, int right);
void QSort(int A[], int left, int right);

#endif