////////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com
// FILE:     environment.h
// CREATED:  2016-04-21 12:03:42
// MODIFIED: 2016-04-22 14:18:35
////////////////////////////////////////////////////////////////////

#ifndef ENV_H
    #define ENV_H
    #include "../_utils/types.h"

    extern const void * Environment;

    // define any functions here
    u32  action_bits          ( void * _self );
    u32  observation_bits     ( void * _self );
    u32  reward_bits          ( void * _self );
    u32  percption_bits       ( void * _self );

    u08  is_valid_action      ( void * _self, u32   action );
    u08  is_valid_observation ( void * _self, u32   observation );
    u08  is_valid_reward      ( void * _self, u32   reward );

    u32  maximum_action       ( void * _self );
    u32  maximum_observation  ( void * _self );
    u32  maximum_reward       ( void * _self );

    u32  minimum_action       ( void * _self );
    u32  minimum_observation  ( void * _self );
    u32  minimum_reward       ( void * _self );

    u32 LG2 ( u32 x );

#endif
