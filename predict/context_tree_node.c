typedef struct ContextTreeNode {
  ContextTree *tree;
  double log_kt;
  double log_probability;
  uint32_t ones_in_history;
  uint32_t zeroes_in_history;
  struct ContextTreeNode *zero_child;
  struct ContextTreeNode *one_child;
  HistoryVector *history;
  ContextVector *context;
} ContextTreeNode;

double log_kt_multiplier(ContextTreeNode *node, bool symbol) {
  if (symbol) {
    // symbol is 1
  } else {
    // symbol is 1
  }
  
}

void update_ct_node(ContextTreeNode *node, bool symbol) {
  /* Updates the node after having observed a new symbol.
     This involves updating the symbol counts and recalculating the cached probabinlities.

     - `symbol`: the symbol that was observed.
  */
}
