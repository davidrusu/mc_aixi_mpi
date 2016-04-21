////////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com
// FILE:     environment.r
// CREATED:  2016-04-21 12:03:42
// MODIFIED: 2016-04-21 14:11:49
////////////////////////////////////////////////////////////////////

#ifndef ENV_R
    #define ENV_R
    #include "types.h"

    // Class Environment:
    struct Environment
    {
        const void * class; // must be first
        u32      _action;
        u08      _is_finished;
        u32      _observation;
        va_list* _options;
        u08      _reward;
        u32      *_valid_actions;
        u32      *_valid_observations;
        u32      *_valid_rewards;
    } Environment; //---------------------------------------------------
    
    #define action(e) (((const struct Environment *)(p)) -> _action)
    #define is_finished(e) (((const struct Environment *)(p)) -> _is_finished)
    #define observation(e) (((const struct Environment *)(p)) -> _observation)
    #define reward(e) (((const struct Environment *)(p)) -> _reward)
#endif