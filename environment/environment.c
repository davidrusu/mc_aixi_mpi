////////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com
// FILE:     environment:.c
// CREATED:  2016-04-21 12:03:42
// MODIFIED: 2016-04-22 00:14:17
////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdlib.h>

#include "../_utils/types.h"
#include "../_utils/macros.h"
#include "../_object/class.h"
#include "../_object/class.r"
#include "environment.h"

// def __init__():
static void * Environment_init ( void * _self, va_list * args )
{
    struct Environment * self = ( struct Environment * ) _self;

    self -> _options    = *args;
    is_finished(self)   = 0x00;
    reward(self)        = 0x00;
    action(self)        = 0x00;

    /// TODO: _valid_actions and _valid_observations and _observation need
    // to be initialized.  Must discuss types with the group first.
    //

    return self;
}//-------------------------------------------------------------------

// def __delete__():
static void * Environment_delete ( void * _self )
{
    struct Environment * self = _self;
    
    free ( self->_options ), self->options = 0;
    free ( self->_observation ), self->observation = 0;
    free ( self->_valid_observations ), self->_valid_observations = 0;
    free ( self->_valid_actions ), self->_valid_actions = 0;
    
    return self;
}//-------------------------------------------------------------------

// def secure-copy():
static void * Environment_cpy ( void * _self ) 
{
    struct Environment * self = _self;

    return new ( Environment , self->options );
}//-------------------------------------------------------------------

// def ___str___():
static void * Environment_str ( void * _self )
{
    struct Environment * self = _self;
    
    // reserve 255 Characters for print string
    char *  pstring = malloc(sizeof(char) * 0xFF);
    
    sprintf ( pstring, "action = %x, observation = %x, reward = %x\n",
             self->action, self->observation, self->reward );
    
    return pstring;
}//-------------------------------------------------------------------

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
} _Environment //-----------------------------------------------------

const void * Environment = & _Environment

// def action_bits():
u32  action_bits          ( void * _self )
{
    struct Environment * self = _self;
    assert ( self->_valid_actions != NULL);
    
    u32 max_action = 0
    
    foreach ( u32 const * action, self->_valid_actions )
        max_action = *action ? *action > max_action : max_action;
    
    return ( u32 ) LG2( max_action );
    
}//-------------------------------------------------------------------

// def observation_bits():
u32  observation_bits     ( void * _self )
{
    struct Environment * self = _self;
    assert ( self->_valid_observations != NULL);

    u32 max_observation = 0
    
    foreach ( u32 const * observation, self->_valid_observations )
        max_observation = *observation ? *observation > max_observation : max_observation;
    
    return ( u32 ) LG2( max_observation );

}//-------------------------------------------------------------------

// def reward_bits():
u32  reward_bits          ( void * _self )
{
    struct Environment * self = _self;
    assert ( self->_valid_rewards != NULL);
    
    u32 max_reward = 0
    
    foreach ( u32 const * reward, self->_valid_rewards )
        max_reward = *reward ? *reward > max_reward : max_reward;
    
    return ( u32 ) LG2( max_reward );
    
}//-------------------------------------------------------------------

// def perception_bits():
u32  percption_bits       ( void * _self )
{
    struct Environment * self = _self;
    return self->reward_bits(self) + self->action_bits(self);
    
}//-------------------------------------------------------------------

// check if the action is valid
u08  is_valid_action      ( void * _self, u32   action )
{
    
    struct Environment * self = _self;
    
    foreach( u32 const * a , self->_valid_actions)
        if ( * a == action ) return TRUE;
    
    return FALSE;
    
}//-------------------------------------------------------------------

// find out if the observation is a valid one
u08  is_valid_observation ( void * _self, u32   observation )
{
    
    struct Environment * self = _self;
    
    foreach( u32 const * o , self->_valid_observations)
        if ( * o == observation ) return TRUE;
    
    return FALSE;
    
}//-------------------------------------------------------------------

// check if the action is a valid action
u08  is_valid_reward      ( void * _self, u32   reward )
{
    
    struct Environment * self = _self;
    
    foreach( u32 const * r , self->_valid_rewards)
        if ( * r == reward ) return TRUE;
    
    return FALSE;
    
}//-------------------------------------------------------------------

// Get maximum action
u32  maximum_action       ( void * _self )
{
    struct Environment * self = _self;
    u16 idx = 0;
    
    foreach ( u32 const * x , self->_valid_actions)
        idx++;
    
    return self->_valid_actions[idx];
    
}//-------------------------------------------------------------------

// Get maximum observation
u32  maximum_observation  ( void * _self )
{
    struct Environment * self = _self;
    u16 idx = 0;
    
    foreach ( u32 const * x , self->_valid_observations)
    idx++;
    
    return self->_valid_observations[idx];
    
}//-------------------------------------------------------------------


// Get maximum reward
u32  maximum_reward       ( void * _self )
{
    struct Environment * self = _self;
    u16 idx = 0;
    
    foreach ( u32 const * x , self->_valid_rewards)
    idx++;
    
    return self->_valid_rewards[idx];
    
}//-------------------------------------------------------------------

// Get minimum action
u32  minimum_action       ( void * _self )
{
    struct Environment * self = _self;
    return self->_valid_actions[0];
    
}//-------------------------------------------------------------------

// Get minimum observation
u32  minimum_observation  ( void * _self )
{
    struct Environment * self = _self;
    return self->_valid_observations[0];
    
}//-------------------------------------------------------------------

// Get minimum reward
u32  minimum_reward       ( void * _self )
{
    struct Environment * self = _self;
    return self->_valid_rewards[0];
    
}//-------------------------------------------------------------------
