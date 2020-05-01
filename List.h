#ifndef _H_C_List
#define _H_C_List

#include <stdint.h>
#include "Mem.h"

#define INVALID_PTR ((void *)0xFFFFFFFFU)

typedef struct _ListNode {
	void *data;
	struct _ListNode *next;
	struct _ListNode *prev;
} ListNode;

typedef struct {
	ListNode *head;
	ListNode *tail;
	uint32_t length;
} List;

typedef int(*NodeComparer)(void *, void *);
typedef uint8_t(*NodeMatcher)(void *);
typedef void(*DataDestructor)(void *);
typedef uint8_t(*Visitor)(void *);

List *List_CreateList(void);
void List_DestroyList(List *list, DataDestructor destructor);

ListNode *List_Prepend(List * list, void * data);

ListNode *List_Pop(List *list);
ListNode *List_Push(List *list, void *data);

ListNode *List_Dequeue(List *list);
ListNode *List_Enqueue(List *list, void *data);

ListNode *List_FindFirst(List *list, NodeMatcher matcher);
ListNode *List_FindNext(ListNode *node, NodeMatcher matcher);

ListNode *List_First(List *list);
ListNode *List_Last(List *list);

ListNode *List_InsertNode(List *list, ListNode *node, void *data);
ListNode *List_RemoveNode(List *list, ListNode *node);

void List_DeleteMatched(List *list, NodeMatcher matcher, DataDestructor destructor);

void List_Traverse(List *list, uint8_t isReverse, Visitor visitor);

void List_QuickSort(List* list, NodeComparer comparer);

uint32_t List_Count(List *list, NodeMatcher matcher);

uint8_t List_IsEmpty(List *list);

#endif

