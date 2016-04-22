////////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com 
// FILE:     coin_flip.h
// CREATED:  2016-04-21 16:23:23
// MODIFIED: 2016-04-21 20:46:38
////////////////////////////////////////////////////////////////////
#ifndef COIN_FLIP_H
    #define COIN_FLIP_H
    #include <stddef.h>

    extern const void * Coin_Flip; 
    // Usage
    //      new ( Coin_Flip , (double) probability );
    //
    

    //GAME CONFIG VARS///////////////////////////////////////////////////
    const double  Default_Probability = 7e-1;
    const u32     Tails               = 0x00000000;
    const u32     Heads               = 0x00000001;
    const u08     Number_States       = 0x00000002;     


