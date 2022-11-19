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

#include <stdio.h>
#include <string.h>

#include "Linked_List.h"

bool visitor(void *data, void *params)
{
    printf((char *)data);
    printf("\n");
    return true;
}

int comparer(void *str1, void *str2)
{
    return strcmp((char *)str1, (char *)str2);
}

int main()
{
    List_t *list = List_CreateList(NULL);

    // 1

    List_Push(list, "node 3");
    List_Push(list, "node 2");
    ListNode_t *node_1 = List_Push(list, "node 1");
    List_Push(list, "node 4");
    List_Push(list, "node 5");
    ListNode_t *node_6 = List_Push(list, "node 6");

    List_Prepend(list, "first 1 node");
    List_Prepend(list, "first 2 node");

    printf("============> 1 Traverse\n");
    List_Traverse(list, visitor, NULL, false);

    // 2

    printf("============> 2 Traverse (isReverse==true)\n");
    List_DeleteNode(list, node_6);
    List_DeleteNode(list, node_1);
    List_Traverse(list, visitor, NULL, false);

    // 3

    printf("============> 3 QuickSort\n");
    List_QuickSort(list, comparer);
    List_Traverse(list, visitor, NULL, false);

    // 4

    printf("============> 4 List_Dequeue\n");
    printf("pop '%s'\n", (char *)List_First(list)->data);
    free(List_Dequeue(list));
    List_Traverse(list, visitor, NULL, false);

    // 5

    printf("============> 5 List_Enqueue\n");
    List_Enqueue(list, "node 7");
    List_Enqueue(list, "node 8");
    List_Traverse(list, visitor, NULL, false);

    // del

    printf("============> Destroy\n");
    printf("list len: %d\n", List_Length(list));
    List_DestroyList(list);

    return 0;
}
