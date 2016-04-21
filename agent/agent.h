#ifndef AGENT_H
    #define AGENT_H
    
    extern const void * Agent;

    static AgentUndo* clone_into_temp                   (Agent* agent)

    static double  average_reward                       ( void * _self );

    static u32  generate_random_action                  ( void * _self );
    static u32  maximum_action                          ( void * _self );
    static u32  maximum_reward                          ( void * _self );
    static u32  model_size                              ( void * _self );

    static void  model_update_action                    ( void * _self );
    static BitVector * encode_action                    ( void * _self, u32 action );

    // decoding
    static u32 decode_action                            (void * _self, BitVector* symbols);
    static u32 decode_observation                       (void * _self, BitVector* symbols);
    static u32 decode_reward                            (void * _self, BitVector* symbols);
    static u32Tuple* decode_percept                     (void * _self, BitVector* symbols);

    // generators
    static u32 generate_action                          (void  * _self);
    static u32 generate_percept                         (void * _self);
    u32Tuple * generate_percept_and_update              (void * _self);

    static u32 history_size                             (void * _self);
    static BitVector * get_predicted_action_probability (void * _self, u32 action);
    static u32 maximum_bits_needed                      (void * _self)

    static void model_revert                            (void* _self, AgentUndo* undo);
    static void model_update_percept                    ( void * _self, u32 observation, u32 reward );


    static BitVector * encode_percept                   ( void * _self, u32 observation, u32 reward);

    static double percept_probability                   (void * _self, u32 observation, u32 reward);
    static double * playout                             (void * _self, u32 horizon);

    static void  search                                 ( void * _self );
    static void  reset                                  ( void * _self );


#endif
