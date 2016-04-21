#define CTW_ITER_STATE_0 0
#define CTW_ITER_STATE_1 1
#define CTW_ITER_STATE_2 2
#define CTW_ITER_STATE_3 3
#define CTW_ITER_STATE_DONE 4

// WARNING: do not modify the tree at all while you are iterating
typedef struct {;
  ContextTree *tree;
  CTWNodeList *stack;
  uint8_t state;
  uint64_t history_index;
} CTWIterator;

CTWIterator *ctw_iter_create(ContextTree *tree) {
  CTWIterator *iter = (CTWIterator *) malloc(sizeof(CTWIterator));
  iter->tree = tree;
  iter->stack = ctw_list_create();
  ctw_list_push(tree->stack, tree->root);
  iter->state = CTW_ITER_STATE_0;
  iter->history_index = 0;
}

void ctw_iter_free(CTWIterator *iter) {
  ctw_list_free(iter->stack);
  free(iter);
}

bool ctw_has_next(CTWIter *iter) {
  return iter->stare != CTW_ITER_STATE_DONE;
}

BitString *ctw_iter_next(CTWIter *iter) {
  BitString *result = bv_create();
  uint64_t hist_lp_end;
  switch(iter->state) {
  case CTW_ITER_STATE_0:
    result = iter->tree->depth; // depth is actually 32bit but w/e
    iter->state = CTW_ITER_STATE_1;
    break;
  case CTW_ITER_STATE_1:
    result = iter->history->size;
    iter->state = CTW_ITER_STATE_2;
    break;
  case CTW_ITER_STATE_2:
    hist_lp_end = iter->history_index + 64;
    if (hist_lp_end > iter->history->size) {
      hist_lp_end = iter->history->size;
    }
    for (; iter->history_index < hist_lp_end; iter->history_index++) {
      result = result << 1;
      if (bv_test(iter->history, iter->history_index)) {
	result += 1;
      }
    }
    if (iter->history_index == iter->history->size) {
      iter->state = CTW_ITER_STATE_3;
    }
    break;
  case CTW_ITER_STATE_3:
    
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
    
    if (iter->stack->size == 0) {
      iter->state = CTW_ITER_STATE_DONE;
    }
    break;
  default:
    perror("ILLEGAL STATE EXCEPTION\n");
  }
  return result;
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
