#ifndef BSCOMPILE_H
#define BSCOMPILE_H

#include "bsctx.h"
#include "bsdep.h"
#include "bslex.h"

#define BSAST_TYPE_TABLE(T)                                                                                            \
  T(BIT_OR)                                                                                                            \
  T(BIT_AND)                                                                                                           \
  T(BIT_XOR)                                                                                                           \
  T(AND)                                                                                                               \
  T(OR)                                                                                                                \
  T(ADD)                                                                                                               \
  T(SUB)                                                                                                               \
  T(MUL)                                                                                                               \
  T(DIV)                                                                                                               \
  T(MOD)                                                                                                               \
  T(SHIFTL)                                                                                                            \
  T(SHIFTR)                                                                                                            \
  T(NEW)                                                                                                               \
  T(LOAD_CONSTANT)                                                                                                     \
  T(LOAD_IMMEDIATE)                                                                                                    \
  T(LOAD_THIS)                                                                                                         \
  T(LOAD_SUPER)                                                                                                        \
  T(LOAD_NULL)                                                                                                         \
  T(LOAD_TRUE)                                                                                                         \
  T(LOAD_FALSE)                                                                                                        \
  T(GET_PROTO)                                                                                                         \
  T(SET_PROTO)                                                                                                         \
  T(DEL_PROTO)                                                                                                         \
  T(GET_ENV)                                                                                                           \
  T(SET_ENV)                                                                                                           \
  T(DEL_ENV)                                                                                                           \
  T(RETURN)                                                                                                            \
  T(CONST_ENV)                                                                                                         \
  T(TRUE_JUMP)                                                                                                         \
  T(GOTO)                                                                                                              \
  T(LABEL)                                                                                                             \
  T(EXPORT)                                                                                                            \
  T(IMPORT)                                                                                                            \
  T(SWITCH)                                                                                                            \
  T(THROW)                                                                                                             \
  T(TRY)

typedef bsu16_t bsast_type_t;

typedef struct bsast_head_s {
  bsast_type_t type;
  bssize_t size;
  bssize_t line;
} bsast_head_t;

typedef struct bsast_s {
  bsast_head_t head;
  union {
    struct bsast_s *node[1];
    bsint_t inum;
    bsfloat_t fnum;
    bsstring_t *str;
  } u;
} bsast_t;

typedef struct bscompile_s {
  bslex_t lex;
  bssize_t top, size;
  bsast_t **stack;
} bscompile_t;

static inline void bscompile_init(bscompile_t *p, bsstream_t *s) {
  bslex_init(&p->lex, s);
  p->top = p->size = 0;
  p->stack = 0;
}

void bscompile_deinit(bsctx_t *c, bscompile_t *p);

#endif
