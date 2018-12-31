#ifndef BSCOMPILE_H
#define BSCOMPILE_H

#include "bsctx.h"
#include "bsdep.h"
#include "bslex.h"
#include "bsoptimize.h"


bsast_ir_t *bsparse_parse(bsctx_t *c, bsstream_t *s);

void bsast_ir_destory(bsctx_t *c, bsast_t *node);

#endif
