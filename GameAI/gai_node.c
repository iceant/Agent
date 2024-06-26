#include <gai_node.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
////

int gai_Node_Init(gai_Node_t* node, const char* name, int type, void* userdata)
{
    if(name){
        sdk_size_t name_size = strlen(name);
        name_size = GAI_NAME_SIZE>name_size?name_size:GAI_NAME_SIZE;
        strncpy(node->name, name, name_size);
        node->name[name_size]='\0';
    }else{
        node->name[0]='\0';
    }
    
    node->type = type;
    node->userdata = userdata;
    
    return 0;
}

