#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "context_tree_node.h"
#include "ctw_list.h"

CTWNodeList *ctw_list_create() {
  CTWNodeList *ctw_list = (CTWNodeList *) malloc(sizeof(CTWNodeList));
  ctw_list->size = 0;
  ctw_list->capacity = 8;
  ContextTreeNode **nodes = (ContextTreeNode **) malloc(ctw_list->capacity * sizeof(ContextTreeNode *));
  if (nodes == NULL) {
    perror("Failed to allocate nodes list");
  }
  
  ctw_list->nodes = nodes;
  return ctw_list;
}

void ctw_list_free(CTWNodeList *ctw_list) {
  // we do not free the nodes in the list, those nodes need to be freed from
  // the ContextTree
  free(ctw_list->nodes);
  free(ctw_list);
}

void __ctw_list_check_bounds(CTWNodeList *ctw_list, uint64_t index) {
  if (index >= ctw_list->size) {
    fprintf(stderr, "CT, Index out of bounds, index: %llu size: %llu\n", index, ctw_list->size);
    perror("Wowowowow. Out of bounds mannn");
  }
}

void __ctw_list_grow(CTWNodeList *ctw_list) {
  uint64_t new_capacity = ctw_list->capacity * 2;
  ContextTreeNode **new_nodes = (ContextTreeNode **) malloc(new_capacity * sizeof(ContextTreeNode *));
  uint64_t i;
  for (i = 0; i < ctw_list->size; i++) {
    new_nodes[i] = ctw_list->nodes[i];
  }
  
  free(ctw_list->nodes);
  ctw_list->capacity = new_capacity;
  ctw_list->nodes = new_nodes;
}

ContextTreeNode *ctw_list_get(CTWNodeList *ctw_list, uint64_t index) {
  __ctw_list_check_bounds(ctw_list, index);
  return ctw_list->nodes[index];
}

void ctw_list_set(CTWNodeList *ctw_list, uint64_t index, ContextTreeNode *node) {
  __ctw_list_check_bounds(ctw_list, index);
  ctw_list->nodes[index] = node;
}

void ctw_list_push(CTWNodeList *ctw_list, ContextTreeNode *node) {
  if (ctw_list->size == ctw_list->capacity) {
    __ctw_list_grow(ctw_list);
  }
  ctw_list->nodes[ctw_list->size] = node;
  ctw_list->size += 1;
}

ContextTreeNode *ctw_list_pop(CTWNodeList *ctw_list) {
  if (ctw_list->size == 0) {
    perror("The list is empty, can't pop\n");
  }
  ContextTreeNode *node = ctw_list->nodes[ctw_list->size-1];
  ctw_list->size -= 1;
  return node;
}

void ctw_list_push_front(CTWNodeList *ctw_list, ContextTreeNode *node) {
  if (ctw_list->size == ctw_list->capacity) {
    __ctw_list_grow(ctw_list);
  }
  uint64_t i;
  for (i = ctw_list->size; i > 0; i--) {
    ctw_list->nodes[i] = ctw_list->nodes[i-1];
  }
  ctw_list->nodes[0] = node;
  ctw_list->size += 1;
}

ContextTreeNode *ctw_list_pop_front(CTWNodeList *ctw_list) {
  if (ctw_list->size == 0) {
    perror("The list is empty, can't pop\n");
  }
  ContextTreeNode *node = ctw_list->nodes[0];

  uint64_t i;
  for (i = 0; i < ctw_list->size-1; i++) {
    ctw_list->nodes[i] = ctw_list->nodes[i+1];
  }
  ctw_list->size -= 1;

  return node;
}

void ctw_list_clear(CTWNodeList *ctw_list) {
  // Just sets size to 0
  // we may want to resize the array if memory is a problem
  ctw_list->size = 0;
}
