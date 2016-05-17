#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bit_vector.h"
#include "context_tree.h"

#define BINARY_MODE 0
#define ASCII_MODE 1

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

  fseek(fp, 0, SEEK_END);
  uint64_t file_size = ftell(fp);
  rewind(fp); // move file pointer back to start of file
  printf("Reading '%s' size: %llu kb\n", file_name, file_size / 1024);
  char line[1000];
  uint64_t line_num = 0;
  while (fgets(line, sizeof(line), fp) != NULL) {
    line_num += 1;
    if (line_num % 2 == 0) {
      printf("\r");
      uint64_t i;
      for (i = 0; i < (ftell(fp) * 80) / (file_size); i++) {
	printf("#");
      }
    }
    int32_t i;
    BitVector *v = bv_create();
    for (i = 0; i < strlen(line); i++) {
      char c = line[i];
      BitVector *vc = bv_from_char(c);
      bv_append(v, vc);
      bv_free(vc);
    }
    ctw_update_vector(tree, v);
    bv_free(v);
  }
  
  fclose(fp);
  printf("\n");
}

#ifdef REPL
int main(int argc, char **argv) {
  printf("repl for testing CTW\n");
  char pattern[1000];
  ContextTree *tree = ctw_create(8 * 10);
  
  uint8_t mode = ASCII_MODE;
  
  for (;;) {
    if (mode == ASCII_MODE) {
      printf("ASCII ");
    } else if (mode == BINARY_MODE) {
      printf("BINARY ");
    } else {
      printf("UNKNOWN %u ", mode);
    }
    
    printf("> ");
    
    if (fgets(pattern, sizeof(pattern), stdin) == NULL) {
      exit(0);
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
    } else if (strcmp(pattern, ":m") == 0) {
      if (mode == BINARY_MODE) {
	mode = ASCII_MODE;
      } else {
	mode = BINARY_MODE;
      }
      continue;
    }

    bool valid = true;
    int32_t i;
    for (i = 0; i < strlen(pattern); i++) {
      char c = pattern[i];
      if (mode == ASCII_MODE) {
	BitVector *v = bv_from_char(c);
	ctw_update_vector(tree, v);
	bv_free(v);
      } else {
	if (c != '0' && c != '1') {
	  ctw_revert(tree, i);
	  printf("Only '0' and '1' are valid in binary mode\n");
	  valid = false;
	  break;
	}
	ctw_update_symbol(tree, c == '1');
      }
    }
    if (!valid) {
      continue;
    }
    
    printf("Prediction:\n");
    BitVector *prediction = ctw_gen_random_symbols(tree, 50 * 8);
    printf("%s", pattern);
    if (mode == ASCII_MODE) {
      bv_print_ascii(prediction);
    } else {
      bv_print(prediction);
    }
    bv_free(prediction);
  }
}
#endif