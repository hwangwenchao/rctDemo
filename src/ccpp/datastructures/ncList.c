#include "ncList.h"
#include "ncCommonDef.h"
#include <stddef.h>
#include <stdlib.h>

bool IsEmpty(List L)
{
    NC_TRACE_ENTRY
    return L->next == NULL;
}

bool IsLast(Position P, List L)
{
    NC_TRACE_ENTRY
    return P->next == NULL;
}

Position Find(ElementType X, List L)
{
    NC_TRACE_ENTRY
    Position P;
    P = L->next;
    while(P != NULL && P->element != X){
        P = P->next;
    }

    return P;
}

void Delete(ElementType X, List L)
{
    NC_TRACE_ENTRY
    Position P;
    Position tempCell;

    P = FindPrevious(X, L);

    if(!IsLast(P, L)){
        tempCell = P->next;
        P->next = tempCell->next;
        free(tempCell);
    }
    NC_TRACE_EXIT
}

Position FindPrevious(ElementType X, List L)
{
    NC_TRACE_ENTRY
    Position P;
    P = L;
    while (P->next != NULL && P->next->element != X){
        P = P->next;
    }
    return P;
}

void Insert(ElementType X, List L, Position P)
{
    NC_TRACE_ENTRY
    Position tempCell;
    tempCell = malloc(sizeof(struct Node));
    if(tempCell == NULL){
        printf("Failed to allocate memory!!!\n");
        return;
    }
    tempCell->element = X;
    tempCell->next = P->next;
    P->next = tempCell;
    NC_TRACE_EXIT
}

void Display(List L)
{
    NC_TRACE_ENTRY
    while(L!=NULL){
        printf("Element:%d\n", L->element);
        L=L->next;
    }
    NC_TRACE_EXIT
}