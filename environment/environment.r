////////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com
// FILE:     environment.r
// CREATED:  2016-04-21 12:03:42
// MODIFIED: 2016-04-22 23:28:26
////////////////////////////////////////////////////////////////////

#ifndef ENV_R
    #define ENV_R
    #include "../_utils/types.h"
    #include <stdarg.h>
    // Class Environment:
    struct Environment
    {
        const void * class; // must be first
        u32      _action;
        u08      _is_finished;
        u32      _observation;
        va_list  _options;
        u08      _reward;
        u32      *_valid_actions;
        u32      *_valid_observations;
        u32      *_valid_rewards;
        u32      num_actions;
    };//---------------------------------------------------
    
    #define action(e) (((const struct Environment *)(e)) -> _action)
    #define is_finished(e) ((const struct Environment *)(e)) -> _is_finished
    #define observation(e) (((const struct Environment *)(e)) -> _observation)
    #define reward(e) (((const struct Environment *)(e)) -> _reward)
    #define valid_actions(e) (((const struct Environment *)(e)) -> _valid_actions)
    #define valid_observations(e) (((const struct Environment *)(e)) -> _valid_observations)
    #define valid_rewards(e) (((const struct Environment *)(e)) -> _valid_rewards)
    
#endif
