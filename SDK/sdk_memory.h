#ifndef INCLUDED_SDK_MEMORY_H
#define INCLUDED_SDK_MEMORY_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

////////////////////////////////////////////////////////////////////////////////
////

void* sdk_memory_alloc(sdk_size_t nBytes, const char* file, sdk_size_t line);
void* sdk_memory_calloc(sdk_size_t nCount, sdk_size_t nBytes, const char* file, sdk_size_t line);
void* sdk_memory_realloc(void* memory, sdk_size_t nBytes, const char* file, sdk_size_t line);
void sdk_memory_free(void* memory, const char* file, sdk_size_t line);

#ifdef USE_MEMORY_POOL
void sdk_memory_init(void);
void sdk_memory_destroy(void);
#endif

////////////////////////////////////////////////////////////////////////////////
////

#define SDK_ALLOC(SZ) sdk_memory_alloc((SZ), __FILE__, __LINE__)
#define SDK_CALLOC(N, SZ) sdk_memory_calloc((N), (SZ), __FILE__, __LINE__)
#define SDK_REALLOC(P, SZ) sdk_memory_realloc((P), (SZ), __FILE__, __LINE__)
#define SDK_RESIZE(P, SZ) (P)=sdk_memory_realloc((P), (SZ), __FILE__, __LINE__)

#define SDK_NEW(P) (P)=SDK_ALLOC(sizeof(*(P)))
#define SDK_NEW0(P) (P)=SDK_CALLOC(1, sizeof(*(P)))

#define SDK_FREE(P) (sdk_memory_free((P), __FILE__, __LINE__), (P)=0)


#endif /* INCLUDED_SDK_MEMORY_H */
