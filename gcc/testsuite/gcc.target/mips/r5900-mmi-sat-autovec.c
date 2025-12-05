/* Test R5900 MMI saturating arithmetic autovectorization.
   Note: GCC autovectorizes unsigned saturating operations.
   For signed saturation, use the builtins directly.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "autovectorization requires -O3" { *-*-* } { "-O0" "-O1" "-O2" "-Os" "-flto" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* Unsigned saturating add - word */
NOMIPS16 void
test_usadd_w (unsigned int * __restrict a, unsigned int * __restrict b,
              unsigned int * __restrict c, int n)
{
  for (int i = 0; i < n; i++)
    {
      unsigned int sum = a[i] + b[i];
      c[i] = sum < a[i] ? ~0U : sum;
    }
}
/* { dg-final { scan-assembler "\\tpadduw\\t" } } */

/* Unsigned saturating add - halfword */
NOMIPS16 void
test_usadd_h (unsigned short * __restrict a, unsigned short * __restrict b,
              unsigned short * __restrict c, int n)
{
  for (int i = 0; i < n; i++)
    {
      unsigned short sum = a[i] + b[i];
      c[i] = sum < a[i] ? 0xFFFF : sum;
    }
}
/* { dg-final { scan-assembler "\\tpadduh\\t" } } */

/* Unsigned saturating add - byte */
NOMIPS16 void
test_usadd_b (unsigned char * __restrict a, unsigned char * __restrict b,
              unsigned char * __restrict c, int n)
{
  for (int i = 0; i < n; i++)
    {
      unsigned char sum = a[i] + b[i];
      c[i] = sum < a[i] ? 0xFF : sum;
    }
}
/* { dg-final { scan-assembler "\\tpaddub\\t" } } */

/* Unsigned saturating subtract - word */
NOMIPS16 void
test_ussub_w (unsigned int * __restrict a, unsigned int * __restrict b,
              unsigned int * __restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] > b[i] ? a[i] - b[i] : 0;
}
/* { dg-final { scan-assembler "\\tpsubuw\\t" } } */

/* Unsigned saturating subtract - halfword */
NOMIPS16 void
test_ussub_h (unsigned short * __restrict a, unsigned short * __restrict b,
              unsigned short * __restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] > b[i] ? a[i] - b[i] : 0;
}
/* { dg-final { scan-assembler "\\tpsubuh\\t" } } */

/* Unsigned saturating subtract - byte */
NOMIPS16 void
test_ussub_b (unsigned char * __restrict a, unsigned char * __restrict b,
              unsigned char * __restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] > b[i] ? a[i] - b[i] : 0;
}
/* { dg-final { scan-assembler "\\tpsubub\\t" } } */
