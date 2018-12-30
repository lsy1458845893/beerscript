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

typedef struct bsast_ir_s {
  bssize_t str_tab_size;
  bsstring_t **str_tab;
  bsast_t **code;
} bsast_ir_t;

bsast_ir_t *bsparse_parse(bsctx_t *c, bsstream_t *s);

void bsast_ir_destory(bsctx_t *c, bsast_t *node);

#endif
