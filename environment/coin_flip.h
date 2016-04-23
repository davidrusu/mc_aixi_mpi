////////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com 
// FILE:     coin_flip.h
// CREATED:  2016-04-21 16:23:23
// MODIFIED: 2016-04-22 22:15:04
////////////////////////////////////////////////////////////////////

#ifndef COIN_FLIP_H
    #define COIN_FLIP_H
    #define HEADS_VAL 0x00000001
    #define TAILS_VAL 0x00000000

    #include <stddef.h>

    extern const void * Coin_Flip; 
    // Usage
    //      new ( Coin_Flip , (double) probability );
    //
    

    //GAME CONFIG VARS///////////////////////////////////////////////////
    //const double  Default_Probability = 7e-1;
    //const u32     Tails               = HEADS_VAL;
    //const u32     Heads               = TAILS_VAL;
    //const u32     Loss                = TAILS_VAL;
    //const u32     Win                 = HEADS_VAL;
    //const u08     Number_States       = 0x00000002;

    double __rp();
    u32Tuple * perform_action ( void * _self, u32 action_t );
    static void CF_print(void * _self);

#endif

