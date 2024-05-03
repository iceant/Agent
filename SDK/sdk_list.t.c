#include <sdk_list.h>
#include <stdio.h>

int main(int argc, char** argv) {
    sdk_list_t list;
    sdk_list_node_t node1;
    sdk_list_node_t node2;
    
    SDK_LIST_INIT(&list);
    SDK_LIST_INIT(&node1);
    SDK_LIST_INIT(&node2);
    
    SDK_LIST_INSERT_AFTER(&list, &node1);
    printf("list: %p\r\n", &list);
    printf("node1: %p\r\n", &node1);
    printf("node2: %p\r\n", &node2);
    printf("%p <-- [list:%p] --> %p\r\n", SDK_LIST_PREV(&list), &list, SDK_LIST_NEXT(&list));
    printf("%p <-- [node1:%p] --> %p\r\n", SDK_LIST_PREV(&node1), &node1, SDK_LIST_NEXT(&node1));
    printf("%p <-- [node2:%p] --> %p\r\n", SDK_LIST_PREV(&node2), &node2, SDK_LIST_NEXT(&node2));
    if(SDK_LIST_PREV(&node1) == &list && SDK_LIST_NEXT(&list)==&node1 && SDK_LIST_NEXT(&node1)==&list)
    {
        printf("Insert node1 after list success!\r\n");
    }else{
        printf("Insert node1 after list failed!\r\n");
        printf("list: %p\r\n", &list);
        printf("node1: %p\r\n", &node1);
        printf("%p <-- [list:%p] --> %p\r\n", SDK_LIST_PREV(&list), &list, SDK_LIST_NEXT(&list));
        printf("%p <-- [node1:%p] --> %p\r\n", SDK_LIST_PREV(&node1), &node1, SDK_LIST_NEXT(&node1));
    }
    
    SDK_LIST_INSERT_BEFORE(&node1, &node2);
    printf("list: %p\r\n", &list);
    printf("node1: %p\r\n", &node1);
    printf("node2: %p\r\n", &node2);
    printf("%p <-- [list:%p] --> %p\r\n", SDK_LIST_PREV(&list), &list, SDK_LIST_NEXT(&list));
    printf("%p <-- [node1:%p] --> %p\r\n", SDK_LIST_PREV(&node1), &node1, SDK_LIST_NEXT(&node1));
    printf("%p <-- [node2:%p] --> %p\r\n", SDK_LIST_PREV(&node2), &node2, SDK_LIST_NEXT(&node2));
    
    if(SDK_LIST_PREV(&node1) == &node2 && SDK_LIST_NEXT(&node2)==&node1 && SDK_LIST_NEXT(&list)==&node2)
    {
        printf("Insert node2 before node1 success!\r\n");
    }else{
        printf("Insert node2 before node1 failed!\r\n");
    }
    
    
    SDK_LIST_REMOVE(&node1);
    
    printf("list: %p\r\n", &list);
    printf("node1: %p\r\n", &node1);
    printf("node2: %p\r\n", &node2);
    printf("%p <-- [list:%p] --> %p\r\n", SDK_LIST_PREV(&list), &list, SDK_LIST_NEXT(&list));
    printf("%p <-- [node1:%p] --> %p\r\n", SDK_LIST_PREV(&node1), &node1, SDK_LIST_NEXT(&node1));
    printf("%p <-- [node2:%p] --> %p\r\n", SDK_LIST_PREV(&node2), &node2, SDK_LIST_NEXT(&node2));
    
    return 0;
}