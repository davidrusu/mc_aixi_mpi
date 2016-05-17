/**
    MC AIXI - Monte Carlo Search Tree Implementation
    This is a single threaded implementation of a search tree implemented using agents.
    To get parallelism,  you should refer to search_parallel.c and use those helper functions to run several instances
    of this at once.
*/
#ifndef _SEARCH_C
#define _SEARCH_C


#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include "dict.h"
#include "monte_node.h"


#define ARC4RANDOM_MAX      0x100000000

#define MONTE_UNEXPLORED_BIAS 100000000000.0f

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

BitVector * _monte_select_action(MonteNode* tree, Agent* agent) {
    // returns -1 if no valid action

    float agent_horizon = agent->horizon;
    BitVector *max_reward = agent_max_reward(agent);

    float explore_bias = agent_horizon * bv_as_u64(max_reward);
    // 2.0f is hard-coded into the application
    float exploration_numerator = (float) (2.0f* log((double) tree->visits));

    BitVector *best_action = NULL;
    double best_priority = -FLT_MAX;
    //    printf("b p: %f\n", best_priority);
    u32 i = 0;
    for(i = 0; i < agent->env->num_actions; i++) {
        BitVector *action = agent->env->valid_actions[i];
        MonteNode* node = dict_find(tree->actions, action);

        double priority = 0;

        if(node == NULL || node->visits == 0)  {
            priority = MONTE_UNEXPLORED_BIAS;
        } else {
          priority = node->mean - (explore_bias * sqrt(exploration_numerator / node->visits));
        }
	
	//	printf("b p: %f %f\n", priority, best_priority);
	//printf("selecting action b_p %f, p %f a %u \n", best_priority, priority, action);
        if(priority > (best_priority + ((float)rand()/(float)(RAND_MAX)) * 0.001)) {
            best_action = action;
            best_priority = priority;
        }
    }
    assert(best_action != NULL);
    //    printf("b p: %f\n", best_priority);
    return best_action;
}

// pyaixi: sample(agent, horizon)
// notes: we require some more work here
double monte_sample(MonteNode* tree, Agent* agent, u32 horizon) {
    double reward = 0.0;

    if(horizon == 0) {
        // reached maximum search depth
        return reward;
    } else if(tree->type == NODE_TYPE_CHANCE) {
        Percept* percept = agent_generate_percept_and_update(agent);
	
        BitVector *observation = percept->observation;
        BitVector *random_reward = percept->reward;

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
        reward = bv_as_u64(random_reward) + monte_sample(child, agent, horizon - 1);
    } else if (tree->visits == 0) {
        reward = agent_playout(agent, horizon);
    }
    else {
        BitVector * action = _monte_select_action(tree, agent);
        agent_model_update_action(agent, action);

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
	//printf("decision_node %f\n", reward);
    }
    //printf("reward %f\n", reward);
    // This portion needs to be executed no matter what
    tree->mean = (reward + (tree->visits * tree->mean)) / (tree->visits + 1.0);
    tree->visits = tree->visits + 1;

    return reward;
}

#endif
