#ifndef _MONTE_NODE_H_
#define _MONTE_NODE_H_

#include "dict.h"

// pyaixi: class members
typedef struct MonteNode {
    double mean ;
    int type;
    int visits;

    // a dictionary of the children to be held, initialized by search.c
    dict_t children;
    dict_t actions;
} MonteNode;

#endif
