#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "bit_vector.h"

BitVector *bv_create() {
  BitVector *bit_vector = (BitVector *) malloc(sizeof(BitVector));
  bit_vector->size = 0;
  bit_vector->capacity = DEFAULT_BIT_VECTOR_CAPACITY;
  bool *bits = (bool *) malloc(bit_vector->capacity * sizeof(bool));
  assert(bits != NULL);
  bit_vector->bits = bits;
  return bit_vector;
}

void bv_free(BitVector *bv) {
  free(bv->bits);
  free(bv);
}

BitVector *bv_clone(BitVector *original) {
  BitVector *clone = bv_create();
  uint64_t i;
  for (i = 0; i < original->size; i++) {
    bv_push(clone, bv_test(original, i));
  }
  return clone;
}

BitVector *bv_from_bool(bool symbol) {
  BitVector *bv = bv_create();
  bv_push(bv, symbol);
  return  bv;
}

BitVector *bv_from_char(char c) {
  BitVector *bv = bv_create();
  int64_t j;
  for (j=sizeof(char) * 8 - 1; j >= 0; j--) {
    bv_push(bv, (c >> j) % 2 == 1);
  }
  return bv;
}

BitVector *bv_from_u32(uint32_t v) {
  BitVector *bv = bv_create();
  int64_t j;
  for (j=sizeof(uint32_t) * 8 - 1; j >= 0; j--) {
    bv_push(bv, (v >> j) % 2 == 1);
  }
  return bv;
}

BitVector *bv_from_u64(uint64_t v) {
  BitVector *bv = bv_create();
  int64_t j;
  for (j=sizeof(uint64_t) * 8 - 1; j >= 0; j--) {
    bv_push(bv, (v >> j) % 2 == 1);
  }
  return bv;
}

BitVector *bv_from_str(char *str) {
  assert(str != NULL);
  BitVector *bv = bv_create();
  uint64_t i = 0;
  while (str[i] != NULL) {
    char c = str[i];
    assert(c == '1' || c == '0');
    bv_push(bv, c == '1');
    i++;
  }
  return bv;
}

BitVector *bv_from_double(double d) {
  // interpret the double bits as an int
  uint64_t v = *((uint64_t *) &d);
  return bv_from_u64(v);
}

BitVector *bv_from_float(float f) {
  // interpret the float bits as an int
  uint32_t v = *((uint32_t *) &f);
  return bv_from_u32(v);
}

uint64_t bv_as_u64(BitVector *bv) {
  uint64_t result = 0;
  int64_t i = sizeof(uint64_t) * 8;
  if (bv->size < i) {
    i = bv->size;
  }
  for (; i > 0; i--) {
    result = result << 1;
    if (bv_test(bv, bv->size - i)) {
      result += 1;
    }
  }
  return result;
}

void bv_append(BitVector *a, BitVector *b) {
  uint64_t i;
  for (i = 0; i < b->size; i++) {
    bv_push(a, bv_test(b, i));
  }
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

bool bv_peek(BitVector *bv) {
  assert(bv->size > 0);
  return bv->bits[bv->size-1];
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

bool bv_eq(BitVector *a, BitVector *b) {
  if (a->size != b->size) {
    return false;
  }
  uint64_t i;
  for (i = 0; i < a->size; i++) {
    if (bv_test(a, i) != bv_test(b, i)) {
      return false;
    }
  }
  return true;
}

char *bv_str(BitVector *bv) {
  char *result = (char *) malloc(sizeof(char) * bv->size + 1);
  uint64_t i;
  for (i = 0; i < bv->size; i++) {
    if (bv_test(bv, i)) {
      result[i] = '1';
    } else {
      result[i] = '0';
    }
  }
  result[bv->size] = NULL;
  return result;
}

void bv_print(BitVector *bv) {
  if (bv->size > 400) {
    printf("Size: %llu\n", bv->size);
    return;
  }
  uint64_t i;
  for (i = 0; i < bv->size; i++) {
    if (i % 32 == 0) {
      printf(" ");
    }
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

void bv_save(BitVector *bv, FILE *fp) {
  fwrite(&(bv->size), sizeof(uint64_t), 1, fp);
  uint64_t i;
  for (i = 0; i < bv->size; i++) {
    bool bit = bv_test(bv, i);
    fwrite(&bit, sizeof(bool), 1, fp);
  }
}

void bv_load(BitVector *bv, FILE *fp) {
  bv_clear(bv);
  uint64_t size;
  fread(&size, sizeof(uint64_t), 1, fp);
  uint64_t i;
  for (i = 0; i < size; i++) {
    bool bit;
    fread(&bit, sizeof(bool), 1, fp);
    bv_push(bv, bit);
  }
}

BitVector *bv_slice(BitVector *bv, uint64_t start, uint64_t end) {
  // inclusive start, exclusive end
  BitVector *slice = bv_create();
  for (uint64_t i = start; i < end; i++) {
    bv_push(slice, bv_test(bv, i));
  }
  return slice;
}

BitVector *bv_truncate(BitVector *bv, uint64_t size) {
  return bv_slice(bv, 0, size);
}
