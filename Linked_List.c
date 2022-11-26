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

#include "Linked_List.h"

#undef NULL
#define NULL List_nullptr

#ifdef LIST_THREAD_SAFED
#define List_Lock(list)   List_MutexAcquire(list->lock)
#define List_UnLock(list) List_MutexRelease(list->lock)
#else
#define List_Lock(list)
#define List_UnLock(list)
#endif

struct List_t {
    ListNode_t *head;
    ListNode_t *tail;
    uint32_t length;
    ListDataDestructor_t destructor;
#ifdef LIST_THREAD_SAFED
    void *lock;
#endif
};

//----------------------------- internal func -----------------------------------

static List_Inline void _cut_prev(ListNode_t *node)
{
    node->prev->next = NULL;
    node->prev       = NULL;
}

static List_Inline void _cut_next(ListNode_t *node)
{
    node->next->prev = NULL;
    node->next       = NULL;
}

static List_Inline void _link_next(ListNode_t *node, ListNode_t *nNode)
{
    ListNode_t *next;

    if (node->next == NULL) {
        node->next  = nNode;
        nNode->prev = node;
    } else {
        next = node->next;
        _cut_next(node);
        node->next  = nNode;
        nNode->prev = node;
        nNode->next = next;
        next->prev  = nNode;
    }
}

static List_Inline void _swap_node(ListNode_t *n1, ListNode_t *n2)
{
    void *data1 = n1->data;
    n1->data    = n2->data;
    n2->data    = data1;
}

static List_Inline void _set_node(ListNode_t *node, void *data)
{
    node->data = data;
}

static void _null_data_destructor(void *nul)
{
    /* nothing todo */
}

static ListNode_t *_list_pop(List_t *list)
{
    ListNode_t *node = NULL;

    if (list->length == 0) {
        return node;
    }

    if (list->head == list->tail) {
        node         = list->head;
        list->head   = NULL;
        list->tail   = NULL;
        list->length = 0;
    } else {
        node       = list->tail;
        list->tail = node->prev;
        _cut_prev(node);
        list->length--;
    }

    return node;
}

static ListNode_t *_list_remove_node(List_t *list, ListNode_t *node)
{
    ListNode_t *prev, *next;

    if (list->length == 0) {
        return NULL;
    }

    if (node == list->head) {
        if (node->next != NULL) {
            list->head = node->next;
            _cut_next(node);
        } else {
            list->head = list->tail = NULL;
        }
    }

    else if (node == list->tail) {
        if (node->prev != NULL) {
            list->tail = node->prev;
            _cut_prev(node);
        } else {
            list->head = list->tail = NULL;
        }
    }

    else if (node->prev == NULL || node->next == NULL) {
        return NULL; // invalid node, skip
    }

    else {
        prev = node->prev;
        next = node->next;
        _cut_prev(node);
        _cut_next(node);
        prev->next = next;
        next->prev = prev;
    }

    list->length--;

    return node;
}

static ListNode_t *_list_find_first(List_t *list, ListNodeMatcher_t matcher, void *params)
{
    ListNode_t *node = list->head;

    while (node != NULL) {
        if (matcher(node->data, params)) break;
        node = node->next;
    }

    return node;
}

//-------------------------------------------------------

List_t *List_CreateList(ListDataDestructor_t destructor)
{
    List_t *list = (List_t *)List_mem_alloc(sizeof(List_t));

    list->length     = 0;
    list->head       = NULL;
    list->tail       = NULL;
    list->destructor = destructor == NULL ? _null_data_destructor : destructor;

#ifdef LIST_THREAD_SAFED
    list->lock = List_MutexNew();
#endif

    return list;
}

void List_DestroyList(List_t *list)
{
    List_Clear(list);
#ifdef LIST_THREAD_SAFED
    List_MutexFree(list->lock);
#endif
    List_mem_free(list);
}

void List_Clear(List_t *list)
{
    ListNode_t *node;

    List_Lock(list);
    {
        node = _list_pop(list);

        while (node != NULL) {
            list->destructor(node->data);
            List_mem_free(node);
            node = _list_pop(list);
        }
    }
    List_UnLock(list);
}

ListNode_t *List_Pop(List_t *list)
{
    ListNode_t *node;
    List_Lock(list);
    node = _list_pop(list);
    List_UnLock(list);
    return node;
}

ListNode_t *List_Push(List_t *list, void *data)
{
    ListNode_t *node;

    node = (ListNode_t *)List_mem_alloc(sizeof(ListNode_t));

    node->data = data;
    node->next = NULL;
    node->prev = NULL;

    List_Lock(list);
    {
        if (list->length == 0) {
            list->head = list->tail = node;
        } else {
            list->tail->next = node;
            node->prev       = list->tail;
            list->tail       = node;
        }

        list->length++;
    }
    List_UnLock(list);

    return node;
}

ListNode_t *List_Prepend(List_t *list, void *data)
{
    ListNode_t *node;

    node = (ListNode_t *)List_mem_alloc(sizeof(ListNode_t));

    node->data = data;
    node->next = NULL;
    node->prev = NULL;

    List_Lock(list);
    {
        if (list->length == 0) {
            list->head = list->tail = node;
        } else {
            node->next       = list->head;
            list->head->prev = node;
            list->head       = node;
        }

        list->length++;
    }
    List_UnLock(list);

    return node;
}

ListNode_t *List_Dequeue(List_t *list)
{
    ListNode_t *node = NULL;

    List_Lock(list);
    {
        if (list->length == 0) {
            List_UnLock(list);
            return node;
        }

        if (list->head == list->tail) {
            node         = list->head;
            list->head   = NULL;
            list->tail   = NULL;
            list->length = 0;
        } else {
            node       = list->head;
            list->head = node->next;
            _cut_next(node);
            list->length--;
        }
    }
    List_UnLock(list);

    return node;
}

ListNode_t *List_Enqueue(List_t *list, void *data)
{
    return List_Push(list, data);
}

ListNode_t *List_FindFirst(List_t *list, ListNodeMatcher_t matcher, void *params)
{
    ListNode_t *node;
    List_Lock(list);
    node = _list_find_first(list, matcher, params);
    List_UnLock(list);
    return node;
}

ListNode_t *List_FindNext(List_t *list, ListNode_t *node,
                          ListNodeMatcher_t matcher, void *params)
{
    ListNode_t *cNode;

    List_Lock(list);
    {
        cNode = node->next;

        while (cNode != NULL) {
            if (matcher(cNode->data, params)) break;
            cNode = cNode->next;
        }
    }
    List_UnLock(list);

    return cNode;
}

ListNode_t *List_First(List_t *list)
{
    ListNode_t *node;
    List_Lock(list);
    node = list->head;
    List_UnLock(list);
    return node;
}

ListNode_t *List_Last(List_t *list)
{
    ListNode_t *node;
    List_Lock(list);
    node = list->tail;
    List_UnLock(list);
    return node;
}

uint32_t List_Length(List_t *list)
{
    uint32_t len;
    List_Lock(list);
    len = list->length;
    List_UnLock(list);
    return len;
}

ListNode_t *List_InsertNode(List_t *list, ListNode_t *node, void *data)
{
    ListNode_t *nNode;

    nNode       = (ListNode_t *)List_mem_alloc(sizeof(ListNode_t));
    nNode->prev = NULL;
    nNode->next = NULL;
    nNode->data = data;

    List_Lock(list);
    {
        _link_next(node, nNode);
        if (node == list->tail) list->tail = nNode;
        list->length++;
    }
    List_UnLock(list);

    return nNode;
}

ListNode_t *List_InsertNodeBefore(List_t *list, ListNode_t *node, void *data)
{
    ListNode_t *nNode;

    nNode       = (ListNode_t *)List_mem_alloc(sizeof(ListNode_t));
    nNode->prev = NULL;
    nNode->next = NULL;
    nNode->data = data;

    List_Lock(list);
    {
        if (node == list->head) {
            list->head       = nNode;
            list->head->next = node;
            node->prev       = list->head;
        } else {
            _link_next(node->prev, nNode);
        }

        list->length++;
    }
    List_UnLock(list);

    return nNode;
}

ListNode_t *List_RemoveNode(List_t *list, ListNode_t *node)
{
    ListNode_t *n;
    List_Lock(list);
    n = _list_remove_node(list, node);
    List_UnLock(list);
    return n;
}

void List_DeleteNode(List_t *list, ListNode_t *node)
{
    List_Lock(list);
    _list_remove_node(list, node);
    list->destructor(node->data);
    List_mem_free(node);
    List_UnLock(list);
}

void List_DeleteMatched(List_t *list, ListNodeMatcher_t matcher, void *params)
{
    ListNode_t *current, *n;

    List_Lock(list);
    {
        current = list->head;

        while (current != NULL) {

            if (matcher(current->data, params)) {
                n       = current->next;
                current = _list_remove_node(list, current);
                list->destructor(current->data);
                List_mem_free(current);
                current = n;
            }

            else {
                current = current->next;
            }
        }
    }
    List_UnLock(list);
}

uint32_t List_Count(List_t *list, ListNodeMatcher_t matcher, void *params)
{
    uint32_t count = 0;
    ListNode_t *node;

    List_Lock(list);
    {
        node = _list_find_first(list, matcher, params);

        if (node == NULL) {
            List_UnLock(list);
            return count;
        }

        count++;

        do {
            if (matcher(node->data, params)) count++;
            node = node->next;
        } while (node != NULL);
    }
    List_UnLock(list);

    return count;
}

bool List_IsEmpty(List_t *list)
{
    uint32_t len;
    List_Lock(list);
    len = list->length;
    List_UnLock(list);
    return len == 0;
}

void List_Traverse(List_t *list, ListVisitor_t visitor, void *params, bool isReverse)
{
    ListNode_t *current;

    List_Lock(list);
    {
        if (isReverse) {

            current = list->tail;

            while (current != NULL) {
                if (!visitor(current->data, params)) break;
                current = current->prev;
            }

        } else {

            current = list->head;

            while (current != NULL) {
                if (!visitor(current->data, params)) break;
                current = current->next;
            }
        }
    }
    List_UnLock(list);
}

//----------------------- quick sort ---------------------------

typedef struct {
    ListNode_t *first;
    ListNode_t *last;
} _sort_data;

static ListNode_t *_sort_part(ListNode_t *_first, ListNode_t *_last, ListNodeComparer_t comparer)
{
    ListNode_t *pit = _first, *first = _first, *last = _last;
    void *base   = _first->data;
    bool isRight = true;

    while (first != last) {
        if (isRight) {
            if (comparer(base, last->data) >= 0) {
                _set_node(pit, last->data);
                pit     = last;
                isRight = false;
                first   = first->next;
            } else {
                last = last->prev;
            }
        } else {
            if (comparer(first->data, base) > 0) {
                _set_node(pit, first->data);
                pit     = first;
                isRight = true;
                last    = last->prev;
            } else {
                first = first->next;
            }
        }
    }

    _set_node(first, base);

    return first;
}

void List_QuickSort(List_t *list, ListNodeComparer_t comparer)
{
    List_t *stack      = List_CreateList(NULL);
    _sort_data *nsData = NULL, *sData = NULL;
    ListNode_t *node = NULL, *middle = NULL;

    List_Lock(list);

    nsData        = (_sort_data *)List_mem_alloc(sizeof(_sort_data));
    nsData->first = list->head;
    nsData->last  = list->tail;

    if (nsData->first == nsData->last) {
        List_mem_free(nsData);
        List_UnLock(list);
        return;
    }

    List_Push(stack, nsData);

    while (!List_IsEmpty(stack)) {

        node  = List_Pop(stack);
        sData = node->data;

        if (sData->first->next != sData->last) {

            middle = _sort_part(sData->first, sData->last, comparer);

            // right
            if (middle != sData->last) {
                nsData        = (_sort_data *)List_mem_alloc(sizeof(_sort_data));
                nsData->first = middle->next;
                nsData->last  = sData->last;
                List_Push(stack, nsData);
            }

            // left
            if (middle != sData->first) {
                nsData        = (_sort_data *)List_mem_alloc(sizeof(_sort_data));
                nsData->first = sData->first;
                nsData->last  = middle->prev;
                List_Push(stack, nsData);
            }
        } else {
            if (comparer(sData->first->data, sData->last->data) > 0) {
                _swap_node(sData->first, sData->last);
            }
        }

        List_mem_free(sData);
        List_mem_free(node);
    }

    List_mem_free(stack);

    List_UnLock(list);
}
