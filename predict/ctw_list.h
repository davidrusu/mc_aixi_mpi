#ifndef _CTW_LIST_
#define _CTW_LIST_

#include <stdint.h>
#include "context_tree_node.h"

typedef struct CTWNodeList {
  uint64_t size;
  uint64_t capacity;
  ContextTreeNode **nodes;
} CTWNodeList;

CTWNodeList *ctw_list_create();

void ctw_list_free(CTWNodeList *);

void __ctw_list_check_bounds(CTWNodeList *, uint64_t);

void __ctw_list_grow(CTWNodeList *);

ContextTreeNode *ctw_list_get(CTWNodeList *, uint64_t);

void ctw_list_set(CTWNodeList *, uint64_t, ContextTreeNode *);

void ctw_list_push(CTWNodeList *, ContextTreeNode *);

ContextTreeNode *ctw_list_pop(CTWNodeList *);

void ctw_list_clear(CTWNodeList *);

#endif
