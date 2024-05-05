#include <gai_fsm.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sdk_memory.h>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////
////
static void Action__Initialize(gai_Action_t * action, void* userdata)
{
    printf("Action[%s] Initialize\r\n", action->node.name);
}
static gai_ActionStatus Action__Update(gai_Action_t * action, void* userdata)
{
    printf("Action[%s] Update\r\n", action->node.name);
    return kGAI_ActionStatus_TERMINATED;
}
static void Action__Cleanup(gai_Action_t * action, void* userdata)
{
    printf("Action[%s] Cleanup\r\n", action->node.name);
}

typedef struct Evaluator_Context{
    int min;
    int max;
}Evaluator_Context;

static int Evaluator__Function(gai_Evaluator_t* evaluator, void* userdata){
    Evaluator_Context* context = evaluator->node.userdata;
    int value  = context->min + (rand() % context->max);
    return value-1;
}
////////////////////////////////////////////////////////////////////////////////
////

int main(int argc, char** argv)
{
    srand(time(0));
    sdk_memory_init();
    Evaluator_Context context={.min=1, .max=2};
    gai_FSM_Machine_t machine;
    gai_FSM_Machine_Init(&machine, "Test", 0);
    
    gai_FSM_State_t idleState;
    gai_FSM_State_t dieState;
    gai_Action_t idleAction;
    gai_Action_t dieAction;
    gai_Evaluator_t isNotAliveEvaluator;
    
    gai_Evaluator_Init(&isNotAliveEvaluator, "isNotAliveEvaluator", Evaluator__Function, &context);
    
    gai_Action_Init(&idleAction, "idleAction", Action__Initialize, Action__Update, Action__Cleanup, 0);
    gai_Action_Init(&dieAction, "dieAction", Action__Initialize, Action__Update, Action__Cleanup, 0);
    
    gai_FSM_State_Init(&idleState, "idleState", &idleAction, 0);
    gai_FSM_State_Init(&dieState, "dieState", &dieAction, 0);
    
    gai_FSM_Transition_t isNotAliveTransition;
    gai_FSM_Transition_Init(&isNotAliveTransition, "isNotAliveTransition", 0, "idleState", "dieState", &isNotAliveEvaluator);
    
    gai_FSM_Machine_AddState(&machine, &idleState);
    gai_FSM_Machine_AddState(&machine, &dieState);
    gai_FSM_Machine_AddTransition(&machine, &isNotAliveTransition);
    
    gai_FSM_Machine_SetState(&machine, &idleState, 0);
    
    while(1){
        gai_FSM_Machine_Update(&machine, 0);
        if(strcmp(dieState.node.name, machine.currentState->node.name)==0 && machine.currentState->action->status==kGAI_ActionStatus_UNINITIALIZED){
            break;
        }
    }
    gai_FSM_Machine_DeInit(&machine);
    sdk_memory_destroy();
    return 0;
}