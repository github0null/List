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
#include <stdlib.h>

#if defined __has_include
#if __has_include("list_conf.h")
#include "list_conf.h"
#endif
#else
#include "list_conf.h"
#endif

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

/**
 * @brief A List Node Comparer Callbk
 * 
 * @param dat1 The data_1's pointer which will be compared
 * @param dat2 The data_2's pointer which will be compared
 * 
 * @return if ret == 0, dat1 == dat2; 
 *         if ret > 0,  dat1 > dat2; 
 *         if ret < 0,  dat1 < dat2;
 */
typedef int (*ListNodeComparer_t)(void *dat1, void *dat2);

/**
 * @brief A List Node Matcher Callbk
 * 
 * @param dat The data pointer
 * @param params The user context data
 * 
 * @return If true, match success, otherwise match failed.
 */
typedef bool (*ListNodeMatcher_t)(void *dat, void *params);

/**
 * @brief A Data Destructor Callbk for 'List_CreateList(...)'
 * 
 * @param dat The data pointer which will be freed
 * 
 * @return none
 */
typedef void (*ListDataDestructor_t)(void *dat);

/**
 * @brief A Visitor Callbk for 'List_Traverse(...)'
 * 
 * @param dat The data pointer
 * @param params The user context data, can be passed by 'List_Traverse(...)'
 * 
 * @return If true, the traverse will be breaked, end early
 */
typedef bool (*ListVisitor_t)(void *dat, void *params);

typedef struct _ListNode {
    struct _ListNode *prev;
    struct _ListNode *next;
    void *data;
} ListNode_t;

typedef struct List_t List_t;

List_t *List_CreateList(ListDataDestructor_t destructor);
void List_DestroyList(List_t *list);

ListNode_t *List_Prepend(List_t *list, void *data);
ListNode_t *List_Push(List_t *list, void *data);
ListNode_t *List_Pop(List_t *list);

ListNode_t *List_Enqueue(List_t *list, void *data);
ListNode_t *List_Dequeue(List_t *list);

ListNode_t *List_FindFirst(List_t *list, ListNodeMatcher_t matcher, void *params);
ListNode_t *List_FindNext(List_t *list, ListNode_t *node, ListNodeMatcher_t matcher, void *params);

ListNode_t *List_First(List_t *list);
ListNode_t *List_Last(List_t *list);
uint32_t List_Length(List_t *list);

ListNode_t *List_InsertNode(List_t *list, ListNode_t *node, void *data);
ListNode_t *List_RemoveNode(List_t *list, ListNode_t *node);

void List_DeleteAll(List_t *list);
void List_DeleteNode(List_t *list, ListNode_t *node);
void List_DeleteMatched(List_t *list, ListNodeMatcher_t matcher, void *params);

void List_Traverse(List_t *list, ListVisitor_t visitor, void *params, bool isReverse);

void List_QuickSort(List_t *list, ListNodeComparer_t comparer);

uint32_t List_Count(List_t *list, ListNodeMatcher_t matcher, void *params);
bool List_IsEmpty(List_t *list);

#endif
