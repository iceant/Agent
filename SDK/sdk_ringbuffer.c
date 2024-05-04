#include <sdk_ringbuffer.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

////////////////////////////////////////////////////////////////////////////////
////
#ifndef ULONG_MAX
#define	ULONG_MAX	((unsigned long)(~0L))		/* 0xFFFFFFFF */
#endif

/*
 * Convert a string to an unsigned long integer.
 *
 * Ignores `locale' stuff.  Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 */
static unsigned long sdk_ringbuffer__strtoul(sdk_ringbuffer_t * nptr, sdk_size_t* endptr, register int base)
{
//    register const char *s = nptr;
    register sdk_ringbuffer_t * s = nptr;
    register unsigned long acc;
    register uint8_t c;
    register unsigned long cutoff;
    register int neg = 0, any, cutlim;
    sdk_size_t nptr_read_idx = nptr->read_idx;
    
    /*
     * See strtol for comments as to the logic used.
     */
    do {
        c = sdk_ringbuffer_pop(s);
//        c = *s++;
    } while (isspace(c));
    if (c == '-') {
        neg = 1;
//        c = *s++;
        c = sdk_ringbuffer_pop(s);
    } else if (c == '+'){
//        c = *s++;
        c = sdk_ringbuffer_pop(s);
    }
    if ((base == 0 || base == 16) &&
        c == '0'
//        && (*s == 'x' || *s == 'X')
        && (sdk_ringbuffer_peek(s)=='x' || sdk_ringbuffer_peek(s)=='X')
        )
    {
//        c = s[1];
        sdk_ringbuffer_advance_read_idx(s);
        c = sdk_ringbuffer_pop(s);
//        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
    cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;
    for (acc = 0, any = 0;; /*c = *s++*/ c = sdk_ringbuffer_pop(s) ) {
        if (isdigit(c))
            c -= '0';
        else if (isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if (any < 0) {
        acc = ULONG_MAX;
        errno = ERANGE;
    } else if (neg){
        acc = -acc;
    }
    if (endptr != 0){
        /**endptr = (char *) (any ? s - 1 : nptr);*/
        *endptr = (any ? sdk_ringbuffer_previous_read_idx(s) : nptr_read_idx);
    }
    return (acc);
}

////////////////////////////////////////////////////////////////////////////////
////

sdk_err_t sdk_ringbuffer_init(sdk_ringbuffer_t* buffer, uint8_t* block, sdk_size_t buffer_size)
{
    buffer->buffer = block;
    buffer->capacity = buffer_size;
    buffer->read_idx = buffer->write_idx = 0;
    return SDK_EOK;
}

void sdk_ringbuffer_reset(sdk_ringbuffer_t* buffer)
{
    buffer->read_idx = buffer->write_idx = 0;
}

sdk_err_t sdk_ringbuffer_put(sdk_ringbuffer_t* buffer, uint8_t data){
    if(sdk_ringbuffer_is_full(buffer)){
        return SDK_EFULL;
    }
    sdk_size_t next_write_idx = buffer->write_idx + 1;
    if(next_write_idx==buffer->capacity){
        next_write_idx = 0;
    }
    buffer->buffer[buffer->write_idx] = data;
    buffer->write_idx = next_write_idx;
    return SDK_EOK;
}

sdk_err_t sdk_ringbuffer_get(sdk_ringbuffer_t* buffer, uint8_t * data)
{
    if(sdk_ringbuffer_is_empty(buffer)){
        return SDK_EEMPTY;
    }
    sdk_size_t next_read_idx = buffer->read_idx + 1;
    if(next_read_idx==buffer->capacity){
        next_read_idx = 0;
    }
    if(data){
        *data = buffer->buffer[buffer->read_idx];
    }
    buffer->read_idx = next_read_idx;
    return SDK_EOK;
}

uint8_t sdk_ringbuffer_pop(sdk_ringbuffer_t* buffer){
    uint8_t data;
    if(sdk_ringbuffer_is_empty(buffer)){
        return SDK_RINGBUFFER_EINVAL;
    }
    sdk_size_t next_read_idx = buffer->read_idx + 1;
    if(next_read_idx==buffer->capacity){
        next_read_idx = 0;
    }
    data = buffer->buffer[buffer->read_idx];
    buffer->read_idx = next_read_idx;
    return data;
}

uint8_t sdk_ringbuffer_peek(sdk_ringbuffer_t* buffer){
    if(sdk_ringbuffer_is_empty(buffer)){
        return SDK_RINGBUFFER_EINVAL;
    }
    return buffer->buffer[buffer->read_idx];
}

sdk_err_t sdk_ringbuffer_try_get(sdk_ringbuffer_t * buffer, uint8_t* data){
    if(sdk_ringbuffer_is_empty(buffer)){
        return SDK_EEMPTY;
    }
    if(data){
        *data = buffer->buffer[buffer->read_idx];
    }
}

sdk_bool_t sdk_ringbuffer_is_empty(sdk_ringbuffer_t* buffer){
    return (buffer->write_idx == buffer->read_idx)?SDK_TRUE:SDK_FALSE;
}

sdk_bool_t sdk_ringbuffer_is_full(sdk_ringbuffer_t* buffer){
    sdk_size_t write_idx = buffer->write_idx + 1;
    if(write_idx==buffer->capacity){
        write_idx = 0;
    }
    return (write_idx==buffer->read_idx)?SDK_TRUE:SDK_FALSE;
}

sdk_size_t sdk_ringbuffer_capacity(sdk_ringbuffer_t* buffer){
    return buffer->capacity;
}

sdk_size_t sdk_ringbuffer_size(sdk_ringbuffer_t* buffer)
{
    return (buffer->write_idx >= buffer->read_idx)
        ?(buffer->write_idx - buffer->read_idx)
        :(buffer->capacity - buffer->read_idx + buffer->write_idx);
}

void sdk_ringbuffer_advance_read_idx(sdk_ringbuffer_t*  buffer)
{
    sdk_size_t next_read_idx = buffer->read_idx + 1;
    if(next_read_idx>=buffer->capacity){
        next_read_idx = 0;
    }
    buffer->read_idx = next_read_idx;
}

sdk_size_t sdk_ringbuffer_previous_read_idx(sdk_ringbuffer_t* buffer)
{
    if(buffer->read_idx==0){
        return buffer->capacity-1;
    }
    return buffer->read_idx-1;
}

/*
 * 0 1 2 3 4 5
 *     |__ read_idx
 */
sdk_err_t sdk_ringbuffer_read(sdk_ringbuffer_t* buffer, int idx, uint8_t* data)
{
    if(sdk_ringbuffer_is_empty(buffer)){
        return SDK_EEMPTY;
    }
    
    sdk_size_t read_idx = (buffer->read_idx + idx) % buffer->capacity;
    if(read_idx==buffer->write_idx){
        return SDK_EOVERFLOW;
    }
    if(data){
        *data = buffer->buffer[read_idx];
    }
    
    return SDK_EOK;
}

unsigned long sdk_ringbuffer_strtoul(sdk_ringbuffer_t * buffer, sdk_size_t* end_idx, register int base)
{
    return sdk_ringbuffer__strtoul(buffer, end_idx, base);
}

sdk_err_t sdk_ringbuffer_strchr(sdk_ringbuffer_t* buffer, int c, sdk_size_t* result)
{
    uint8_t data;
    sdk_err_t err;
    int idx = 0;
    sdk_size_t size = sdk_ringbuffer_size(buffer);
    do {
        err = sdk_ringbuffer_read(buffer, idx, &data);
        if(err!=SDK_EOK) return err;
        if (data == c)
        {
            if(result){
                *result = idx;
                return SDK_EOK;
            }
        }
        idx+=1;
    } while (idx<size);
    return SDK_ERROR;
}

int sdk_ringbuffer_strncmp(sdk_ringbuffer_t * buffer, const char *s2, register sdk_size_t n)
{
    sdk_err_t err;
    uint8_t u1, u2;
    int idx = 0;
    sdk_size_t size = sdk_ringbuffer_size(buffer);
    
    while (n-- > 0)
    {
//        u1 = (unsigned char) *s1++;
        err = sdk_ringbuffer_read(buffer, idx, &u1);
        if(err!=SDK_EOK){
            return -1;
        }
        u2 = (unsigned char) *s2++;
        if (u1 != u2)
            return u1 - u2;
        idx+=1;
        if (idx == size)
            return 0;
    }
    return 0;
}

sdk_err_t sdk_ringbuffer_strstr(sdk_ringbuffer_t* buffer, const char* s2, sdk_size_t* result)
{
    sdk_size_t size2 = strlen(s2);
    uint8_t ch;
    int idx = 0;
    int find_idx = 0;
    sdk_err_t err;
    sdk_size_t i;
    while(1){
        err = sdk_ringbuffer_read(buffer, idx, &ch);
        if(err!=SDK_EOK) return err;
        if(ch == *s2){
            find_idx = idx;
            for(i=1; i<size2; i++){
                err = sdk_ringbuffer_read(buffer, ++idx, &ch);
                if(err!=SDK_EOK) return err;
                if(ch!=s2[i]){
                    break;
                }
            }
            if(i==size2){
                if(result){
                    *result = find_idx;
                }
                return SDK_EOK;
            }else{
                idx = find_idx;
            }
        }
        
        idx+=1;
    }
}

int sdk_ringbuffer_memcmp(sdk_ringbuffer_t* buffer, int idx, const uint8_t* s2, sdk_size_t count)
{
    uint8_t ch;
    sdk_err_t err;
    while(count-- > 0){
        err = sdk_ringbuffer_read(buffer, idx, &ch);
        if(err!=SDK_EOK){
            return -1;
        }
        if(ch!=*s2++){
            sdk_ringbuffer_read(buffer, idx-1, &ch);
            return (ch < s2[-1])?-1:1;
        }
        idx+=1;
    }
    return 0;
}



