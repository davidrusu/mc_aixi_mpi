////////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com
// FILE:     class.c
// CREATED:  2016-04-21 12:03:42
// MODIFIED: 2016-04-21 12:03:42
////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdlib.h>

#include "class.h"
#include "class.r"

void * new ( const void * _class, ... )
{
    const struct Class * class = _class;
    void * mem = calloc( 1, class->size );
    
    //TODO: Replace with better error handling.
    assert ( mem );
    
    * ( const struct Class ** ) mem = class;
    
    // This handles and vars passed to the constructor.
    if ( class->__init__ )
    {
        va_list args;

        va_start( args, _class );                 // intialize '...'
          mem = class->__init__( mem, &args );    // call constructor
        va_end( args );                           // clean
    }
    
    return mem;
}

void delete ( void * self )
{
    const struct Class ** parent = self;

    if ( self && * parent && ( * parent )->__delete__ )
        self = ( * parent )->__delete__(self);
    
    free(self)
}

void * cpy ( const void * self )
{
    const struct Class * const * parent = self;

    assert ( self && parent && ( * parent )->__copy__);
    return ( * parent )->__copy__(self);
}

void * print ( const void * self )
{
    const struct Class * const * parent = self;
    
    printf("%s",self->__str__(self));
    
    return p_string;
}
