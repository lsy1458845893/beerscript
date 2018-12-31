#include "bsparse.h"

typedef struct bsast_type_s {
  bsu8_t istok : 1;
  bsu8_t type : 7;
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

typedef struct bsast_ir_s {
  bssize_t str_tab_size;
  bsstring_t **str_tab;
  bsast_t **code;
} bsast_ir_t;

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


