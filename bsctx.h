#ifndef BSCTX_H
#define BSCTX_H

#include "bsdep.h"

typedef struct bsctx_s bsctx_t;

typedef void *(*bsctx_alloc_t)(void *udat, void *sp, bssize_t size, char *file, char *func, int line);
typedef void (*bsctx_cleanup_t)(void *udat);

struct bsctx_s {
  void *udat;
  bsctx_alloc_t alloc;
  bsctx_cleanup_t cleanup;
  bsjbuf_t *jbuf;
};

static inline bsret_t bsctx_init(bsctx_t *c, bsctx_alloc_t alloc, bsctx_cleanup_t cleanup, void *udat) {
  c->udat = udat;
  c->alloc = alloc;
  c->cleanup = cleanup;
  c->jbuf = 0;
  return 0;
}

static inline void bsctx_deinit(bsctx_t *c) {
  if (c->cleanup)
    c->cleanup(c->udat);
}

#define bs_alloc(c, sp, size) bs_alloc_debug(c, sp, size, (char *)__FILE__, (char *)__FUNCTION__, __LINE__)

static inline void *bs_alloc_debug(bsctx_t *c, void *sp, bssize_t size, char *file, char *func, int line) {
  if (c->alloc)
    return c->alloc(c->udat, sp, size, file, func, line);
  else
    return 0;
}

#endif
