#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "../bit_vector.h"
#include "ctw_list.h"
#include "context_tree.h"

ContextTree *ctw_create(uint32_t depth) {
  ContextTree *tree = (ContextTree *) malloc(sizeof(ContextTree));
  tree->depth = depth;
  tree->root = ctw_node_create();
  tree->history = bv_create();
  tree->context = ctw_list_create();
  return tree;
}

void ctw_free(ContextTree *tree) {
  ctw_node_free(tree->root);
  bv_free(tree->history);
  ctw_list_free(tree->context);
  free(tree);
}

void ctw_clear(ContextTree *tree) {
  bv_clear(tree->history);
  ctw_node_free(tree->root);
  tree->root = ctw_node_create();
  ctw_list_clear(tree->context);
}

uint64_t ctw_size(ContextTree *tree) {
  return ctw_node_size(tree->root);
}

void ctw_print(ContextTree *tree) {
  printf("Context Tree { depth: %d, size: %llu}\n", tree->depth, ctw_size(tree));
  printf("History: ");
  bv_print(tree->history);
}

void ctw_update_context(ContextTree *tree) {
  if (tree->history->size < tree->depth) {
    perror("Not enough history to update context\n");
  }
  ctw_list_free(tree->context);
  tree->context = ctw_list_create();
  ctw_list_push(tree->context, tree->root);
  ContextTreeNode *node = tree->root;
  uint64_t update_depth = 1;
  int64_t i;
  for (i = tree->history->size-1; i >= 0; i--) {
    bool symbol = bv_test(tree->history, i);

    if (symbol && node->one_child != NULL) {
      node = node->one_child;
    } else if (!symbol && node->zero_child != NULL) {
      node = node->zero_child;
    } else {
      ContextTreeNode *new_node = ctw_node_create();
      if (symbol) {
	node->one_child = new_node;
      } else {
	node->zero_child = new_node;
      }
      node = new_node;
    }
    ctw_list_push(tree->context, node);
    update_depth += 1;
    if (update_depth > tree->depth) {
      break;
    }
  }
}

void ctw_revert(ContextTree *tree, uint64_t n) {
  uint64_t i;
  for (i = 0; i < n; i++) {
    if (tree->history->size == 0) {
      return;
    }
    
    bool symbol = bv_pop(tree->history);
    
    if (tree->history->size >= tree->depth) {
      ctw_update_context(tree);
      
      int64_t j;
      for (j = tree->depth-1; j >= 0; j--) {
	ctw_node_revert(ctw_list_get(tree->context, j), symbol);
      }
    }
  }
}

void ctw_update_symbol(ContextTree *tree, bool symbol) {
  if (tree->history->size >= tree->depth) {
    ctw_update_context(tree);
    int64_t i;
    for (i = tree->depth-1; i >= 0; i--) {
      ctw_node_update(ctw_list_get(tree->context, i), symbol);
    }
  }
  bv_push(tree->history, symbol);
}

void ctw_update_vector(ContextTree *tree, BitVector *symbols) {
  uint64_t i;
  for (i = 0; i < symbols->size; i++) {
    bool symbol = bv_test(symbols, i);
    ctw_update_symbol(tree, symbol);
  }
}

void ctw_update_history(ContextTree *tree, BitVector *symbols) {
  bv_append(tree->history, symbols);
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

double ctw_predict_vector(ContextTree *tree, BitVector *symbols) {
  if (tree->history->size + symbols->size <= tree->depth) {
    return pow(0.5, symbols->size);
  }

  double prob_history = tree->root->log_probability;
  ctw_update_vector(tree, symbols);
  double prob_sequence = tree->root->log_probability;
  ctw_revert(tree, symbols->size);
  return exp(prob_sequence - prob_history);
}

BitVector *ctw_gen_random_symbols_and_update(ContextTree *tree, uint64_t n) {
  BitVector *symbols = bv_create();
  uint64_t i;
  for (i = 0; i < n; i++) {
    double p = ((double) rand()) / ((double) RAND_MAX);
    bool symbol;
    if (p < ctw_predict_symbol(tree, true)) {
      symbol = true;
    } else {
      symbol = false;
    }
    bv_push(symbols, symbol);
    ctw_update_symbol(tree, symbol);
  }
  return symbols;
}

BitVector *ctw_gen_random_symbols(ContextTree *tree, uint64_t n) {
  BitVector *symbols = ctw_gen_random_symbols_and_update(tree, n);
  ctw_revert(tree, n);
  return symbols;
}

void ctw_revert_history(ContextTree *tree, uint64_t n) {
  // Shrinks the history without affecting the context tree.

  if (tree->history->size < n) {
    perror("not enough history to revert\n");
  }
  // TODO: this is not very efficient, should add a function to bitvector
  uint64_t i;
  for (i = 0; i < n; i++) {
    bv_pop(tree->history);
  }
}

void ctw_save(ContextTree *tree, char *file_name) {
  uint64_t count = 0;
  
  FILE *fp = fopen(file_name, "w");
  fwrite(&(tree->depth), sizeof(uint32_t), 1, fp);
  bv_save(tree->history, fp);
  
  CTWNodeList *stack = ctw_list_create();
  ctw_list_push(stack, tree->root);
  while (stack->size > 0) {
    ContextTreeNode *node = ctw_list_pop(stack);
    fwrite(&(node->log_kt), sizeof(double), 1, fp);
    fwrite(&(node->log_probability), sizeof(double), 1, fp);
    fwrite(&(node->ones_in_history), sizeof(uint32_t), 1, fp);
    fwrite(&(node->zeroes_in_history), sizeof(uint32_t), 1, fp);

    bool zero_child;
    bool one_child;
    
    if (node->one_child != NULL) {
      ctw_list_push(stack, node->one_child);
      one_child = true;
    } else {
      one_child = false;
    }
    
    if (node->zero_child != NULL) {
      ctw_list_push(stack, node->zero_child);
      zero_child = true;
    } else {
      zero_child = false;
    }
    
    fwrite(&zero_child, sizeof(bool), 1, fp);
    fwrite(&one_child, sizeof(bool), 1, fp);
    count += 1;
  }
  fclose(fp);
  printf("Wrote %llu nodes to %s\n", count, file_name);
}

void ctw_load(ContextTree *tree, char *file_name) {
  ctw_clear(tree);
  
  uint64_t count = 0;
  FILE *fp = fopen(file_name, "r");
  fread(&(tree->depth), sizeof(uint32_t), 1, fp);
  bv_load(tree->history, fp);
  printf("Read history\n");
  
  CTWNodeList *stack = ctw_list_create();
  ContextTreeNode *parent = NULL;
  do {
    ContextTreeNode *node = ctw_node_create();
    if (stack->size == 0 && parent == NULL) {
      tree->root = node;
    }
    if (parent != NULL) {
      parent->zero_child = node;
    } else if (stack->size != 0) {
      ContextTreeNode *one_parent = ctw_list_pop(stack);
      one_parent->one_child = node;
    }
    
    fread(&(node->log_kt), sizeof(double), 1, fp);
    fread(&(node->log_probability), sizeof(double), 1, fp);
    fread(&(node->ones_in_history), sizeof(uint32_t), 1, fp);
    fread(&(node->zeroes_in_history), sizeof(uint32_t), 1, fp);
    
    bool zero_child;
    fread(&zero_child, sizeof(bool), 1, fp);
    if (zero_child) {
      parent = node;
    } else {
      parent = NULL;
    }

    bool one_child;
    fread(&one_child, sizeof(bool), 1, fp);
    if (one_child) {
      ctw_list_push(stack, node);
    }
    
    count += 1;
  } while (stack->size > 0 || parent != NULL);
  fclose(fp);
  
  printf("Read %llu nodes to %s\n", count, file_name);
}
