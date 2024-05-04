#include <sdk_pool.h>

////////////////////////////////////////////////////////////////////////////////
////

static sdk_err_t replenish(sdk_pool_t * pool)
{
    sdk_size_t size = pool->chunk_size * pool->object_size;
    char* start = pool->allocator->alloc(size);
    if(!start){
        return SDK_ENOMEM;
    }
    char* last = &start[(pool->chunk_size-1)*pool->object_size];
    for(char* p = start; p<last; p+=pool->object_size){
        ((sdk_pool_link_t*)p)->next = (sdk_pool_link_t*)(p+pool->object_size);
    }
    ((sdk_pool_link_t*)last)->next = 0;
    pool->list = (sdk_pool_link_t*)start;
    return SDK_EOK;
}
////////////////////////////////////////////////////////////////////////////////
////

sdk_err_t sdk_pool_init(sdk_pool_t* pool, sdk_pool_chunk_allocator_t* allocator, sdk_size_t object_size, sdk_size_t chunk_size)
{
    pool->list = 0;
    pool->allocator = allocator;
    pool->chunk_size = chunk_size;
    pool->object_size = object_size>=sizeof(sdk_pool_link_t)?object_size:sizeof(sdk_pool_link_t);
    pool->instance_count = 0;
    return SDK_EOK;
}

void sdk_pool_destroy(sdk_pool_t * pool){
    pool->allocator->release();
    pool->list = 0;
    pool->instance_count = 0;
}

void* sdk_pool_alloc(sdk_pool_t* pool)
{
    if(!pool->list){
        replenish(pool);
    }
    sdk_pool_link_t * p = pool->list;
    pool->list = p->next;
    ++pool->instance_count;
    return p;
}

void sdk_pool_free(sdk_pool_t* pool, void* object)
{
    if(!object) return;
    
    sdk_pool_link_t * p = (sdk_pool_link_t*) object;
    p->next = pool->list;
    pool->list = p;
    --pool->instance_count;
}




