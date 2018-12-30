#ifndef BSVM_H
#define BSVM_H

#define BSOP_TYPE_TABLE(T)                                                                                            \
  T(BIT_OR)                                                                                                            \
  T(BIT_AND)                                                                                                           \
  T(BIT_XOR)                                                                                                           \
  T(AND)                                                                                                               \
  T(OR)                                                                                                                \
  T(ADD)                                                                                                               \
  T(SUB)                                                                                                               \
  T(MUL)                                                                                                               \
  T(DIV)                                                                                                               \
  T(MOD)                                                                                                               \
  T(SHIFTL)                                                                                                            \
  T(SHIFTR)                                                                                                            \
  T(NEW)                                                                                                               \
  T(LOAD_CONSTANT)                                                                                                     \
  T(LOAD_IMMEDIATE)                                                                                                    \
  T(LOAD_THIS)                                                                                                         \
  T(LOAD_SUPER)                                                                                                        \
  T(LOAD_NULL)                                                                                                         \
  T(LOAD_TRUE)                                                                                                         \
  T(LOAD_FALSE)                                                                                                        \
  T(GET_PROTO)                                                                                                         \
  T(SET_PROTO)                                                                                                         \
  T(DEL_PROTO)                                                                                                         \
  T(GET_ENV)                                                                                                           \
  T(SET_ENV)                                                                                                           \
  T(DEL_ENV)                                                                                                           \
  T(RETURN)                                                                                                            \
  T(CONST_ENV)                                                                                                         \
  T(TRUE_JUMP)                                                                                                         \
  T(GOTO)                                                                                                              \
  T(LABEL)                                                                                                             \
  T(EXPORT)                                                                                                            \
  T(IMPORT)                                                                                                            \
  T(SWITCH)                                                                                                            \
  T(THROW)                                                                                                             \
  T(TRY)

#endif