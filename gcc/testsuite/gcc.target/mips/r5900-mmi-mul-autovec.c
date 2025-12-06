/* Test R5900 MMI multiply autovectorization */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -ftree-vectorize" } */
/* { dg-skip-if "vectorization requires -O3" { *-*-* } { "-O0" "-O1" "-O2" "-Os" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* Test short multiply loop - uses PMULTH + PMFHL.LH */
NOMIPS16 void
test_vec_mul_short (short *__restrict a, short *__restrict b, short *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] * b[i];
}
/* { dg-final { scan-assembler "\\tpmulth\\t" } } */
/* { dg-final { scan-assembler "\\tpmfhl.lh\\t" } } */
