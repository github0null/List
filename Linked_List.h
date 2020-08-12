#ifndef _H_C_List
#define _H_C_List

#include <stdint.h>
#include <list_conf.h>

#ifndef List_nullptr
#define List_nullptr ((void *)0xFFFFFFFFU)
#endif

#ifndef List_Lock
#define List_Lock()
#endif

#ifndef List_UnLock
#define List_UnLock()
#endif

#ifndef List_Inline_Func
#define List_Inline_Func inline
#endif

#ifndef List_malloc
#define List_malloc malloc
#endif

#ifndef List_free
#define List_free free
#endif

typedef int(*NodeComparer)(void *, void *);
typedef uint8_t(*NodeMatcher)(void *node, void *params);
typedef void(*DataDestructor)(void *);
typedef uint8_t(*Visitor)(void *);

typedef struct _ListNode {
	void *data;
	struct _ListNode *next;
	struct _ListNode *prev;
} ListNode;

typedef struct List List;

List *List_CreateList(DataDestructor destructor);
void List_DestroyList(List *list);

ListNode *List_Prepend(List * list, void * data);
ListNode *List_Push(List *list, void *data);
ListNode *List_Pop(List *list);

ListNode *List_Enqueue(List *list, void *data);
ListNode *List_Dequeue(List *list);

ListNode *List_FindFirst(List *list, NodeMatcher matcher, void *params);
ListNode *List_FindNext(ListNode *node, NodeMatcher matcher, void *params);

ListNode *List_First(List *list);
ListNode *List_Last(List *list);
uint32_t List_Length(List *list);

ListNode *List_InsertNode(List *list, ListNode *node, void *data);
ListNode *List_RemoveNode(List *list, ListNode *node);

void List_DeleteAll(List *list);
void List_DeleteNode(List *list, ListNode *node);
void List_DeleteMatched(List *list, NodeMatcher matcher, void *params);

void List_Traverse(List *list, uint8_t isReverse, Visitor visitor);

void List_QuickSort(List* list, NodeComparer comparer);

uint32_t List_Count(List *list, NodeMatcher matcher, void *params);

uint8_t List_IsEmpty(List *list);

#endif

