#ifndef _CTW_NODE_
#define _CTW_NODE_

#include <stdbool.h>

typedef struct ContextTreeNode {
  double log_kt;
  double log_probability;
  uint32_t ones_in_history;
  uint32_t zeroes_in_history;
  struct ContextTreeNode *zero_child;
  struct ContextTreeNode *one_child;
} ContextTreeNode;

ContextTreeNode *ctw_node_create();

void ctw_node_free(ContextTreeNode *);

bool ctw_node_is_leaf(ContextTreeNode *);

uint32_t ctw_node_visits(ContextTreeNode *);

double ctw_node_log_kt_multiplier(ContextTreeNode *, bool);

void ctw_node_update_log_probability(ContextTreeNode *);

void ctw_node_revert(ContextTreeNode *, bool);

uint32_t ctw_node_size(ContextTreeNode *);

void ctw_node_update(ContextTreeNode *, bool);

void ctw_node_print(ContextTreeNode *);
#endif
