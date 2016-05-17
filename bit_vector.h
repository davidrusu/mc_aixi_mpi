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

BitVector *bv_clone(BitVector *);

BitVector *bv_from_bool(bool);

BitVector *bv_from_char(char);

BitVector *bv_from_u32(uint32_t);

BitVector *bv_from_u64(uint64_t);

void bv_append(BitVector *, BitVector *);

uint64_t bv_as_u64(BitVector *);

void __bv_check_bounds(BitVector *, uint64_t);

void __bv_grow(BitVector *);

bool bv_test(BitVector *, uint64_t);

void bv_set(BitVector *, uint64_t, bool);

void bv_push(BitVector *, bool);

bool bv_peek(BitVector *);

bool bv_pop(BitVector *);

void bv_clear(BitVector *);

bool bv_eq(BitVector *, BitVector *);

char *bv_str(BitVector *bv);

void bv_print(BitVector *);

void bv_print_ascii(BitVector *);

void bv_save(BitVector *, FILE *);

void bv_load(BitVector *, FILE *);

BitVector *bv_slice(BitVector *bv, uint64_t start, uint64_t end);

BitVector *bv_truncate(BitVector *bv, uint64_t size);

#endif
