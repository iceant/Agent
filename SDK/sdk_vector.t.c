#include <sdk_vector.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    sdk_vector_t v1;
    sdk_vector_init(&v1, 0);
    
    for(int i=0; i<10; i++){
        sdk_vector_resize_append(&v1, (void*)(i+1));
    }
    
    for(sdk_size_t i=0; i<v1.size; i++){
        void* item = 0;
        sdk_vector_get(&v1, i, &item);
        printf("[%d] %d\r\n", i, (int)item);
    }
    
    while(v1.size>0){
        sdk_vector_resize_remove(&v1, 0);
        printf("Vector.Size=%d, Vector.Capacity=%d\r\n", v1.size, v1.capacity);
    }
    
    return 0;
}