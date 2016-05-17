#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../bit_vector.h"
#include "context_tree.h"

#define STR(x) #x

#define expect(expectation) printf("expect '%s': %s\n", STR(expectation), (expectation) ? "Passed" : "Failed");

uint64_t update_with_rand_int(ContextTree *tree) {
  BitVector *bv = bv_from_u64((uint64_t) rand());
  ctw_update_vector(tree, bv);
  uint64_t result = bv->size;
  bv_free(bv);
  return result;
}

void test_ctw_create() {
  printf("testing create\n");
  uint64_t depth = rand() % 100;
  ContextTree *tree = ctw_create(depth);
  
  expect(tree->depth == depth);
  expect(ctw_size(tree) == 1);
  expect(tree->history->size == 0);
}

void test_revert() {
  printf("testing revert\n");
  ContextTree *tree = ctw_create(10);

  expect(tree->history->size == 0);

  uint64_t update_size = update_with_rand_int(tree);

  double initial_prediction = ctw_predict_symbol(tree, true);
  
  expect(tree->history->size == update_size);
  expect(ctw_size(tree) == update_size);

  printf("%llu %llu\n", ctw_size(tree), update_size);
  
  ctw_revert(tree, update_size);
  expect(ctw_predict_symbol(tree, true) == initial_prediction);

  update_with_rand_int(tree);
  initial_prediction = ctw_predict_symbol(tree, true);
  uint64_t second_update_size = update_with_rand_int(tree);
  ctw_revert(tree, second_update_size);
  
  expect(ctw_predict_symbol(tree, true) == initial_prediction);
}

#ifdef TEST
int main(int argc, char **argv) {
  srand(0);
  test_ctw_create();
  test_revert();
}
#endif