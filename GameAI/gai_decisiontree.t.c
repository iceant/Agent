#include <gai_decisiontree.h>
#include <stdio.h>
#include <sdk_memory.h>

////////////////////////////////////////////////////////////////////////////////
////
static int Evaluator__isAlive(gai_Evaluator_t* self, void* userdata)
{
    return 1;
}

static void Action__Initialize(gai_Action_t* action, void* userdata){
    printf("Action[%s] Initialize\r\n", action->node.name);
}

static gai_ActionStatus Action__Update(gai_Action_t* action, void* userdata){
    printf("Action[%s] Update\r\n", action->node.name);
    if(action->status==kGAI_ActionStatus_UNINITIALIZED){
        action->status=kGAI_ActionStatus_RUNNING;
    }
    if(action->status==kGAI_ActionStatus_RUNNING){
        action->status = kGAI_ActionStatus_TERMINATED;
    }
//    if(action->status == kGAI_ActionStatus_TERMINATED){
//        action->status=kGAI_ActionStatus_UNINITIALIZED;
//    }
    return action->status;
}

static void Action__Cleanup(gai_Action_t* action, void* userdata){
    printf("Action[%s] Cleanup\r\n", action->node.name);
}
////////////////////////////////////////////////////////////////////////////////
////

int main(int argc, char** argv) {
    sdk_memory_init();
    
    gai_DecisionTree_t tree;
    gai_DecisionTree_Init(&tree, "SoldierDT", 0);
    
    gai_DecisionTreeBranch_t isAliveBranch;
    gai_DecisionTreeBranch_t criticalBranch;
    gai_DecisionTreeBranch_t moveFleeBranch;
    gai_DecisionTreeBranch_t enemyBranch;
    
    gai_Action_t dieAction;
    
    gai_Evaluator_t isAliveEvaluator;
    
    ////////////////////////////////////////////////////////////////////////////////
    ////
    
    gai_Action_Init(&dieAction, "dieAction", Action__Initialize/*Initialize*/, Action__Update/*Update*/, Action__Cleanup/*Cleanup*/, 0/*userdata*/);
    
    gai_Evaluator_Init(&isAliveEvaluator, "isAliveEvaluator", Evaluator__isAlive, 0);
    
    gai_DecisionTreeBranch_Init(&isAliveBranch, "isAliveBranch", &isAliveEvaluator, 0);
    
    gai_DecisionTreeBranch_AddChild(&isAliveBranch, (gai_Node_t *) &criticalBranch);
    gai_DecisionTreeBranch_AddChild(&isAliveBranch, (gai_Node_t *) &dieAction);
    
    gai_DecisionTree_SetBranch(&tree, &isAliveBranch);
    
    gai_DecisionTree_Update(&tree, 0);
    
    sdk_memory_destroy();
    return 0;
}
