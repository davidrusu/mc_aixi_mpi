////////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com 
// FILE:     coin_flip.r
// CREATED:  2016-04-21 13:53:50
// MODIFIED: 2016-04-22 15:42:44
////////////////////////////////////////////////////////////////////
#ifndef COIN_FLIP_R
    #define COIN_FLIP_R
    #include "../_utils/types.h"
   struct Coin_Flip {
       struct Environment _;
       double probability;       
   };
    #define probability(e) (((const struct Coin_Flip*)(e)) -> probability)

    //typedef enum { Tails , Heads } _action_enum;
    //typedef enum { Tails , Heads } _observation_enum;
    //typedef enum { Loss , Win } _reward_enum;

    static double default_probability = 7e-1;
#endif
