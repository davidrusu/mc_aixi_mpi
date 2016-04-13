/*
 * Vaughan Hilts
 * Implements a basic modified hash table for C; no generics and only used for monte-carlo searches.
 * This implementation has NOT been tested fully yet...
 */

// http://stackoverflow.com/a/16129177/1554844
// modified from the above source, uses linear probing & lacks RAII

#include <stddef.h>
#include <stdlib.h>
#include "monte_node.c"

typedef struct dict_entry_s {
    int key;
    MonteNode* value;
} dict_entry_s;

typedef struct dict_s {
    int len;
    int cap;
    dict_entry_s *entry;
} dict_s, *dict_t;

int dict_find_index(dict_t dict, const int key) {
    for (int i = 0; i < dict->len; i++) {
        if (dict->entry[i].key == key) {
            return i;
        }
    }
    return -1;
}

MonteNode* dict_find(dict_t dict, const int key) {
    int idx = dict_find_index(dict, key);
    return idx == -1 ? NULL : dict->entry[idx].value;
}

void dict_add(dict_t dict, const int key, MonteNode* value) {
    int idx = dict_find_index(dict, key);
    if (idx != -1) {
        dict->entry[idx].value = value;
        return;
    }
    if (dict->len == dict->cap) {
        dict->cap *= 2;
        dict->entry = realloc(dict->entry, dict->cap * sizeof(dict_entry_s));
    }
    dict->entry[dict->len].key = key;
    dict->entry[dict->len].value = value;
    dict->len++;
}

dict_t dict_new(void) {
    dict_s proto = {0, 10, malloc(10 * sizeof(dict_entry_s))};
    dict_t d = malloc(sizeof(dict_s));
    *d = proto;
    return d;
}

void dict_free(dict_t dict) {

    // be sure to cleanup all these pointers...
    // todo: RAII violation? Technically, the dictionary did not create these...
    // #WhoCares
    for (int i = 0; i < dict->len; i++) {
        free(dict->entry[i].value);
    }

    free(dict->entry);
    free(dict);
}