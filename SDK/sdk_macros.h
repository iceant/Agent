#ifndef INCLUDED_SDK_MACROS_H
#define INCLUDED_SDK_MACROS_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

////////////////////////////////////////////////////////////////////////////////
////

#define SDK_OFFSETOF(TYPE, MEMBER) ((sdk_size_t)&((TYPE*)0)->MEMBER)
#define SDK_CONTAINER_OF(PTR, TYPE, MEMBER) ((TYPE*)((char*)(PTR) - SDK_OFFSETOF(TYPE, MEMBER)))

#define SDK_ARRAY_SIZE(A) (sizeof(A)/sizeof(A[0]))

#endif /* INCLUDED_SDK_MACROS_H */
