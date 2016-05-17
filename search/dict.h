#ifndef _DICT_H_
#define _DICT_H_

#include <stddef.h>
#include <stdlib.h>

typedef struct MonteNode MonteNode;

typedef struct dict_entry_s {
    void * key;
    MonteNode* value;
} dict_entry_s;

typedef struct dict_s {
    int len;
    int cap;
    dict_entry_s *entry;
} dict_s, *dict_t;

int dict_find_index(dict_t, void *);

MonteNode* dict_find(dict_t, void *);

void dict_add(dict_t, void *, MonteNode *);

dict_t dict_new();

void dict_free(dict_t);

#endif
