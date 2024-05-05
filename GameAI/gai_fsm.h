#ifndef INCLUDED_GAI_FSM_H
#define INCLUDED_GAI_FSM_H

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

#define GAI_NODE_TYPE_FSM_MACHINE       0x201
#define GAI_NODE_TYPE_FSM_STATE         0x202
#define GAI_NODE_TYPE_FSM_TRANSITION    0x203

////////////////////////////////////////////////////////////////////////////////
////
typedef struct gai_FSM_Transition_s{
    gai_Node_t node;
    gai_Evaluator_t * evaluator;
    char fromStateName[GAI_NAME_SIZE];
    char toStateName[GAI_NAME_SIZE];
}gai_FSM_Transition_t;

typedef struct gai_FSM_State_s{
    gai_Node_t node;
    gai_Action_t * action;
    sdk_vector_t transitions;
}gai_FSM_State_t;

typedef struct gai_FSM_Machine_s{
    gai_Node_t node;
    gai_FSM_State_t * currentState;
    sdk_vector_t states;
    sdk_vector_t transitions;
}gai_FSM_Machine_t;

////////////////////////////////////////////////////////////////////////////////
////

int gai_FSM_State_Init(gai_FSM_State_t * state, const char* name, gai_Action_t* action, void* userdata);

////////////////////////////////////////////////////////////////////////////////
////

int gai_FSM_Transition_Init(gai_FSM_Transition_t* transition, const char* name, void* userdata,
                            const char* fromStateName, const char* toStateName, gai_Evaluator_t* evaluator);

////////////////////////////////////////////////////////////////////////////////
////

int gai_FSM_Machine_Init(gai_FSM_Machine_t* machine, const char* name, void* userdata);

void gai_FSM_Machine_DeInit(gai_FSM_Machine_t * machine);

gai_FSM_State_t* gai_FSM_Machine_FindState(gai_FSM_Machine_t * machine, const char* stateName);

gai_FSM_Transition_t * gai_FSM_Machine_FindTransition(gai_FSM_Machine_t* machine
        , const char* fromStateName, const char* toStateName);

void gai_FSM_Machine_SetState(gai_FSM_Machine_t * machine, gai_FSM_State_t * state, void* userdata);

int gai_FSM_Machine_AddState(gai_FSM_Machine_t * machine, gai_FSM_State_t * state);

int gai_FSM_Machine_AddTransition(gai_FSM_Machine_t * machine, gai_FSM_Transition_t* transition);

void gai_FSM_Machine_Update(gai_FSM_Machine_t* machine, void* userdata);

#endif /* INCLUDED_GAI_FSM_H */
