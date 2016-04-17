#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "context_tree.c"
//#include "../bit_vector.c"

int main(int argc, char **argv) {
  printf("repl for testing CTW\n");
  char pattern[1000];
  ContextTree *tree = ctw_create_tree(8 * 30);
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
      int64_t j;
      for (j=sizeof(char) * 8 - 1; j >= 0; j--) {
	ctw_update_symbol(tree, (c >> j) % 2 == 1);
      }
    }
    printf("Prediction:\n");
    BitVector *prediction = ctw_gen_random_symbols(tree, 8 * 100);
    bv_print_ascii(prediction);

    destroy_bit_vector(prediction);
    //ctw_clear(tree);
  }
  return 0;
}

