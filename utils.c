/*
  Utility functions
 */

#include "bit_vector.h"

BitVector* util_encode(uint64_t integer_symbol) {
  BitVector* vector = bv_from_u64(integer_symbol);
  return vector;
}
