#include <sdk_pool.h>
#include <stdio.h>
#include <sdk_memory.h>
#include <stdlib.h>
#include <sdk_macros.h>
////////////////////////////////////////////////////////////////////////////////
////

#define ALLOCATOR_SIZE (1024*1024*10)

typedef struct Allocator_S{
    struct Allocator_S* next;
    uint8_t * memory;
    sdk_size_t size;
    sdk_size_t used;
}Allocator_T;

static Allocator_T* Allocator__Instance = 0;

static sdk_err_t Allocator_Init(void){
    Allocator__Instance=0;
    return SDK_EOK;
}

static void* Allocator_Alloc(sdk_size_t size)
{
    if(!Allocator__Instance){
        Allocator_T * p;
        SDK_NEW(p);
        p->next = Allocator__Instance;
        p->memory = SDK_ALLOC(size);
        p->size = size;
        p->used = size;
        Allocator__Instance = p;
        return p->memory;
    }else{
        Allocator_T * p = Allocator__Instance;
        for(Allocator_T* q = p; q!=0; q=p->next){
            if((q->size - q->used)>=size){
                void* m = q->memory + q->used;
                q->used += size;
                return m;
            }
        }
        
        SDK_NEW(p);
        p->next = Allocator__Instance;
        p->memory = SDK_ALLOC(size);
        p->size = size;
        p->used = size;
        Allocator__Instance = p;
        return p->memory;
    }
    
}

static void Allocator_Release(void){
    Allocator_T * p = Allocator__Instance;
    for(Allocator_T* q = p; q!=0; q=p->next){
        SDK_FREE(q->memory);
        p = q;
        SDK_FREE(q);
    }
    Allocator__Instance = 0;
}

////////////////////////////////////////////////////////////////////////////////
////
static sdk_pool_chunk_allocator_t allocator={.alloc=Allocator_Alloc, .release=Allocator_Release};
static sdk_pool_t pool;

int main(int argc, char** argv)
{
    sdk_memory_init();
    sdk_pool_init(&pool, &allocator, 100, 0x0F);
    
    void* mem_array[10];
    
    for(int i=0; i<SDK_ARRAY_SIZE(mem_array); i++){
        mem_array[i] = sdk_pool_alloc(&pool);
        printf("Allocated: %p\r\n", mem_array[i]);
    }
    for(int i=0; i<SDK_ARRAY_SIZE(mem_array); i++){
        printf("Free: %p\r\n", mem_array[i]);
        sdk_pool_free(&pool, mem_array[i]);
        mem_array[i] = 0;
    }
    
    for(int i=0; i<SDK_ARRAY_SIZE(mem_array); i++){
        mem_array[i] = sdk_pool_alloc(&pool);
        printf("Allocated: %p\r\n", mem_array[i]);
        sdk_pool_free(&pool, mem_array[i]);
    }
    
    
    sdk_pool_destroy(&pool);
    sdk_memory_destroy();
}