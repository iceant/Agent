#include <gai_action.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
////

int gai_Action_Init(gai_Action_t* action, const char* name
        , void (*Initialize)(struct gai_Action_s*, void* userdata)
        , gai_ActionStatus (*Update)(struct gai_Action_s*, void* userdata)
        , void (*Cleanup)(struct gai_Action_s*, void* userdata)
        , void* userdata)
{
    gai_Node_Init(&action->node, name, GAI_NODE_TYPE_ACTION, userdata);
    
    action->Initialize = Initialize;
    action->Update = Update;
    action->Cleanup = Cleanup;
    
    action->status = kGAI_ActionStatus_UNINITIALIZED;
    
    return 0;
}

void gai_Action_Initialize(gai_Action_t* action, void* userdata)
{
    if(action->status==kGAI_ActionStatus_UNINITIALIZED){
        if(action->Initialize){
            action->Initialize(action, userdata);
        }
    }
    action->status = kGAI_ActionStatus_RUNNING;
}

gai_ActionStatus gai_Action_Update(gai_Action_t* action, void* userdata)
{
    if(action->status==kGAI_ActionStatus_TERMINATED){
        return kGAI_ActionStatus_TERMINATED;
    }else if(action->status==kGAI_ActionStatus_RUNNING){
        if(action->Update){
            action->status = action->Update(action, userdata);
        }else{
            action->status = kGAI_ActionStatus_TERMINATED;
        }
    }
    return action->status;
}

void gai_Action_Cleanup(gai_Action_t* action, void* userdata)
{
    if(action->status==kGAI_ActionStatus_TERMINATED){
        if(action->Cleanup){
            action->Cleanup(action, userdata);
        }
    }
    action->status = kGAI_ActionStatus_UNINITIALIZED;
}


