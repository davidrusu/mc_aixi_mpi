/*
  Utility functions
 */

#include "bit_vector.h"

BitVector* util_encode(uint64_t integer_symbol, int bit_count) {
  BitVector* vector = bv_from_uint64_t(integer_symbol);
  return vector;
}
