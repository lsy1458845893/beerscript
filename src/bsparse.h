#ifndef BSCOMPILE_H
#define BSCOMPILE_H

#include "bsctx.h"
#include "bsdep.h"
#include "bslex.h"

typedef struct bsast_type_s {
  bsu16_t istok : 1;
  bsu16_t type : 15;
} bsast_type_t;

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

typedef struct bsparse_s {
  bslex_t lex;
  bssize_t top, size;
  bsast_t **stack;
} bsparse_t;

static inline void bsparse_init(bsparse_t *p, bsstream_t *s) {
  bslex_init(&p->lex, s);
  p->top = p->size = 0;
  p->stack = 0;
}

void bsparse_deinit(bsctx_t *c, bsparse_t *p);
void bsast_destory(bsctx_t *c, bsast_t *node);


#endif
