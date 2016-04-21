////////////////////////////////////////////////////////////////////
// AUTHOR:   Dan Morouney <069001422>
// FILE:     agent.c
// CREATED:  2016-04-20 14:42:09
// MODIFIED: 2016-04-20 22:54:05
////////////////////////////////////////////////////////////////////

#include "types.h"
#include "class.r"
#include "new.h"
#include <stddef.h>

typedef enum { action_update, percept_update } update_enum;

struct Agent 
{
    const void * class; // must be first
    void *  ( * average_reward ) ( void * _self );
    void *  ( * generate_random_action ) ( void * _self );
    void *  ( * maximum_action ) ( void * _self );
    void *  ( * maximum_reward ) ( void * _self );
    void *  ( * model_size ) ( void * _self );
    void *  ( * model_update_action ) ( void * _self );
    void *  ( * model_update_perception ) ( void * _self );
    void *  ( * search ) ( void * _self );
    void *  ( * reset ) ( void * _self );
    void *  environment;
    va_list             _options;
    double              total_reward;
    update_enum         last_update;
    u32                 age;
    u32                 learning_period;

};

static void * Agent_init ( void * _self, va_list * args )
{
    struct Agent * self = _self;
    self -> environment = cpy ( va_args ( * args , struct Environment ) );
    self -> age = 0;
    self -> learning_period = va_args ( * args, u32 );
    self -> last_update = action_update;
    self -> _options = * args;
    self -> total_reward = 0.0;
    return;
}

static void * Agent_delete ( void * _self )
{
    // free any memory that might be allocated by the class
    return NULL;
}

static void * Agent_cpy ( void * _self )
{
    struct Agent * self = _self;
    return new ( Agent, self -> _options );
}

static void * Agent_str ( void * _self )
{
    return NULL;
}

static void * Agent_average_reward ( void * _self )
{
    struct Agent * self = _self;
    double average = 0.0;
    if ( self -> age > 0 )
        average = ( self -> total_reward ) / ( self -> age );
    return average;
}

static void * Agent_generate_random_action ( void * _self )
{
    struct Agent * self = _self;
    return choose_random ( environment -> _valid_actions );
}

static void * Agent_maximum_action ( void * _self )
{
    struct Agent * self = _self;
    return self -> environment -> maximum_action ( self -> environment );
}
      
static void * Agent_maximum_reward ( void * _self )
{
    struct Agent * self = _self;
    return self -> environment -> maximum_reward ( self -> environment );
}

// no overriding in C so these method is left off and can put in later
//static void * Agent_model_size ( void * _self );
//static void * Agent_model_update_action ( void * _self );
//static void * Agent_model_update_perception ( void * _self );

static void * Agent_search (void * _self )
{
    struct Agent * self = _self;
    return self -> maximum_action(self);
}

static void * Agent_reset ( void * _self )
{
    struct Agent * self = _self;
    self -> age = 0;
    self -> total_reward = 0.0;
    self -> last_update = action_update;
    return;
}

static const struct Class _Agent = {
    Agent_init,
    Agent_delete,
    Agent_cpy,
    Agent_str };

const void * Agent = & _Agent;
