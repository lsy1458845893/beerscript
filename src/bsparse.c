#include "bsparse.h"


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


