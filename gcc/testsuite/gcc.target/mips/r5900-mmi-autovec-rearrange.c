/* Test autovectorization of MMI data rearrangement operations.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -ftree-vectorize" } */
/* { dg-skip-if "vectorization requires -O3" { *-*-* } { "-O0" "-O1" "-O2" "-Os" } { "" } } */

typedef signed char v16qi __attribute__((vector_size(16)));
typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));

/* Test vec_unpack - widening unpack operations */

/* Unpack signed V16QI to V8HI - should use pextlb + pcgtb for sign extend */
void unpack_qi_to_hi_signed (signed char *__restrict src, short *__restrict dst, int n)
{
  for (int i = 0; i < n; i++)
    dst[i] = src[i];
}

/* Unpack unsigned V16QI to V8HI - should use pextlb with zero */
void unpack_qi_to_hi_unsigned (unsigned char *__restrict src, unsigned short *__restrict dst, int n)
{
  for (int i = 0; i < n; i++)
    dst[i] = src[i];
}

/* Unpack signed V8HI to V4SI - should use pextlh + pcgth for sign extend */
void unpack_hi_to_si_signed (short *__restrict src, int *__restrict dst, int n)
{
  for (int i = 0; i < n; i++)
    dst[i] = src[i];
}

/* Unpack unsigned V8HI to V4SI - should use pextlh with zero */
void unpack_hi_to_si_unsigned (unsigned short *__restrict src, unsigned int *__restrict dst, int n)
{
  for (int i = 0; i < n; i++)
    dst[i] = src[i];
}

/* Test explicit vec_interleave via shuffle operations */

/* Interleave low bytes - should use pextlb */
v16qi interleave_low_qi (v16qi a, v16qi b)
{
  return __builtin_shuffle (a, b,
    (v16qi){0, 16, 1, 17, 2, 18, 3, 19, 4, 20, 5, 21, 6, 22, 7, 23});
}

/* Interleave high bytes - should use pextub */
v16qi interleave_high_qi (v16qi a, v16qi b)
{
  return __builtin_shuffle (a, b,
    (v16qi){8, 24, 9, 25, 10, 26, 11, 27, 12, 28, 13, 29, 14, 30, 15, 31});
}

/* Interleave low halfwords - should use pextlh */
v8hi interleave_low_hi (v8hi a, v8hi b)
{
  return __builtin_shuffle (a, b, (v8hi){0, 8, 1, 9, 2, 10, 3, 11});
}

/* Interleave high halfwords - should use pextuh */
v8hi interleave_high_hi (v8hi a, v8hi b)
{
  return __builtin_shuffle (a, b, (v8hi){4, 12, 5, 13, 6, 14, 7, 15});
}

/* Interleave low words - should use pextlw */
v4si interleave_low_si (v4si a, v4si b)
{
  return __builtin_shuffle (a, b, (v4si){0, 4, 1, 5});
}

/* Interleave high words - should use pextuw */
v4si interleave_high_si (v4si a, v4si b)
{
  return __builtin_shuffle (a, b, (v4si){2, 6, 3, 7});
}

/* Test vec_pack_trunc - narrowing pack operations */

/* Pack V8HI to V16QI - should use ppacb */
void pack_hi_to_qi (short *__restrict src, signed char *__restrict dst, int n)
{
  for (int i = 0; i < n; i++)
    dst[i] = (signed char)src[i];
}

/* Pack V4SI to V8HI - should use ppach */
void pack_si_to_hi (int *__restrict src, short *__restrict dst, int n)
{
  for (int i = 0; i < n; i++)
    dst[i] = (short)src[i];
}

/* Pack V2DI to V4SI - should use ppacw */
void pack_di_to_si (long long *__restrict src, int *__restrict dst, int n)
{
  for (int i = 0; i < n; i++)
    dst[i] = (int)src[i];
}

/* { dg-final { scan-assembler "pextlb" } } */
/* { dg-final { scan-assembler "pextub" } } */
/* { dg-final { scan-assembler "pextlh" } } */
/* { dg-final { scan-assembler "pextuh" } } */
/* { dg-final { scan-assembler "pextlw" } } */
/* { dg-final { scan-assembler "pextuw" } } */
/* { dg-final { scan-assembler "ppacb" } } */
/* { dg-final { scan-assembler "ppach" } } */
/* { dg-final { scan-assembler "ppacw" } } */
