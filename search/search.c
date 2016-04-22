/**
    MC AIXI - Monte Carlo Search Tree Implemenation
    This is a single threaded implementation of a search tree implemented using agents.
    To get parallelism,  you should refer to search_parallel.c and use those helper functions to run several instances
    of this at once.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "dict.h"
#include "monte_node.h"
#include "../agent/agent.h"
#include "../environment/environment.r"
#include "../_utils/types.h"

#define ARC4RANDOM_MAX      0x100000000

#define MONTE_UNEXPLORED_BIAS 1000000000.0f

// pyaixi: __init__
MonteNode* monte_create_tree(u32 nodeType) {
    MonteNode* root = (MonteNode* ) malloc(sizeof(MonteNode));

    // It's better to be explicit, as this is a direct port
    root->mean = 0.0;
    root->type = nodeType;
    root->visits = 0;

    // Be careful, this could cause leaks if we forget to deallocate later...
    root->children = dict_new();
    root->actions = dict_new();

    return root;
}

int _monte_select_action(MonteNode* tree, struct Agent* agent) {
    // returns -1 if no vaild action

    float agent_horizon = agent->horizon;
    float agent_max_reward = Agent_maximum_reward(agent)/1;

    float explore_bias = agent_horizon * agent_max_reward;
    // 2.0f is hard-coded into the application
    float exploration_numerator = (float) (2.0f* log((double) tree->visits));

    printf("%f", exploration_numerator);

    // desu??? Mondaiji-tachi ga Isekai kara Kuru Sou Desu yo?
    int best_action = 0;
    double best_priority = FLT_MAX;

    u32 i = 0;
    for(i = 0; i < agent->environment->num_actions; i++) {
        u32 action = agent->environment->_valid_actions[i];
        MonteNode* node = dict_find(tree->actions, action);

        double priority = 0;

        if(node == NULL  || node->visits == 0)  {
            priority = MONTE_UNEXPLORED_BIAS;
        } else {
          priority = node->mean + (explore_bias * sqrt(exploration_numerator / node->visits));
        }

        if(priority > (best_priority + ((float)rand()/(float)(RAND_MAX/1) * 0.001))) {
            best_action = action;
            best_priority = priority;
        }

    }
    return best_action;
}


// pyaixi: sample(agent, horizon)
// notes: we require some more work here
float monte_sample(MonteNode* tree, struct Agent* agent, u32 horizon) {
    double reward = 0.0;

    if(horizon == 0) {
        // reached maximum search depth
        return reward;
    } else if(tree->type == NODE_TYPE_CHANCE) {
        u32Tuple* tuple = Agent_generate_percept_and_update(agent);

        u32 observation = tuple->first;
        u32 random_reward = tuple->second;

        bool notInTreeYet = dict_find(tree->children, observation) == NULL;

        if(notInTreeYet) {
            MonteNode* newChild = monte_create_tree(NODE_TYPE_DECISION);
            dict_add(tree->children, observation, newChild);
        }

        // Grab a monte node that is a child of.
        MonteNode* child = dict_find(tree->children, observation);

        // NOTE: This is not in PyAIXI; this is a sanity checker
        if(child == NULL) {
            printf("wtf?? child was not found.. abort!\n");
            exit(1337);
        }

        // Recurse
        reward = random_reward + monte_sample(child, agent, horizon - 1);
    } else if(tree->visits == 0) {
        reward = Agent_playout(agent, horizon);
    }
    else {
        u32 action = _monte_select_action(tree, agent);
        Agent_model_update_action(agent, action);

        MonteNode* child = dict_find(tree->actions, action);
        if(child == NULL) {
            child = monte_create_tree(NODE_TYPE_CHANCE);
    	    dict_add(tree->actions, action, child);
        }

        if(child == NULL) {
            printf("wtf??? child was not found.. abort!\n");
            exit(1338);
        }

        // pyaixi note: this should be just horizon, not horizon -1... or maybe it  should be -1
        // the authors do not really know
        reward = monte_sample(child, agent, horizon);
    }

    // This portion needs to be executed no matter what
    tree->mean = (reward + (tree->visits * tree->mean)) / (tree->visits + 1.0);
    tree->visits = tree->visits + 1;

    return reward;
}
