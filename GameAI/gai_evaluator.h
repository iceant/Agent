#ifndef INCLUDED_GAI_EVALUATOR_H
#define INCLUDED_GAI_EVALUATOR_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_GAI_NODE_H
#include <gai_node.h>
#endif /*INCLUDED_GAI_NODE_H*/

////////////////////////////////////////////////////////////////////////////////
////

typedef struct gai_Evaluator_s{
    gai_Node_t node;
    int (*Evaluate)(struct gai_Evaluator_s*, void * userdata);
}gai_Evaluator_t;

////////////////////////////////////////////////////////////////////////////////
////

int gai_Evaluator_Init(gai_Evaluator_t* evaluator, const char* name
                       , int (*Evaluate)(gai_Evaluator_t*, void * userdata), void* userdata);

int gai_Evaluator_Evaluate(gai_Evaluator_t* evaluator, void* userdata);


#endif /* INCLUDED_GAI_EVALUATOR_H */
