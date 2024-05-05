#ifndef INCLUDED_GAI_ACTION_H
#define INCLUDED_GAI_ACTION_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_GAI_NODE_H
#include <gai_node.h>
#endif /*INCLUDED_GAI_NODE_H*/

////////////////////////////////////////////////////////////////////////////////
////

typedef enum gai_ActionStatus{
    kGAI_ActionStatus_UNINITIALIZED=0,
    kGAI_ActionStatus_RUNNING,
    kGAI_ActionStatus_TERMINATED,
}gai_ActionStatus;

typedef struct gai_Action_s{
    gai_Node_t node;
    gai_ActionStatus status;
    void (*Initialize)(struct gai_Action_s*, void* userdata);
    gai_ActionStatus (*Update)(struct gai_Action_s*, void* userdata);
    void (*Cleanup)(struct gai_Action_s*, void* userdata);
}gai_Action_t;

////////////////////////////////////////////////////////////////////////////////
////

int gai_Action_Init(gai_Action_t* action, const char* name
                    , void (*Initialize)(struct gai_Action_s*, void* userdata)
                    , gai_ActionStatus (*Update)(struct gai_Action_s*, void* userdata)
                    , void (*Cleanup)(struct gai_Action_s*, void* userdata)
                    , void* userdata);

void gai_Action_Initialize(gai_Action_t* action, void* userdata);

gai_ActionStatus gai_Action_Update(gai_Action_t* action, void* userdata);

void gai_Action_Cleanup(gai_Action_t* action, void* userdata);

#endif /* INCLUDED_GAI_ACTION_H */
