#ifndef AGENT_H
    #define AGENT_H

    typedef struct Agent
    {
        struct Environment * environment;
        va_list              _options;
        double               total_reward;
        update_enum          last_update;
        u32                  age;
        u32                  horizon;
        u32                  learning_period;
        ContextTree*         context_tree;
    } Agent;


    static AgentUndo* Agent_clone_into_temp                   (Agent* self);

    static double  Agent_average_reward                       ( Agent* self);

    static u32  Agent_generate_random_action                  ( Agent* self);
    static u32  Agent_maximum_action                          ( Agent* self  );
    static u32  Agent_maximum_reward                          ( Agent* self  );
    static u32  Agent_model_size                              ( Agent* self  );

    static void  Agent_model_update_action                    ( Agent* self, u32 action );
    static BitVector * Agent_encode_action                    ( Agent* self , u32 action );

    // decoding
    static u32 Agent_decode_action                            (Agent* self , BitVector* symbols);
    static u32 Agent_decode_observation                       (Agent* self , BitVector* symbols);
    static u32 Agent_decode_reward                            (Agent* self, BitVector* symbols);
    static u32Tuple* Agent_decode_percept                     (Agent* self , BitVector* symbols);

    // generators
    static u32 Agent_generate_action                          (Agent* self);
    static u32 Agent_generate_percept                         (Agent* self );
    u32Tuple * Agent_generate_percept_and_update              (Agent* self );

    static u32 Agent_history_size                             (Agent* self );
    static BitVector * Agent_get_predicted_action_probability (Agent* self , u32 action);
    static u32 Agent_maximum_bits_needed                      (Agent* self );

    static void Agent_model_revert                            (Agent* self , AgentUndo* undo);
    static void Agent_model_update_percept                    ( Agent* self , u32 observation, u32 reward );


    static BitVector * Agent_encode_percept                   ( Agent* self, u32 observation, u32 reward);

    static double Agent_percept_probability                   (Agent* self, u32 observation, u32 reward);
    static double Agent_playout                             (Agent* self, u32 horizon);

    static u32 Agent_search                                 ( Agent* self);
    static void  Agent_reset                                  ( Agent* self );
#endif
