#ifndef INCLUDED_GAI_BEHAVIORTREE_H
#define INCLUDED_GAI_BEHAVIORTREE_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_GAI_NODE_H
#include <gai_node.h>
#endif /*INCLUDED_GAI_NODE_H*/

#ifndef INCLUDED_GAI_ACTION_H
#include <gai_action.h>
#endif /*INCLUDED_GAI_ACTION_H*/

#ifndef INCLUDED_GAI_EVALUATOR_H
#include <gai_evaluator.h>
#endif /*INCLUDED_GAI_EVALUATOR_H*/

#ifndef INCLUDED_SDK_VECTOR_H
#include <sdk_vector.h>
#endif /*INCLUDED_SDK_VECTOR_H*/



////////////////////////////////////////////////////////////////////////////////
////
#define GAI_BT_NODE_TYPE_TREE           0x1000
#define GAI_BT_NODE_TYPE_ACTION         0x1001
#define GAI_BT_NODE_TYPE_CONDITION      0x1002
#define GAI_BT_NODE_TYPE_SELECTOR       0x1003
#define GAI_BT_NODE_TYPE_SEQUENCE       0x1004

typedef struct gai_BehaviorTreeNode_s{
    gai_Node_t node;
    gai_Action_t * action;
    sdk_vector_t children;
    gai_Evaluator_t * evaluator;
    struct gai_BehaviorTreeNode_s* parent;
}gai_BehaviorTreeNode_t;

typedef struct gai_BehaviorTree_s{
    gai_Node_t node;
    gai_BehaviorTreeNode_t* root;
    gai_BehaviorTreeNode_t* currentNode;
}gai_BehaviorTree_t;
////////////////////////////////////////////////////////////////////////////////
////

int gai_BehaviorTreeNode_Init(gai_BehaviorTreeNode_t* btNode, const char* name, int type, void* userdata);

void gai_BehaviorTreeNode_DeInit(gai_BehaviorTreeNode_t* btNode);

void gai_BehaviorTreeNode_SetAction(gai_BehaviorTreeNode_t* btNode, gai_Action_t* action);

void gai_BehaviorTreeNode_SetEvaluator(gai_BehaviorTreeNode_t* btNode, gai_Evaluator_t* evaluator);

void gai_BehaviorTreeNode_SetType(gai_BehaviorTreeNode_t * btNode, int type);

////////////////////////////////////////////////////////////////////////////////
//// Child Functions

void gai_BehaviorTreeNode_AddChild(gai_BehaviorTreeNode_t* btNode, gai_BehaviorTreeNode_t* child);

int gai_BehaviorTreeNode_ChildIndex(gai_BehaviorTreeNode_t* btNode, gai_BehaviorTreeNode_t* child);

gai_BehaviorTreeNode_t* gai_BehaviorTreeNode_GetChild(gai_BehaviorTreeNode_t* btNode, int idx);

sdk_size_t gai_BehaviorTreeNode_GetNumberOfChildren(gai_BehaviorTreeNode_t* btNode);

gai_BehaviorTreeNode_t* gai_BehaviorTreeNode_FindChild(gai_BehaviorTreeNode_t* btNode, const char* childName);

gai_BehaviorTreeNode_t* gai_BehaviorTreeNode_GetParent(gai_BehaviorTreeNode_t * btNode);

////////////////////////////////////////////////////////////////////////////////
//// BehaviorTree Nodes

int gai_BehaviorTree_Action_Init(gai_BehaviorTreeNode_t* actionNode, const char* name, gai_Action_t * action, void* userdata);
int gai_BehaviorTree_Condition_Init(gai_BehaviorTreeNode_t* evaluatorNode, const char* name, gai_Evaluator_t * evaluator, void* userdata);
int gai_BehaviorTree_Selector_Init(gai_BehaviorTreeNode_t* selectorNode, const char* name, void* userdata);
int gai_BehaviorTree_Sequence_Init(gai_BehaviorTreeNode_t* btNode, const char* name, void* userdata);

////////////////////////////////////////////////////////////////////////////////
//// BehaviorTree
int gai_BehaviorTree_Init(gai_BehaviorTree_t * tree, const char* name, void* userdata);

void gai_BehaviorTree_SetRoot(gai_BehaviorTree_t* tree, gai_BehaviorTreeNode_t* root);

void gai_BehaviorTree_Update(gai_BehaviorTree_t* self, void* userdata);

#endif /* INCLUDED_GAI_BEHAVIORTREE_H */
