#include <gai_decisiontree.h>
#include <string.h>

int gai_DecisionTreeBranch_Init(gai_DecisionTreeBranch_t* branch, const char* name
        , gai_Evaluator_t *evaluator, void* userdata)
{
    gai_Node_Init(&branch->node, name, GAI_NODE_TYPE_DECISION_BRANCH, userdata);
    branch->evaluator = evaluator;
    sdk_vector_init(&branch->children, 0);
    return 0;
}

void gai_DecisionTreeBranch_DeInit(gai_DecisionTreeBranch_t* branch){
    sdk_vector_destroy(&branch->children);
}

void gai_DecisionTreeBranch_AddChild(gai_DecisionTreeBranch_t* branch, gai_Node_t* child){
    sdk_vector_resize_append(&branch->children, child);
}

void gai_DecisionTreeBranch_SetEvaluator(gai_DecisionTreeBranch_t* branch, gai_Evaluator_t * evaluator){
    branch->evaluator = evaluator;
}

gai_Action_t * gai_DecisionTreeBranch_Evaluate(gai_DecisionTreeBranch_t* branch, void* userdata){
    int children_idx = branch->evaluator->Evaluate(branch->evaluator, userdata); /*  */
    gai_Node_t* branch_node=0;
    sdk_err_t err = sdk_vector_get(&branch->children, children_idx, (void **) &branch_node);
    if(err!=SDK_EOK){
        return 0;
    }
    if(branch_node->type==GAI_NODE_TYPE_DECISION_BRANCH){
        gai_DecisionTreeBranch_t * child_branch = (gai_DecisionTreeBranch_t *) branch_node;
        return gai_DecisionTreeBranch_Evaluate(child_branch, userdata);
    }else if(branch_node->type==GAI_NODE_TYPE_ACTION){
        return (gai_Action_t *) branch_node;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
////

int gai_DecisionTree_Init(gai_DecisionTree_t* tree, const char* name, void* userdata){
    gai_Node_Init(&tree->node, name, GAI_NODE_TYPE_DECISION_TREE, userdata);
    tree->branch = 0;
    tree->currentAction = 0;
    sdk_vector_init(&tree->branches, 0);
    return 0;
}

void gai_DecisionTree_DeInit(gai_DecisionTree_t * tree){
    sdk_vector_destroy(&tree->branches);
    tree->branch = 0;
    tree->currentAction = 0;
}

int gai_DecisionTree_AddBranch(gai_DecisionTree_t* tree, gai_DecisionTreeBranch_t * branch)
{
    sdk_vector_append(&tree->branches, branch);
    return 0;
}

gai_DecisionTreeBranch_t * gai_DecisionTree_FindBranch(gai_DecisionTree_t* tree, const char* branchName)
{
    sdk_size_t i;
    gai_DecisionTreeBranch_t * branch;
    for(i = 0; i<tree->branches.size; i++){
        sdk_vector_get(&tree->branches, i, (void **) &branch);
        if(strcmp(branchName, branch->node.name)==0){
            return branch;
        }
    }
}

void gai_DecisionTree_SetBranch(gai_DecisionTree_t* tree, gai_DecisionTreeBranch_t* branch)
{
    tree->branch = branch;
}

void gai_DecisionTree_Update(gai_DecisionTree_t* tree, void* userdata)
{
    if(!tree->branch){
        return;
    }
    
    if(!tree->currentAction){
        tree->currentAction = gai_DecisionTreeBranch_Evaluate(tree->branch, userdata);
        tree->currentAction->Initialize(tree->currentAction, userdata);
    }
    
    if(tree->currentAction){
        gai_ActionStatus status = tree->currentAction->Update(tree->currentAction, userdata);
        if(status==kGAI_ActionStatus_TERMINATED){
            tree->currentAction->Cleanup(tree->currentAction, userdata);
            tree->currentAction = 0;
        }
    }
}

