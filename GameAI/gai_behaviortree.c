#include <gai_behaviortree.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
////

typedef struct gai_BehaviorTree_EvaluateResult_s{
    bool result;
    gai_BehaviorTreeNode_t* node;
}gai_BehaviorTree_EvaluateResult_t;

static gai_BehaviorTree_EvaluateResult_t gai_BehaviorTree_Sequence_Evaluate(gai_BehaviorTree_t * self
                                                                  , gai_BehaviorTreeNode_t * node
                                                                  , sdk_size_t index, void* userdata);

static gai_BehaviorTree_EvaluateResult_t gai_BehaviorTree_Selector_Evaluate(gai_BehaviorTree_t * self
                                                                   , gai_BehaviorTreeNode_t * node, void* userdata)
{
    sdk_size_t i;
    gai_BehaviorTreeNode_t * child;
    gai_BehaviorTree_EvaluateResult_t result={.result=false, .node=0};
    for(i=0; i<node->children.size; i++){
        sdk_vector_get(&node->children, i, (void **) &child);
        if(child->node.type==GAI_BT_NODE_TYPE_ACTION){
            result.result=true;
            result.node=child;
            return result;
        }else if(child->node.type==GAI_BT_NODE_TYPE_CONDITION){
            /* CONDITION is invalid in Selector  */
            result.result=false;
            result.node=0;
            return result;
        }else if(child->node.type==GAI_BT_NODE_TYPE_SELECTOR){
            gai_BehaviorTree_EvaluateResult_t eval_result = gai_BehaviorTree_Selector_Evaluate(self, child, userdata);
            if(eval_result.result){
                return eval_result;
            }
        }else if(child->node.type==GAI_BT_NODE_TYPE_SEQUENCE){
            gai_BehaviorTree_EvaluateResult_t eval_result = gai_BehaviorTree_Sequence_Evaluate(self, child, 0, userdata);
            if(eval_result.result){
                return eval_result;
            }
        }
    }
    return result;
}

static gai_BehaviorTree_EvaluateResult_t gai_BehaviorTree_Sequence_Evaluate(gai_BehaviorTree_t * self
                                                                  , gai_BehaviorTreeNode_t * node
                                                                  , sdk_size_t index, void* userdata)
{
    sdk_size_t i;
    gai_BehaviorTreeNode_t * child;
    gai_BehaviorTree_EvaluateResult_t result={.result=false, .node= 0};
    for(i=index; i<node->children.size; i++){
        sdk_vector_get(&node->children, i, (void **) &child);
        if(child->node.type==GAI_BT_NODE_TYPE_ACTION){
            result.result = true;
            result.node = child;
            return result;
        }else if(child->node.type==GAI_BT_NODE_TYPE_CONDITION){
            int eval_result = gai_Evaluator_Evaluate(child->evaluator, userdata);
            if(!eval_result){
                return result;
            }
        }else if(child->node.type == GAI_BT_NODE_TYPE_SELECTOR){
            gai_BehaviorTree_EvaluateResult_t eval_result = gai_BehaviorTree_Selector_Evaluate(self, child, userdata);
            if(!eval_result.result){
                return result;
            }else if(eval_result.result==true && eval_result.node){
                return eval_result;
            }
        }else if(child->node.type==GAI_BT_NODE_TYPE_SEQUENCE){
            gai_BehaviorTree_EvaluateResult_t eval_result = gai_BehaviorTree_Sequence_Evaluate(self, child, 0, userdata);
            if(!eval_result.result){
                return result;
            }else if(eval_result.result==true && eval_result.node){
                return eval_result;
            }
        }
    }
    result.result = true;
    return result;
}

static gai_BehaviorTreeNode_t* gai_BehaviorTree_Node_Evaluate(gai_BehaviorTree_t * self
                                                                        , gai_BehaviorTreeNode_t* node, void* userdata)
{
    if(node->node.type==GAI_BT_NODE_TYPE_ACTION){
        return node;
    }else if(node->node.type==GAI_BT_NODE_TYPE_CONDITION){
        /* WRONG */
    }else if(node->node.type==GAI_BT_NODE_TYPE_SELECTOR){
        gai_BehaviorTree_EvaluateResult_t result = gai_BehaviorTree_Selector_Evaluate(self, node, userdata);
        if(result.result){
            return result.node;
        }
    }else if(node->node.type==GAI_BT_NODE_TYPE_SEQUENCE){
        gai_BehaviorTree_EvaluateResult_t result = gai_BehaviorTree_Sequence_Evaluate(self, node, 0, userdata);
        if(result.result){
            return result.node;
        }
    }
}

static gai_BehaviorTreeNode_t* gai_BehaviorTree_Continue_Evaluate(gai_BehaviorTree_t * self
        , gai_BehaviorTreeNode_t* node, void* userdata)
{
    gai_BehaviorTreeNode_t* parentNode = node->parent;
    gai_BehaviorTreeNode_t* childNode = node;
    while(parentNode){
        if(parentNode->node.type==GAI_BT_NODE_TYPE_SEQUENCE){
            int childIndex = gai_BehaviorTreeNode_ChildIndex(parentNode, childNode);
            if(childIndex < gai_BehaviorTreeNode_GetNumberOfChildren(parentNode)){
                gai_BehaviorTree_EvaluateResult_t evaluateResult
                        = gai_BehaviorTree_Sequence_Evaluate(self, parentNode, childIndex+1, userdata);
                return evaluateResult.node;
            }
        }
        childNode = parentNode;
        parentNode = childNode->parent;
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
////

int gai_BehaviorTreeNode_Init(gai_BehaviorTreeNode_t* btNode, const char* name, int type, void* userdata)
{
    gai_Node_Init(&btNode->node, name, type, userdata);
    btNode->action = 0;
    sdk_vector_init(&btNode->children, 0);
    btNode->evaluator = 0;
    btNode->parent = 0;
}


void gai_BehaviorTreeNode_DeInit(gai_BehaviorTreeNode_t* btNode){
    sdk_vector_destroy(&btNode->children);
}


void gai_BehaviorTreeNode_SetAction(gai_BehaviorTreeNode_t* btNode, gai_Action_t* action){
    btNode->action = action;
}

void gai_BehaviorTreeNode_SetEvaluator(gai_BehaviorTreeNode_t* btNode, gai_Evaluator_t* evaluator)
{
    btNode->evaluator = evaluator;
}

void gai_BehaviorTreeNode_SetType(gai_BehaviorTreeNode_t * btNode, int type)
{
    btNode->node.type = type;
}

void gai_BehaviorTreeNode_AddChild(gai_BehaviorTreeNode_t* btNode, gai_BehaviorTreeNode_t* child){
    sdk_vector_resize_append(&btNode->children, child);
    child->parent = btNode;
}

int gai_BehaviorTreeNode_ChildIndex(gai_BehaviorTreeNode_t* btNode, gai_BehaviorTreeNode_t* child)
{
    sdk_size_t i;
    gai_BehaviorTreeNode_t* node;
    for(i = 0; i<btNode->children.size; i++){
        sdk_vector_get(&btNode->children, i, (void **) &node);
        if(node==child){
            return i;
        }
    }
    return -1;
}

gai_BehaviorTreeNode_t* gai_BehaviorTreeNode_GetChild(gai_BehaviorTreeNode_t* btNode, int idx)
{
    gai_BehaviorTreeNode_t* node=0;
    if(idx<0 || idx>=btNode->children.size){
        return 0;
    }
    sdk_vector_get(&btNode->children, idx, (void **) &node);
    return node;
}

sdk_size_t gai_BehaviorTreeNode_GetNumberOfChildren(gai_BehaviorTreeNode_t* btNode)
{
    return btNode->children.size;
}

gai_BehaviorTreeNode_t* gai_BehaviorTreeNode_FindChild(gai_BehaviorTreeNode_t* btNode, const char* childName)
{
    sdk_size_t i;
    gai_BehaviorTreeNode_t* node=0;
    for(i = 0; i<btNode->children.size; i++){
        sdk_vector_get(&btNode->children, i, (void **) &node);
        if(strcmp(childName, node->node.name)==0){
            return node;
        }
    }
    return 0;
}

gai_BehaviorTreeNode_t* gai_BehaviorTreeNode_GetParent(gai_BehaviorTreeNode_t * btNode)
{
    return btNode->parent;
}

////////////////////////////////////////////////////////////////////////////////
////

int gai_BehaviorTree_Action_Init(gai_BehaviorTreeNode_t* actionNode, const char* name, gai_Action_t * action, void* userdata)
{
    gai_BehaviorTreeNode_Init(actionNode, name, GAI_BT_NODE_TYPE_ACTION, userdata);
    actionNode->action = action;
    return 0;
}

int gai_BehaviorTree_Condition_Init(gai_BehaviorTreeNode_t* evaluatorNode, const char* name, gai_Evaluator_t * evaluator, void* userdata)
{
    gai_BehaviorTreeNode_Init(evaluatorNode, name, GAI_BT_NODE_TYPE_CONDITION, userdata);
    evaluatorNode->evaluator = evaluator;
    return 0;
}

int gai_BehaviorTree_Selector_Init(gai_BehaviorTreeNode_t* selectorNode, const char* name, void* userdata)
{
    gai_BehaviorTreeNode_Init(selectorNode, name, GAI_BT_NODE_TYPE_SELECTOR, userdata);
    return 0;
}

int gai_BehaviorTree_Sequence_Init(gai_BehaviorTreeNode_t* btNode, const char* name, void* userdata)
{
    gai_BehaviorTreeNode_Init(btNode, name, GAI_BT_NODE_TYPE_SEQUENCE, userdata);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
////

int gai_BehaviorTree_Init(gai_BehaviorTree_t * tree, const char* name, void* userdata)
{
    gai_Node_Init(&tree->node, name, GAI_BT_NODE_TYPE_TREE, userdata);
    tree->currentNode = 0;
    tree->root = 0;
    return 0;
}

void gai_BehaviorTree_Update(gai_BehaviorTree_t* self, void* userdata)
{
    if(self->currentNode==0){
        self->currentNode = gai_BehaviorTree_Node_Evaluate(self, self->root, userdata);
    }
    
    if(self->currentNode){
        gai_ActionStatus status = self->currentNode->action->status;
        if(status==kGAI_ActionStatus_UNINITIALIZED){
            gai_Action_Initialize(self->currentNode->action, userdata);
        }else if(status==kGAI_ActionStatus_TERMINATED){
            gai_Action_Cleanup(self->currentNode->action, userdata);
            self->currentNode = gai_BehaviorTree_Continue_Evaluate(self, self->currentNode, userdata);
        }else if(status==kGAI_ActionStatus_RUNNING){
            gai_Action_Update(self->currentNode->action, userdata);
        }
    }
}

void gai_BehaviorTree_SetRoot(gai_BehaviorTree_t* tree, gai_BehaviorTreeNode_t* root){
    tree->root = root;
}