#ifndef NEW_H
#define NEW_H

#include <stddef.h>

void * new      ( const void * class, ... );
void delete     ( void * item );
void * cpy      ( const void * self );
void * print    ( const void * self );

#endif
