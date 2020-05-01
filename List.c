#include "List.h"

#define true 1
#define false 0

inline void _cut_prev(ListNode *node)
{
	node->prev->next = INVALID_PTR;
	node->prev = INVALID_PTR;
}

inline void _cut_next(ListNode *node)
{
	node->next->prev = INVALID_PTR;
	node->next = INVALID_PTR;
}

inline void _link_next(ListNode *node, ListNode *nNode)
{
	ListNode *next;
	if (node->next == INVALID_PTR)
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
	List *list = malloc(sizeof(List));
	list->length = 0;
	list->head = INVALID_PTR;
	list->tail = INVALID_PTR;
	return list;
}

void List_DestroyList(List *list, DataDestructor destructor)
{
	ListNode *node = List_Pop(list);
	while (node != INVALID_PTR)
	{
		destructor(node->data);
		free(node);
		node = List_Pop(list);
	}
	free(list);
}

ListNode *List_Pop(List *list)
{
	ListNode *node = INVALID_PTR;
	if (list->length == 0)
	{
		return node;
	}
	if (list->head == list->tail)
	{
		node = list->head;
		list->head = INVALID_PTR;
		list->tail = INVALID_PTR;
		list->length = 0;
	}
	else
	{
		node = list->tail;
		list->tail = node->prev;
		_cut_prev(node);
		list->length--;
	}
	return node;
}

ListNode *List_Push(List *list, void *data)
{
	ListNode *node = malloc(sizeof(ListNode));

	node->data = data;
	node->next = INVALID_PTR;
	node->prev = INVALID_PTR;

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

	return node;
}

ListNode *List_Prepend(List *list, void *data)
{
	ListNode *node = malloc(sizeof(ListNode));

	node->data = data;
	node->next = INVALID_PTR;
	node->prev = INVALID_PTR;

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

	return node;
}

ListNode *List_Dequeue(List *list)
{
	ListNode *node = INVALID_PTR;
	if (list->length == 0)
	{
		return node;
	}
	if (list->head == list->tail)
	{
		node = list->head;
		list->head = INVALID_PTR;
		list->tail = INVALID_PTR;
		list->length = 0;
	}
	else
	{
		node = list->head;
		list->head = node->next;
		_cut_next(node);
		list->length--;
	}
	return node;
}

ListNode *List_Enqueue(List *list, void *data)
{
	return List_Push(list, data);
}

ListNode *List_FindFirst(List *list, NodeMatcher matcher)
{
	ListNode *node = list->head;

	while (node != INVALID_PTR)
	{
		if (matcher(node->data))
			break;
		node = node->next;
	}

	return node;
}

ListNode *List_FindNext(ListNode *node, NodeMatcher matcher)
{
	ListNode *cNode = node->next;

	while (cNode != INVALID_PTR)
	{
		if (matcher(cNode->data))
			break;
		cNode = cNode->next;
	}

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

ListNode *List_InsertNode(List *list, ListNode *node, void *data)
{
	ListNode *nNode = malloc(sizeof(ListNode));
	nNode->data = data;
	_link_next(node, nNode);
	if (node == list->tail)
	{
		list->tail = nNode;
	}
	list->length++;
	return nNode;
}

ListNode *List_RemoveNode(List *list, ListNode *node)
{
	ListNode *prev, *next;

	if (list->length == 0)
		return INVALID_PTR;

	if (node == list->head)
	{
		if (node->next != INVALID_PTR)
		{
			list->head = node->next;
			_cut_next(node);
		}
		else {
			list->head = list->tail = INVALID_PTR;
		}
	}
	else if (node == list->tail)
	{
		if (node->prev != INVALID_PTR)
		{
			list->tail = node->prev;
			_cut_prev(node);
		}
		else {
			list->head = list->tail = INVALID_PTR;
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

	return node;
}

void List_DeleteMatched(List *list, NodeMatcher matcher, DataDestructor destructor)
{
	ListNode *current = list->head, *n;
	while (current != INVALID_PTR)
	{
		if (matcher(current->data))
		{
			n = current->next;
			current = List_RemoveNode(list, current);
			destructor(current->data);
			free(current);
			current = n;
		}
		else
		{
			current = current->next;
		}
	}
}

uint32_t List_Count(List *list, NodeMatcher matcher)
{
	uint32_t count = 0;
	ListNode *node = List_FindFirst(list, matcher);

	if (node == INVALID_PTR)
		return count;

	count++;

	do
	{
		if (matcher(node->data))
			count++;
		node = node->next;
	} while (node != INVALID_PTR);

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
	List *stack = List_CreateList();
	_sort_data *nsData = malloc(sizeof(_sort_data)), *sData = INVALID_PTR;
	ListNode *node = INVALID_PTR, *middle = INVALID_PTR;

	nsData->first = list->head;
	nsData->last = list->tail;

	if (nsData->first == nsData->last)
		return;

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
				nsData = malloc(sizeof(_sort_data));
				nsData->first = middle->next;
				nsData->last = sData->last;
				List_Push(stack, nsData);
			}

			//left
			if (middle != sData->first)
			{
				nsData = malloc(sizeof(_sort_data));
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

		free(sData);
		free(node);
	}

	free(stack);

	return;
}

void List_Traverse(List *list, uint8_t isReverse, Visitor visitor)
{
	ListNode *current = isReverse ? list->tail : list->head;

	while (current != INVALID_PTR)
	{
		if (visitor(current->data))
			break;
		current = isReverse ? current->prev : current->next;
	}
}
