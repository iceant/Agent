#include <sdk_vector.h>
#include <sdk_memory.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
////
#define ELEMENT_SIZE sizeof(void*)

////////////////////////////////////////////////////////////////////////////////
////

sdk_err_t sdk_vector_init(sdk_vector_t* vector, sdk_size_t capacity)
{
    vector->capacity = capacity;
    vector->size = 0;
    if(vector->capacity==0){
        vector->items = 0;
    }else{
        vector->items = SDK_ALLOC(ELEMENT_SIZE * capacity);
        if(!vector->items){
            return SDK_ENOMEM;
        }
    }
    return SDK_EOK;
}

void sdk_vector_destroy(sdk_vector_t * vector){
    SDK_FREE(vector->items);
    vector->size = 0;
}

sdk_err_t sdk_vector_append(sdk_vector_t* vector, void* item)
{
    if(vector->size>=vector->capacity){
        return SDK_EFULL;
    }
    vector->items[vector->size++]=item;
    return SDK_EOK;
}

sdk_err_t sdk_vector_resize_append(sdk_vector_t* vector, void* item)
{
    sdk_err_t err;
    while(vector->size>=vector->capacity){
        err = sdk_vector_resize(vector, (vector->capacity==0)?1:(vector->capacity*2));
        if(err!=SDK_EOK){
            return err;
        }
    }
    vector->items[vector->size++]=item;
    return SDK_EOK;
}

sdk_err_t sdk_vector_resize(sdk_vector_t* vector, sdk_size_t new_capacity)
{
    if(new_capacity==0){
        SDK_FREE(vector->items);
        vector->size = 0;
    }else if(new_capacity == vector->capacity){
        return SDK_EOK;
    }else if(new_capacity > vector->capacity){
        void** items = SDK_ALLOC(ELEMENT_SIZE * new_capacity);
        if(!items){
            return SDK_ENOMEM;
        }
        memcpy(items, vector->items, vector->size * ELEMENT_SIZE);
        SDK_FREE(vector->items);
        vector->items = items;
    }else{
        void** items = SDK_REALLOC(vector->items, new_capacity * ELEMENT_SIZE);
        if(!items){
            return SDK_ENOMEM;
        }
        vector->items = items;
        if(vector->size > new_capacity){
            vector->size = new_capacity;
        }
    }
    
    vector->capacity = new_capacity;
    return SDK_EOK;
}

sdk_err_t sdk_vector_get(sdk_vector_t* vector, sdk_size_t idx, void** item)
{
    if(idx>=0 && idx<vector->size){
        if(item){
            *item = vector->items[idx];
        }
        return SDK_EOK;
    }
    return SDK_EINVAL;
}

sdk_err_t sdk_vector_set(sdk_vector_t* vector, sdk_size_t idx, void* item){
    if(idx<0 || idx>= vector->size){
        return SDK_EINVAL;
    }
    vector->items[idx] = item;
    return SDK_EOK;
}

sdk_err_t sdk_vector_remove(sdk_vector_t* vector, sdk_size_t idx){
    if(idx<0 || idx>= vector->size){
        return SDK_EINVAL;
    }
    sdk_size_t i;
    for(i=idx; i<(vector->size-1); i++){
        vector->items[i] = vector->items[i+1];
    }
    vector->items[i] = NULL;
    vector->size--;
    return SDK_EOK;
}

sdk_err_t sdk_vector_resize_remove(sdk_vector_t* vector, sdk_size_t idx)
{
    if(idx<0 || idx>= vector->size){
        return SDK_EINVAL;
    }
    sdk_size_t i;
    for(i=idx; i<(vector->size-1); i++){
        vector->items[i] = vector->items[i+1];
    }
    vector->items[i] = NULL;
    vector->size--;
    if(vector->size <= vector->capacity/4){
        sdk_vector_resize(vector, vector->capacity/2);
    }
    
    return SDK_EOK;
}
