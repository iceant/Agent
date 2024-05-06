#include <sdk_hashtable.h>
#include <stdio.h>
#include <sdk_memory.h>

static void hashtable_print(sdk_hashtable_t * hashtable, sdk_hashtable_node_t* node, void* arg){
    printf("[%d:%d]=%d\r\n",  (sdk_uint_t)node->key, node->hash, (sdk_uint_t)node->val);
}

static void any_print(sdk_any_t* any){
    switch (any->type) {
        case SDK_HASHTABLE_CHAR:{
            printf("%c", any->val.char_val);
            break;
        }
        case SDK_HASHTABLE_DOUBLE:
        {
            printf("%f", any->val.double_val);
            break;
        }
        case SDK_HASHTABLE_FLOAT:{
            printf("%f", any->val.float_val);
            break;
        }
        case SDK_HASHTABLE_INTEGER:{
            printf("%"SDK_PRIu, any->val.int_val);
            break;
        }
        case SDK_HASHTABLE_STRING:{
            printf("%s", any->val.string_val);
            break;
        }
        case SDK_HASHTABLE_POINTER:{
            printf("%p", any->val.pointer_val);
            break;
        }
    }
}

static void hashtable_any_print(sdk_hashtable_t* ht, sdk_hashtable_node_t* n, void* arg){
    printf("[");
    any_print((sdk_any_t *)n->key);
    printf(":%d]", n->hash);
    printf("=");
    any_print((sdk_any_t *)n->val);
    printf("\r\n");
}

int main(int argc, char** argv)
{
    sdk_memory_init();
    sdk_hashtable_t htable;
    sdk_hashtable_init(&htable, 2048, sdk_any_key_ops, sdk_any_val_ops);
    for(int i=0; i<10; i++){
        char* value = SDK_ALLOC(sizeof(char) * 16);
        snprintf(value, sizeof(value), "%d", i);
        sdk_any_t any_str_key={.type=SDK_HASHTABLE_STRING, .val.string_val=value};
        
        sdk_any_t any_int_val={.type=SDK_HASHTABLE_INTEGER, .val.int_val=i+10};
        
        sdk_any_t any_int_key={.type=SDK_HASHTABLE_INTEGER, .val.int_val=i};
        sdk_hashtable_put(&htable, &any_int_key, &any_int_val);
        
        sdk_any_t any_int_key2={.type=SDK_HASHTABLE_INTEGER, .val.int_val=i+20};
        sdk_hashtable_put(&htable, &any_str_key, &any_int_val);
        
    }
    
    sdk_hashtable_foreach(&htable, hashtable_any_print, 0);
    
    printf("num_cols: %lld\r\n", sdk_hashtable_numcol(&htable));
    
    sdk_hashtable_destroy(&htable);
    sdk_memory_destroy();
    return 0;
}