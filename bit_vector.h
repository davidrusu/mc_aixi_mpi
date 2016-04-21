#ifndef _BIT_VECTOR_
#define _BIT_VECTOR_

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#define DEFAULT_BIT_VECTOR_CAPACITY 16
#define BIT_VECTOR_GROW_RATE 2

typedef struct BitVector {
  uint64_t size;
  uint64_t capacity;
  bool *bits;
} BitVector;

BitVector *bv_create();

void bv_free(BitVector *);

BitVector *bv_from_char(char);

void bv_append(BitVector *, BitVector *);

void __bv_check_bounds(BitVector *, uint64_t);

void __bv_grow(BitVector *);

bool bv_test(BitVector *, uint64_t);

void bv_set(BitVector *, uint64_t, bool);

void bv_push(BitVector *, bool);

bool bv_peek(BitVector *);

bool bv_pop(BitVector *);

void bv_clear(BitVector *);

void bv_print(BitVector *);

void bv_print_ascii(BitVector *);

void bv_save(BitVector *, FILE *);

void bv_load(BitVector *, FILE *);
#endif
