#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bit_vector.h"
#include "context_tree.h"

int main(int argc, char **argv) {
  printf("repl for testing CTW\n");
  printf("Size of ct_node %lu\n", sizeof(ContextTreeNode));
  char pattern[1000];
  ContextTree *tree = ctw_create(8 * 20);
  for (;;) {
    fgets(pattern, sizeof(pattern), stdin);
    if (pattern[0] == 'h') {
      ctw_print(tree);
      continue;
    }
    pattern[strlen(pattern)-1] = 0; // remove the \n
    int32_t i;
    for (i = 0; i < strlen(pattern); i++) {
      char c = pattern[i];
      BitVector *v = bv_from_char(c);
      ctw_update_vector(tree, v);
      bv_free(v);
    }
    printf("Prediction:\n");
    BitVector *prediction = ctw_gen_random_symbols(tree, 8 * 100);
    bv_print_ascii(prediction);
    bv_free(prediction);
    //ctw_clear(tree);
  }
  return 0;
}

