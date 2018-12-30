

#include "../bsctx.h"
#include "memory-inspector.h"

#include <stdio.h>

#include "../bserr.h"
#include "../bslex.h"
#include "../bsstring.h"

typedef struct const_str_stream {
  char *str;
  uint16_t cursor;
} const_str_stream_t;

bsu8_t const_str_read(bsstream_t *s) {
  const_str_stream_t *c = (const_str_stream_t *)(s->udat);
  // printf("read ch %x:%c\n", c->str[c->cursor], c->str[c->cursor]);
  if (c->str[c->cursor])
    return c->str[c->cursor++];
  else {
    s->isopen = 0;
    return 0;
  }
}

#define GET_ERROR_STRING(n) #n,
#define GET_TOKEN_STRING(n, e, s) e,

const char *err_tab[] = {BSERROR_TABLE(GET_ERROR_STRING)};
const char *type_tab[] = {BSLEX_SYMBOL_LIST(GET_TOKEN_STRING) BSLEX_KEYWORD_LIST(GET_TOKEN_STRING)};

const char *src = "import test; // import test !\n"
                  "\"test\";/*test*/\n"
                  "test &= 01 | 0x32 + 0.345 * this;\n"
                  "if (var i in obj) console.log(i);\n"
                  "[ 345, 0xF5, .34, 34 ^ 7];\n";

int main(int argc, char const *argv[]) {
  memory_inspector_t inspector = {0, 0, 0};
  bsctx_t ctx, *c = &ctx;
  bslex_t _lex, *lex = &_lex;
  bsctx_init(c, memory_inspector_alloc, memory_inspector_cleanup, (void *)&inspector);
  const_str_stream_t cstr = {.str = src, .cursor = 0};
  bsstream_t stream = {.udat = &cstr, .isopen = 1, .readable = 1, .read = const_str_read, .cleanup = 0};
  bsjbuf_t jbuf;
  c->jbuf = &jbuf;
  bslex_init(lex, &stream);
  bsret_t err;
  if (!(err = bs_setjmp(&jbuf))) {
    bsu16_t type;
    while (type = bslexi_next(c, lex)) {
      switch (type) {
      case BSTOK_FLOAT:
        printf("<float %f> ", bslex_get_fnum(lex));
        break;
      case BSTOK_INT:
        printf("<int %d:0x%x> ", bslex_get_inum(lex));
        break;
      case BSTOK_ID:
        printf("<id:%s> ", bslex_get_str(lex)->buf);
        break;
      case BSTOK_STRING:
        printf("<str:'%s'> ", bslex_get_str(lex)->buf);
        break;
      default:
        printf("<%s> ", type_tab[type]);
      }
    }
    printf("\n");
  } else
    printf("ERROR:%s !\n", err_tab[err]);
  bslex_deinit(c, lex);
  bsctx_deinit(c);
  return 0;
}
