////////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com 
// FILE:     coin_flip.h
// CREATED:  2016-04-21 16:23:23
// MODIFIED: 2016-04-22 22:15:04
////////////////////////////////////////////////////////////////////

#ifndef COIN_FLIP_H
#define COIN_FLIP_H
#include <stddef.h>
#include "../bit_vector.h"

typedef struct Env {
    int num_actions;
    BitVector **valid_actions;
    BitVector **valid_observations;
    BitVector **valid_rewards;
    BitVector *action;
    BitVector *observation;
    BitVector *reward;
    bool is_finished;
    double bias;
} Env;

typedef struct Percept {
    BitVector *observation;
    BitVector *reward;
} Percept;

Env * env_create(double);

Percept * percept_create(BitVector *observation, BitVector *reward);

Percept *perform_action(Env *env, BitVector *action);
uint32_t action_bits(Env *env);
uint32_t observation_bits(Env *env);
uint32_t reward_bits(Env *env);
uint32_t percept_bits(Env *env);

bool is_valid_action(Env *env, BitVector *action);
bool is_valid_observation(Env *env, BitVector *observation);
bool is_valid_reward(Env *env, BitVector *reward);
bool is_valid_percept(Env *env, Percept *percept);

Percept *env_generate_random_percept(Env *env);
BitVector *env_max_reward(Env *env);
#endif

