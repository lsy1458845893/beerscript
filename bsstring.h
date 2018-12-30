#ifndef BSSTRING_H
#define BSSTRING_H

#include "bsctx.h"
#include "bsdep.h"

enum {
  BSSTRING_TYPE_C_CONST = 0,
  BSSTRING_TYPE_C_DYNAMIC,
  BSSTRING_TYPE_RAW_DYNAMIC,
};

typedef struct bsstring_s {
  bssize_t refer;
  bssize_t length;
  bsu8_t type;
  bsu8_t *buf;
} bsstring_t;

bsstring_t *bsstring_create_c_const(bsctx_t *c, const char *src);
bsstring_t *bsstring_create_c_buffer(bsctx_t *c, char *src);
bsstring_t *bsstring_create_c_buffer_len(bsctx_t *c, char *src, bssize_t length);
bsstring_t *bsstring_create_buffer(bsctx_t *c, char *src, bssize_t length);

void bsstring_refer(bsctx_t *c, bsstring_t *str);
void bsstring_unrefer(bsctx_t *c, bsstring_t *str);

#endif
