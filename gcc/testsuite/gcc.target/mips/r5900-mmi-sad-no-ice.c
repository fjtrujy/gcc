/* Test sum-of-absolute-differences pattern compiles without ICE.
   This pattern from RetroArch's rpng_encode.c caused an ICE in
   prepare_cmp_insn before the abs<mode>2 pattern condition was
   fixed to exclude V16QI mode for MMI-only targets.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "ICE only occurs at -O3" { *-*-* } { "-O0" "-O1" "-O2" "-Os" "-flto" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef __SIZE_TYPE__ size_t;

/* abs() for int8_t values.  */
static inline int abs_i8 (int x) { return x < 0 ? -x : x; }

/* This pattern caused ICE in prepare_cmp_insn at optabs.cc:4597.
   The vectorizer tried to use V16QI abs which R5900 doesn't support.  */
NOMIPS16 unsigned
count_sad (const uint8_t *data, size_t len)
{
  size_t i;
  unsigned cnt = 0;
  for (i = 0; i < len; i++)
    {
      if (data[i])
        cnt += abs_i8 ((int8_t)data[i]);
    }
  return cnt;
}

/* Simpler variant without the if check.  */
NOMIPS16 unsigned
count_sad_simple (const uint8_t *data, size_t len)
{
  size_t i;
  unsigned cnt = 0;
  for (i = 0; i < len; i++)
    cnt += abs_i8 ((int8_t)data[i]);
  return cnt;
}

/* Variant using stdlib abs().  */
NOMIPS16 unsigned
count_sad_stdlib (const uint8_t *data, size_t len)
{
  size_t i;
  unsigned cnt = 0;
  for (i = 0; i < len; i++)
    cnt += __builtin_abs ((int)((int8_t)data[i]));
  return cnt;
}
