#ifndef _DICT_H_
#define _DICT_H_

#include <stddef.h>
#include <stdlib.h>

typedef struct MonteNode MonteNode;

typedef struct dict_entry_s {
    int key;
    MonteNode* value;
} dict_entry_s;

typedef struct dict_s {
    int len;
    int cap;
    dict_entry_s *entry;
} dict_s, *dict_t;

int dict_find_index(dict_t, const int);

MonteNode* dict_find(dict_t, const int);

void dict_add(dict_t, const int, MonteNode*);

dict_t dict_new(void);

void dict_free(dict_t);

#endif
