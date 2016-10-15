#ifndef AGENT_H
#define AGENT_H
#include <stdarg.h>
#include "../environment/env.h"
#include "../_utils/types.h"
#include "../bit_vector.h"
#include "../predict/context_tree.h"

typedef struct Agent {
    Env * env;
    u64 total_reward;
    update_enum last_update;
    u64 age;
    u32 horizon;
    u32 learning_period;
    ContextTree * context_tree;
} Agent;

Agent* agent_create(Env *env, u32 learning_period);

AgentUndo* agent_clone_into_temp(Agent *self);

double agent_average_reward(Agent *self);

BitVector * agent_generate_random_action(Agent *self);
u64 agent_model_size(Agent* self);

void agent_model_update_action(Agent *self, BitVector *action);

Percept * agent_decode_percept(Agent* self, BitVector* symbols);

// generators
BitVector * agent_generate_action(Agent* self);
Percept * agent_generate_percept(Agent* self);
Percept * agent_generate_percept_and_update(Agent* self);

u64 agent_history_size(Agent* self);
double agent_get_predicted_action_probability(Agent* self , BitVector *action);
u32 agent_maximum_bits_needed(Agent* self);

void agent_model_revert(Agent* self , AgentUndo* undo);
void agent_model_update_percept(Agent* self, Percept *percept);

BitVector * agent_encode_percept(Agent* self, Percept *percept);

double agent_percept_probability(Agent* self, Percept *percept);
double agent_playout(Agent* self, u32 horizon);

BitVector * agent_search(Agent* self);
BitVector * agent_search_mean(Agent* self, double *mean);
void agent_reset(Agent* self);

BitVector * agent_max_reward(Agent *self);

#endif
