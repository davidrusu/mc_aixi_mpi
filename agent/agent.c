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
#include "agent.r"

static void * Agent_init ( void * _self, va_list * args )
{
    struct Agent * self = _self;
    self -> environment = cpy(va_arg(* args , struct Environment *));
    self -> age = 0;
    self -> learning_period = va_arg ( * args, u32 );
    self -> last_update = action_update;
    self -> total_reward = 0.0;

    // TODO: Fill me in with a real value
    self->context_tree = ctw_create(50);

    return self;
}

static void * Agent_delete ( void * _self )
{
    // free any memory that might be allocated by the class
    return NULL;
}

const void * Agent;
static void * Agent_cpy ( void * _self )
{
    struct Agent * self = _self; 
    return new ( Agent, self->environment, self->learning_period);
}

static void * Agent_str (const void * _self )
{
    return NULL;
}

static u32 history_size(void * _self) {
    struct Agent * self = _self;
    return self->context_tree->history->size;
}

// ------------------------------------ secret sauce
static const struct Class _Agent = {
    Agent_init,
    Agent_delete,
    Agent_cpy,
    Agent_str };

const void * Agent = & _Agent;
// shhh.. -------------------------------------------

static AgentUndo* clone_into_temp(struct Agent* agent) {
  AgentUndo* undo = (AgentUndo *) malloc(sizeof(AgentUndo));
    undo->age = agent->age;
    undo->total_reward = agent->total_reward;
    undo->history_size = history_size(agent);
    undo->last_update = agent->last_update;
}

static BitVector * encode_action(void * _self, u32 action) {
   struct Agent * self = _self;
   BitVector* vector = bv_from_uint32(action);
   ctw_update_history(self->context_tree, vector);
   self->age++;
   self->last_update = action_update;
   return vector;
}

static u32 decode_action(void * _self, BitVector* symbols) {
    return bv_peek_uint32(symbols);
}

static u32 decode_observation(void * _self, BitVector* symbols) {
    return bv_peek_uint32(symbols);
}

static u32 decode_reward(void * _self, BitVector* symbols) {
    return bv_peek_uint32(symbols);
}

static double average_reward ( void * _self )
{
    struct Agent * self = _self;
    double average = 0.0;
    if ( self -> age > 0 )
        average = ( self -> total_reward ) / ( self -> age );
    return average;
}

static u32 generate_random_action ( void * _self )
{
    struct Agent * self = _self;
    return choose_random ( self->environment -> _valid_actions );
}

static u32 maximum_action ( void * _self )
{
    struct Agent * self = _self;
    return  maximum_action ( self -> environment );
}
      
static void * maximum_reward ( void * _self )
{
    struct Agent * self = _self;
    return  maximum_reward ( self -> environment );
}

// pyaixi: model_size
static u32 model_size ( void * _self ) {
   struct Agent * self = _self;
   return ctw_size(self->context_tree);
}

static void model_update_action ( void * _self, u32 action) {
   struct Agent * self = _self;
   BitVector* action_symbols = encode_action(_self, action);
   ctw_update_history(self->context_tree, action_symbols);
   self->age++;
   self->last_update = action_update;
}

static u32Tuple* decode_percept(void * _self, BitVector* symbols) {
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
  u32 reward = decode_reward(_self, reward_symbols);
  u32 observation = decode_observation(_self, observation_symbols);

  u32Tuple* tuple = malloc(sizeof(u32Tuple));
  tuple->first = observation;
  tuple->second = reward;

  return tuple;
}

static u32 generate_action(void  * _self) {
   struct Agent * self = _self;
   BitVector* random = ctw_gen_random_symbols(self->context_tree, 32);
   return decode_action(_self, random);
}

static u32 generate_percept(void * _self) {
   // RIP: method not used, but I've left it here as a marker
}

u32Tuple * generate_percept_and_update(void * _self) {
   struct Agent * self = _self;
   BitVector* random = ctw_gen_random_symbols_and_update(self->context_tree, 32);
   u32Tuple* tuple = decode_percept(_self, random);

   self->total_reward += tuple->first;
   self->last_update = percept_update;

   return tuple;
}

static double get_predicted_action_probability(void * _self, u32 action) {
    struct Agent * self = _self;
    BitVector* symbols =  encode_action(_self, action);
    return ctw_predict_vector(self->context_tree, symbols);
}

static u32 maximum_bits_needed(void * _self) {
    return 32; // this is stupid
}

static void model_revert(void* _self, AgentUndo* undo) {
   struct Agent * self = _self;
    while(history_size(self) > undo->history_size)  {
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

static BitVector * encode_percept ( void * _self, u32 observation, u32 reward) {
    BitVector* a = bv_from_uint32(observation);
    BitVector* b = bv_from_uint32(reward);
    bv_append(a, b);
    return a;
}

static void model_update_percept ( void * _self, u32 observation, u32 reward ) {
    struct Agent * self = _self;
    BitVector* symbols = encode_percept(self, observation, reward);

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

static double percept_probability(void * _self, u32 observation, u32 reward) {
    struct Agent * self = _self;
    BitVector* symbols = encode_percept(self, observation, reward);
    return ctw_predict_vector(self->context_tree, symbols);
}

static double playout(void * _self, u32 horizon) {
    struct Agent * self = _self;
    double total_reward = 0;
    u32 i = 0;

    for(i = 0; i < horizon; i++) {
        u32 action = generate_random_action(self);
        model_update_action(self, action);

        u32Tuple* tuple = generate_percept_and_update(self);
        total_reward += tuple->second;
    }

    return total_reward;
}

static u32 search (void * _self )
{
    struct Agent * self = _self;
    return maximum_action(self);

}

static void reset ( void * _self )
{
    struct Agent * self = _self;
    ctw_clear(self->context_tree);

    self -> age = 0;
    self -> total_reward = 0.0;
    self -> last_update = action_update;

    return;
}




    
