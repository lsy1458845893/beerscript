#include "bslex.h"
#include "bserr.h"
#include <stdio.h>

static bsret_t bslex_resize(bsctx_t *c, bslex_t *lex) {
  bssize_t size = lex->size;
  if (lex->size <= 16) { // size <= 16
    if (lex->size < 4)
      lex->size = 4;
    while (lex->top >= lex->size)
      lex->size *= 2;
  } else if (lex->size <= 2048) { // 16 < size <= 2048
    while (lex->top >= lex->size)
      lex->size *= 2;
    while (lex->top * 4 < lex->size)
      lex->size /= 2;
  } else { // 2048 < size
    while (lex->top >= lex->size)
      lex->size += 1024;
    while (lex->top * 4 < lex->size)
      lex->size /= 2;
  }
  if (size != lex->size) {
    lex->buf = bs_alloc(c, lex->buf, lex->size);
    if (!lex->buf) {
      lex->top = lex->size = 0;
      lex->type = BSTOK_EOF;
      return BSERROR_NO_MEMORY;
    }
  }
  return 0;
}

static void bslexi_resize(bsctx_t *c, bslex_t *lex) {
  bsret_t err = bslex_resize(c, lex);
  if (err)
    bs_longjmp(c->jbuf, err);
}

void bslex_deinit(bsctx_t *c, bslex_t *lex) {
  if (lex->stream && lex->stream->cleanup) {
    lex->stream->cleanup(lex->stream);
    lex->stream = 0;
  }
  if (lex->buf)
    bs_alloc(c, lex->buf, 0);
}

static bsu8_t bslexi_stream_get(bsctx_t *c, bslex_t *lex) {
  if (lex->stream) {
    if (lex->stream->isopen) {
      if (lex->stream->readable) {
        if (lex->stream->read)
          return lex->stream->read(lex->stream);
        else { // throw error (lex->read == NULL)
          lex->lastch = 0;
          bs_longjmp(c->jbuf, BSERROR_LEX_STREAM_READ_FUNTION_IS_NULL);
        }
      } else { // throw error (lex->readable == 0){
        lex->lastch = 0;
        bs_longjmp(c->jbuf, BSERROR_LEX_STREAM_CANNOT_READ);
      }
    } else { // not open
      lex->lastch = 0;
      if (lex->stream->cleanup)
        lex->stream->cleanup(lex->stream);
      lex->stream = 0;
      return 0;
    }
  } else
    return '\0';
}

static bsu8_t bslexi__get(bsctx_t *c, bslex_t *lex) {
  bsu8_t ch = 0;
  if (lex->lastch) {
    ch = lex->lastch;
    lex->lastch = '\0';
    lex->lastch = bslexi_stream_get(c, lex);
  } else
    ch = lex->lastch = bslexi_stream_get(c, lex);

  if (lex->lastch == '\n') {
    lex->line++;
    lex->colunm = 1;
  } else
    lex->colunm++;
  return ch;
}

static bsu8_t bslexi__read(bsctx_t *c, bslex_t *lex) {
  if (lex->lastch)
    return lex->lastch;
  else {
    bsu8_t ch = lex->lastch = bslexi_stream_get(c, lex);
    if (ch == '\n') {
      lex->line++;
      lex->colunm = 1;
    } else
      lex->colunm++;
    return ch;
  }
}

static void bslexi_push(bsctx_t *c, bslex_t *lex, bsu8_t ch) {
  lex->buf[lex->top++] = ch;
  bslexi_resize(c, lex);
}

static inline bsu16_t bslex__clean_ret(bslex_t *lex, bsu16_t type) {
  lex->lastch = '\0';
  return lex->type = type;
}

static inline bsu8_t bslexi__match(bsctx_t *c, bslex_t *lex, bsu8_t ch) {
  if (bslexi__read(c, lex) == ch) {
    bslexi__get(c, lex);
    return 1;
  } else
    return 0;
}

#define bslex_clean(t) return bslex__clean_ret(lex, BSTOK_##t)
#define bslex_ret(t) return lex->type = BSTOK_##t
#define bslexi_read() bslexi__read(c, lex)
#define bslexi_get() bslexi__get(c, lex)
#define bslexi_match(ch) bslexi__match(c, lex, ch)

static bsu64_t bslexi_match_hex(bsctx_t *c, bslex_t *lex) { // start with <0x>
  bsu64_t i = 0;
  bsu8_t ch = bslexi_read();
  if ('0' <= ch && ch <= '9')
    i += ch - '0';
  else if ('a' <= ch && ch <= 'f')
    i += ch - 'a' + 10;
  else if ('A' <= ch && ch <= 'F')
    i += ch - 'A' + 10;
  else
    bs_longjmp(c->jbuf, BSERROR_LEX_UNEXCEPTED_TOKEN);
  bslexi_get();
  while (1) {
    ch = bslexi_read();
    if ('0' <= ch && ch <= '9') {
      i <<= 4;
      i += ch - '0';
    } else if ('a' <= ch && ch <= 'f') {
      i <<= 4;
      i += ch - 'a' + 10;
    } else if ('A' <= ch && ch <= 'F') {
      i <<= 4;
      i += ch - 'A' + 10;
    } else
      return i;
    bslexi_get();
  }
}

static bsu64_t bslexi_match_bin(bsctx_t *c, bslex_t *lex) { // start with <0x>
  bsu64_t i = 0;
  bsu8_t ch = bslexi_read();
  if ('0' <= ch && ch <= '1')
    i += ch - '0';
  else
    bs_longjmp(c->jbuf, BSERROR_LEX_UNEXCEPTED_TOKEN);
  bslexi_get();
  while (1) {
    ch = bslexi_read();
    if ('0' <= ch && ch <= '1') {
      i <<= 1;
      i += ch - '0';
    } else
      return i;
    bslexi_get();
  }
}

static bsu64_t bslexi_match_oct(bsctx_t *c, bslex_t *lex) { // start with <0>
  bsu64_t i = 0;
  bsu8_t ch = bslexi_read();
  while (1) {
    ch = bslexi_read();
    if ('0' <= ch && ch <= '7') {
      i <<= 3;
      i += ch - '0';
    } else
      return i;
    bslexi_get();
  }
}

static bsfloat_t bslexi_match_dec(bsctx_t *c, bslex_t *lex) { // start with <0>
  bsu8_t ch = bslexi_read();
  bsfloat_t t = 0.1;
  bsfloat_t f = 0;
  while (1) {
    ch = bslexi_read();
    if ('0' <= ch && ch <= '9') {
      bslexi_get();
      f += t * (ch - '0');
      t *= 0.1;
    } else
      return f;
  }
}

static bsfloat_t bslexi_match_int(bsctx_t *c, bslex_t *lex) { // start with <0-9>
  bsu8_t ch = bslexi_read();
  bsu64_t i = 0;
  while (1) {
    ch = bslexi_read();
    if ('0' <= ch && ch <= '9') {
      bslexi_get();
      i *= 10;
      i += ch - '0';
    } else
      return i;
  }
}

static void bslexi_match_id(bsctx_t *c, bslex_t *lex) { // start with <0-9>
  bsu8_t ch = bslexi_read();
  if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_' || ch == '$')
    bslexi_push(c, lex, ch);
  else
    bs_longjmp(c->jbuf, BSERROR_LEX_UNEXCEPTED_TOKEN);
  bslexi_get();
  while (1) {
    ch = bslexi_read();
    if (('0' <= ch && ch <= '9') || ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_' || ch == '$')
      bslexi_push(c, lex, bslexi_get());
    else
      return;
  }
}

enum {
#define BSLEX_KEYWORD_TABLE_INDEX(name, str, len) BSLEX_INDEX_##name,
  BSLEX_KEYWORD_LIST(BSLEX_KEYWORD_TABLE_INDEX)
#undef BSLEX_KEYWORD_TABLE_INDEX
};
static const char *bslex_tok_str[] = {
#define BSLEX_KEYWORD_TABLE_STRING(name, str, len) str,
    BSLEX_KEYWORD_LIST(BSLEX_KEYWORD_TABLE_STRING)
#undef BSLEX_KEYWORD_TABLE_STRING
};

static const bsu8_t bslex_tok_len[] = {
#define BSLEX_KEYWORD_TABLE_STRING(name, str, len) len,
    BSLEX_KEYWORD_LIST(BSLEX_KEYWORD_TABLE_STRING)
#undef BSLEX_KEYWORD_TABLE_STRING
};

#define bslex_type_check(type)                                                                                         \
  if (lex->top == bslex_tok_len[BSLEX_INDEX_##type] &&                                                                 \
      bs_memcmp(lex->buf, bslex_tok_str[BSLEX_INDEX_##type], lex->top) == 0)                                           \
    return BSTOK_##type;

static bsu16_t bslexi_match_keyword(bsctx_t *c, bslex_t *lex) {
  switch (lex->buf[0]) {
  case 'a':
    bslex_type_check(ASYNC);
    bslex_type_check(AWAIT);
    return 0;
  case 'b':
    bslex_type_check(BREAK);
    return 0;
  case 'c':
    bslex_type_check(CASE);
    bslex_type_check(CATCH);
    bslex_type_check(CLASS);
    bslex_type_check(CONST);
    bslex_type_check(CONTINUE);
    return 0;
  case 'd':
    bslex_type_check(DEFAULT);
    bslex_type_check(DELETE);
    bslex_type_check(DO);
    return 0;
  case 'e':
    bslex_type_check(ELSE);
    bslex_type_check(EXPORT);
    bslex_type_check(EXTENDS);
    return 0;
  case 'f':
    bslex_type_check(FALSE);
    bslex_type_check(FOR);
    bslex_type_check(FUNCTION);
    return 0;
  case 'i':
    bslex_type_check(IF);
    bslex_type_check(IMPORT);
    bslex_type_check(IN);
    bslex_type_check(INSTANCEOF);
    return 0;
  case 'l':
    bslex_type_check(LET);
    return 0;
  case 'n':
    bslex_type_check(NEW);
    bslex_type_check(NULL);
    return 0;
  case 'r':
    bslex_type_check(RETURN);
    return 0;
  case 's':
    bslex_type_check(STATIC);
    bslex_type_check(SUPER);
    bslex_type_check(SWITCH);
    return 0;
  case 't':
    bslex_type_check(THIS);
    bslex_type_check(THROW);
    bslex_type_check(TRUE);
    bslex_type_check(TRY);
    return 0;
  case 'v':
    bslex_type_check(VAR);
    return 0;
  case 'w':
    bslex_type_check(WHILE);
    return 0;
  default:
    return 0;
  }
}

static bsu8_t bslex_str_backslash(bsu8_t ch) {
  switch (ch) {
  case '0':
    return '\0';
  case 'a':
    return '\a';
  case 'b':
    return '\b';
  case 'f':
    return '\f';
  case 'n':
    return '\n';
  case 'r':
    return '\r';
  case 't':
    return '\t';
  case 'v':
    return '\v';
  default:
    return ch;
  }
}

#include <stdio.h>

bsu16_t bslexi_next(bsctx_t *c, bslex_t *lex) {
  if (!c->jbuf)
    return BSERROR_LEX_JBUF_CANNOT_NULL;
  switch (lex->type) { // cleanup last
  case BSTOK_ID:
  case BSTOK_STRING:
    bsstring_unrefer(c, lex->u.str);
  }
  lex->type = BSTOK_EOF;
  bslexi_resize(c, lex);
  if (lex->top != 0)
    bs_longjmp(c->jbuf, BSERROR_LEX_BUFFER_NOT_CLEAN);
  if (lex->lastch || lex->stream) { // lexer next start
    bsu8_t ch = 0;
  check:
    ch = bslexi_read();
    switch (ch) {
    case ' ':
    case '\n':
    case '\r': // empty space
      bslexi_get();
      goto check;

    case '\0':
      bslex_clean(EOF);

    case '=':
      bslexi_get();
      if (bslexi_match('='))
        bslex_clean(EQ_EQ); // "=="
      else if (bslexi_match('>'))
        bslex_clean(ARROW); // "=>"
      else
        bslex_ret(EQ); // "="

    case '>':
      bslexi_get();
      if (bslexi_match('>')) {
        if (bslexi_match('='))
          bslex_clean(GT_GT_EQ); // ">>="
        else
          bslex_ret(GT_GT); // ">>"
      } else if (bslexi_match('='))
        bslex_ret(GT_EQ); // ">="
      else
        bslex_ret(GT); // ">"

    case '<':
      bslexi_get();
      if (bslexi_match('<')) {
        if (bslexi_match('='))
          bslex_clean(LE_LE_EQ); // "<<="
        else
          bslex_ret(LE_LE); // "<<"
      } else if (bslexi_match('='))
        bslex_ret(LE_EQ); // "<="
      else
        bslex_ret(LE); // "<"

    case '!':
      bslexi_get();
      if (bslexi_match('='))
        bslex_clean(NOT_EQ); // "!="
      else
        bslex_ret(NOT); // "!"

    case '-':
      bslexi_get();
      if (bslexi_match('-'))
        bslex_clean(DEC); // "--"
      else if (bslexi_match('='))
        bslex_clean(SUB_EQ); // "-="
      else
        bslex_ret(SUB); // "-"

    case '+':
      bslexi_get();
      if (bslexi_match('+'))
        bslex_clean(INC); // "++"
      else if (bslexi_match('='))
        bslex_clean(ADD_EQ); // "+="
      else
        bslex_ret(ADD); // "+"

    case '*':
      bslexi_get();
      if (bslexi_match('='))
        bslex_clean(MUL_EQ); // "*="
      else
        bslex_ret(MUL); // "*"

    case '/':
      bslexi_get();
      if (bslexi_match('/')) { // "//"
        while (1) {
          ch = bslexi_get();
          if (ch == '\n')
            goto check;
          else if (ch == '\0')
            bslex_ret(EOF);
        }
      } else if (bslexi_match('*')) { // "/*"
        while (1) {
          ch = bslexi_get();
          if (bslexi_match('*')) { // "/*...*"
            if (bslexi_match('/')) // "/*...*/"
              goto check;
          } else if (ch == '\0')
            bslex_ret(EOF);
        }
      } else if (bslexi_match('='))
        bslex_clean(DIV_EQ); // "/="
      else
        bslex_ret(DIV); // "/"

    case '%':
      bslexi_get();
      if (bslexi_match('='))
        bslex_clean(MOD_EQ); // "%="
      else
        bslex_ret(MOD); // "%"

    case '&':
      bslexi_get();
      if (bslexi_match('&'))
        bslex_clean(AND); // "&&"
      else if (bslexi_match('='))
        bslex_clean(AND_EQ); // "&="
      else
        bslex_ret(BIT_AND); // "&"

    case '|':
      bslexi_get();
      if (bslexi_match('|'))
        bslex_clean(OR); // "||"
      else if (bslexi_match('='))
        bslex_clean(OR_EQ); // "|="
      else
        bslex_ret(BIT_OR); // "|"

    case '^':
      bslexi_get();
      if (bslexi_match('='))
        bslex_clean(XOR_EQ); // "^="
      else
        bslex_ret(BIT_XOR); // "^"

    case '(':
      bslex_clean(PAREN_L);
    case ')':
      bslex_clean(PAREN_R);
    case '{':
      bslex_clean(BRACE_L);
    case '}':
      bslex_clean(BRACE_R);
    case '[':
      bslex_clean(BRACKET_L);
    case ']':
      bslex_clean(BRACKET_R);
    case '.':
      bslexi_get();
      ch = bslexi_read();
      if ('0' <= ch && ch <= '9') {
        lex->u.fnum = bslexi_match_dec(c, lex);
        bslex_ret(FLOAT);
      } else
        bslex_ret(DOT);
    case ',':
      bslex_clean(COM);
    case ';':
      bslex_clean(SEM);
    case '"':
      bslexi_get();
      while (1) {
        if (bslexi_match('"')) {
          lex->u.str = bsstring_create_buffer(c, lex->buf, lex->top);
          if (lex->u.str) {
            lex->type = BSTOK_STRING;
            lex->top = 0;
            bslexi_resize(c, lex);
            return lex->type;
          } else
            bs_longjmp(c->jbuf, BSERROR_NO_MEMORY);
        } else if (bslexi_match('\\')) {
          bslexi_push(c, lex, bslexi_get());
        } else if (bslexi_match('\0')) {
          lex->top = 0;
          bslex_ret(EOF);
        } else
          bslexi_push(c, lex, bslexi_get());
      }

    case '\'':
      bslexi_get();
      while (1) {
        if (bslexi_match('\'')) {
          lex->u.str = bsstring_create_buffer(c, lex->buf, lex->top);
          if (lex->u.str) {
            lex->type = BSTOK_STRING;
            lex->top = 0;
            bslexi_resize(c, lex);
            return lex->type;
          } else
            bs_longjmp(c->jbuf, BSERROR_NO_MEMORY);
        } else if (bslexi_match('\\')) {
          bslexi_push(c, lex, bslexi_get());
        } else if (bslexi_match('\0')) {
          lex->top = 0;
          bslex_ret(EOF);
        } else
          bslexi_push(c, lex, bslexi_get());
      }

    default:                        // id or keyword or number
      if ('0' <= ch && ch <= '9') { // number
        bsu64_t i = 0;
        if (bslexi_match('0')) {
          if (bslexi_match('x') || bslexi_match('X')) { // match 0x...
            lex->u.inum = bslexi_match_hex(c, lex);
            bslex_ret(INT);
          } else if (bslexi_match('b') || bslexi_match('B')) {
            lex->u.inum = bslexi_match_bin(c, lex);
            bslex_ret(INT);
          } else if (bslexi_match('.')) {
            lex->u.fnum = bslexi_match_dec(c, lex);
            bslex_ret(FLOAT);
          } else {
            lex->u.inum = bslexi_match_oct(c, lex);
            bslex_ret(INT);
          }
        } else {
          bsu64_t i = bslexi_match_int(c, lex);
          if (bslexi_match('.')) {
            bsfloat_t f = bslexi_match_dec(c, lex);
            lex->u.fnum = i + f;
            bslex_ret(FLOAT);
          } else {
            lex->u.inum = i;
            bslex_ret(INT);
          }
        }
      } else if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_' || ch == '$') { // id or keyword
        bslexi_match_id(c, lex);
        bsu16_t type;
        if (type = bslexi_match_keyword(c, lex)) {
          lex->top = 0;
          lex->type = type;
          bslexi_resize(c, lex);
          return type;
        }
        lex->u.str = bsstring_create_c_buffer_len(c, lex->buf, lex->top);
        lex->top = 0;
        if (lex->u.str) {
          lex->type = BSTOK_ID;
          bslexi_resize(c, lex);
          return lex->type;
        } else
          bs_longjmp(c->jbuf, BSERROR_NO_MEMORY);
      }
      bs_longjmp(c->jbuf, BSERROR_LEX_UNEXCEPTED_TOKEN);
    }
  } else { // lexer stop
    return lex->type = BSTOK_EOF;
  }
}
