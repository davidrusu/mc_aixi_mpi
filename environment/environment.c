#include <assert.h>
#include <stdlib.h>

#include "types.h"
#include "new.h"
#include "class.r"
#include "environment.h"

// Class Environment:
struct Environment 
{
    const void * class; // must be first

    u64      _action;
    u08      is_finished;

    // TODO: Find out if this is is the correct way to hold the
    // observations. 
    b_lst    _observation;
    
    va_list* _options;

    u08      _reward;

    u64      *_valid_actions;
    b_lst    *_valid_observations;
}

// __init__()
static void * Environment_init ( void * _self, va_list * args )
{
    struct Environment * self = _self;
    self->_options      = args;

    self->is_finished   = 0x00;
    self->reward        = 0x00;
    self->_action       = 0x0000000000000000;

    /// TODO: _valid_actions and _valid_observations and _observation need
    // to be initialized.  Must discuss types with the group first.
    //

    return self;
}

static void * Environment_delete ( void * _self )
{
    struct Environment * self = _self;
    
    free ( self->_options ), self->options = 0;
    free ( self->_observation ), self->observation = 0;
    free ( self->_valid_observations ), self->_valid_observations = 0;
    free ( self->_valid_actions ), self->_valid_actions = 0;
    
    return self;
}

static void * Environment_cpy ( void * _self ) 
{
    struct Environment * self = _self;

    return new ( Environment , self->options );
}

static void * Environment_str ( void * _self )
{
    char *  pstring;
    // TODO: Return a string verson on the class... why?
    
    return pstring;
}

static void * Environment_action_bits ( void * _self )
{
    // TODO: DISCUSS BIT STRINGS BEFORE ADDING THIS
    // This should calculate the maximum number of bits needed
    u32 max_bits = 0;
    u32  = 
}



static const struct Class _Environment = {
    Environment_init,
    Environment_delete,
    Environment_cpy,
    Environment_str,
    Environment_action_bits,
    Environment_observation_bits,
    Environment_reward_bits,
    Environment_perception_bits,
    Environment_is_valid_action
    Environment_is_valid_observation,
    Environment_is_valid_reward,
    Environment_maximum_action,
    Environment_maximum_observation,
    Environment_maximum_reward,
    Environment_minimum_action,
    Environment_minimum_observation,
    Environment_minimum_reward,
    Environment_perform_action,
    Environment_print
}

const void * Environment = & _Environment


