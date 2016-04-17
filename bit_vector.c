#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_BIT_VECTOR_CAPACITY 16
#define BIT_VECTOR_GROW_RATE 2

typedef struct BitVector {
  uint64_t size;
  uint64_t capacity;
  bool *bits;
} BitVector;

BitVector *create_bit_vector() {
  BitVector *bit_vector = (BitVector *) malloc(sizeof(BitVector));
  bit_vector->size = 0;
  bit_vector->capacity = DEFAULT_BIT_VECTOR_CAPACITY;
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
  uint64_t new_capacity = bv->capacity * BIT_VECTOR_GROW_RATE;
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

void bv_push(BitVector *bv, bool bit) {
  if (bv->size == bv->capacity) {
    __bv_grow(bv);
  }
  bv->bits[bv->size] = bit;
  bv->size += 1;
}

bool bv_pop(BitVector *bv) {
  assert(bv->size > 0);
  bool bit = bv->bits[bv->size-1];
  bv->size -= 1;
  return bit;
}

void bv_clear(BitVector *bv) {
  //we just reset the size to 0, we may want to shrink the list later
  bv->size = 0;
}

void bv_print(BitVector *bv) {
  printf("Size: %llu\n", bv->size);
  if (bv->size > 100) {
    return;
  }
  uint64_t i;
  for (i = 0; i < bv->size; i++) {
    if (bv_test(bv, i)) {
      printf("1");
    } else {
      printf("0");
    }
  }
  printf("\n");
}

void bv_print_ascii(BitVector *bv) {
  uint64_t i;
  for (i = 0; i < bv->size - (bv->size % 8); i += 8) {
    char c = 0;
    uint64_t j;
    for (j = 0; j < 8; j++) {
      c = c << 1;
      if (bv_test(bv, j+i)) {
	c += 1;
      }
    }
    printf("%c", c);
  }
  printf("\n");
}
