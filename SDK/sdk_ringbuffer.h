#ifndef INCLUDED_SDK_RINGBUFFER_H
#define INCLUDED_SDK_RINGBUFFER_H

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

typedef struct sdk_ringbuffer_s{
    uint8_t * buffer;
    sdk_size_t capacity;
    sdk_size_t read_idx;
    sdk_size_t write_idx;
}sdk_ringbuffer_t;

#define SDK_RINGBUFFER_EINVAL       (-1U)

////////////////////////////////////////////////////////////////////////////////
////

sdk_err_t sdk_ringbuffer_init(sdk_ringbuffer_t* buffer, uint8_t* block, sdk_size_t buffer_size);

void sdk_ringbuffer_reset(sdk_ringbuffer_t* buffer);

sdk_err_t sdk_ringbuffer_put(sdk_ringbuffer_t* buffer, uint8_t data);

sdk_err_t sdk_ringbuffer_get(sdk_ringbuffer_t* buffer, uint8_t * data);

sdk_err_t sdk_ringbuffer_try_get(sdk_ringbuffer_t * buffer, uint8_t* data);

uint8_t sdk_ringbuffer_pop(sdk_ringbuffer_t* buffer);

uint8_t sdk_ringbuffer_peek(sdk_ringbuffer_t* buffer);

sdk_err_t sdk_ringbuffer_read(sdk_ringbuffer_t* buffer, int idx, uint8_t* data);

void sdk_ringbuffer_advance_read_idx(sdk_ringbuffer_t*  buffer);

sdk_bool_t sdk_ringbuffer_is_empty(sdk_ringbuffer_t* buffer);

sdk_bool_t sdk_ringbuffer_is_full(sdk_ringbuffer_t* buffer);

sdk_size_t sdk_ringbuffer_capacity(sdk_ringbuffer_t* buffer);

sdk_size_t sdk_ringbuffer_size(sdk_ringbuffer_t* buffer);

sdk_size_t sdk_ringbuffer_previous_read_idx(sdk_ringbuffer_t* buffer);

unsigned long sdk_ringbuffer_strtoul(sdk_ringbuffer_t * buffer, sdk_size_t* end_idx, register int base);
sdk_err_t sdk_ringbuffer_strchr(sdk_ringbuffer_t* buffer, int c, sdk_size_t* result);
int sdk_ringbuffer_strncmp(sdk_ringbuffer_t * buffer, const char *s2, register sdk_size_t n);
sdk_err_t sdk_ringbuffer_strstr(sdk_ringbuffer_t* buffer, int idx, const char* s2, sdk_size_t* result);

int sdk_ringbuffer_memcmp(sdk_ringbuffer_t* buffer, int idx, const uint8_t* s2, sdk_size_t count);

#endif /* INCLUDED_SDK_RINGBUFFER_H */
