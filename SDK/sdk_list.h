#ifndef INCLUDED_SDK_LIST_H
#define INCLUDED_SDK_LIST_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_MACROS_H
#include <sdk_macros.h>
#endif /*INCLUDED_SDK_MACROS_H*/


////////////////////////////////////////////////////////////////////////////////
////

typedef struct sdk_list_node_s{
    struct sdk_list_node_s* next;
    struct sdk_list_node_s* prev;
}sdk_list_node_t;

typedef sdk_list_node_t sdk_list_t;

////////////////////////////////////////////////////////////////////////////////
////
#define SDK_LIST_NEXT(L) (L)->next
#define SDK_LIST_PREV(L) (L)->prev
#define SDK_LIST_PREV_NEXT(L) SDK_LIST_NEXT(SDK_LIST_PREV(L))
#define SDK_LIST_NEXT_PREV(L) SDK_LIST_PREV(SDK_LIST_NEXT(L))

#define SDK_LIST_INIT(L) \
do{                      \
    SDK_LIST_NEXT(L) = SDK_LIST_PREV(L) = (L); \
}while(0)

#define SDK_LIST_INSERT_AFTER(L, N) \
do{                                 \
    SDK_LIST_NEXT(N) = SDK_LIST_NEXT_PREV(L); \
    SDK_LIST_NEXT_PREV(L) = (N);    \
    SDK_LIST_NEXT(L) = (N);         \
    SDK_LIST_PREV(N) = (L); \
}while(0)

/*
 X <-- N <-- L
 */
#define SDK_LIST_INSERT_BEFORE(L, N) \
do{                                  \
    SDK_LIST_PREV_NEXT(L) = (N);     \
    SDK_LIST_PREV(N) = SDK_LIST_PREV(L); \
    SDK_LIST_NEXT(N) = (L);          \
    SDK_LIST_PREV(L) = (N);          \
}while(0)

#define SDK_LIST_REMOVE(N) \
do{                        \
    SDK_LIST_PREV_NEXT(N) = SDK_LIST_NEXT(N); \
    SDK_LIST_NEXT_PREV(N) = SDK_LIST_PREV(N); \
    SDK_LIST_PREV(N) = SDK_LIST_NEXT(N) = (N);\
}while(0)

#endif /* INCLUDED_SDK_LIST_H */
