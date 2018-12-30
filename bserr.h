#ifndef BSERR_H
#define BSERR_H

#define BSERROR_TABLE(E)                                                                                               \
  E(OK)                                                                                                                \
  E(NO_MEMORY)                                                                                                         \
  E(LEX_STREAM_CANNOT_READ)                                                                                            \
  E(LEX_STREAM_READ_FUNTION_IS_NULL)                                                                                   \
  E(LEX_UNEXCEPTED_TOKEN)                                                                                              \
  E(LEX_BUFFER_NOT_CLEAN)                                                                                              \
  E(LEX_JBUF_CANNOT_NULL)

enum {
#define ESERR_GET_FIRST_NAME(n) BSERROR_##n,
  BSERROR_TABLE(ESERR_GET_FIRST_NAME)
#undef ESERR_GET_FIRST_NAME
};

#endif