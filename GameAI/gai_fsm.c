#include <gai_fsm.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
////

int gai_FSM_Machine_Init(gai_FSM_Machine_t* machine, const char* name, void* userdata)
{
    gai_Node_Init(&machine->node, name, GAI_NODE_TYPE_FSM_MACHINE, userdata);
    machine->currentState = 0;
    sdk_vector_init(&machine->states, 0);
    sdk_vector_init(&machine->transitions, 0);
    
    return 0;
}

void gai_FSM_Machine_DeInit(gai_FSM_Machine_t * machine){
    sdk_vector_destroy(&machine->states);
    sdk_vector_destroy(&machine->transitions);
    machine->currentState = 0;
}

gai_FSM_State_t* gai_FSM_Machine_FindState(gai_FSM_Machine_t * machine, const char* stateName)
{
    sdk_size_t  i;
    gai_FSM_State_t * state=0;
    
    if(!stateName){
        return 0;
    }
    
    for(i=0; i<machine->states.size; i++){
        sdk_vector_get(&machine->states, i, (void **) &state);
        if(strcmp(state->node.name, stateName)==0){
            return state;
        }
    }
    return 0;
}

gai_FSM_Transition_t * gai_FSM_Machine_FindTransition(gai_FSM_Machine_t* machine
                                                      , const char* fromStateName, const char* toStateName)
{
    sdk_size_t i;
    gai_FSM_Transition_t * transition;
    for(i=0; i<machine->transitions.size; i++){
        sdk_vector_get(&machine->transitions, i, (void **) &transition);
        if(strcmp(transition->fromStateName, fromStateName)==0 && strcmp(transition->toStateName, toStateName)==0){
            return transition;
        }
    }
    return 0;
}

int gai_FSM_Machine_AddState(gai_FSM_Machine_t * machine, gai_FSM_State_t * state)
{
    sdk_vector_resize_append(&machine->states, state);
    return 0;
}

int gai_FSM_Machine_AddTransition(gai_FSM_Machine_t * machine, gai_FSM_Transition_t* transition)
{
    sdk_vector_resize_append(&machine->transitions, transition);
    gai_FSM_State_t * state = gai_FSM_Machine_FindState(machine, transition->fromStateName);
    if(state){
        sdk_vector_resize_append(&state->transitions, transition);
    }
    
    return 0;
}

void gai_FSM_Machine_SetState(gai_FSM_Machine_t * machine, gai_FSM_State_t * state, void* userdata)
{
    if(machine->currentState){
        gai_Action_Cleanup(machine->currentState->action, userdata);
//        machine->currentState->action->Cleanup(machine->currentState->action, userdata);
//        machine->currentState->action->status = kGAI_ActionStatus_TERMINATED;
    }
    machine->currentState = state;
//    machine->currentState->action->Initialize(machine->currentState->action, userdata);
    gai_Action_Initialize(machine->currentState->action, userdata);
}

static const char* gai_FSM_Machine_EvaluateTransitions(sdk_vector_t * transitions, void* userdata){
    sdk_size_t i;
    gai_FSM_Transition_t * transition;
    for(i=0; i<transitions->size; i++){
        sdk_vector_get(transitions, i, (void **) &transition);
        if(transition->evaluator->Evaluate(transition->evaluator, userdata)){
            return transition->toStateName;
        }
    }
    return 0;
}

void gai_FSM_Machine_Update(gai_FSM_Machine_t* machine, void* userdata)
{
    if(machine->currentState){
        gai_ActionStatus status = gai_Action_Status(machine->currentState->action);
        if(status==kGAI_ActionStatus_RUNNING){
            gai_Action_Update(machine->currentState->action, userdata);
        }else if(status==kGAI_ActionStatus_TERMINATED){
            const char* toStateName = gai_FSM_Machine_EvaluateTransitions(&machine->currentState->transitions, userdata);
            gai_Action_Cleanup(machine->currentState->action, userdata);
            if(toStateName){
                machine->currentState = gai_FSM_Machine_FindState(machine, toStateName);
                gai_Action_Initialize(machine->currentState->action, userdata);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
////

int gai_FSM_State_Init(gai_FSM_State_t * state, const char* name, gai_Action_t* action, void* userdata)
{
    gai_Node_Init(&state->node, name, GAI_NODE_TYPE_FSM_STATE, userdata);
    state->action = action;
    sdk_vector_init(&state->transitions, 0);
    return 0;
}

void gai_FSM_State_DeInit(gai_FSM_State_t* state){
    sdk_vector_destroy(&state->transitions);
}
////////////////////////////////////////////////////////////////////////////////
////

int gai_FSM_Transition_Init(gai_FSM_Transition_t* transition, const char* name, void* userdata,
                            const char* fromStateName, const char* toStateName, gai_Evaluator_t* evaluator)
{
    gai_Node_Init(&transition->node, name, GAI_NODE_TYPE_FSM_TRANSITION, userdata);
    strcpy(transition->fromStateName, fromStateName);
    strcpy(transition->toStateName, toStateName);
    transition->evaluator = evaluator;
    
    return 0;
}

