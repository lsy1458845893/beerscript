#ifndef BSLEX_H
#define BSLEX_H

#define BSLEX_SYMBOL_LIST(T)                                                                                           \
  T(EOF, "<EOF>", 0)                                                                                                   \
  T(ID, "<id>", 0)                                                                                                     \
  T(STRING, "<string>", 0)                                                                                             \
  T(INT, "<int>", 0)                                                                                                   \
  T(FLOAT, "<float>", 0)                                                                                               \
  T(EQ_EQ, "==", 2)                                                                                                    \
  T(ARROW, "=>", 2)                                                                                                    \
  T(EQ, "=", 1)                                                                                                        \
  T(GT_GT_EQ, ">>=", 3)                                                                                                \
  T(GT_GT, ">>", 2)                                                                                                    \
  T(GT_EQ, ">=", 2)                                                                                                    \
  T(GT, ">", 1)                                                                                                        \
  T(LE_LE_EQ, "<<=", 3)                                                                                                \
  T(LE_LE, "<<", 2)                                                                                                    \
  T(LE_EQ, "<=", 2)                                                                                                    \
  T(LE, "<", 1)                                                                                                        \
  T(NOT_EQ, "!=", 2)                                                                                                   \
  T(NOT, "!", 1)                                                                                                       \
  T(DEC, "--", 2)                                                                                                      \
  T(SUB_EQ, "-=", 2)                                                                                                   \
  T(SUB, "-", 1)                                                                                                       \
  T(INC, "++", 2)                                                                                                      \
  T(ADD_EQ, "+=", 2)                                                                                                   \
  T(ADD, "+", 1)                                                                                                       \
  T(MUL_EQ, "*=", 2)                                                                                                   \
  T(MUL, "*", 1)                                                                                                       \
  T(DIV_EQ, "/=", 2)                                                                                                   \
  T(DIV, "/", 1)                                                                                                       \
  T(MOD_EQ, "%=", 2)                                                                                                   \
  T(MOD, "%", 1)                                                                                                       \
  T(AND, "&&", 2)                                                                                                      \
  T(AND_EQ, "&=", 2)                                                                                                   \
  T(BIT_AND, "&", 1)                                                                                                   \
  T(OR, "||", 2)                                                                                                       \
  T(OR_EQ, "|=", 2)                                                                                                    \
  T(BIT_OR, "|", 1)                                                                                                    \
  T(XOR_EQ, "^=", 2)                                                                                                   \
  T(BIT_XOR, "^", 1)                                                                                                   \
  T(PAREN_L, "(", 1)                                                                                                   \
  T(PAREN_R, ")", 1)                                                                                                   \
  T(BRACE_L, "{", 1)                                                                                                   \
  T(BRACE_R, "}", 1)                                                                                                   \
  T(BRACKET_L, "[", 1)                                                                                                 \
  T(BRACKET_R, "]", 1)                                                                                                 \
  T(DOT, ".", 1)                                                                                                       \
  T(COM, ",", 1)                                                                                                       \
  T(SEM, ";", 1)

#define BSLEX_KEYWORD_LIST(T)                                                                                          \
  T(ASYNC, "async", 5)                                                                                                 \
  T(AWAIT, "await", 5)                                                                                                 \
  T(BREAK, "break", 5)                                                                                                 \
  T(CASE, "case", 4)                                                                                                   \
  T(CATCH, "catch", 5)                                                                                                 \
  T(CLASS, "class", 5)                                                                                                 \
  T(CONST, "const", 5)                                                                                                 \
  T(CONTINUE, "continue", 8)                                                                                           \
  T(DEFAULT, "default", 7)                                                                                             \
  T(DELETE, "delete", 6)                                                                                               \
  T(DO, "do", 2)                                                                                                       \
  T(ELSE, "else", 4)                                                                                                   \
  T(EXPORT, "export", 6)                                                                                               \
  T(EXTENDS, "extends", 7)                                                                                             \
  T(FALSE, "false", 5)                                                                                                 \
  T(FOR, "for", 3)                                                                                                     \
  T(FUNCTION, "function", 8)                                                                                           \
  T(IF, "if", 2)                                                                                                       \
  T(IMPORT, "import", 6)                                                                                               \
  T(IN, "in", 2)                                                                                                       \
  T(INSTANCEOF, "instanceof", 10)                                                                                      \
  T(LET, "let", 3)                                                                                                     \
  T(NEW, "new", 3)                                                                                                     \
  T(NULL, "null", 4)                                                                                                   \
  T(RETURN, "return", 6)                                                                                               \
  T(STATIC, "static", 6)                                                                                               \
  T(SUPER, "super", 5)                                                                                                 \
  T(SWITCH, "switch", 6)                                                                                               \
  T(THIS, "this", 4)                                                                                                   \
  T(THROW, "throw", 5)                                                                                                 \
  T(TRUE, "true", 4)                                                                                                   \
  T(TRY, "try", 3)                                                                                                     \
  T(VAR, "var", 3)                                                                                                     \
  T(WHILE, "while", 5)

enum {
#define BSLEX_TOKEN_NAMES(n, ...) BSTOK_##n,
  BSLEX_SYMBOL_LIST(BSLEX_TOKEN_NAMES) BSLEX_KEYWORD_LIST(BSLEX_TOKEN_NAMES)
#undef BSLEX_TOKEN_NAMES
};

#include "bsctx.h"
#include "bsdep.h"
#include "bsstream.h"
#include "bsstring.h"

typedef struct bslex_s {
  bsstream_t *stream;
  bssize_t top, size;
  bsu8_t *buf;
  bsu8_t lastch;
  bsu16_t type;
  bssize_t line;
  bssize_t colunm;
  union {
    bsint_t inum;
    bsfloat_t fnum;
    bsstring_t *str;
  } u;
} bslex_t;

static inline void bslex_init(bslex_t *lex, bsstream_t *stream) {
  lex->stream = stream;
  lex->size = lex->top = 0;
  lex->buf = 0;
  lex->lastch = 0;
  lex->type = BSTOK_EOF;
  lex->line = 1;
  lex->colunm = 1;
}

void bslex_deinit(bsctx_t *c, bslex_t *lex);

bsu16_t bslexi_next(bsctx_t *c, bslex_t *lex);

static inline bssize_t bslex_get_type(bslex_t *lex) { return lex->type; }
static inline bsint_t bslex_get_inum(bslex_t *lex) { return lex->u.inum; }
static inline bsfloat_t bslex_get_fnum(bslex_t *lex) { return lex->u.fnum; }
static inline bsstring_t *bslex_get_str(bslex_t *lex) { return lex->u.str; }

#endif
