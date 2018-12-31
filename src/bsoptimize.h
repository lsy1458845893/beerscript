#ifndef BSOPTIMIZE_H
#define BSOPTIMIZE_H

#define BSIR_TYPE_TABLE(T)                                                                                             \
  T(ADD)                                                                                                               \
  T(SUB)                                                                                                               \
  T(DIV)                                                                                                               \
  T(MUL)                                                                                                               \
  T(MOD)

#include "bsdep.h"

typedef struct bsir_code_s {
  bsu8_t type;
} bsir_code_t;

typedef struct bsir_block_s {
  bssize_t size;
  bsir_code_t *code;
} bsir_block_t;

typedef struct bsir_func_s {
  bssize_t bsize;
  bsir_block_t *block;
} bsir_func_t;

#endif
