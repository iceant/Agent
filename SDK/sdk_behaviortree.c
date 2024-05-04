#include <sdk_behaviortree.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
////
static void dummy_Init(sdk_BehaviorTreeNode_t* node){

}

int dummy_Process(sdk_BehaviorTreeNode_t* node){
    return BT_STATE_UNKNOWN;
}
////////////////////////////////////////////////////////////////////////////////
////

sdk_err_t sdk_BehaviorTreeNode_Init(sdk_BehaviorTreeNode_t* node, char* name, int type, void* userdata)
{
    if(name){
        node->name = strdup(name);
    } else{
        node->name = 0;
    }
    
    node->type = type;
    SDK_LIST_INIT(&node->node);
    SDK_LIST_INIT(&node->child_nodes);
    node->state = BT_STATE_UNKNOWN;
    node->Init = dummy_Init;
    node->Update = dummy_Process;
    node->userdata= userdata;
    return SDK_EOK;
}

sdk_err_t sdk_BehaviorTree_Init(sdk_BehaviorTree_t * tree, char* name, void* userdata)
{
    if(name){
        tree->name = strdup(name);
    }else{
        tree->name = 0;
    }
    
    SDK_LIST_INIT(&tree->nodes);
    tree->current_node = 0;
    tree->userdata = userdata;
    return SDK_EOK;
}

sdk_err_t sdk_BehaviorTree_AddNode(sdk_BehaviorTree_t* tree, sdk_BehaviorTreeNode_t* node)
{
    SDK_LIST_INSERT_BEFORE(&tree->nodes, &node->node);
    return SDK_EOK;
}

sdk_BehaviorTreeNode_t * sdk_BehaviorTree_FindNode(sdk_BehaviorTree_t* tree, const char* nodeName){
    sdk_list_node_t * node;
    for(node = SDK_LIST_NEXT(&tree->nodes); node!=&tree->nodes; ){
        sdk_BehaviorTreeNode_t * bt_node = SDK_CONTAINER_OF(node, sdk_BehaviorTreeNode_t, node);
        if(strcmp(bt_node->name, nodeName)==0){
            return bt_node;
        }
    }
    return NULL;
}

sdk_err_t sdk_BehaviorTree_RemoveNode(sdk_BehaviorTree_t* tree, sdk_BehaviorTreeNode_t* node){
    SDK_LIST_REMOVE(&node->node);
    return SDK_EOK;
}

void sdk_BehaviorTree_SetCurrentNode(sdk_BehaviorTree_t* tree, sdk_BehaviorTreeNode_t* node)
{
    tree->current_node = node;
}

int sdk_BehaviorTree_Update(sdk_BehaviorTree_t* tree)
{
    if(!tree->current_node){
        return BT_STATE_UNKNOWN;
    }
    int result = 0;
    tree->current_node->Init(tree->current_node);
    result = tree->current_node->Update(tree->current_node);
    tree->current_node->DeInit(tree->current_node);
    return result;
}

