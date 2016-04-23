////////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com 
// FILE:     1d_maze.c
// CREATED:  2016-04-23 01:54:24
// MODIFIED: 2016-04-23 03:37:42
////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include "../_utils/types.h"
#include "../_utils/macros.h"
#include "class.h"
#include "class.r"
#include "environment.h"
#include "environment.r"
#include "1d_maze.h"
#include "1d_maze.r"

#define in ,
const u32 _N_ACTIONS        = 0x00000003;
const enum _ACTIONS { STAY, LEFT, RIGHT };

const u32 _N_REWARDS        = 0x00000003;
const enum _REWARDS { LOSS, WIN, WALL };

const u32 _N_ELEMENTS       = 0x00000004;
const enum _ELEMENTS { EMPTY, PLAYER, EXIT, WALL };

#define MAX_SPACES  16 //u32 / 2 bits for each element
const u32 _N_POSSIBLE_SPACES = 0x00000009;


const u32 _N_POSSIBLE_BOARDS = ((_N_SPACES_BOARD - 2) * (_N_SPACES_BOARD - 3));

void * CF_init ( void * _self, va_list * args ) 
{
    TRACE("Setting up environment","CF_init");

    struct Coin_Flip * self =
        ((const struct Class *) Environment) -> __init__( _self , args );
    
    if ( MAX_SPACES < _N_SPACES_BOARD ) {
        TRACE(" TO MANY SPACES ON BOARD "," CF_INIT ");
        exit(-1);
    }
    
    BLOCK_START
        u32 * _POSSIBLE_BOARDS = generate_boards();
        self->_._valid_observations = malloc ( sizeof(u32) * _N_POSSIBLE_BOARDS );
    
        foreach(u32 * ob in _POSSIBLE_BOARDS) 
        {   self->_._valid_observations = *ob;
            #ifdef DEBUG 
                TRACE("VALID BOARD = %x\n",*ob);
            #endif
        }
        free(_POSSIBLE_BOARDS);
    BLOCK_END

    BLOCK_START
        #ifdef DEBUG 
            TRACE("Setting up valid actions","CF_init");
        #endif
        self->_._valid_actions = malloc ( sizeof(u32) * _N_ACTIONS );
        _ACTIONS _a[_N_ACTIONS];
        for ( u08 i = 0; i < _N_ACTIONS; i++ )
            self->_._valid_actions[i] = _a[i];

        self -> _ . num_actions  = _N_ACTIONS;
    BLOCK_END
        
    BLOCK_START
        #ifdef DEBUG 
            TRACE("Setting up valid rewards","CF_init");
        #endif
        self->_._valid_rewards = malloc ( sizeof(u32) * _N_REWARDS );
        _REWARDS _r[_N_REWARDS];
        for ( u08 i = 0; i < _N_REWARDS; i++ )
            self->_._valid_rewards[i] = _a[i];

        self -> _ . num_rewards  = _N_REWARDS;
    BLOCK_END
   
    
   srand(time(NULL));
   u32 random_index = rand() % _N_POSSIBLE_BOARDS;
   self->_._observation = self->_._valid_observations[random_index];
    
   return self;
}

u32 * generate_boards(void) {
    u32 * tmp = malloc ( sizeof(u32) * _N_POSSIBLE_BOARDS );
    u08 player_space = 1;
    u08 exit_space = 2;

    foreach(u32 *board, tmp) {
        //build walls
        bit_set(0,*board); bit_set(1,*board);
        bit_set((_N_POSSIBLE_SPACES * 2)-1,*board);
        bit_set((_N_POSSIBLE_SPACES * 2)-2,*board);
        
        if (exit_space > (_N_POSSIBLE_SPACES - 1))
        {    player_space++;
             exit_space = 1;
        }
        if (exit_space == player_space )
            exit_space++;
        
        // set player and exit
        bit_set((player_space * 2) - 1,*board);
        bit_set((exit_space * 2), *board);
    }

    return tmp;
}

void bit_set ( u08 bit , u32 * bstring ){     bstring =         
u32 __rand_d (u08 start , u08 finish ){     return rand()%(start-finish)+start; } 
double __rp() { return (double) rand() / (double)RAND_MAX; }
