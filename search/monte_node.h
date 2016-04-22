#ifndef _MONTE_NODE_H_
#define _MONTE_NODE_H_

#define NODE_TYPE_CHANCE 0
#define NODE_TYPE_DECISION 1

#include "dict.h"
#include "../agent/agent.h"
#include "../_utils/types.h"

// pyaixi: class members
typedef struct MonteNode {
    double mean ;
    int type;
    int visits;

    // a dictionary of the children to be held, initialized by search.c
    dict_t children;
    dict_t actions;
} MonteNode;

MonteNode* monte_create_tree(u32 nodeType);
u32 _monte_select_action(MonteNode* tree, Agent* agent);
float monte_sample(MonteNode* tree, Agent* agent, u32 horizon);


#endif
