#include <sdk_memory.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
////
#if defined(USE_TLSF)

#include <tlsf.h>

#ifndef TLSF_POOL_SIZE
#define TLSF_POOL_SIZE 102400
#endif

static void* sdk_memory__pool = 0;

void sdk_memory_init(void){
    if(sdk_memory__pool){
        free(sdk_memory__pool);
        sdk_memory__pool = 0;
    }
    sdk_memory__pool = malloc(TLSF_POOL_SIZE);
    init_memory_pool(TLSF_POOL_SIZE, sdk_memory__pool);
}

void sdk_memory_destroy(void){
    destroy_memory_pool(sdk_memory__pool);
}

void* sdk_memory_alloc(sdk_size_t nBytes, const char* file, sdk_size_t line)
{
    return tlsf_malloc(nBytes);
}

void* sdk_memory_calloc(sdk_size_t nCount, sdk_size_t nBytes, const char* file, sdk_size_t line)
{
    return tlsf_calloc(nCount, nBytes);
}

void* sdk_memory_realloc(void* memory, sdk_size_t nBytes, const char* file, sdk_size_t line)
{
    return tlsf_realloc(memory, nBytes);
}

void sdk_memory_free(void* memory, const char* file, sdk_size_t line)
{
    if(memory){
        tlsf_free(memory);
    }
}
#else
void* sdk_memory_alloc(sdk_size_t nBytes, const char* file, sdk_size_t line)
{
    return malloc(nBytes);
}

void* sdk_memory_calloc(sdk_size_t nCount, sdk_size_t nBytes, const char* file, sdk_size_t line)
{
    return calloc(nCount, nBytes);
}

void* sdk_memory_realloc(void* memory, sdk_size_t nBytes, const char* file, sdk_size_t line)
{
    return realloc(memory, nBytes);
}

void sdk_memory_free(void* memory, const char* file, sdk_size_t line)
{
    if(memory){
        free(memory);
    }
}
#endif
