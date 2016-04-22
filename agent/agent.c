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
#include "../_utils/macros.h"
#include "../_object/class.r"
#include "../_object/class.h"
#include "../predict/context_tree.h"
#include "../bit_vector.h"
#include "../environment/environment.r"
#include "../environment/environment.h"


typedef struct Agent
{
    struct Environment * environment;
    va_list              _options;
    double               total_reward;
    update_enum          last_update;
    u32                  age;
    u32                  learning_period;
    ContextTree*         context_tree;
} Agent;


static void * Agent_init ( Agent* self, va_list * args )
{
    self -> environment = cpy(va_arg(* args , struct Environment *));
    self -> age = 0;
    self -> learning_period = va_arg ( * args, u32 );
    self -> last_update = action_update;
    self -> total_reward = 0.0;

    // TODO: Fill me in with a real value (should be the defined depth value...)
    self->context_tree = ctw_create(50);

    return self;
}

static void * Agent_delete ( void * _self )
{
    // free any memory that might be allocated by the class
    return NULL;
}

static u32 Agent_history_size(Agent* self) {
    return self->context_tree->history->size;
}

static AgentUndo* Agent_clone_into_temp(Agent* self) {
  AgentUndo* undo = (AgentUndo *) malloc(sizeof(AgentUndo));
    undo->age = self->age;
    undo->total_reward = self->total_reward;
    undo->history_size = Agent_history_size(self);
    undo->last_update = self->last_update;
}

static BitVector * Agent_encode_action(Agent* self, u32 action) {
   BitVector* vector = bv_from_uint32(action);
   ctw_update_history(self->context_tree, vector);
   self->age++;
   self->last_update = action_update;
   return vector;
}

static u32 Agent_decode_action(Agent* self, BitVector* symbols) {
    return bv_peek_uint32(symbols);
}

static u32 Agent_decode_observation(Agent* self, BitVector* symbols) {
    return bv_peek_uint32(symbols);
}

static u32 Agent_decode_reward(Agent * self, BitVector* symbols) {
    return bv_peek_uint32(symbols);
}

static double Agent_average_reward ( Agent * self)
{
    double average = 0.0;
    if ( self -> age > 0 )
        average = ( self -> total_reward ) / ( self -> age );
    return average;
}

static u32 Agent_generate_random_action ( Agent * self)
{
    // TODO: Replace this with the NUMBER of valid actions
    int actionIndex = rand() % 2;
    return self->environment->_valid_actions[actionIndex];
}

static u32 Agent_maximum_action ( Agent* self)
{
    // TODO: wtf???
    return maximum_reward(self->environment);
}

static u32 Agent_maximum_reward ( Agent* self)
{
    // TODO: wtff???
    return maximum_reward(self->environment);
}

// pyaixi: model_size
static u32 Agent_model_size ( Agent* self) {
   return ctw_size(self->context_tree);
}

static void Agent_model_update_action ( Agent* self, u32 action) {
   BitVector* action_symbols = Agent_encode_action(self, action);
   ctw_update_history(self->context_tree, action_symbols);
   self->age++;
   self->last_update = action_update;
}

static u32Tuple* Agent_decode_percept(Agent* self, BitVector* symbols) {
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

static u32 Agent_generate_action(Agent* self) {
   BitVector* random = ctw_gen_random_symbols(self->context_tree, 32);
   return Agent_decode_action(self, random);
}

static u32 Agent_generate_percept(Agent* self) {
   // RIP: method not used, but I've left it here as a marker
}

u32Tuple * Agent_generate_percept_and_update(Agent*  self) {
   BitVector* random = ctw_gen_random_symbols_and_update(self->context_tree, 32);
   u32Tuple* tuple = Agent_decode_percept(self, random);

   self->total_reward += tuple->first;
   self->last_update = percept_update;

   return tuple;
}

static double Agent_get_predicted_action_probability(Agent* self, u32 action) {
    BitVector* symbols =  Agent_encode_action(self, action);
    return ctw_predict_vector(self->context_tree, symbols);
}

static u32 Agent_maximum_bits_needed(Agent * self) {
    return 32; // this is stupid
}

static void Agent_model_revert(Agent * self, AgentUndo* undo) {
    while(Agent_history_size(self) > undo->history_size)  {
        if(self->last_update == percept_update){
            ctw_revert(self->context_tree, 32);
            self->last_update = action_update;
        } else {
	    ctw_revert_history(self->context_tree, 32);
            self->last_update = percept_update;
        }
    }

    self->age = undo->age;
    self->total_reward = undo->total_reward;
    self->last_update = undo->last_update;
    return;
}

static BitVector * Agent_encode_percept ( Agent * self, u32 observation, u32 reward) {
    BitVector* a = bv_from_uint32(observation);
    BitVector* b = bv_from_uint32(reward);
    bv_append(a, b);
    return a;
}

static void Agent_model_update_percept ( Agent * self, u32 observation, u32 reward ) {
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

static double Agent_percept_probability(Agent* self, u32 observation, u32 reward) {
    BitVector* symbols = Agent_encode_percept(self, observation, reward);
    return ctw_predict_vector(self->context_tree, symbols);
}

static double Agent_playout(Agent* self, u32 horizon) {
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

static u32 Agent_search (Agent* self)
{
    return Agent_maximum_action(self);
}

static void Agent_reset ( Agent* self )
{
    ctw_clear(self->context_tree);

    self -> age = 0;
    self -> total_reward = 0.0;
    self -> last_update = action_update;

    return;
}




    
