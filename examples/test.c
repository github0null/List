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

bool visitor_print(void *data, void *params)
{
    printf((char *)data);
    printf("\n");
    return true;
}

bool visitor_print_with_arrow(void *data, void *params)
{
    printf("'%s' -> ", (char *)data);
    return true;
}

int comparer(void *str1, void *str2)
{
    return strcmp((char *)str1, (char *)str2);
}

int main()
{
    List_t *list = List_CreateList(NULL);
    
    printf("\n==================== Test 'Traverse' 'Foreach' 'QuickSort' ======================\n");

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
    List_Traverse(list, visitor_print, NULL, false);

    // 2

    printf("============> 2 Traverse (isReverse==true)\n");
    List_DeleteNode(list, node_6);
    List_DeleteNode(list, node_1);
    List_Traverse(list, visitor_print, NULL, false);

    // 3

    printf("============> 3 QuickSort\n");
    List_QuickSort(list, comparer);
    List_Traverse(list, visitor_print, NULL, false);

    // 4

    printf("============> 4 List_Dequeue\n");
    printf("pop '%s'\n", (char *)List_First(list)->data);
    free(List_Dequeue(list));
    List_Traverse(list, visitor_print, NULL, false);

    // 5

    printf("============> 5 List_Enqueue\n");
    List_Enqueue(list, "node 7");
    List_Enqueue(list, "node 8");
    List_Traverse(list, visitor_print, NULL, false);

    //
    printf("============> Foreach Safe (will remove node 5 and 7 after loop done)\n");
    {
        ListNode_t *ele, *t;
        List_ForeachSafe(list, ele, t)
        {
            printf("'%s' -> ", (char *)ele->data);

            if (strstr((char *)ele->data, "5")) {
                List_DeleteNode(list, ele);
            }

            if (strstr((char *)ele->data, "7")) {
                List_DeleteNode(list, ele);
            }
        }
    }
    printf("\n");

    //
    printf("============> Foreach\n");
    {
        ListNode_t *ele;
        List_Foreach(list, ele)
        {
            printf("'%s' -> ", (char *)ele->data);
        }
    }
    printf("\n");

    // clear
    printf("============> Clear (Remove and delete all node)\n");
    List_Clear(list);
    List_Traverse(list, visitor_print_with_arrow, NULL, false);

    ///////////////////////////////////////////////////////////////////////

    printf("\n==================== Test 'Push' 'Pop' 'Insert' ======================\n");

    //
    printf("\n============> Push 50 nodes\n");
    for (size_t i = 0; i < 50; i++) {
        char *str = malloc(64);
        memset(str, 0, 64);
        sprintf(str, "node %d", i + 1);
        List_Push(list, str);
    }
    List_Traverse(list, visitor_print_with_arrow, NULL, false);

    printf("\n============> Pop 4 nodes\n");
    for (size_t i = 0; i < 4; i++) {
        ListNode_t *n = List_Pop(list);
        if (n != NULL) List_DeleteNode(list, n);
    }
    List_Traverse(list, visitor_print_with_arrow, NULL, false);

    printf("\n============> Dequeue 3 nodes\n");
    for (size_t i = 0; i < 3; i++) {
        ListNode_t *n = List_Dequeue(list);
        if (n != NULL) List_DeleteNode(list, n);
    }
    List_Traverse(list, visitor_print_with_arrow, NULL, false);

    printf("\n============> Insert 'node 111' after 'node 7' and 'node last'\n");
    {
        ListNode_t *node_7 = NULL, *n;

        List_Foreach(list, n)
        {
            if (strcmp((const char *)n->data, "node 7") == 0) {
                node_7 = n;
                break;
            }
        }

        if (node_7) {
            List_InsertNode(list, node_7, (void *)"node 111");
            List_InsertNode(list, List_Last(list), (void *)"node 111");
            List_Traverse(list, visitor_print_with_arrow, NULL, false);
        } else {
            printf("error, not found node 7");
        }
    }

    printf("\n============> Insert 'node 333' before 'node 13' and 'node first'\n");
    {
        ListNode_t *node_13 = NULL, *n;

        List_Foreach(list, n)
        {
            if (strcmp((const char *)n->data, "node 13") == 0) {
                node_13 = n;
                break;
            }
        }

        if (node_13) {
            List_InsertNodeBefore(list, node_13, (void *)"node 333");
            List_InsertNodeBefore(list, List_First(list), (void *)"node 333");
            List_Traverse(list, visitor_print_with_arrow, NULL, false);
        } else {
            printf("error, not found node 13");
        }
    }

    printf("\n============> Delete First Node\n");
    List_DeleteNode(list, List_First(list));
    List_Traverse(list, visitor_print_with_arrow, NULL, false);

    printf("\n============> Delete Last Node\n");
    List_DeleteNode(list, List_Last(list));
    List_Traverse(list, visitor_print_with_arrow, NULL, false);

    // reverse foreach
    printf("\n============> Traverse Reverse\n");
    List_Traverse(list, visitor_print_with_arrow, NULL, true);

    // destroy list

    printf("\n============> Destroy (len: %d)\n", List_Length(list));
    List_DestroyList(list);

    return 0;
}
