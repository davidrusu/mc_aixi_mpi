#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "context_tree.c"

ContextTree *create_context_tree(uint64_t depth) {
  // Constructs a new context tree
  
  ContextTree *tree = (ContextTree *) malloc(sizeof(ContextTree));
  // do some more init
  return tree;
}

void update(ContextTree *tree, bool *bitstring, uint64_t bitstring_size) {
  // updates the CT on the newly observed bitstring
}

void update_history(ContextTree *tree, bool *bitstring, uint64_t bitstring_size) {
  // updates the tree's history to include the given bitstring
}

void clear(ContextTree *tree) {
  // clears the tree, essentially resets the tree to its starting state
}

bool *generate_random_bitstring(ContextTree *tree, uint64_t num_bits) {
  // generates a bitstring `num_bits` long sampled from the CT

  // for now we just generate a random bitstring
  bool *bits = (bool *) malloc(num_bits * sizeof(bool));
  uint64_t i;
  for (i = 0; i < num_bits; i++) {
    bits[i] = rand() % 2 == 0;
  }

  return bits;
}

double predict(ContextTree *tree, bool *bitstring, uint64_t bitstring_size) {
  // Returns the probability of the bitstring conditional on the CT's history
  
  // for now just returning a random probability
  return ((float) (rand() % 100)) / 100.0;
}


void revert(ContextTree *tree, uint64_t num_bits) {
  // reverts the context tree to the state it was at before it was trained on `num_bits` number of bits

  // does nothing for now
}

void revert_history(ContextTree *tree, uint64_t num_bits) {
  // shrinks the history without affecting the context tree.
}

uint64_t size(ContextTree *tree) {
  // returns the number of nodes in the context tree
  return rand() % 50000;
}

