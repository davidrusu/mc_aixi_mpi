////////////////////////////////////////////////////////////////////
// AUTHOR:   Dan Morouney <069001422>
// FILE:     agent.c
// CREATED:  2016-04-20 14:42:09
// MODIFIED: 2016-04-20 17:01:41
////////////////////////////////////////////////////////////////////

#include "types.h"
#include "class.r"
#include "new.h"
#include <stddef.h>

struct Agent {
    const void * class; // must be first
    void *  ( * average_reward ) ( const void * self );
    void *  ( * generate_random_action ) ( const void * self );
    void *  ( * maximum_action ) ( const void * self );
    void *  ( * maximum_reward ) ( const void * self );
    void *  ( * model_size ) ( const void * self );
    void *  ( * model_update_action ) ( const void * self );
    void *  ( * model_update_perception ) ( const void * self );
    void *  ( * search ) ( const void * self );
    void *  environment 
    u32         age;
    u32         last_update;
    u32         learning_period;
    u32         total_reward;
};

static void * Agent_init ( void * _self, va_list * args );
static void * Agent_delete ( void * _self );
static void * Agent_cpy ( void * _self );
static void * Agent_str ( void * _self );
static void * Agent_average_reward ( void * _self );
static void * Agent_generate_random_action ( void * _self );
static void * Agent_maximum_action ( void * _self );
static void * Agent_maximum_reward ( void * _self );
static void * Agent_model_size ( void * _self );
static void * Agent_model_update_action ( void * _self );
static void * Agent_model_update_perception ( void * _self );
static void * Agent_search_ (void * _self );

