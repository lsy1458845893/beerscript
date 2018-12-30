#ifndef BSDEP_H
#define BSDEP_H

#include <stdint.h>

typedef uint32_t bssize_t;
typedef int32_t bsint_t;
typedef float bsfloat_t;
typedef uint16_t bsret_t;
typedef uint8_t bsu8_t;
typedef uint16_t bsu16_t;
typedef uint64_t bsu64_t;

#include <string.h>

#define bs_memcpy memcpy
#define bs_strlen strlen
#define bs_memcmp memcmp

#include <setjmp.h>

typedef jmp_buf bsjbuf_t;
#define bs_setjmp(jbuf) setjmp(*(jbuf))
#define bs_longjmp(jbuf, err) longjmp(*(jbuf), err)


#define BS_STRUCT_COMPACT

#endif