////////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com 
// FILE:     coin_flip.h
// CREATED:  2016-04-21 16:23:23
// MODIFIED: 2016-04-22 17:54:18
////////////////////////////////////////////////////////////////////

#ifndef 1D_MAZE_H
    #define 1D_MAZE_H
    #include <stddef.h>

    extern const void * 1D; 
    // Usage
    //      new ( 1D , (u32) Squares );
    //
    
   
    //GAME CONFIG VARS///////////////////////////////////////////////////
    const u16 SPACES_TOTAL      =   9;
    const u16 SPACES_MOVEABLE   =   SPACES_TOTAL - 2;
    const u16 POSSIBLE_BOARDS   =   SPACES_MOVEABLE * (SPACES_MOVEABLE -1)

    /// actions
    const u08 NONE              =   0;
    const u08 LEFT              =   1;
    const u08 RIGHT             =   2;

    /// board
    const u08 WALL              =   3;
    const u08 PLAYER            =   2;
    const u08 EXIT              =   1;
    const u08 EMPTY             =   0;



    double __rp();
    static u32Tuple * perform_action ( void * _self, u32 action_t );
    static void CF_print(void * _self);

#endif

