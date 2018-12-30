#ifndef BSSTREAM_H
#define BSSTREAM_H

#include "bsdep.h"

typedef struct bsstream_s bsstream_t;
typedef bsu8_t (*bsstream_read_t)(bsstream_t *s);
typedef void (*bsstream_write_t)(bsstream_t *s, bsu8_t ch);
typedef void (*bsstream_cleanup_t)(bsstream_t *s);

struct bsstream_s {
  void *udat;
  bsu8_t readable : 1;
  bsu8_t writeable : 1;
  bsu8_t isopen : 1;
  bsstream_read_t read;
  bsstream_write_t write;
  bsstream_cleanup_t cleanup;
};

#endif
