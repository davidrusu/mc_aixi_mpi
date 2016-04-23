///////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com
// FILE:     coin_flip.c
// CREATED:  2016-04-21 12:03:42
// MODIFIED: 2016-04-22 21:50:07
////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include "../_utils/types.h"
#include "../_utils/macros.h"
#include "class.h"
#include "class.r"
#include "environment.h"
#include "environment.r"
#include "coin_flip.h"
#include "coin_flip.r"

void * CF_init ( void * _self, va_list * args ) 
{
    struct Coin_Flip * self =
        ((const struct Class *) Environment) -> __init__( _self , args );
    
    self -> _ . num_actions             = 2;
    
    self -> _ . _valid_actions          = malloc ( 2 * sizeof ( u32 ) );
    self -> _ . _valid_actions[0]       = 0;
    self -> _ . _valid_actions[1]       = 1;
       
    self -> _ . _valid_observations     = malloc ( 2 * sizeof ( u32 ) );
    self -> _ . _valid_observations[0]  = 0;
    self -> _ . _valid_observations[1]  = 1;
   
    self -> _ . _valid_rewards          = malloc ( 2 * sizeof ( u32 ) );
    self -> _ . _valid_rewards[0]       = 0;
    self -> _ . _valid_rewards[1]       = 1;
   
   double probability_t = va_arg ( * args , double );
   if ( probability_t <= 0.0001 || probability_t >= 1.0001 ) probability_t = 0.7;
    
    #ifdef DEBUG
        TRACE ( "Probability = %d\n", probability_t );
    #endif
    
   self -> probability = probability_t;

   srand(time(NULL));
   u32 random_index = rand() % 2;
   self->_._observation = self->_._valid_observations[random_index];
    
   //reward(self) = 0;
   return self;
}

double __rp() { return (double) rand() / (double)RAND_MAX; }

u32Tuple* perform_action ( void * _self, u32 action_t )
{
    struct Coin_Flip * self = _self;

    #ifdef DEBUG
        TRACE ( "Action = %d\n", action_t );
    #endif
    
    BLOCK_START
        u08 is_valid = 0x00; 
        foreach ( u32 const * a , valid_actions(self) )
            if ( * a == action_t ) is_valid = !(is_valid);
        assert ( is_valid != 0x00 );
    BLOCK_END

    self -> _ . _action = action_t;
    
    u32 observation_t , reward_t;
    
    if (__rp() < probability(self) ){
        observation_t = 1;
        reward_t = ( action_t == 0 ) ? 0 : 1;
    } else {
        observation_t = 0;
        reward_t = ( action_t == 0 ) ? 1 : 0;
    }
    
    #ifdef DEBUG
        TRACE ( "Observation = %d Reward = %d\n", observation_t, reward_t );
    #endif

    self -> _ . _observation    = observation_t;
    self -> _ . _reward         = reward_t;

    u32Tuple* tuple = malloc (sizeof(u32Tuple));
    tuple -> first = observation_t;
    tuple -> second = reward_t;

    return tuple;
}

static void CF_print(void * _self)
{
    struct Coin_Flip * self = _self;
    printf ("Prediction = %x, Observation = %x, Reward = %x\n",
            action(self),observation(self),reward(self));
}

static const struct Class _Coin_Flip = {
    sizeof(struct Coin_Flip), CF_init, NULL, NULL, NULL
};

const void * Coin_Flip = & _Coin_Flip;
