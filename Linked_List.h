/*
    MIT License

    Copyright (c) 2020 github0null

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef _H_C_List
#define _H_C_List

#include <stdint.h>
#include <stdbool.h>
#include <list_conf.h>

#ifndef List_nullptr
#define List_nullptr ((void *)0)
#endif

#ifndef List_Inline
#define List_Inline
#endif

#ifndef List_malloc
#define List_malloc malloc
#endif

#ifndef List_free
#define List_free free
#endif

#ifdef LIST_THREAD_SAFED

#ifndef List_MutexNew
#error "We need 'List_MutexNew' in os !"
#endif

#ifndef List_MutexFree
#error "We need 'List_MutexFree' in os !"
#endif

#ifndef List_MutexAcquire
#error "We need 'List_MutexAcquire' in os !"
#endif

#ifndef List_MutexRelease
#error "We need 'List_MutexRelease' in os !"
#endif

#endif

typedef int (*NodeComparer)(void *dat1, void *dat2);
typedef bool (*NodeMatcher)(void *dat, void *params);
typedef void (*DataDestructor)(void *dat);
typedef bool (*Visitor)(void *dat, void *params);

typedef struct _ListNode
{
    void *data;
    struct _ListNode *next;
    struct _ListNode *prev;
} ListNode;

typedef struct List List;

List *List_CreateList(DataDestructor destructor);
void List_DestroyList(List *list);

ListNode *List_Prepend(List *list, void *data);
ListNode *List_Push(List *list, void *data);
ListNode *List_Pop(List *list);

ListNode *List_Enqueue(List *list, void *data);
ListNode *List_Dequeue(List *list);

ListNode *List_FindFirst(List *list, NodeMatcher matcher, void *params);
ListNode *List_FindNext(List *list, ListNode *node, NodeMatcher matcher, void *params);

ListNode *List_First(List *list);
ListNode *List_Last(List *list);
uint32_t List_Length(List *list);

ListNode *List_InsertNode(List *list, ListNode *node, void *data);
ListNode *List_RemoveNode(List *list, ListNode *node);

void List_DeleteAll(List *list);
void List_DeleteNode(List *list, ListNode *node);
void List_DeleteMatched(List *list, NodeMatcher matcher, void *params);

void List_Traverse(List *list, bool isReverse, Visitor visitor, void *params);
void List_QuickSort(List *list, NodeComparer comparer);

uint32_t List_Count(List *list, NodeMatcher matcher, void *params);
bool List_IsEmpty(List *list);

// --- default callbacks

bool List_DefEqualMatcher(void *dat, void *params);

#endif
