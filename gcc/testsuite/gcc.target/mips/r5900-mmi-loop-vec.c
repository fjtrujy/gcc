/* Test R5900 MMI auto-vectorization of loops.
   The compiler should automatically vectorize simple loops using
   MMI 128-bit SIMD instructions.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -O3 -ftree-vectorize" } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* Simple vector add loop - should be vectorized using paddw.  */
NOMIPS16 void
vec_add_loop (int *__restrict a, int *__restrict b, int *__restrict c, int n)
{
    for (int i = 0; i < n; i++)
        c[i] = a[i] + b[i];
}

/* Simple vector sub loop - should be vectorized using psubw.  */
NOMIPS16 void
vec_sub_loop (int *__restrict a, int *__restrict b, int *__restrict c, int n)
{
    for (int i = 0; i < n; i++)
        c[i] = a[i] - b[i];
}

/* Simple vector AND loop - should be vectorized using pand.  */
NOMIPS16 void
vec_and_loop (int *__restrict a, int *__restrict b, int *__restrict c, int n)
{
    for (int i = 0; i < n; i++)
        c[i] = a[i] & b[i];
}

/* Verify vectorized MMI instructions are generated.  */
/* { dg-final { scan-assembler "\\tpaddw\\t" } } */
/* { dg-final { scan-assembler "\\tpsubw\\t" } } */
/* { dg-final { scan-assembler "\\tpand\\t" } } */

