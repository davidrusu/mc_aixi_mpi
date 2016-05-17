///////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com
// FILE:     coin_flip.c
// CREATED:  2016-04-21 12:03:42
// MODIFIED: 2016-04-23 03:39:52
////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>
#include "coin_flip.h"

double rp() { return ((double) rand()) / RAND_MAX; }

Env * env_create(double bias) {
    Env *env = (Env *) malloc(sizeof(Env));
    env->num_actions = 2;

    env->valid_actions = (BitVector **) malloc(sizeof(BitVector **) * 2);
    env->valid_actions[0] = bv_from_bool(false);
    env->valid_actions[1] = bv_from_bool(true);
       
    env->valid_observations = (BitVector **) malloc(sizeof(BitVector **) * 2);
    env->valid_observations[0] = bv_from_bool(false);
    env->valid_observations[1] = bv_from_bool(true);
   
    env->valid_rewards = (BitVector **) malloc(sizeof(BitVector **) * 2);
    env->valid_rewards[0] = bv_from_bool(false);
    env->valid_rewards[1] = bv_from_bool(true);
    env->bias = bias;

    env->observation = env->valid_observations[0]; // set the initial observation
    env->action = env->valid_actions[0];
    env->reward = env->valid_rewards[0];
    return env;
}

Percept * percept_create(BitVector *observation, BitVector *reward) {
    Percept *percept = (Percept *) malloc(sizeof(Percept));
    percept->observation = observation;
    percept->reward = reward;
    return percept;
}

Percept * perform_action(Env *env, BitVector *action) {
    //TODO: Leaking some memory here.
    assert(is_valid_action(env, action));

    env->action = action;
    
    BitVector *observation;
    BitVector *reward;
    
    if (rp() > env->bias){
        observation = bv_from_bool(true);
    } else {
        observation = bv_from_bool(false);
    }
    
    reward = bv_from_bool(bv_eq(action, observation)); // yeah..

    env->observation = observation;
    env->reward = reward;

    return percept_create(observation, reward);
}

uint32_t action_bits(Env *env) {
    return 1;
}

uint32_t observation_bits(Env *env) {
    return 1;
}

uint32_t reward_bits(Env *env) {
    return 1;
}

uint32_t percept_bits(Env *env) {
    return observation_bits(env) + reward_bits(env);
}

bool is_valid_action(Env *env, BitVector *action) {
    uint64_t i;
    for (i = 0; i < env->num_actions; i++) {
        if (bv_eq(action, env->valid_actions[i])) {
            return true;
        }
    }
    return false;
}

bool is_valid_observation(Env *env, BitVector *observation) {
    return bv_eq(observation, bv_from_bool(false)) || bv_eq(observation, bv_from_bool(true));
}

bool is_valid_reward(Env *env, BitVector *reward) {
    return bv_eq(reward, bv_from_bool(false)) || bv_eq(reward, bv_from_bool(true));
}

bool is_valid_percept(Env *env, Percept *percept) {
    return is_valid_observation(env, percept->observation) && is_valid_reward(env, percept->reward);
}

Percept *env_generate_random_percept(Env *env) {
    return percept_create(env->valid_observations[rand() % 2], env->valid_rewards[rand() % 2]);
}

BitVector *env_max_reward(Env *env) {
    return env->valid_rewards[1];
}