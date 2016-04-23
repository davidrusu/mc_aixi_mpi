#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bit_vector.h"
#include "context_tree.h"

void load_from_file(ContextTree *tree) {
  char file_name[1000];
  printf("Enter a file name: ");
  fgets(file_name, sizeof(file_name), stdin);
  file_name[strlen(file_name)-1] = 0;
  FILE *fp = fopen(file_name, "r");
  if (fp == NULL) {
    printf("File not found '%s'\n", file_name);
    return;
  }
  char line[1000];
  uint64_t line_num = 0;
  while (fgets(line, sizeof(line), fp) != NULL) {
    line_num += 1;
    if (line_num % 100 == 0) {
      printf("\rprocessing line %llu\t", line_num);
    }
    int32_t i;
    for (i = 0; i < strlen(line); i++) {
      char c = line[i];
      BitVector *v = bv_from_char(c);
      ctw_update_vector(tree, v);
      bv_free(v);
    }
  }
  fclose(fp);
}

int main(int argc, char **argv) {
  printf("repl for testing CTW\n");
  printf("Size of ct_node %lu\n", sizeof(ContextTreeNode));
  char pattern[1000];
  ContextTree *tree = ctw_create(8*30);
  
  for (;;) {
    if (fgets(pattern, sizeof(pattern), stdin) == NULL) {
      continue;
    }
    pattern[strlen(pattern)-1] = 0; // remove \n
    if (strcmp(pattern, ":?") == 0) {
      ctw_print(tree);
      continue;
    } else if (strcmp(pattern, ":s") == 0) {
      //save the tree
      ctw_save(tree, "./tree.dat");
      //ctw_node_print(tree->root);
      continue;
    } else if (strcmp(pattern, ":l") == 0) {
      ctw_load(tree, "./tree.dat");
      //ctw_node_print(tree->root);
      continue;
    } else if (strcmp(pattern, ":f") == 0) {
      load_from_file(tree);
      continue;
    }
    int32_t i;
    for (i = 0; i < strlen(pattern); i++) {
      char c = pattern[i];
      BitVector *v = bv_from_char(c);
      ctw_update_vector(tree, v);
      bv_free(v);
    }
    printf("Prediction:\n");
    BitVector *prediction = ctw_gen_random_symbols_and_update(tree, 8 * 100);
    bv_print_ascii(prediction);
    bv_free(prediction);
    
    //ctw_clear(tree);
  }
  return 0;
}
