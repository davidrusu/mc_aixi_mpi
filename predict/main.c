#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "context_tree.c"
//#include "../bit_vector.c"

int main(int argc, char **argv) {
  printf("repl for testing CTW\n");
  char pattern[1000];
  ContextTree *tree = ctw_create_tree(10);
  for (;;) {
    scanf("%s", pattern);
    int32_t i;
    for (i = 0; i < strlen(pattern); i++) {
      ctw_update_symbol(tree, pattern[i] == '1');
    }
    printf("Prediction:\n");
    BitVector *prediction = ctw_gen_random_symbols(tree, 50);
    bv_print(prediction);

    destroy_bit_vector(prediction);
    //ctw_clear(tree);
  }
  return 0;
}
