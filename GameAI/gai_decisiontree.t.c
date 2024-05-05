#include <gai_decisiontree.h>
#include <stdio.h>
#include <sdk_memory.h>
#include <stdlib.h>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////
////
typedef struct RandomSelect_Evaluator_Context{
    int min;
    int max;
}RandomSelect_Evaluator_Context;

static int Evaluator__isAlive(gai_Evaluator_t* self, void* userdata)
{
    return 1;
}

static int Evaluator__RandomSelect(gai_Evaluator_t* self, void* userdata)
{
    RandomSelect_Evaluator_Context* context = self->node.userdata;
    
    int value =  (context->min + (rand() % context->max))-1;
    
    printf("Evaluator__RandomSelect: %d\r\n", value);
    
    return value;
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
    
    srand(time(NULL));
    
    sdk_memory_init();
    RandomSelect_Evaluator_Context randomSelectEvaluatorContext={.min =1, .max=2};
    
    gai_DecisionTree_t tree;
    gai_DecisionTree_Init(&tree, "SoldierDT", 0);
    
    gai_DecisionTreeBranch_t isAliveBranch;
    gai_DecisionTreeBranch_t criticalBranch;
    gai_DecisionTreeBranch_t moveFleeBranch;
    gai_DecisionTreeBranch_t enemyBranch;
    
    gai_Action_t dieAction;
    gai_Action_t moveToPositionAction;
    gai_Action_t fleeAction;
    
    gai_Evaluator_t isAliveEvaluator;
    gai_Evaluator_t hasCriticalHealthEvaluator;
    gai_Evaluator_t hasMovePositionEvaluator;
    gai_Evaluator_t hasEnemyEvaluator;
    
    ////////////////////////////////////////////////////////////////////////////////
    ////
    
    
    gai_Action_Init(&moveToPositionAction, "moveToPositionAction"
                    , Action__Initialize/*Initialize*/, Action__Update/*Update*/, Action__Cleanup/*Cleanup*/, 0/*userdata*/);
    
    gai_Action_Init(&fleeAction, "fleeAction"
            , Action__Initialize/*Initialize*/, Action__Update/*Update*/, Action__Cleanup/*Cleanup*/, 0/*userdata*/);
    
    gai_Action_Init(&dieAction, "dieAction", Action__Initialize/*Initialize*/, Action__Update/*Update*/, Action__Cleanup/*Cleanup*/, 0/*userdata*/);
    
    gai_Evaluator_Init(&isAliveEvaluator, "isAliveEvaluator", Evaluator__RandomSelect, &randomSelectEvaluatorContext);
    gai_Evaluator_Init(&hasCriticalHealthEvaluator, "hasCriticalHealthEvaluator", Evaluator__RandomSelect, &randomSelectEvaluatorContext);
    gai_Evaluator_Init(&hasMovePositionEvaluator, "hasMovePositionEvaluator", Evaluator__RandomSelect, &randomSelectEvaluatorContext);
    
//    gai_DecisionTreeBranch_Init(&criticalBranch, "criticalBranch", &hasCriticalHealthEvaluator, 0);
//    gai_DecisionTreeBranch_AddChild(&criticalBranch, (gai_Node_t *) &moveFleeBranch);
//    gai_DecisionTreeBranch_AddChild(&criticalBranch, (gai_Node_t *) &moveFleeBranch);
    
    gai_DecisionTreeBranch_Init(&moveFleeBranch, "moveFleeBranch", &hasMovePositionEvaluator, 0);
    gai_DecisionTreeBranch_AddChild(&moveFleeBranch, (gai_Node_t *) &moveToPositionAction);
    gai_DecisionTreeBranch_AddChild(&moveFleeBranch, (gai_Node_t *) &fleeAction);
    
    gai_DecisionTreeBranch_Init(&isAliveBranch, "isAliveBranch", &isAliveEvaluator, 0);
    gai_DecisionTreeBranch_AddChild(&isAliveBranch, (gai_Node_t *) &moveFleeBranch);
    gai_DecisionTreeBranch_AddChild(&isAliveBranch, (gai_Node_t *) &dieAction);
    
    gai_DecisionTree_AddBranch(&tree, &isAliveBranch);
    
    gai_DecisionTree_SetBranch(&tree, &isAliveBranch);
    
    gai_DecisionTree_Update(&tree, 0);
    
    gai_DecisionTree_DeInit(&tree);
    
    sdk_memory_destroy();
    return 0;
}
