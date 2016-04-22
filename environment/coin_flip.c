///////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com
// FILE:     coin_flip.c
// CREATED:  2016-04-21 12:03:42
// MODIFIED: 2016-04-21 21:53:30
////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "../_utils/types.h"
#include "../_utils/macros.h"
#include "coin_flip.h"

void * CF_init ( void * _self, va_list * args ) 
{
    struct Coin_flip * self = 
        ((const struct Class *) Environment) -> __init__( _self , args );
   
   valid_actions(self)          = malloc ( Number_States * sizeof ( u32 ) );
   valid_actions(self)[0]       = Tails;
   valid_actions(self)[1]       = Heads;
       
   valid_observation(self)      = malloc ( Number_States * sizeof ( u32 ) );
   valid_observations(self)[0]  = Tails;
   valid_observations(self)[1]  = Heads;
   
   valid_rewards(self)          = malloc ( Number_States * sizeof ( u32 ) );
   valid_rewards(self)[0]       = Loss;
   valid_rewards(self)[1]       = Win;
   
   double probability_t = va_args ( * args , double );
   if ( probability_t <= 0.0001 || probability_t >= 1.0001 ) probability_t = Default_Probability;
    
    #ifdef DEBUG
        TRACE ( "Probability = %d\n", probability_t );
    #endif
    
   probability(self) = probability_t;

   srand(time(NULL));
   u32 random_index = rand() % 2;
   observation(self) = valid_observation(self)[random_index];
    
   reward(self) = 0;
   return self;
}

double __rp() { return (double) rand() / (double)RAND_MAX; }

static u32Tuple perform_action ( void * _self, u32 action_t )
{
    Coin_Flip * self = _self;

    #ifdef DEBUG
        TRACE ( "Action = %d\n", action_t );
    #endif
    
    BLOCK_START
        u08 is_valid = 0x00; 
        foreach ( u32 const * a , valid_actions(self) )
            if ( * a == action_t ) is_valid = !(is_valid);
        assert ( is_valid != 0x00 );
    BLOCK_END

    action(self) = action_t;
    
    u32 observation_t , reward_t;
    
    if (__rp() < probability(self) ){
        observation_t = Heads;
        reward_t = ( action == Tails ) ? Loss : Win;
    } else {
        observation_t = Tails;
        reward_t = ( action == Tails ) ? Win : Loss;
    }
    
    #ifdef DEBUG
        TRACE ( "Observation = %d Reward = %d\n", observation_t, reward_t );
    #endif

    observation(self) = observation_t;
    reward(self) = reward_t;

    u32Tuple* tuple = malloc (sizeof(u32Tuple));
    tuple -> first = observation_t;
    tuple -> second = reward_t;

    return tuple;
}

static void CF_print(void * _self)
{
    printf ("Prediction = %x, Observation = %x, Reward = %x\n",
            prediction(self),observation(self),reward(self));
}

static const struct _Coin_Flip = {
    sizeof(struct Coind_Flip), CF_init, NULL, NULL, NULL
};

const void * Coin_Flip = & _Coin_Flip;
