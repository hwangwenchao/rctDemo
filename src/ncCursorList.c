#include "ncCursorList.h"

struct Node{
    ElementType element;
    Position next;
};

struct Node CursorSpace[SPACE_SIZE];

static Position CursorAlloc(void)
{
    Position P;
    P = CursorSpace[0].next;
    CursorSpace[0].next = CursorSpace[P].next;

    return P;
}

static Position CursorFree(Position P)
{
    CursorSpace[P].next = CursorSpace[0].next;
    CursorSpace[0].next = P;
}

bool IsEmpty(List L)
{
    return CursorSpace[L].next == NULL;
}

bool IsLast(Position P, List L)
{
    return CursorSpace[P].next == 0;
}

Position Find(ElementType X, List L)
{
    Position P;
    P = CursorSpace[L].next;
    while(P && CursorSpace[P].element != X)
    {
        P= CursorSpace[P].next;
    }
    return P;
}

void Delete(ElementType X, List L)
{
    Position P;
    Position tmpCell;
    P = FindPrevious(X, L);
    if(!IsLast(P, L))
    {
        tmpCell = CursorSpace[P].next;
        CursorSpace[P].next = CursorSpace[tmpCell].next;
        CursorFree(tmpCell);
    }
}

void Insert(ElementType X, List L, Position P)
{
    Position tmpCell;
    tmpCell = CursorAlloc();
    if(tmpCell == 0)
    {
        printf("Failed to allocte memory!\n");
    }
    CursorSpace[tmpCell].element = X;
    CursorSpace[tmpCell].next = CursorSpace[P].next;
    CursorSpace[P].next = tmpCell;
}