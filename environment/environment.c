/*
 * Environment Class :: taken from pyaixi/environment.py
 * Compiled and modified by Robert Morouney
 * April 2016
 *
 */

#include <assert.h>
#include <stdlib.h>

#include "types.h"
#include "new.h"
#include "class.r"
#include "environment.h"
#include "macros.h"

// Class Environment:
struct Environment 
{
    const void * class; // must be first

    u32      _action;
    u08      is_finished;

    // TODO: Find out if this is is the correct way to hold the
    // observations. 
    u32      _observation;
    
    va_list* _options;

    u08      _reward;

    u32      *_valid_actions;
    u32      *_valid_observations;
}

// def __init__():
static void * Environment_init ( void * _self, va_list * args )
{
    struct Environment * self = _self;
    self->_options      = args;

    self->is_finished   = 0x00;
    self->reward        = 0x00;
    self->_action       = 0x00;

    /// TODO: _valid_actions and _valid_observations and _observation need
    // to be initialized.  Must discuss types with the group first.
    //

    return self;
}

// def __delete__():
static void * Environment_delete ( void * _self )
{
    struct Environment * self = _self;
    
    free ( self->_options ), self->options = 0;
    free ( self->_observation ), self->observation = 0;
    free ( self->_valid_observations ), self->_valid_observations = 0;
    free ( self->_valid_actions ), self->_valid_actions = 0;
    
    return self;
}

// def secure-copy():
static void * Environment_cpy ( void * _self ) 
{
    struct Environment * self = _self;

    return new ( Environment , self->options );
}

// def ___str___():
static void * Environment_str ( void * _self )
{
    struct Environment * self = _self;
    
    // reserve 255 Characters for print string
    char *  pstring = malloc(sizeof(char) * 0xFF);
    
    sprintf ( pstring, "action = %x, observation = %x, reward = %x\n",
             self->action, self->observation, self->reward );
    
    return pstring;
}

// def action_bits():
static void * Environment_action_bits ( void * _self )
{
    struct Environment * self = _self;
    assert ( self->_valid_actions != NULL);
    
    u32 max_action = 0
    
    foreach ( u32 const * action, self->_valid_actions )
        max_action = *action ? *action > max_action : max_action;
    
    return ( u32 ) LG2( max_action );
    
}

// def observation_bits():
static void * Environment_observation_bits ( void * _self )
{
    struct Environment * self = _self;
    assert ( self->_valid_observations != NULL);

    u32 max_observation = 0
    
    foreach ( u32 const * observation, self->_valid_observations )
        max_observation = *observation ? *observation > max_observation : max_observation;
    
    return ( u32 ) LG2( max_observation );

}

// def reward_bits():
static void * Environment_reward_bits ( void * _self )
{
    struct Environment * self = _self;
    assert ( self->_valid_rewards != NULL);
    
    u32 max_reward = 0
    
    foreach ( u32 const * reward, self->_valid_rewards )
        max_reward = *reward ? *reward > max_reward : max_reward;
    
    return ( u32 ) LG2( max_reward );
    
}

// def perception_bits():
static void * Environment_perception_bits ( void * _self )
{
    struct Environment * self = _self;
    return self->reward_bits(self) + self->action_bits(self);
    
}

// check if the action is valid
static void * Environment_is_valid_action ( void * _self, u32 action )
{
    
    struct Environment * self = _self;
    
    foreach( u32 const * a , self->_valid_actions)
        if ( * a == action ) return TRUE;
    
    return FALSE;
    
}

// find out if the observation is a valid one
static void * Environment_is_valid_observation ( void * _self, u32 observation )
{
    
    struct Environment * self = _self;
    
    foreach( u32 const * o , self->_valid_observations)
        if ( * o == observation ) return TRUE;
    
    return FALSE;
    
}

// check if the action is a valid action
static void * Environment_is_valid_reward ( void * _self, u32 reward )
{
    
    struct Environment * self = _self;
    
    foreach( u32 const * r , self->_valid_rewards)
        if ( * r == reward ) return TRUE;
    
    return FALSE;
    
}

// Get maximum action
static void * Environment_maximum_action ( void * _self )
{
    struct Environment * self = _self;
    u16 idx = 0;
    
    foreach ( u32 const * x , self->_valid_actions)
        idx++;
    
    return self->_valid_actions[idx];
    
}

// Get maximum observation
static void * Environment_maximum_observation ( void * _self )
{
    struct Environment * self = _self;
    u16 idx = 0;
    
    foreach ( u32 const * x , self->_valid_observations)
    idx++;
    
    return self->_valid_observations[idx];
    
}


// Get maximum reward
static void * Environment_maximum_reward ( void * _self )
{
    struct Environment * self = _self;
    u16 idx = 0;
    
    foreach ( u32 const * x , self->_valid_rewards)
    idx++;
    
    return self->_valid_rewards[idx];
    
}

// Get minimum action
static void * Environment_minimum_action ( void * _self )
{
    struct Environment * self = _self;
    return self->_valid_actions[0];
    
}

// Get minimum observation
static void * Environment_minimum_observation ( void * _self )
{
    struct Environment * self = _self;
    return self->_valid_observations[0];
    
}

// Get minimum reward
static void * Environment_minimum_reward ( void * _self )
{
    struct Environment * self = _self;
    return self->_valid_rewards[0];
    
}

// Perform an action on the environment
static void * Environment_perform_action ( void * _self, u32 action )
{
    struct Environment * self = _self;
    //NO OP.  This will be overwritten by the inheriting class.
    
}


// Print the state of the environment
static void * Environment_print ( void * _self )
{
    struct Environment * self = _self;
    printf("%s",self->__str__());
    return;
}

// This is kind of complicated but it basically uses the parent object class to define
// the enironment object pointer which holds all of the accessor methods.  This is all
// in the hopes that anyone can take this code and call Environment e = new(environment,...)
// then class methods can be called with e.method()
//
static const struct Class _Environment = {
    Environment_init,                   // done
    Environment_delete,                 // done
    Environment_cpy,                    // done
    Environment_str,                    // done
    Environment_observation_bits,       // done
    Environment_observation_bits,       // done
    Environment_reward_bits,            // done
    Environment_perception_bits,        // done
    Environment_is_valid_action         // done
    Environment_is_valid_observation,   // done
    Environment_is_valid_reward,        // done
    Environment_maximum_action,         // done
    Environment_maximum_observation,    // done
    Environment_maximum_reward,         // done
    Environment_minimum_action,         // done
    Environment_minimum_observation,    // done
    Environment_minimum_reward,         // done
    Environment_perform_action,         //
    Environment_print                   //
}

const void * Environment = & _Environment


