////////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com
// FILE:     coin_flip.c
// CREATED:  2016-04-21 12:03:42
// MODIFIED: 2016-04-21 12:03:42
////////////////////////////////////////////////////////////////////

typedef enum { Tails , Heads } _action_enum;
typedef enum { Tails , Heads } _observation_enum;
typedef enum { Loss , Win } _reward_enum;

double default_probability = 7e-1;

struct Coin_flip {
    void * class;   // must be first;
    struct Environment * _;
    

};

void * CF_init ( void * _self, va_list * args ) 
{
    struct Coin_flip * self = _self;
    self -> _ ->__init__(self-> _ , // FINISH THIS WHEN YOUR AWWAKE
    //───────████  
    //───────███ 
    //▄█████▄███  
    //─▀████████   
    //---──▄████▄
    
}
