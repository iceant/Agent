#include <gai_evaluator.h>

int gai_Evaluator_Init(gai_Evaluator_t* evaluator, const char* name
        , int (*Evaluate)(gai_Evaluator_t*, void * userdata)
        , void* userdata)
{
    gai_Node_Init(&evaluator->node, name, GAI_NODE_TYPE_EVALUATOR, userdata);
    
    evaluator->Evaluate = Evaluate;
    
    return 0;
    
}

int gai_Evaluator_Evaluate(gai_Evaluator_t* evaluator, void* userdata)
{
    return evaluator->Evaluate(evaluator, userdata);
}



