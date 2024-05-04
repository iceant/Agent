#ifndef INCLUDED_SDK_BEHAVIORTREE_H
#define INCLUDED_SDK_BEHAVIORTREE_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_SDK_ERRORS_H
#include <sdk_errors.h>
#endif /*INCLUDED_SDK_ERRORS_H*/

#ifndef INCLUDED_SDK_LIST_H
#include <sdk_list.h>
#endif /*INCLUDED_SDK_LIST_H*/

////////////////////////////////////////////////////////////////////////////////
////

/*
 * Evaluator: 估值器，对条件进行判断 bool Evaluator::Evaluate(EvaluatorContext);
 */


////////////////////////////////////////////////////////////////////////////////
////
#define BT_TYPE_SEQUENCE    (1<<0)
#define BT_TYPE_SELECTOR    (1<<1)
#define BT_TYPE_ACTOR       (1<<2)

#define BT_STATE_UNKNOWN  0
#define BT_STATE_SUCCESS (1)
#define BT_STATE_FAILURE (2)
#define BT_STATE_RUNNING (4)

typedef struct sdk_BehaviorTreeNode_s{
    char* name;
    int type;
    sdk_list_node_t node;
    sdk_list_t child_nodes;
    int state;
    void (*Init)(struct sdk_BehaviorTreeNode_s* node);
    int (*Update)(struct sdk_BehaviorTreeNode_s* node);
    void (*DeInit)(struct sdk_BehaviorTreeNode_s* node);
    void* userdata;
}sdk_BehaviorTreeNode_t;

typedef struct sdk_BehaviorTree_s{
    char* name;
    sdk_list_t nodes;
    sdk_BehaviorTreeNode_t * current_node;
    void* userdata;
}sdk_BehaviorTree_t;

////////////////////////////////////////////////////////////////////////////////
////

sdk_err_t sdk_BehaviorTreeNode_Init(sdk_BehaviorTreeNode_t* node, char* name, int type, void* userdata);

sdk_err_t sdk_BehaviorTree_Init(sdk_BehaviorTree_t * tree, char* name, void* userdata);

sdk_err_t sdk_BehaviorTree_AddNode(sdk_BehaviorTree_t* tree, sdk_BehaviorTreeNode_t* node);

sdk_BehaviorTreeNode_t * sdk_BehaviorTree_FindNode(sdk_BehaviorTree_t* tree, const char* nodeName);

sdk_err_t sdk_BehaviorTree_RemoveNode(sdk_BehaviorTree_t* tree, sdk_BehaviorTreeNode_t* node);

void sdk_BehaviorTree_SetCurrentNode(sdk_BehaviorTree_t* tree, sdk_BehaviorTreeNode_t* node);

int sdk_BehaviorTree_Update(sdk_BehaviorTree_t* tree);

#endif /* INCLUDED_SDK_BEHAVIORTREE_H */
