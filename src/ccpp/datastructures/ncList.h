#ifndef __LIST_H__
#define __LIST_H__

#include "ncCommonDef.h"
#include <stdbool.h>

struct Node;
typedef struct Node* ptrToNode;
typedef ptrToNode List;
typedef ptrToNode Position;

struct Node{
    ElementType element;
    Position next;
};

List MakeEmpty(List L);
bool IsEmpty(List L);
bool IsLast(Position P, List L);
Position Find(ElementType X, List L);
void Delete(ElementType x, List L);
Position FindPrevious(ElementType X, List L);
void Insert(ElementType X, List L, Position P);
void DeleteList(List L);
Position Header(List L);
Position First(List L);
Position Advance(Position P);
ElementType Retrieve(Position P);


#endif