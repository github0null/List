#include <Linked_List.h>

#define true 1
#define false 0

#undef NULL
#define NULL List_nullptr

inline void _cut_prev(ListNode *node)
{
    node->prev->next = NULL;
    node->prev = NULL;
}

inline void _cut_next(ListNode *node)
{
    node->next->prev = NULL;
    node->next = NULL;
}

inline void _link_next(ListNode *node, ListNode *nNode)
{
    ListNode *next;
    if (node->next == NULL)
    {
        node->next = nNode;
        nNode->prev = node;
    }
    else
    {
        next = node->next;
        _cut_next(node);
        node->next = nNode;
        nNode->prev = node;
        nNode->next = next;
        next->prev = nNode;
    }
}

inline void _swap_node(ListNode *n1, ListNode *n2)
{
    void *data1 = n1->data;
    n1->data = n2->data;
    n2->data = data1;
}

inline void _set_node(ListNode *node, void *data)
{
    node->data = data;
}

//-------------------------------------------------------

ListNode *_sort_part(ListNode *_first, ListNode *_last, NodeComparer comparer)
{
    ListNode *pit = _first, *first = _first, *last = _last;
    void *base = _first->data;
    uint8_t isRight = true;

    while (first != last)
    {
        if (isRight)
        {
            if (comparer(base, last->data) >= 0)
            {
                _set_node(pit, last->data);
                pit = last;
                isRight = false;
                first = first->next;
            }
            else
            {
                last = last->prev;
            }
        }
        else
        {
            if (comparer(first->data, base) > 0)
            {
                _set_node(pit, first->data);
                pit = first;
                isRight = true;
                last = last->prev;
            }
            else
            {
                first = first->next;
            }
        }
    }

    _set_node(first, base);

    return first;
}

List *List_CreateList()
{
    List_Lock();
    List *list = (List *)List_malloc(sizeof(List));
    list->length = 0;
    list->head = NULL;
    list->tail = NULL;
    List_UnLock();
    return list;
}

void List_DestroyList(List *list, DataDestructor destructor)
{
    List_Lock();
    List_RemoveAll(list, destructor);
    List_free(list);
    List_UnLock();
}

void List_RemoveAll(List *list, DataDestructor destructor)
{
    List_Lock();

    ListNode *node = List_Pop(list);
    while (node != NULL)
    {
        destructor(node->data);
        List_free(node);
        node = List_Pop(list);
    }

    List_UnLock();
}

ListNode *List_Pop(List *list)
{
    List_Lock();

    ListNode *node = NULL;
    if (list->length == 0)
    {
        List_UnLock();
        return node;
    }

    if (list->head == list->tail)
    {
        node = list->head;
        list->head = NULL;
        list->tail = NULL;
        list->length = 0;
    }
    else
    {
        node = list->tail;
        list->tail = node->prev;
        _cut_prev(node);
        list->length--;
    }

    List_UnLock();

    return node;
}

ListNode *List_Push(List *list, void *data)
{
    List_Lock();

    ListNode *node = (ListNode *)List_malloc(sizeof(ListNode));

    node->data = data;
    node->next = NULL;
    node->prev = NULL;

    if (list->length == 0)
    {
        list->head = list->tail = node;
    }
    else
    {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }

    list->length++;

    List_UnLock();

    return node;
}

ListNode *List_Prepend(List *list, void *data)
{
    List_Lock();

    ListNode *node = (ListNode *)List_malloc(sizeof(ListNode));

    node->data = data;
    node->next = NULL;
    node->prev = NULL;

    if (list->length == 0)
    {
        list->head = list->tail = node;
    }
    else
    {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }

    list->length++;

    List_UnLock();

    return node;
}

ListNode *List_Dequeue(List *list)
{
    List_Lock();

    ListNode *node = NULL;
    if (list->length == 0)
    {
        List_UnLock();
        return node;
    }

    if (list->head == list->tail)
    {
        node = list->head;
        list->head = NULL;
        list->tail = NULL;
        list->length = 0;
    }
    else
    {
        node = list->head;
        list->head = node->next;
        _cut_next(node);
        list->length--;
    }

    List_UnLock();

    return node;
}

ListNode *List_Enqueue(List *list, void *data)
{
    return List_Push(list, data);
}

ListNode *List_FindFirst(List *list, NodeMatcher matcher)
{
    List_Lock();

    ListNode *node = list->head;

    while (node != NULL)
    {
        if (matcher(node->data))
            break;
        node = node->next;
    }

    List_UnLock();

    return node;
}

ListNode *List_FindNext(ListNode *node, NodeMatcher matcher)
{
    List_Lock();

    ListNode *cNode = node->next;

    while (cNode != NULL)
    {
        if (matcher(cNode->data))
            break;
        cNode = cNode->next;
    }

    List_UnLock();

    return cNode;
}

ListNode *List_First(List *list)
{
    return list->head;
}

ListNode *List_Last(List *list)
{
    return list->tail;
}

uint32_t List_Length(List *list)
{
    return list->length;
}

ListNode *List_InsertNode(List *list, ListNode *node, void *data)
{
    List_Lock();

    ListNode *nNode = (ListNode *)List_malloc(sizeof(ListNode));
    nNode->data = data;
    _link_next(node, nNode);
    if (node == list->tail)
    {
        list->tail = nNode;
    }
    list->length++;

    List_UnLock();

    return nNode;
}

ListNode *List_RemoveNode(List *list, ListNode *node)
{
    ListNode *prev, *next;

    List_Lock();

    if (list->length == 0)
    {
        List_UnLock();
        return NULL;
    }

    if (node == list->head)
    {
        if (node->next != NULL)
        {
            list->head = node->next;
            _cut_next(node);
        }
        else
        {
            list->head = list->tail = NULL;
        }
    }
    else if (node == list->tail)
    {
        if (node->prev != NULL)
        {
            list->tail = node->prev;
            _cut_prev(node);
        }
        else
        {
            list->head = list->tail = NULL;
        }
    }
    else
    {
        prev = node->prev;
        next = node->next;
        _cut_prev(node);
        _cut_next(node);
        prev->next = next;
        next->prev = prev;
    }

    list->length--;

    List_UnLock();

    return node;
}

void List_DeleteMatched(List *list, NodeMatcher matcher, DataDestructor destructor)
{
    List_Lock();

    ListNode *current = list->head, *n;

    while (current != NULL)
    {
        if (matcher(current->data))
        {
            n = current->next;
            current = List_RemoveNode(list, current);
            destructor(current->data);
            List_free(current);
            current = n;
        }
        else
        {
            current = current->next;
        }
    }

    List_UnLock();
}

uint32_t List_Count(List *list, NodeMatcher matcher)
{
    List_Lock();

    uint32_t count = 0;
    ListNode *node = List_FindFirst(list, matcher);

    if (node == NULL)
    {
        List_UnLock();
        return count;
    }

    count++;

    do
    {
        if (matcher(node->data))
            count++;
        node = node->next;
    } while (node != NULL);

    List_UnLock();

    return count;
}

uint8_t List_IsEmpty(List *list)
{
    return list->length == 0;
}

typedef struct
{
    ListNode *first;
    ListNode *last;
} _sort_data;

void List_QuickSort(List *list, NodeComparer comparer)
{
    List_Lock();

    List *stack = List_CreateList();
    _sort_data *nsData = (_sort_data *)List_malloc(sizeof(_sort_data)), *sData = NULL;
    ListNode *node = NULL, *middle = NULL;

    nsData->first = list->head;
    nsData->last = list->tail;

    if (nsData->first == nsData->last)
    {
        List_UnLock();
        return;
    }

    List_Push(stack, nsData);

    while (!List_IsEmpty(stack))
    {
        node = List_Pop(stack);
        sData = node->data;

        if (sData->first->next != sData->last)
        {

            middle = _sort_part(sData->first, sData->last, comparer);

            //right
            if (middle != sData->last)
            {
                nsData = (_sort_data *)List_malloc(sizeof(_sort_data));
                nsData->first = middle->next;
                nsData->last = sData->last;
                List_Push(stack, nsData);
            }

            //left
            if (middle != sData->first)
            {
                nsData = (_sort_data *)List_malloc(sizeof(_sort_data));
                nsData->first = sData->first;
                nsData->last = middle->prev;
                List_Push(stack, nsData);
            }
        }
        else
        {
            if (comparer(sData->first->data, sData->last->data) > 0)
            {
                _swap_node(sData->first, sData->last);
            }
        }

        List_free(sData);
        List_free(node);
    }

    List_free(stack);

    List_UnLock();

    return;
}

void List_Traverse(List *list, uint8_t isReverse, Visitor visitor)
{
    List_Lock();

    ListNode *current = isReverse ? list->tail : list->head;

    while (current != NULL)
    {
        if (visitor(current->data))
            break;
        current = isReverse ? current->prev : current->next;
    }

    List_UnLock();
}
