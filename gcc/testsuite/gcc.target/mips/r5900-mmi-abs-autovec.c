/* Test R5900 MMI parallel absolute value autovectorization.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "autovectorization requires -O3" { *-*-* } { "-O0" "-O1" "-O2" "-Os" "-flto" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* Autovectorizable absolute value - word */
NOMIPS16 void
abs_int (int * __restrict a, int * __restrict b, int n)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] < 0 ? -a[i] : a[i];
}
/* { dg-final { scan-assembler "\\tpabsw\\t" } } */

/* Autovectorizable absolute value - halfword */
NOMIPS16 void
abs_short (short * __restrict a, short * __restrict b, int n)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] < 0 ? -a[i] : a[i];
}
/* { dg-final { scan-assembler "\\tpabsh\\t" } } */
