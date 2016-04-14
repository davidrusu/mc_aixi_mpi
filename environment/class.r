#ifndef CLASS_H
#define CLASS_H

#include <stdarg.h>

#include "types.h"

struct Class {
    void *  ( * __init__ )              ( void * self, va_list      args );
    void *  ( * __delete__ )            ( void * self );
    void *  ( * __copy__ )              ( const void * self );
    void *  ( * __str__ )               ( const void * self );
    
    void *  ( * action_bits )           ( const void * self );
    void *  ( * observation_bits )      ( const void * self );
    void *  ( * reward_bits )           ( const void * self );
    void *  ( * percption_bits )        ( const void * self );
    
    void *  ( * _is_valid_action )      ( const void * self, u08   action );
    void *  ( * _is_valid_observation ) ( const void * self, u32   observation );
    void *  ( * _is_valid_reward )      ( const void * self, u08   reward );
    
    void *  ( * _maximum_action )       ( const void * self );
    void *  ( * _maximum_observation )  ( const void * self );
    void *  ( * _maximum_reward )       ( const void * self );

    void *  ( * _minimum_action )       ( const void * self );
    void *  ( * _minimum_observation )  ( const void * self );
    void *  ( * _minimum_reward )       ( const void * self );

    void *  ( * perform_action )        ( const void * self, u08    action );
    void *  ( * print )                 ( const void * self );
};

#endif
