////////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com 
// FILE:     agent.r
// CREATED:  2016-04-21 14:39:38
// MODIFIED: 2016-04-21 14:44:18
////////////////////////////////////////////////////////////////////
#ifndef AGENT_R
    #define AGENT_R
    #include <stddef.h>
    #include "../_utils/types.h"
    #include "../environment/environment.h"

    // #include the context tree here ...

    typedef enum { action_update, percept_update } update_enum;
    
    struct Agent
    {
        const void *    class; // must be first
        Environment *   environment;
        va_list         _options;
        double          total_reward;
        update_enum     last_update;
        u32             age;
        u32             learning_period;
        ContextTree*    context_tree;
    };
    
    typedef struct {
        u32             first;
        u32             second;
    } u32Tuple;
    
    typedef struct {
        u32             age;
        u32             total_reward;
        u32             history_size;
        update_enum     last_update;
    } AgentUndo;
 
#endif
