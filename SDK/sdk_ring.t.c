#include <sdk_ring.h>
#include <stdio.h>

#define OBJECT_SIZE sizeof(object_t)
#define OBJECT_COUNT 1000
#define BLOCK_SIZE (OBJECT_COUNT*OBJECT_SIZE)

typedef struct object_s{
    char name[16];
    int age;
}object_t;

static uint8_t block[BLOCK_SIZE];

int main(int argc, char** argv) {
    sdk_ring_t ring;
    sdk_ring_init(&ring, block, OBJECT_SIZE, OBJECT_COUNT);
    char buf[32];
    for(int i=0; i<10; i++){
        object_t object;
        snprintf(object.name, sizeof(object.name), "Name_%d", i);
        object.age = 30+i;
        sdk_ring_put(&ring, &object);
    }
    
    printf("Use read...\r\n");
    for(int i=0; i<10; i++){
        object_t object;
//        sdk_ring_get(&ring, &object);
        sdk_ring_read(&ring, i, &object);
        printf("[%d] %s - %d , size:%llu\r\n", i, object.name, object.age, sdk_ring_size(&ring));
    }
    
    printf("Use peek...\r\n");
    for(int i=0; i<10; i++){
        object_t * object_p=0;
//        sdk_ring_get(&ring, &object);
        sdk_ring_peek(&ring, i, (void **) &object_p);
        printf("[%d] %s - %d , size:%llu\r\n", i, object_p->name, object_p->age, sdk_ring_size(&ring));
    }
    
    return 0;
}