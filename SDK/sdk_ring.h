#ifndef INCLUDED_SDK_RING_H
#define INCLUDED_SDK_RING_H

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

typedef struct sdk_ring_s{
    void* pool;
    sdk_size_t object_size;
    sdk_size_t capacity;
    sdk_size_t read_idx;
    sdk_size_t write_idx;
}sdk_ring_t;

////////////////////////////////////////////////////////////////////////////////
////
sdk_err_t sdk_ring_init(sdk_ring_t* ring, void* pool, sdk_size_t object_size, sdk_size_t capacity);

sdk_err_t sdk_ring_put(sdk_ring_t* ring, void* object);

sdk_err_t sdk_ring_get(sdk_ring_t* ring, void* result);

sdk_err_t sdk_ring_try_get(sdk_ring_t* ring, void* result);

sdk_err_t sdk_ring_read(sdk_ring_t* ring, int idx, void* result);

sdk_size_t sdk_ring_size(sdk_ring_t* ring);

sdk_size_t sdk_ring_capacity(sdk_ring_t* ring);

void sdk_ring_reset(sdk_ring_t * ring);

sdk_bool_t sdk_ring_is_empty(sdk_ring_t* ring);
sdk_bool_t sdk_ring_is_full(sdk_ring_t* ring);


#endif /* INCLUDED_SDK_RING_H */
