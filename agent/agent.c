////////////////////////////////////////////////////////////////////
// AUTHOR:   Dan Morouney <069001422>
// FILE:     agent.c
// CREATED:  2016-04-20 14:42:09
// MODIFIED: 2016-04-20 22:54:05
////////////////////////////////////////////////////////////////////

#include <stddef.h>
#include <stdlib.h>
#include "../_utils/types.h"
#include "../predict/context_tree.h"
#include "agent.h"
#include "../search/monte_node.h"
#include <assert.h>

Agent *agent_create(Env *env, u32 learning_period) {
    Agent *self = (Agent *) malloc(sizeof(Agent));
    self->env = env;
    self->age = 0;
    self->learning_period = learning_period;
    self->last_update = action_update;
    self->total_reward = 0;
    self->horizon = 6;
    u32 depth = 32;

    self->context_tree = ctw_create(depth);

    agent_reset(self);
    return self;
}

void Agent_delete(void *_self) {
    //TODO: ...
}

AgentUndo *agent_clone_into_temp(Agent *self) {
    AgentUndo *undo = (AgentUndo *) malloc(sizeof(AgentUndo));
    undo->age = self->age;
    undo->total_reward = self->total_reward;
    undo->history_size = agent_history_size(self);
    undo->last_update = self->last_update;
    return undo;
}

Percept *agent_decode_percept(Agent *self, BitVector *symbols) {
    BitVector *reward = bv_slice(symbols, 0, reward_bits(self->env));
    BitVector *observation =
            bv_slice(symbols, reward_bits(self->env), reward_bits(self->env) + observation_bits(self->env));

    Percept *percept = percept_create(observation, reward);
    if (!is_valid_percept(self->env, percept)) {
        percept = NULL;
        //TODO: leaking percept >:(
    }
    return percept;
}

BitVector *agent_encode_percept(Agent *self, Percept *percept) {
    BitVector * result = bv_clone(percept->reward);
    bv_append(result, percept->observation);
    return result;
}

BitVector *agent_generate_action(Agent *self) {
    assert(self->last_update == percept_update);
    return ctw_gen_random_symbols(self->context_tree, action_bits(self->env));
}

Percept *agent_generate_percept(Agent *self) {
    BitVector *random = ctw_gen_random_symbols_and_update(self->context_tree, 64);

    Percept *percept = agent_decode_percept(self, random);
    if (percept == NULL) {
        percept = env_generate_random_percept(self->env);
        ctw_revert(self->context_tree, percept_bits(self->env));
        BitVector *symbols = agent_encode_percept(self, percept);
        ctw_update_vector(self->context_tree, symbols);
        bv_free(symbols);
    }
    return percept;
}

Percept *agent_generate_percept_and_update(Agent *self) {
    Percept *percept = agent_generate_percept(self);
    self->total_reward += bv_as_u64(percept->reward);
    self->last_update = percept_update;
    return percept;
}

double agent_get_predicted_action_probability(Agent *self, BitVector * action) {
    return ctw_predict_vector(self->context_tree, action);
}

u64 agent_history_size(Agent *self) {
    return self->context_tree->history->size;
}

u32 agent_maximum_bits_needed(Agent *self) {
    return percept_bits(self->env);
}

void agent_model_revert(Agent *self, AgentUndo *undo) {
    while (agent_history_size(self) > undo->history_size) {
        if (self->last_update == percept_update) {
            ctw_revert(self->context_tree, percept_bits(self->env));
            self->last_update = action_update;
        } else {
            ctw_revert_history(self->context_tree, action_bits(self->env));
            self->last_update = percept_update;
        }
    }

    if (agent_history_size(self) != undo->history_size) {
        printf("hist size should be equal %llu %llu\n", agent_history_size(self), undo->history_size);
        exit(1034109);
    }

    self->age = undo->age;
    self->total_reward = undo->total_reward;
    self->last_update = undo->last_update;
}

u64 agent_model_size(Agent *self) {
    return ctw_size(self->context_tree);
}

void agent_model_update_action(Agent *self, BitVector * action) {
    assert(is_valid_action(self->env, action));
    ctw_update_history(self->context_tree, action);
    self->age++;
    self->last_update = action_update;
}

void agent_model_update_percept(Agent *self, Percept *percept) {
    BitVector *symbols = agent_encode_percept(self, percept);

    // Are we still learning?
    if ((self->learning_period > 0) && (self->age > self->learning_period)) {
        printf("not learning any more\n");
        ctw_update_history(self->context_tree, symbols);
    } else {
        ctw_update_vector(self->context_tree, symbols);
    }

    self->total_reward += bv_as_u64(percept->reward);
    self->last_update = percept_update;
}

double agent_percept_probability(Agent *self, Percept *percept) {
    BitVector *symbols = agent_encode_percept(self, percept);
    double result = ctw_predict_vector(self->context_tree, symbols);
    bv_free(symbols);
    return result;
}

double agent_playout(Agent *self, u32 horizon) {
    double total_reward = 0;

    for (u32 i = 0; i < horizon; i++) {
        BitVector * action = agent_generate_random_action(self);
        agent_model_update_action(self, action);

        Percept *percept = agent_generate_percept_and_update(self);
        total_reward += bv_as_u64(percept->reward);
    }
    return total_reward;
}

void agent_reset(Agent *self) {
    ctw_clear(self->context_tree);

    self->age = 0;
    self->total_reward = 0;
    self->last_update = action_update;
}

BitVector * agent_search_mean(Agent *self, double *mean) {
    AgentUndo *undo = agent_clone_into_temp(self);

    MonteNode *node = monte_create_tree(NODE_TYPE_DECISION);

    // 300 sims
    for (u32 i = 0; i < 10; i++) {
        monte_sample(node, self, self->horizon);
        agent_model_revert(self, undo);
    }


    //printf("Enter action: ");
    //char pattern[100];

    //fgets(pattern, sizeof(pattern), stdin);

    BitVector *best_action = agent_generate_random_action(self);
    //u32 best_action = (u32) atoi(pattern);

    double best_mean = -1;
    for (u32 i = 0; i < self->env->num_actions; i++) {
        BitVector * action = self->env->valid_actions[i];
        MonteNode *search_node = dict_find(node->actions, action);

        if (search_node != NULL) {
            double node_mean = search_node->mean;
            if (node_mean > best_mean) {
                best_mean = node_mean;
                best_action = action;
                printf("best_mean %f %s \n", best_mean, bv_str(best_action));
                //TODO: leaking best_action bit vector
            }
        }
    }

    *mean = best_mean;
    return best_action;
}

BitVector * agent_search(Agent *self) {
    double mean;
    return agent_search_mean(self, &mean);
}

double agent_average_reward(Agent *self) {
    double average = 0.0;
    if (self->age > 0)
        average = ((double) self->total_reward) / (self->age);
    return average;
}

BitVector * agent_generate_random_action(Agent *self) {
    int actionIndex = rand() % self->env->num_actions;
    return self->env->valid_actions[actionIndex];
}

BitVector * agent_max_reward(Agent *self) {
    return env_max_reward(self->env);
}