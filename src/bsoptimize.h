#ifndef BSOPTIMIZE_H
#define BSOPTIMIZE_H

#define BSIR_TYPE_TABLE(T)                                                                                             \
  T(NOP)                                                                                                               \
  T(ADD)                                                                                                               \
  T(SUB)                                                                                                               \
  T(DIV)                                                                                                               \
  T(MUL)                                                                                                               \
  T(MOD)                                                                                                               \
  T(SHL)                                                                                                               \
  T(SHR)                                                                                                               \
  T(AND)                                                                                                               \
  T(OR)                                                                                                                \
  T(NOT)                                                                                                               \
  T(XOR)                                                                                                               \
  T(NEG)                                                                                                               \
  T(INC)                                                                                                               \
  T(DEC)                                                                                                               \
  T(EQ)                                                                                                                \
  T(NE)                                                                                                                \
  T(GT)                                                                                                                \
  T(GE)                                                                                                                \
  T(LT)                                                                                                                \
  T(LE)                                                                                                                \
  T(LAND)                                                                                                              \
  T(LOR)                                                                                                               \
  T(LNOT)                                                                                                              \
  T(GET_ENV)                                                                                                           \
  T(SET_ENV)                                                                                                           \
  T(GET_PROTO)                                                                                                         \
  T(SET_PROTO)                                                                                                         \
  T(PUSH_TABLE)                                                                                                        \
  T(PUSH_CONST)                                                                                                        \
  T(PUSH_TRUE)                                                                                                         \
  T(PUSH_FALSE)                                                                                                        \
  T(PUSH_NULL)                                                                                                         \
  T(POP)                                                                                                               \
  T(JUMP)                                                                                                              \
  T(GOTO)                                                                                                              \
  T(TRY)                                                                                                               \
  T(THROW)                                                                                                             \
  T(RET)

#include "bsdep.h"
#include "bsstring.h"

typedef struct bsir_code_s {
  bsu8_t type;
} bsir_code_t;

typedef struct bsir_block_s {
  bssize_t size;
  bsir_code_t *code;
} bsir_block_t;

typedef struct bsir_func_s {
  bsstring_t *funcname;
  bsstring_t *filename;
  bssize_t bsize;
  bsir_block_t *block;
  bssize_t psize;
  bsstring_t **param;
  bssize_t fsize;
  struct bsir_func_s *func;
} bsir_func_t;

#endif
