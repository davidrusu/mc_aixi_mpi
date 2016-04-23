////////////////////////////////////////////////////////////////////
// AUTHOR:   Dan Morouney <069001422>
// FILE:     agent.c
// CREATED:  2016-04-20 14:42:09
// MODIFIED: 2016-04-20 22:54:05
////////////////////////////////////////////////////////////////////

#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include "../_utils/types.h"
#include "../_object/class.h"
#include "../predict/context_tree.h"
#include "../bit_vector.h"
#include "../environment/environment.r"
#include "../environment/environment.h"
#include "agent.h"
#include "../search/monte_node.h"
#include "../_utils/macros.h"

Agent* Agent_init ( Agent* self, void * _env, u32 learn  )
{
    const struct Coin_Flip * env = _env;
    TRACE("Prepping to build agent\n", "agent");

    self -> environment = cpy ( env );
    self -> age = 0;
    self -> learning_period = learn;
    self -> last_update = action_update;
    self -> total_reward = 0.0;
    self -> horizon = 6;

    TRACE("Building context tree for Agent", "agent");
    self->context_tree = ctw_create(10);

    return self;
}

  void * Agent_delete ( void * _self )
{
    // free any memory that might be allocated by the class
    return NULL;
}

  u32 Agent_history_size(Agent* self) {
    return self->context_tree->history->size;
}

  AgentUndo* Agent_clone_into_temp(Agent* self) {
    AgentUndo* undo = (AgentUndo *) malloc(sizeof(AgentUndo));
    undo->age = self->age;
    undo->total_reward = self->total_reward;
    undo->history_size = Agent_history_size(self);
    undo->last_update = self->last_update;
    return undo;
}

  BitVector * Agent_encode_action(Agent* self, u32 action) {
   BitVector* vector = bv_from_uint32(action);
   ctw_update_history(self->context_tree, vector);
   self->age++;
   self->last_update = action_update;
   return vector;
}

  u32 Agent_decode_action(Agent* self, BitVector* symbols) {
    return bv_peek_uint32(symbols);
}

  u32 Agent_decode_observation(Agent* self, BitVector* symbols) {
    return bv_peek_uint32(symbols);
}

  u32 Agent_decode_reward(Agent * self, BitVector* symbols) {
    return bv_peek_uint32(symbols);
}

  double Agent_average_reward ( Agent * self, u32 cycles)
{
    double average = 0.0;
    if ( self -> age > 0 )
        average = ( self -> total_reward ) / ( cycles );
    return average;
}

  u32 Agent_generate_random_action ( Agent * self)
{
    // TODO: Replace this with the NUMBER of valid actions
    int actionIndex = rand() % self->environment->num_actions;
    return self->environment->_valid_actions[actionIndex];
}

  u32 Agent_maximum_action ( Agent* self)
{
    // TODO: wtf???
    return maximum_reward(self->environment);
}

  u32 Agent_maximum_reward ( Agent* self)
{
    // TODO: wtff???
    return maximum_reward(self->environment);
}

// pyaixi: model_size
  u32 Agent_model_size ( Agent* self) {
   return ctw_size(self->context_tree);
}

  void Agent_model_update_action ( Agent* self, u32 action) {
   BitVector* action_symbols = Agent_encode_action(self, action);
   ctw_update_history(self->context_tree, action_symbols);
   self->age++;
   self->last_update = action_update;
}

  u32Tuple* Agent_decode_percept(Agent* self, BitVector* symbols) {
  uint64_t i;
  BitVector* reward_symbols = bv_create();
  BitVector* observation_symbols = bv_create();

  for (i = 0; i < 64; i++) {
    bool bit = bv_test(symbols, i);
    if(i > 31) {
     bv_push(observation_symbols, bit);
    }
    else {
     bv_push(reward_symbols, bit);
    }
  }

  // Decode both
  u32 reward = Agent_decode_reward(self, reward_symbols);
  u32 observation = Agent_decode_observation(self, observation_symbols);

  u32Tuple* tuple = malloc(sizeof(u32Tuple));
  tuple->first = observation;
  tuple->second = reward;

  return tuple;
}

  u32 Agent_generate_action(Agent* self) {
   BitVector* random = ctw_gen_random_symbols(self->context_tree, 32);
   return Agent_decode_action(self, random);
}

  u32 Agent_generate_percept(Agent* self) {
   // RIP: method not used, but I've left it here as a marker
      return 0;
}

u32Tuple * Agent_generate_percept_and_update(Agent*  self) {
   BitVector* random = ctw_gen_random_symbols_and_update(self->context_tree, 64);
   u32Tuple* tuple = Agent_decode_percept(self, random);

   self->total_reward += tuple->first;
   self->last_update = percept_update;
   return tuple;
}

  double Agent_get_predicted_action_probability(Agent* self, u32 action) {
    BitVector* symbols =  Agent_encode_action(self, action);
    return ctw_predict_vector(self->context_tree, symbols);
}

  u32 Agent_maximum_bits_needed(Agent * self) {
    return 32; // this is stupid
}

  void Agent_model_revert(Agent * self, AgentUndo* undo) {
    while(Agent_history_size(self) > undo->history_size)  {
        if(self->last_update == percept_update){
            ctw_revert(self->context_tree, 32);
            self->last_update = action_update;
        } else {
	    ctw_revert_history(self->context_tree, 32);
            self->last_update = percept_update;
        }
    }
    if (Agent_history_size(self) != undo->history_size) {
      printf("hist size should be equal %u %u\n", Agent_history_size(self), undo->history_size);
      exit(1034109);
    }
    self->age = undo->age;
    self->total_reward = undo->total_reward;
    self->last_update = undo->last_update;
    return;
}

  BitVector * Agent_encode_percept ( Agent * self, u32 observation, u32 reward) {
    BitVector* a = bv_from_uint32(observation);
    BitVector* b = bv_from_uint32(reward);
    bv_append(a, b);
    return a;
}

  void Agent_model_update_percept ( Agent * self, u32 observation, u32 reward ) {
    BitVector* symbols = Agent_encode_percept(self, observation, reward);
    
    // Are we still learning?
    if((self->learning_period > 0 ) && (self->age > self->learning_period)) {
       ctw_update_history(self->context_tree, symbols);
    } else {
       ctw_update_vector(self->context_tree, symbols);
    }

    self->total_reward += reward;
    self->last_update = percept_update;
    return;
}

  double Agent_percept_probability(Agent* self, u32 observation, u32 reward) {
    BitVector* symbols = Agent_encode_percept(self, observation, reward);
    return ctw_predict_vector(self->context_tree, symbols);
}

  double Agent_playout(Agent* self, u32 horizon) {
    double total_reward = 0;
    u32 i = 0;

    for(i = 0; i < horizon; i++) {
        u32 action = Agent_generate_random_action(self);
        Agent_model_update_action(self, action);

        u32Tuple* tuple = Agent_generate_percept_and_update(self);
        total_reward += tuple->second;
    }

    return total_reward;
}

  u32 Agent_search (Agent* self)
{
    // This function was not implemented properly
    AgentUndo* undo = Agent_clone_into_temp(self);

    MonteNode* node = monte_create_tree(NODE_TYPE_DECISION);

    u32 i = 0;
    // 300 sims
    for(i = 0; i < 300;i++ ) {
        monte_sample(node, self, self->horizon);
        Agent_model_revert(self, undo);
    }

    u32 best_action = Agent_generate_random_action(self);
    double best_mean = (double)((u32)-1);

    for(i = 0; i < self->environment->num_actions; i++) {
        u32 action =  self->environment->_valid_actions[i];
        MonteNode* searchNode = dict_find(node->actions, action);

        if(searchNode != NULL) {
            double mean = searchNode->mean + ((float)rand()/(float)(RAND_MAX)) * 0.00001;
            if(mean > best_mean) {
                best_mean = mean;
                best_action = action;
            }
        }
    }

    return best_action;
}

  void Agent_reset ( Agent* self )
{
    ctw_clear(self->context_tree);

    self -> age = 0;
    self -> total_reward = 0.0;
    self -> last_update = action_update;

    return;
}




    
