#include "context_tree_node.c"

typedef struct CTNodeList {
  uint64_t size;
  uint64_t capacity;
  ContextTreeNode **nodes;
} CTNodeList;

CTNodeList *create_ct_node_list() {
  CTNodeList *node_list = (CTNodeList *) malloc(sizeof(CTNodeList));
  node_list->size = 0;
  node_list->capacity = DEFAULT_BIT_VECTOR_CAPACITY;
  ContextTreeNode **nodes = (ContextTreeNode **) malloc(node_list->capacity * sizeof(ContextTreeNode *));
  assert(nodes != NULL);
  node_list->nodes = nodes;
  return node_list;
}

void destroy_ct_node_list(CTNodeList *ct_list) {
  // we do not free the nodes in the list, those nodes need to be freed from
  // the ContextTree
  free(ct_list->nodes);
  free(ct_list);
}

void __ct_list_check_bounds(CTNodeList *ct_list, uint64_t index) {
  if (index >= ct_list->size) {
    printf("CT, Index out of bounds, index: %llu size: %llu\n", index, ct_list->size);
    assert(false);
  }
}

void __ct_list_grow(CTNodeList *ct_list) {
  uint64_t new_capacity = ct_list->capacity * 2;
  ContextTreeNode *new_nodes = (ContextTreeNode *) malloc(new_capacity * sizeof(ContextTreeNode));
  uint64_t i;
  for (i = 0; i < ct_list->size; i++) {
    new_nodes[i] = ct_list->nodes[i];
  }
  
  free(ct_list->nodes);
  ct_list->capacity = new_capacity;
  ct_list->nodes = new_nodes;
}

ContextTreeNode *ct_list_get(CTNodeList *ct_list, uint64_t index) {
  __ct_list_check_bounds(ct_list, index);
  return ct_list->nodes[index];
}

void ct_list_set(CTNodeList *ct_list, uint64_t index, ContextTreeNode *node) {
  __ct_list_check_bounds(ct_list, index);
  ct_list->nodes[index] = node;
}

void ct_list_push(CTNodeList *ct_list, ContextTreeNode *node) {
  if (ct_list->size == ct_list->capacity) {
    __ct_list_grow(ct_list);
  }
  ct_list->nodes[ct_list->size] = bit;
  ct_list->size += 1;
}

ContextTreeNode *ct_list_pop(CTNodeList *ct_list) {
  assert(ct_list->size > 0);
  ContextTreeNode *node = ct_list->nodes[ct_list->size-1];
  ct_list->size -= 1;
  return node;
}

void ct_list_clear(CTNodeList *ct_list) {
  // Just sets size to 0
  // we may want to resize the array if memory is a problem
  ct_list->size = 0;
}
