#ifndef INCLUDED_GAI_DECISIONTREE_H
#define INCLUDED_GAI_DECISIONTREE_H

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
#define GAI_NODE_TYPE_DECISION_BRANCH       0x101
#define GAI_NODE_TYPE_DECISION_TREE         0x102

typedef struct gai_DecisionTreeBranch_s{
    gai_Node_t node;
    sdk_vector_t children;
    gai_Evaluator_t* evaluator;
}gai_DecisionTreeBranch_t;

typedef struct gai_DecisionTree_s{
    gai_Node_t node;
    gai_DecisionTreeBranch_t* branch;
    gai_Action_t * currentAction;
}gai_DecisionTree_t;

////////////////////////////////////////////////////////////////////////////////
////

int gai_DecisionTreeBranch_Init(gai_DecisionTreeBranch_t* branch, const char* name
                                , gai_Evaluator_t *evaluator, void* userdata);

void gai_DecisionTreeBranch_DeInit(gai_DecisionTreeBranch_t* branch);

void gai_DecisionTreeBranch_AddChild(gai_DecisionTreeBranch_t* branch, gai_Node_t * child /* Branch or Action */);

void gai_DecisionTreeBranch_SetEvaluator(gai_DecisionTreeBranch_t* branch, gai_Evaluator_t * evaluator);

gai_Action_t * gai_DecisionTreeBranch_Evaluate(gai_DecisionTreeBranch_t* branch, void* userdata);

////////////////////////////////////////////////////////////////////////////////
////

int gai_DecisionTree_Init(gai_DecisionTree_t* tree, const char* name, void* userdata);

void gai_DecisionTree_DeInit(gai_DecisionTree_t * tree);

void gai_DecisionTree_SetBranch(gai_DecisionTree_t* tree, gai_DecisionTreeBranch_t* branch);

void gai_DecisionTree_Update(gai_DecisionTree_t* tree, void* userdata);


#endif /* INCLUDED_GAI_DECISIONTREE_H */
