#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_BIT_VECTOR_CAPACITY 16
#define DEFAULT_BIT_VECTOR_GROW_RATE 2.0

typedef struct BitVector {
  uint64_t size;
  uint64_t capacity;
  float grow_rate;
  bool *bits;
} BitVector;

BitVector *create_bit_vector() {
  BitVector *bit_vector = (BitVector *) malloc(sizeof(BitVector));
  bit_vector->size = 0;
  bit_vector->capacity = DEFAULT_BIT_VECTOR_CAPACITY;
  bit_vector->grow_rate = DEFAULT_BIT_VECTOR_GROW_RATE;
  bool *bits = (bool *) malloc(bit_vector->capacity * sizeof(bool));
  assert(bits != NULL);
  bit_vector->bits = bits;
  return bit_vector;
}

void destroy_bit_vector(BitVector *bv) {
  free(bv->bits);
  free(bv);
}

void __bv_check_bounds(BitVector *bv, uint64_t index) {
  if (index >= bv->size) {
    printf("BV, Index out of bounds, index: %llu size: %llu\n", index, bv->size);
    assert(false);
  }
}

void __bv_grow(BitVector *bv) {
  uint64_t new_capacity = (uint64_t) (bv->capacity * bv->grow_rate);
  if (new_capacity == bv->capacity) {
    // failed to grow, happens in cases like (int) (1 * 1.5) == 1
    new_capacity += 1;
  }
  bool *new_bits = (bool *) malloc(new_capacity * sizeof(bool));
  uint64_t i;
  for (i = 0; i < bv->size; i++) {
    new_bits[i] = bv->bits[i];
  }
  
  free(bv->bits);
  bv->capacity = new_capacity;
  bv->bits = new_bits;
}

bool bv_test(BitVector *bv, uint64_t index) {
  __bv_check_bounds(bv, index);
  return bv->bits[index];
}

void bv_set(BitVector *bv, uint64_t index, bool bit) {
  __bv_check_bounds(bv, index);
  bv->bits[index] = bit;
}

void push(BitVector *bv, bool bit) {
  if (bv->size == bv->capacity) {
    __bv_grow(bv);
  }
  bv->bits[bv->size] = bit;
  bv->size += 1;
}

bool pop(BitVector *bv) {
  assert(bv->size > 0);
  bool bit = bv->bits[bv->size-1];
  bv->size -= 1;
  return bit;
}

