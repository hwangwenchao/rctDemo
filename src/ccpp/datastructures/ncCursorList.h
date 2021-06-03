#ifndef __NC_CURSOR_LIST_H__
#define __NC_CURSOR_LIST_H__
#include "ncCommonDef.h"
#include <stdbool.h>

typedef int ptrToNode;
typedef ptrToNode List;
typedef ptrToNode Position;

void InitCursorSpace(void);
List MakeEmpty(List L);
bool IsEmpty(const List L);
bool IsLast(const List L);
Position Find(ElementType X, const List L);
Position FindPrevious(ElementType X, const List L);
void Insert(ElementType X, List L, Position P);
void Delete(ElementType X, List L);
Position Header(const List L);
Position First(const List L);
Position Adcvandce(const Position P);
ElementType Retrieve(const Position P);

static Position CursorAlloc(void);
static Position CursorFree(Position P);

#endif