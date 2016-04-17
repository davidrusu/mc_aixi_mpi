#ifndef _CTW_TREE_
#define _CTW_TREE_


#include "context_tree_node.h"
#include "ctw_list.h"

typedef struct ContextTree {
  uint32_t depth;
  ContextTreeNode *root;
  BitVector *history;
  CTWNodeList *context;
} ContextTree;

ContextTree *ctw_create(uint32_t);

void ctw_free(ContextTree *);

void ctw_clear(ContextTree *);

uint64_t ctw_size(ContextTree *);

void ctw_print(ContextTree *);

void ctw_update_context(ContextTree *);

void ctw_revert(ContextTree *, uint64_t);

void ctw_update_symbol(ContextTree *, bool);

void ctw_update_vector(ContextTree *, BitVector *);

double ctw_predict_symbol(ContextTree *, bool);

double ctw_predict_vector(ContextTree *, BitVector *);

BitVector *ctw_gen_random_symbols_and_update(ContextTree *, uint64_t);

BitVector *ctw_gen_random_symbols(ContextTree *, uint64_t);

void ctw_revert_history(ContextTree *, uint64_t);

#endif
