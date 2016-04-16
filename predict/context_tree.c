#include "context_tree_node.c"
#include "../bit_vector.c"
#include "ct_node_list.c"

typedef struct ContextTree {
  uint32_t depth;
  ContextTreeNode *root;
  BitVector *history;
  CTNodeList *context;
} ContextTree;

ContextTree *ctw_create_tree(uint32_t depth) {
  ContextTree *tree = (ContextTree *) malloc(sizeof(ContextTree));
  tree->depth = depth;
  tree->root = ctw_create_node();
  tree->history = create_bit_vector();
  tree->context = create_ct_node_list();
}

void ctw_destroy_tree(ContextTree *tree) {
  ctw_destroy_node(tree->root);
  destroy_bit_vector(tree->history);
  destroy_ct_node_list(tree->context);
  free(tree);
}

void ctw_clear(ContextTree *tree) {
  bv_clear(tree->history);
  ctw_destroy_node(tree->root);
  tree->root = ctw_create_node();
  ct_list_clear(tree->context);
}

BitVector *ctw_gen_random_symbols(ContextTree *tree, uint64_t n) {
  BitVector *symbols = ctw_gen_random_symbols_and_update(tree, n);
  ctw_revert(tree, n);
  return symbols;
}

BitVector *ctw_gen_random_symbols_and_update(ContextTree *tree, uint64_t n) {
  BitVector *symbols = create_bit_vector();
  uint64_t i;
  for (i = 0; i < n; i++) {
    double p = ((double) rand()) / ((double) RAND_MAX);
    bool symbol;
    if (p < ctw_predict(tree, true)) {
      symbol = true;
    } else {
      symbol = false;
    }
    bv_push(symbols, symbol);
    ctw_update_symbol(tree, symbol);
  }
  return symbols;
}

double ctw_predict_symbol(ContextTree *tree, bool symbol) {
  if (tree->history->size + 1 <= tree->depth) {
    return 0.5;
  }
  double prob_history = tree->root->log_probability;
  ctw_update_symbol(tree, symbol);
  double prob_sequence = tree->root->log_probability;
  ctw_revert(tree, 1);
  return exp(prob_sequence - prob_history);
}

double cte_predict_vector(ContextTree *tree, BitVector *symbols) {
  if (tree->history->size + symbols->size <= tree->depth) {
    return pow(0.5, symbols->size);
  }

  double prob_history = tree->root->log_probability;
  ctw_update_vector(tree, symbol_list);
  double prob_sequence = tree->root->log_probability;
  ctw_revert(tree, symbols->size);
  return exp(prob_sequence - prob_history);
}

void ctw_revert(ContextTree *tree, uint64_t n) {
  uint64_t i;
  for (i = 0; i < n; i++) {
    if (tree->history->size == 0) {
      return;
    }
    bool symbol = bv_pop(tree->history);

    if (tree->history->size >= tree->depth) {
      ctw_update_contect(tree);

      uint64_t j;
      for (j = tree->depth-1; j >= 0; j--) {
	ctw_node_revert(ct_list_get(tree->context, j), symbol);
      }
    }
  }
}

void ctw_revert_history(ContextTree *tree, uint64_t n) {
  // Shrinks the history without affecting the context tree.

  asser(tree->history->size >= n);
  // TODO: this is not very efficient, should add a function to bitvector
  uint64_t i;
  for (i = 0; i < n; i++) {
    bv_pop(tree->history);
  }
}

uint64_t ctw_size(ContextTree *tree) {
  return ctw_node_size(tree->root);
}

void ctw_update_symbol(ContextTree *tree, bool symbol) {
  if (tree->history->size >= tree->depth) {
    ctw_update_context(tree);
    uint64_t i;
    for (i = tree->depth-1; i >= 0; i++) {
      ctw_node_update(ct_list_get(tree->context, i), symbol);
    }
  }
  bv_push(tree->history, symbol);
}

void ctw_update_vector(ContextTree *tree, BitVector *symbols) {
  uint64_t i;
  for (i = 0; i < symbols->size; i++) {
    bool symbol = bv_test(symbols, i);
    ctw_update_context(tree, symbol);
  }
}

vod ctw_update_context(ContextTree *tree) {
  assert(tree->history->size >= tree->depth);
  destroy_ct_node_list(tree->context);
  tree->context = create_ct_node_list();
  ct_list_push(tree->context, tree->root);
  ContextTreeNode *node = tree->root;
  uint64_t update_depth = 1;
  uint64_t i;
  for (i = tree->history->size-1; i >= 0; i--) {
    bool symbol = bv_test(tree->history, i);

    if (symbol && node->one_child != NULL) {
      node = node->one_child;
    } else if (!symbol && node->zero_child != NULL) {
      node = node->zero_child;
    } else {
      ContextTree new_node = ctw_create_node();
      if (symbol) {
	node->one_child = new_node;
      } else {
	node->zero_child = new_node;
      }
      node = new_node;
    }
    ct_list_push(tree->context, node);
    update_depth += 1;
    if (update_depth > tree->depth) {
      break;
    }
  }
}
