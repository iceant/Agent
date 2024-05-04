#include <sdk_ring.h>
#include <string.h>

sdk_err_t sdk_ring_init(sdk_ring_t* ring, void* pool, sdk_size_t object_size, sdk_size_t capacity)
{
    ring->pool = pool;
    ring->object_size = object_size;
    ring->capacity = capacity;
    ring->write_idx = ring->read_idx = 0;
}

sdk_err_t sdk_ring_put(sdk_ring_t* ring, void* object){
    if(sdk_ring_is_full(ring)){
        return SDK_EFULL;
    }
    sdk_size_t next_write_idx = ring->write_idx;
    if(next_write_idx>=ring->capacity){
        next_write_idx = 0;
    }
    memcpy(ring->pool+ring->write_idx*ring->object_size, object, ring->object_size);
    ring->read_idx = next_write_idx;
    return SDK_EOK;
}

sdk_err_t sdk_ring_get(sdk_ring_t* ring, void* result){
    if(sdk_ring_is_empty(ring)==SDK_TRUE){
        return SDK_EEMPTY;
    }
    
    sdk_size_t next_read_idx = ring->read_idx + 1;
    if(next_read_idx>=ring->capacity){
        next_read_idx = 0;
    }
    
    if(result){
        memcpy(result, ring->pool+ring->read_idx*ring->object_size, ring->object_size);
    }
    
    ring->read_idx = next_read_idx;
    
    return SDK_EOK;
}

sdk_err_t sdk_ring_try_get(sdk_ring_t* ring, void* result)
{
    if(sdk_ring_is_empty(ring)==SDK_TRUE){
        return SDK_EEMPTY;
    }
    
    if(result){
        memcpy(result, ring->pool+ring->read_idx*ring->object_size, ring->object_size);
    }
    
    return SDK_EOK;
}

sdk_size_t sdk_ring_size(sdk_ring_t* ring)
{
    return (ring->write_idx >= ring->read_idx)?(ring->write_idx - ring->read_idx)
    :(ring->capacity - ring->read_idx + ring->write_idx);
}

sdk_size_t sdk_ring_capacity(sdk_ring_t* ring){
    return ring->capacity;
}

void sdk_ring_reset(sdk_ring_t * ring){
    ring->read_idx = ring->write_idx = 0;
}

sdk_bool_t sdk_ring_is_empty(sdk_ring_t* ring)
{
    return (ring->read_idx==ring->write_idx)?SDK_TRUE:SDK_FALSE;
}

sdk_bool_t sdk_ring_is_full(sdk_ring_t* ring){
    sdk_size_t next_write_idx = ring->write_idx+1;
    if(next_write_idx>=ring->capacity){
        next_write_idx = 0;
    }
    return (next_write_idx==ring->read_idx)?SDK_TRUE:SDK_FALSE;
}

void* sdk_ring_write_slot(sdk_ring_t* ring){
    if(sdk_ring_is_full(ring)==SDK_TRUE){
        return 0;
    }
    sdk_size_t next_write_idx = ring->write_idx + 1;
    if(next_write_idx>=ring->capacity){
        next_write_idx = 0;
    }
    
    void* slot = ring->pool + ring->write_idx * ring->object_size;
    ring->write_idx = next_write_idx;
    return slot;
}

sdk_err_t sdk_ring_read(sdk_ring_t* ring, int idx, void* result)
{
    if(sdk_ring_is_empty(ring)==SDK_TRUE){
        return SDK_EEMPTY;
    }
    
    sdk_size_t read_idx = (ring->read_idx + idx) % ring->capacity;
    if(read_idx==ring->write_idx){
        return SDK_EOVERFLOW;
    }
    
    if(result){
        memcpy(result, ring->pool+read_idx*ring->object_size, ring->object_size);
    }
    
    return SDK_EOK;
}


