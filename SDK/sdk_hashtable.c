#include <sdk_hashtable.h>
#include <sdk_memory.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
////
#define BUCKET_SIZE sizeof(sdk_vector_t*)
#define GROWTH_FACTOR 2

////////////////////////////////////////////////////////////////////////////////
////
static sdk_hashtable_node_t * sdk_hashtable__get_node(sdk_hashtable_t* hashtable, const void* key){
    sdk_hashtable_node_t * result = 0;
    sdk_hashtable_node_t * node = 0;
    sdk_vector_t * bucket = 0;
    
    uint32_t computed_hash;
    sdk_size_t bucket_idx;
    
    computed_hash = hashtable->key_ops.hash(key, hashtable->key_ops.arg);
    bucket_idx = computed_hash % hashtable->capacity;
    bucket = hashtable->buckets[bucket_idx];
    
    if(bucket){
        for(sdk_size_t i=0; i<bucket->size; i++){
            sdk_vector_get(bucket, i, (void **) &node);
            if(node->hash == computed_hash){
                if(hashtable->key_ops.eq(node->key, key, hashtable->key_ops.arg)){
                    result = node;
                    break;
                }
            }
        }
    }
    return result;
}

static sdk_vector_t * sdk_hashtable__new_bucket(void){
    sdk_vector_t * v;
    SDK_NEW(v);
    if(!v){
        return 0;
    }
    sdk_err_t err = sdk_vector_init(v, 0);
    if(err!=SDK_EOK){
        return 0;
    }
    
    return v;
}

static sdk_err_t sdk_hashtable__grow(sdk_hashtable_t* hashtable, sdk_size_t new_capacity){
    sdk_vector_t ** new_buckets;
    sdk_vector_t * bucket;
    sdk_hashtable_node_t * node;
    sdk_size_t new_idx;
    
    new_buckets = SDK_ALLOC(BUCKET_SIZE * new_capacity);
    if(!new_buckets){
        return SDK_ENOMEM;
    }
    
    for(sdk_size_t i=0; i<new_capacity; i++){
        new_buckets[i] = 0;
    }
    
    // rehash
    for(sdk_size_t i=0; i<hashtable->capacity ; i++){
        bucket = hashtable->buckets[i];
        if(bucket){
            for(sdk_size_t j=0; j<bucket->size; j++){
                sdk_vector_get(bucket, j, (void **) &node);
                // compute new id for new bucket
                new_idx = node->hash % new_capacity;
                if(new_buckets[new_idx]==0){
                    new_buckets[new_idx] = sdk_hashtable__new_bucket();
                }
                sdk_vector_resize_append(new_buckets[new_idx], node);
            }
        }
    }
    
    hashtable->capacity = new_capacity;
    SDK_FREE(hashtable->buckets);
    hashtable->buckets = new_buckets;
    
    return SDK_EOK;
}

static sdk_size_t sdk_hashtable__bucket_numcol(sdk_vector_t * bucket){
    if(!bucket) return 0;
    return (bucket->size==0)?0:(bucket->size-1);
}


////////////////////////////////////////////////////////////////////////////////
////

sdk_err_t sdk_hashtable_init(sdk_hashtable_t* hashtable, sdk_size_t capacity
                             , sdk_hashtable_key_ops_t key_ops, sdk_hashtable_val_ops_t val_ops)
{
    hashtable->key_ops = key_ops;
    hashtable->val_ops = val_ops;
    hashtable->size = 0;
    hashtable->capacity = capacity;
    hashtable->buckets = SDK_ALLOC(hashtable->capacity * BUCKET_SIZE);
    if(!hashtable->buckets){
        return SDK_ENOMEM;
    }
    for(sdk_size_t i=0; i<hashtable->capacity; i++){
        hashtable->buckets[i] = 0;
    }
    
    return SDK_EOK;
}

void sdk_hashtable_destroy(sdk_hashtable_t* hashtable){
    sdk_vector_t *bucket;
    sdk_hashtable_node_t * node;
    sdk_size_t i;
    sdk_size_t j;
    for(i=0; i<hashtable->capacity; i++){
        bucket = hashtable->buckets[i];
        if(bucket){
            for(j=0; j<bucket->size; j++){
                sdk_vector_get(bucket, j, (void **) &node);
                hashtable->key_ops.free(node->key, hashtable->key_ops.arg);
                hashtable->val_ops.free(node->val, hashtable->val_ops.arg);
                SDK_FREE(node);
            }
            sdk_vector_destroy(bucket);
        }
    }
    SDK_FREE(hashtable->buckets);
}

void* sdk_hashtable_get(sdk_hashtable_t* hashtable, const void* key){
    sdk_hashtable_node_t * node = sdk_hashtable__get_node(hashtable, key);
    if(node){
        return node->val;
    }
    return 0;
}

sdk_bool_t sdk_hashtable_contains(sdk_hashtable_t *hashtable, const void* key){
    sdk_hashtable_node_t * node = sdk_hashtable__get_node(hashtable, key);
    return node==0?SDK_FALSE:SDK_TRUE;
}

sdk_err_t sdk_hashtable_put(sdk_hashtable_t* hashtable, const void* key, const void* value)
{
    sdk_hashtable_node_t * node;
    sdk_size_t bucket_idx;
    
    node = sdk_hashtable__get_node(hashtable, key);
    if(node){
        hashtable->val_ops.free(node->val, hashtable->val_ops.arg);
        node->val = value?hashtable->val_ops.cp(value, hashtable->val_ops.arg):0;
    }else{
        // key don't exist
        SDK_NEW(node);
        if(!node){
            return SDK_ENOMEM;
        }
        node->hash = hashtable->key_ops.hash(key, hashtable->key_ops.arg);
        bucket_idx = node->hash % hashtable->capacity;
        if(hashtable->buckets[bucket_idx]==0){
            sdk_vector_t* bucket = sdk_hashtable__new_bucket();
            if(bucket==0){
                return SDK_ENOMEM;
            }
            hashtable->buckets[bucket_idx] = bucket;
        }
        node->key = hashtable->key_ops.cp(key, hashtable->key_ops.arg);
        node->val = hashtable->val_ops.cp(value, hashtable->val_ops.arg);
        
        sdk_vector_resize_append(hashtable->buckets[bucket_idx], node);
        hashtable->size++;
        if(hashtable->size > hashtable->capacity * GROWTH_FACTOR){
            sdk_hashtable__grow(hashtable, hashtable->capacity * GROWTH_FACTOR);
        }
    }
    
    return SDK_EOK;
}


sdk_size_t sdk_hashtable_numcol(sdk_hashtable_t* hashtable){
    sdk_size_t result =  0;
    for(sdk_size_t i=0; i<hashtable->capacity; i++){
        result+= sdk_hashtable__bucket_numcol(hashtable->buckets[i]);
    }
    return result;
}

void sdk_hashtable_foreach(sdk_hashtable_t* hashtable
        , void (*apply)(sdk_hashtable_t * hashtable, sdk_hashtable_node_t * node, void* userdata)
        , void* userdata)
{
    sdk_size_t i;
    sdk_size_t j;
    sdk_vector_t * bucket;
    sdk_hashtable_node_t * node;
    
    if(!apply){
        return;
    }
    
    for(i=0; i<hashtable->capacity; i++){
        bucket = hashtable->buckets[i];
        if(bucket){
            for(j=0; j<bucket->size; j++){
                sdk_vector_get(bucket, j, (void **) &node);
                apply(hashtable, node, userdata);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
////
static uint32_t hash_fmix32(uint32_t h){
    h ^= h >> 16;
    h *= 0x3243f6a9U;
    h ^= h >> 16;
    return h;
}

////////////////////////////////////////////////////////////////////////////////
////

uint32_t sdk_string_hash(const void* data, void* arg){
    //djb2
    uint32_t hash = (const uint32_t) 5381;
    const char *str = (const char*) data;
    char c;
    while((c=*str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash_fmix32(hash);
}

void* sdk_string_cp(const void* data, void* arg)
{
    const char *input = (const char*) data;
    size_t input_length = strlen(input) + 1;
    char *result;
    result = SDK_ALLOC(sizeof(*result) * input_length);
    if(!result){
        return 0;
    }
    strcpy(result, input);
    return result;
}
sdk_bool_t sdk_string_eq(const void* data1, const void* data2, void* arg){
    const char *str1 = (const char*) data1;
    const char *str2 = (const char*) data2;
    return !(strcmp(str1, str2)) ? SDK_TRUE : SDK_FALSE;
}

void sdk_string_free(void* data, void* arg){
    SDK_FREE(data);
}

sdk_hashtable_key_ops_t sdk_string_key_ops={.hash=sdk_string_hash, .cp=sdk_string_cp, .free=sdk_string_free, .eq=sdk_string_eq, .arg=0};
sdk_hashtable_val_ops_t sdk_string_val_ops={.cp=sdk_string_cp, .free=sdk_string_free, .eq=sdk_string_eq, .arg=0};

////////////////////////////////////////////////////////////////////////////////
//// Fibonacci Hashing

#define HASH_W 32
#if (HASH_W==16)
#define HASH_A ((uint16_t)40503)
#elif (HASH_W==32)
#define HASH_A ((uint32_t)2654435769U)
#elif (HASH_W==64)
#define HASH_A ((uint64_t)11400714819323198485U)
#endif
#define HASH_M 3

static uint32_t hash_multip(uint32_t x, uint32_t m){
    return (x * HASH_A) >> (HASH_W - m);
}

/* better, but more operation */
static uint32_t hash_multip2(uint32_t x, uint32_t m){
    x ^= x >> (HASH_W - m);
    return (x * HASH_A) >> (HASH_W - m);
}

////////////////////////////////////////////////////////////////////////////////
////


uint32_t sdk_integer_hash(const void* data, void* arg){
    sdk_uint_t data_i = (sdk_uint_t)data;
    return hash_multip2(data_i, HASH_M);
}

void* sdk_integer_cp(const void* data, void* arg){
    sdk_uint_t data_i = (sdk_uint_t)data;
    return (void*)data_i;
}

sdk_bool_t sdk_integer_eq(const void* data1, const void* data2, void* arg){
    sdk_uint_t data1_i = (sdk_uint_t)data1;
    sdk_uint_t data2_i = (sdk_uint_t)data2;
    return (data1_i == data2_i)?SDK_TRUE:SDK_FALSE;
}

void sdk_integer_free(void* data, void* arg){
    // NO OP
}

sdk_hashtable_key_ops_t sdk_integer_key_ops={.hash=sdk_integer_hash, .cp=sdk_integer_cp, .free=sdk_integer_free, .eq=sdk_integer_eq, .arg=0};
sdk_hashtable_val_ops_t sdk_integer_val_ops={.cp=sdk_integer_cp, .free=sdk_integer_free, .eq=sdk_integer_eq, .arg=0};

////////////////////////////////////////////////////////////////////////////////
////

uint32_t sdk_any_hash(const void* data, void* arg)
{
    sdk_any_t * any = (sdk_any_t *)data;
    switch (any->type) {
        case SDK_HASHTABLE_CHAR:{
            char v = any->val.char_val;
            return hash_multip2(v, HASH_M);
        }
        case SDK_HASHTABLE_DOUBLE:
        {
            sdk_uint_t data_i = (sdk_uint_t)any->val.double_val;
            return hash_multip2(data_i, HASH_M);
        }
        case SDK_HASHTABLE_FLOAT:
        {
            sdk_uint_t data_i = (sdk_uint_t)any->val.float_val;
            return hash_multip2(data_i, HASH_M);
        }
        case SDK_HASHTABLE_INTEGER:
        {
            sdk_uint_t data_i = (sdk_uint_t)any->val.int_val;
            return hash_multip2(data_i, HASH_M);
        }
        case SDK_HASHTABLE_STRING:{
            return sdk_string_hash(any->val.string_val, arg);
        }
        case SDK_HASHTABLE_POINTER:{
            sdk_uint_t data_i = (sdk_uint_t)any->val.pointer_val;
            return hash_multip2(data_i, HASH_M);
        }
        default:{
            sdk_uint_t data_i = (sdk_uint_t)any->val.pointer_val;
            return hash_multip2(data_i, HASH_M);
        }
    }
}
void* sdk_any_cp(const void* data, void* arg){
    sdk_any_t * any = (sdk_any_t *)data;
    sdk_any_t * new_any;
    SDK_NEW(new_any);
    if(!new_any){
        return 0;
    }
    memcpy(new_any, any, sizeof(*any));
    return new_any;
}
sdk_bool_t sdk_any_eq(const void* data1, const void* data2, void* arg){
    sdk_any_t *any1 = (sdk_any_t*)data1;
    sdk_any_t *any2 = (sdk_any_t*)data2;
    
    if(any1->type!=any2->type){
        return SDK_FALSE;
    }
    
    switch (any1->type) {
        case SDK_HASHTABLE_CHAR:{
            return (any1->val.char_val==any2->val.char_val)?SDK_TRUE:SDK_FALSE;
        }
        case SDK_HASHTABLE_DOUBLE:
        {
            return (any1->val.double_val==any2->val.double_val)?SDK_TRUE:SDK_FALSE;
        }
        case SDK_HASHTABLE_FLOAT:
        {
            return (any1->val.float_val==any2->val.float_val)?SDK_TRUE:SDK_FALSE;
        }
        case SDK_HASHTABLE_INTEGER:
        {
            return (any1->val.int_val==any2->val.int_val)?SDK_TRUE:SDK_FALSE;
        }
        case SDK_HASHTABLE_STRING:{
            return (strcmp(any1->val.string_val, any2->val.string_val)==0)?SDK_TRUE:SDK_FALSE;
        }
        case SDK_HASHTABLE_POINTER:{
            return (any1->val.pointer_val==any2->val.pointer_val)?SDK_TRUE:SDK_FALSE;
        }
    }
}
void sdk_any_free(void* data, void* arg){
    if(data){
        sdk_any_t * any = (sdk_any_t*)data;
        if(any->type==SDK_HASHTABLE_STRING){
            SDK_FREE(any->val.string_val);
        }
        SDK_FREE(data);
    }
}

sdk_hashtable_key_ops_t sdk_any_key_ops={.hash=sdk_any_hash, .cp=sdk_any_cp, .free=sdk_any_free, .eq=sdk_any_eq, .arg=0};
sdk_hashtable_val_ops_t sdk_any_val_ops={.cp=sdk_any_cp, .free=sdk_any_free, .eq=sdk_any_eq, .arg=0};
