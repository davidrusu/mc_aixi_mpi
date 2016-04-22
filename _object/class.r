////////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com
// FILE:     class.r
// CREATED:  2016-04-21 12:03:42
// MODIFIED: 2016-04-21 23:54:25
////////////////////////////////////////////////////////////////////

#ifndef CLASS_R
#define CLASS_R

#include <stdarg.h>
#include "../_utils/types.h"

struct Class {
    size_t  size;
    void *  ( * __init__ )              ( void * self, va_list      args );
    void *  ( * __delete__ )            ( void * self );
    void *  ( * __copy__ )              ( const void * self );
    void *  ( * __str__ )               ( const void * self );
};



#endif
