#include <sdk_ringbuffer.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 255

static uint8_t buffer_block[BUFFER_SIZE];

int main(int argc, char** argv)
{
    sdk_ringbuffer_t buffer;
    sdk_ringbuffer_init(&buffer, buffer_block, BUFFER_SIZE);
    
    for(int i=0; i<256; i++){
        sdk_ringbuffer_put(&buffer, i);
//        printf("[%d] %c\r\n", i, i);
    }
    
    buffer.read_idx=49;
    sdk_size_t next_idx = 0;
    unsigned long value = sdk_ringbuffer_strtoul(&buffer, &next_idx, 10);
    printf("Value: %ld, next_idx = %ld\r\n", value, next_idx);
    
    uint8_t data;
    buffer.read_idx = 255;
    sdk_ringbuffer_read(&buffer, 49, &data);
    printf("Read Offset 1: '%c'\r\n", data);
    
    buffer.read_idx = 58;
    sdk_ringbuffer_read(&buffer, -2, &data);
    printf("Read Offset -2: '%c'\r\n", data);
    
    buffer.read_idx = 0;
    sdk_size_t find_idx = 0;
    sdk_err_t err = sdk_ringbuffer_strchr(&buffer, '1', &find_idx);
    printf("'1' index: %d, err=%d\r\n", find_idx, err);
    
    char* number = "01230";
    buffer.read_idx=48;
    int strncmp_result = sdk_ringbuffer_strncmp(&buffer, number, strlen(number));
    printf("strncmp result: %d\r\n", strncmp_result);
    
    buffer.read_idx=48;
    number = "3456";
    err = sdk_ringbuffer_strstr(&buffer, 0, number, &find_idx);
    printf("strstr: err=%d, find_idx=%d\r\n", err, find_idx);
    
    int memcmp = sdk_ringbuffer_memcmp(&buffer, 51, (uint8_t*)number, strlen(number));
    printf("memcmp: %d\r\n", memcmp);
    
    return 0;
}