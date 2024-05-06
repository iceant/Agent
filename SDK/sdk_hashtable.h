#ifndef INCLUDED_SDK_HASHTABLE_H
#define INCLUDED_SDK_HASHTABLE_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_SDK_ERRORS_H
#include <sdk_errors.h>
#endif /*INCLUDED_SDK_ERRORS_H*/

#ifndef INCLUDED_SDK_VECTOR_H
#include <sdk_vector.h>
#endif /*INCLUDED_SDK_VECTOR_H*/


////////////////////////////////////////////////////////////////////////////////
////

typedef struct sdk_hashtable_key_ops_s{
    uint32_t (*hash)(const void *data, void *arg);
    void* (*cp)(const void *data, void *arg);
    void (*free)(void *data, void *arg);
    sdk_bool_t (*eq)(const void *data1, const void *data2, void *arg);
    void *arg;
}sdk_hashtable_key_ops_t;

typedef struct sdk_hashtable_val_ops_s{
    void* (*cp)(const void *data, void *arg);
    void (*free)(void *data, void *arg);
    sdk_bool_t (*eq)(const void *data1, const void *data2, void *arg);
    void *arg;
}sdk_hashtable_val_ops_t;

typedef struct sdk_hashtable_node_s {
    uint32_t hash;
    void *key;
    void *val;
} sdk_hashtable_node_t;

typedef struct sdk_hashtable_s{
    sdk_size_t capacity;
    sdk_size_t size;
    sdk_vector_t ** buckets;
    sdk_hashtable_key_ops_t key_ops;
    sdk_hashtable_val_ops_t val_ops;
}sdk_hashtable_t;

typedef struct sdk_any_s{
    int type;
    union {
        char char_val;
        sdk_uint_t int_val;
        double double_val;
        float float_val;
        char * string_val;
        void* pointer_val;
    }val;
}sdk_any_t;

////////////////////////////////////////////////////////////////////////////////
//// Value Types
#define SDK_HASHTABLE_CHAR       (1<<0)
#define SDK_HASHTABLE_INTEGER    (1<<1)
#define SDK_HASHTABLE_DOUBLE     (1<<2)
#define SDK_HASHTABLE_FLOAT      (1<<3)
#define SDK_HASHTABLE_STRING     (1<<4)
#define SDK_HASHTABLE_POINTER    (1<<5)

////////////////////////////////////////////////////////////////////////////////
////

sdk_err_t sdk_hashtable_init(sdk_hashtable_t* hashtable, sdk_size_t capacity
                             , sdk_hashtable_key_ops_t key_ops, sdk_hashtable_val_ops_t val_ops);

void sdk_hashtable_destroy(sdk_hashtable_t* hashtable);

void* sdk_hashtable_get(sdk_hashtable_t* hashtable, const void* key);

sdk_bool_t sdk_hashtable_contains(sdk_hashtable_t *hashtable, const void* key);

sdk_err_t sdk_hashtable_put(sdk_hashtable_t* hashtable, const void* key, const void* value);

sdk_size_t sdk_hashtable_numcol(sdk_hashtable_t* hashtable);

void sdk_hashtable_foreach(sdk_hashtable_t* hashtable
                           , void (*apply)(sdk_hashtable_t * hashtable, sdk_hashtable_node_t * node, void* userdata)
                           , void* userdata);
////////////////////////////////////////////////////////////////////////////////
////
uint32_t sdk_string_hash(const void* data, void* arg);
void* sdk_string_cp(const void* data, void* arg);
sdk_bool_t sdk_string_eq(const void* data1, const void* data2, void* arg);
void sdk_string_free(void* data, void* arg);

extern sdk_hashtable_key_ops_t sdk_string_key_ops;
extern sdk_hashtable_val_ops_t sdk_string_val_ops;

////////////////////////////////////////////////////////////////////////////////
////
uint32_t sdk_integer_hash(const void* data, void* arg);
void* sdk_integer_cp(const void* data, void* arg);
sdk_bool_t sdk_integer_eq(const void* data1, const void* data2, void* arg);
void sdk_integer_free(void* data, void* arg);

extern sdk_hashtable_key_ops_t sdk_integer_key_ops;
extern sdk_hashtable_val_ops_t sdk_integer_val_ops;

////////////////////////////////////////////////////////////////////////////////
////
uint32_t sdk_any_hash(const void* data, void* arg);
void* sdk_any_cp(const void* data, void* arg);
sdk_bool_t sdk_any_eq(const void* data1, const void* data2, void* arg);
void sdk_any_free(void* data, void* arg);

extern sdk_hashtable_key_ops_t sdk_any_key_ops;
extern sdk_hashtable_val_ops_t sdk_any_val_ops;

#endif /* INCLUDED_SDK_HASHTABLE_H */
