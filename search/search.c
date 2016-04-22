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
#include "dict.h"
#include "monte_node.h"
// #inclued "../bit_vector.h"
#include "../agent/agent.h"

// TODO: Verify if these actually matter or not
#define NODE_TYPE_CHANCE 0
#define NODE_TYPE_DECISION 1

// Constants used for the class attributes
// pyaixi: these are class attributes
#define MONTE_EXPLORATION_CONSTANT 2.0
#define MONTE_UNEXPLORED_BIAS 1000000000.0

// pyaixi: __init__
// notes: the node type should match from the above above node type constants.
// clients should call from one of those.
MonteNode* monte_create_tree(int nodetype) {
    MonteNode* root = (MonteNode* ) malloc(sizeof(MonteNode));

    // It's better to be explicit, as this is a direct port
    root->mean = 0.0;
    root->type = nodetype;
    root->visits = 0;

    // Be careful, this could cause leaks if we forget to deallocate later...
    root->children = dict_new();
    root->actions = dict_new();

    return root;
}

int _monte_select_action(MonteNode* tree, struct Agent* agent) {
    // returns -1 if no vaild action
  
    // todo: populate these with agent.horizon
    // and agent.maximum_reward
    float agent_horizon = 0;
    float agent_max_reward = Agent_maximum_reward(agent)/1;

    float explore_bias = agent_horizon * agent_max_reward;
    // 2.0f is hard-coded into the application
    float exploration_numerator = (float) (2.0f* log((double) tree->visits));

    printf("%f", exploration_numerator);

    // desu???
    int best_action = 0;

    // TODO: ??? profit
    // todo: implement the loop from lines #182
    // to lines 202; it's straight forward but

    // for each valid action...
        // do the update and selection procedure randomly
    // end for

    return best_action;
}


// pyaixi: sample(agent, horizon)
// notes: we require some more work here
// todo: resolve the agent pointer to some structure
float monte_sample(MonteNode* tree, struct Agent* agent, int horizon) {
    float reward = 0.0;

    if(horizon == 0) {
        // reached maximum search depth
        return reward;
    } else if(tree->type == NODE_TYPE_CHANCE) {
        // todo: require use to be able actually fill in these values
        int observation = 0;
        int random_reward = 0;

        // todo: implement this properly with an agent API

        // pyaixi: agent.generate_percept_and_update()
        // line: 124


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
        monte_sample(child, agent, horizon - 1);
    } else if(tree->visits == 0) {
        // TODO: Agent simulation will be required to play this out
        // pyaixi: agent.playout(horizon)
        // line  138
        // simulate
        reward = 5;
    }
    else {
        // should this be not an int???
        int action = _monte_select_action(tree, agent);
        // TODO: implement line #143 - agent.model_update_action(action)

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
