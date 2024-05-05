#include <gai_behaviortree.h>
#include <stdio.h>
#include <sdk_memory.h>
#include <stdlib.h>
#include <time.h>
////////////////////////////////////////////////////////////////////////////////
////
typedef struct Evaluator_Context{
    int min;
    int max;
}Evaluator_Context;

int Evaluators_IsNotAlive(gai_Evaluator_t* evaluator, void* userdata){
    Evaluator_Context* context = evaluator->node.userdata;
    int value = context->min + (rand() % context->max);
    printf("Evaluator[%s] Execute, return: %d\r\n", evaluator->node.name, value-1);
    return value-1;
}

int Evaluators_Random(gai_Evaluator_t* evaluator, void* userdata){
    Evaluator_Context* context = evaluator->node.userdata;
    int value = context->min + (rand() % context->max);
    printf("Evaluator[%s] Execute, return: %d\r\n", evaluator->node.name, value-1);
    return value-1;
}

static void Action__Initialize(gai_Action_t* action, void* userdata)
{
    printf("Action[%s] Initialize\r\n", action->node.name);
}

static gai_ActionStatus Action__Update(gai_Action_t* action, void* userdata)
{
    printf("Action[%s] Update\r\n", action->node.name);
    return kGAI_ActionStatus_TERMINATED;
}

static void Action__CleanUp(gai_Action_t* action, void* userdata)
{
    printf("Action[%s] CleanUp\r\n", action->node.name);
}


////////////////////////////////////////////////////////////////////////////////
////


int main(int argc, char** argv) {
    srand(time(0));
    sdk_memory_init();
    
    Evaluator_Context context = {.min = 1, .max = 2};
    gai_BehaviorTree_t behaviorTree;
    
    gai_BehaviorTree_Init(&behaviorTree, "SoldierBehaviorTree", 0);
    
    gai_BehaviorTreeNode_t root;
    gai_BehaviorTree_Selector_Init(&root, "root", 0);
    /* ---------------------------------------------------------------------------------------------------- */
    gai_BehaviorTreeNode_t sequence1;
    gai_BehaviorTree_Sequence_Init(&sequence1, "sequence1", 0);
    
    gai_Evaluator_t isNotAliveEvaluator;
    gai_Evaluator_Init(&isNotAliveEvaluator, "isNotAliveEvaluator", Evaluators_IsNotAlive,  &context);
    gai_BehaviorTreeNode_t isNotAliveCondition;
    gai_BehaviorTree_Condition_Init(&isNotAliveCondition, "isNotAliveCondition", &isNotAliveEvaluator, 0);
    
    gai_Action_t dieAction;
    gai_Action_Init(&dieAction, "dieAction", Action__Initialize, Action__Update, Action__CleanUp, 0);
    gai_BehaviorTreeNode_t dieActionNode;
    gai_BehaviorTree_Action_Init(&dieActionNode, "dieActionNode", &dieAction, 0);
    
    
    gai_BehaviorTreeNode_AddChild(&sequence1, &isNotAliveCondition);
    gai_BehaviorTreeNode_AddChild(&sequence1, &dieActionNode);
    gai_BehaviorTreeNode_AddChild(&root, &sequence1);
    
    /* ---------------------------------------------------------------------------------------------------- */
    gai_BehaviorTreeNode_t sequence2;
    gai_BehaviorTree_Sequence_Init(&sequence2, "sequence2", 0);
    
    gai_Evaluator_t hasCriticalHealthEvaluator;
    gai_Evaluator_Init(&hasCriticalHealthEvaluator, "hasCriticalHealthEvaluator", Evaluators_Random,  &context);
    gai_BehaviorTreeNode_t hasCriticalHealthCondition;
    gai_BehaviorTree_Condition_Init(&hasCriticalHealthCondition, "hasCriticalHealthCondition", &hasCriticalHealthEvaluator, 0);
    
    gai_Action_t fleeAction;
    gai_Action_Init(&fleeAction, "fleeAction", Action__Initialize, Action__Update, Action__CleanUp, 0);
    gai_BehaviorTreeNode_t fleeActionNode;
    gai_BehaviorTree_Action_Init(&fleeActionNode, "fleeActionNode", &fleeAction, 0);
    
    
    gai_BehaviorTreeNode_AddChild(&sequence2, &hasCriticalHealthCondition);
    gai_BehaviorTreeNode_AddChild(&sequence2, &fleeActionNode);
    gai_BehaviorTreeNode_AddChild(&root, &sequence2);
    
    
    /* ---------------------------------------------------------------------------------------------------- */
    gai_Action_t idleAction;
    gai_Action_Init(&idleAction, "idleAction", Action__Initialize, Action__Update, Action__CleanUp, 0);
    gai_BehaviorTreeNode_t idleActionNode;
    gai_BehaviorTree_Action_Init(&idleActionNode, "idleActionNode", &idleAction, 0);
    
    gai_BehaviorTreeNode_AddChild(&root, &idleActionNode);
    
    /* ---------------------------------------------------------------------------------------------------- */
    gai_BehaviorTree_SetRoot(&behaviorTree, &root);
    
    /* ---------------------------------------------------------------------------------------------------- */
    while(1){
        gai_BehaviorTree_Update(&behaviorTree, 0);
        for(int i=0; i<0x1fffffff; i++);
    }
    
    sdk_memory_destroy();
    return 0;
}