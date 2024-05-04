#ifndef INCLUDED_SDK_POOL_H
#define INCLUDED_SDK_POOL_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_SDK_ERRORS_H
#include <sdk_errors.h>
#endif /*INCLUDED_SDK_ERRORS_H*/

////////////////////////////////////////////////////////////////////////////////
////

typedef struct sdk_pool_chunk_allocator_s{
    void* (*alloc)(sdk_size_t size);
    void (*release)(void);
}sdk_pool_chunk_allocator_t;

typedef struct sdk_pool_link_s{
    struct sdk_pool_link_s* next;
}sdk_pool_link_t;

typedef struct sdk_pool_s{
    sdk_pool_chunk_allocator_t *allocator;
    sdk_pool_link_t * list;
    sdk_size_t object_size;
    sdk_size_t chunk_size;
    sdk_size_t instance_count;
}sdk_pool_t;

////////////////////////////////////////////////////////////////////////////////
////

sdk_err_t sdk_pool_init(sdk_pool_t* pool, sdk_pool_chunk_allocator_t* allocator, sdk_size_t object_size, sdk_size_t chunk_size);

void* sdk_pool_alloc(sdk_pool_t* pool);

void sdk_pool_free(sdk_pool_t* pool, void* object);

void sdk_pool_destroy(sdk_pool_t * pool);

#endif /* INCLUDED_SDK_POOL_H */
