#include "ncCursorList.h"
#include "ncList.h"
#include "ncSorts.h"
#include "ncCommonDef.h"
#include <stdio.h>

int main()
{
    printf("----------------start--------------------\n");

    List L;

    Position tempCell = (ptrToNode)malloc(sizeof(struct Node));
    tempCell->element = 1;
    tempCell->next = NULL;
    //IsEmpty(L);
    Insert(1, L, tempCell);
    Display(L);

    printf("-----------------------------------------\n");

    int A[] = {2, 5, 733, 4, 1, 90};
    DispalyEx(A, 6);
    InsertionSort(A, 6);
    HalfInsertionSort(A, 6);
    ShellSort(A, 6);
    BubbleSort(A, 6);
    QuickSort(A, 6);
    DispalyEx(A, 6);

    printf("-----------------end---------------------\n");
    return 0;
}