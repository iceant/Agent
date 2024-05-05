#ifndef INCLUDED_GAI_NODE_H
#define INCLUDED_GAI_NODE_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_SDK_ERRORS_H
#include <sdk_errors.h>
#endif /*INCLUDED_SDK_ERRORS_H*/

////////////////////////////////////////////////////////////////////////////////
////
#define GAI_NAME_SIZE 32

#define GAI_NODE_TYPE_ACTION                1
#define GAI_NODE_TYPE_EVALUATOR             2
#define GAI_NODE_TYPE_DECISION_BRANCH       3
#define GAI_NODE_TYPE_DECISION_TREE         4

////////////////////////////////////////////////////////////////////////////////
////

typedef struct gai_Node_s{
    char name[GAI_NAME_SIZE];
    int  type;
    void* userdata;
}gai_Node_t;

////////////////////////////////////////////////////////////////////////////////
////

int gai_Node_Init(gai_Node_t* node, const char* name, int type, void* userdata);


#endif /* INCLUDED_GAI_NODE_H */
