#include "bsstring.h"

bsstring_t *bsstring_create_c_const(bsctx_t *c, const char *src) {
  bsstring_t *str = bs_alloc(c, 0, sizeof(bsstring_t));
  if (str) {
    str->buf = src;
    str->length = bs_strlen(src);
    str->type = BSSTRING_TYPE_C_CONST;
    str->refer = 1;
    return str;
  } else
    return 0;
}

bsstring_t *bsstring_create_c_buffer_len(bsctx_t *c, char *src, bssize_t length) {
  bsstring_t *str = bs_alloc(c, 0, sizeof(bsstring_t));
  if (str) {
    str->length = length;
    str->buf = bs_alloc(c, 0, str->length + 1);
    if (str->buf) {
      bs_memcpy(str->buf, src, str->length);
      str->buf[str->length] = 0;
      str->type = BSSTRING_TYPE_C_DYNAMIC;
      str->refer = 1;
      return str;
    } else
      return 0;
  } else
    return 0;
}

bsstring_t *bsstring_create_c_buffer(bsctx_t *c, char *src) {
  return bsstring_create_c_buffer_len(c, src, bs_strlen(src));
}

bsstring_t *bsstring_create_buffer(bsctx_t *c, char *src, bssize_t length) {
  for (bssize_t i = 0; i < length; i++) {
    if (src[i] == 0) {
      bsstring_t *str = bs_alloc(c, 0, sizeof(bsstring_t));
      if (str) {
        str->length = length;
        str->buf = bs_alloc(c, 0, str->length);
        if (str->buf) {
          bs_memcpy(str->buf, src, str->length);
          str->type = BSSTRING_TYPE_RAW_DYNAMIC;
          str->refer = 1;
          return str;
        } else
          return 0;
      } else
        return 0;
    }
  }
  return bsstring_create_c_buffer_len(c, src, length);
}

void bsstring_refer(bsctx_t *c, bsstring_t *str) { str->refer++; }

void bsstring_unrefer(bsctx_t *c, bsstring_t *str) {
  str->refer--;
  if (str->refer == 0) {
    switch (str->type) {
    case BSSTRING_TYPE_C_DYNAMIC:
    case BSSTRING_TYPE_RAW_DYNAMIC:
      bs_alloc(c, str->buf, 0);
    }
    bs_alloc(c, str, 0);
  }
}
