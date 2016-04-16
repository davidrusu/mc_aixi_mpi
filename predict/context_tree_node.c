#include <math.h>
#include <stdinf.h>

typedef struct ContextTreeNode {
  double log_kt;
  double log_probability;
  uint32_t ones_in_history;
  uint32_t zeroes_in_history;
  struct ContextTreeNode *zero_child;
  struct ContextTreeNode *one_child;
} ContextTreeNode;

ContextTreeNode *ctw_create_node() {
  ContextTreeNode *node = (ContextTreeNode *) malloc(sizeof(ContextTreeNode));
  node->log_kt = 0.0;
  node->log_probability = 0.0;
  node->ones_in_history = 0;
  node->zeroes_in_history = 0;
  node->zero_child = NULL;
  node->one_child = NULL;
  return node;
}

void ctw_destroy_node(ContextTreeNode *node) {
  if (node == NULL) {
    return;
  }
  ctw_destroy_node(node->zero_child);
  ctw_destroy_node(node->one_child);
  free(node);
}

bool ctw_is_leaf_node(ContextTreeNode *node) {
  return node->zero_child == NULL && node->one_child == NULL;
}

double ctw_log_kt_multiplier(ContextTreeNode *node, bool symbol) {
  uint32_t numerator;
  if (symbol) {
    // symbol is 1
    numerator = node->ones_in_history;
  } else {
    // symbol is 0
    numerator = node->zeroes_in_history;
  }

  uint32_t denominator = ctw_visits(node);
  return log((numerator + 0.5) / (denomeinator + 1.0));
}

uint32_t ctw_visits(ContextTreeNode *node) {
  return node->ones_in_history + node->zeroes_in_history;
}

void ctw_node_revert(ContextTreeNode *node, bool symbol) {
  // This is called in a loop from leaf to root, so we know that the
  // node's children have already been treated
  
  if (symbol && node->ones_in_history > 0) {
    // symbol is 1
    node->ones_in_history -= 1;
  } else if (!symbol && node->zeroes_in_history > 0) {
    // symbol is 0
    node->zeroes_in_history -= 1;
  }

  // need to remove redundant nodes, since this has already been called on
  // the node's children, they may have 0 visits now
  if (symbol) {
    if (node->one_child != NULL && ctw_visits(node->one_child) == 0) {
      free(node->one_child);
      node->one_child = NULL;
    }
  } else {
    if (node->zero_child != NULL && ctw_visits(node->zero_child) == 0) {
      free(node->zero_child);
      node->zero_child = NULL;
    }
  }

  node->log_kt -= ctw_log_kt_multiplier(node, symbol);
  ctw_update_log_probability(node);
}

uint32_t ctw_node_size(ContextTreeNode *node) {
  uint32_t zero_size = 0;
  if (node->zero_child != NULL) {
    zero_size = ctw_node_size(node->zero_child);
  }
  
  uint32_t one_size = 0;
  if (node->one_child != NULL) {
    one_size = ctw_node_size(node->one_child);
  }

  return 1 + zero_size + one_size;
}

void ctw_update_log_probability(ContextTreeNode *node) {
  if (ctw_is_leaf_node(node)) {
    node->log_probability = node->log_kt;
  } else {
    double log_child_prob = 0.0;
    if (node->zero_child != null) {
      log_child_prob += node->zero_child->log_probability;
    }
    if (node->one_child != null) {
      log_child_prob += node->one_child->log_probability;
    }

    double a, b;
    if (node->log_kt >= log_child_prob) {
      a = node->log_kt;
      b = log_child_prob;
    } else {
      a = log_child_prob;
      b = node->log_kt;
    }
    node->log_probability = log(0.5) + a + log1p(exp(b - a));
  }
}

void ctw_node_update(ContextTreeNode *node, bool symbol) {
  node->log_kt + ctw_log_kt_multiplier(node, symbol);

  ctw_update_log_probability(node, symbol);
  
  if (symbol) {
    node->ones_in_history += 1;
  } else {
    node->zeroes_in_history += 1;
  } 
}
