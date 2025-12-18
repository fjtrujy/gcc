/* Test R5900 MMI autovectorization.
   The compiler automatically vectorizes loops using 128-bit integer
   SIMD operations provided by the R5900's MMI (Multimedia Instructions).  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -ftree-vectorize -O3" } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* Test basic integer add loop - vectorized using paddw (4x32-bit add).  */
NOMIPS16 void
test_vec_add_int (int *__restrict a, int *__restrict b, int *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] + b[i];
}
/* { dg-final { scan-assembler "\\tpaddw\\t" } } */

/* Test basic integer sub loop - vectorized using psubw (4x32-bit sub).  */
NOMIPS16 void
test_vec_sub_int (int *__restrict a, int *__restrict b, int *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] - b[i];
}
/* { dg-final { scan-assembler "\\tpsubw\\t" } } */

/* Test short add loop - vectorized using paddh (8x16-bit add).  */
NOMIPS16 void
test_vec_add_short (short *__restrict a, short *__restrict b, short *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] + b[i];
}
/* { dg-final { scan-assembler "\\tpaddh\\t" } } */

/* Test byte add loop - vectorized using paddb (16x8-bit add).  */
NOMIPS16 void
test_vec_add_byte (signed char *__restrict a, signed char *__restrict b, signed char *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] + b[i];
}
/* { dg-final { scan-assembler "\\tpaddb\\t" } } */

/* Test signed max for int - vectorized using pmaxw (4x32-bit signed max).
   Note: R5900 only has SIGNED min/max instructions. Unsigned loops will
   use scalar code.  */
NOMIPS16 void
test_vec_max_int (int *__restrict a, int *__restrict b, int *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] > b[i] ? a[i] : b[i];
}
/* { dg-final { scan-assembler "\\tpmaxw\\t" } } */

/* Test signed min for int - vectorized using pminw (4x32-bit signed min).  */
NOMIPS16 void
test_vec_min_int (int *__restrict a, int *__restrict b, int *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] < b[i] ? a[i] : b[i];
}
/* { dg-final { scan-assembler "\\tpminw\\t" } } */

/* Test signed max for short - vectorized using pmaxh (8x16-bit signed max).  */
NOMIPS16 void
test_vec_max_short (short *__restrict a, short *__restrict b, short *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] > b[i] ? a[i] : b[i];
}
/* { dg-final { scan-assembler "\\tpmaxh\\t" } } */

/* Test signed min for short - vectorized using pminh (8x16-bit signed min).  */
NOMIPS16 void
test_vec_min_short (short *__restrict a, short *__restrict b, short *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] < b[i] ? a[i] : b[i];
}
/* { dg-final { scan-assembler "\\tpminh\\t" } } */

/* Test integer AND loop - vectorized using pand (128-bit AND).  */
NOMIPS16 void
test_vec_and_int (int *__restrict a, int *__restrict b, int *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] & b[i];
}
/* { dg-final { scan-assembler "\\tpand\\t" } } */

/* Test integer OR loop - vectorized using por (128-bit OR).  */
NOMIPS16 void
test_vec_or_int (int *__restrict a, int *__restrict b, int *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] | b[i];
}
/* { dg-final { scan-assembler "\\tpor\\t" } } */

/* Test integer XOR loop - vectorized using pxor (128-bit XOR).  */
NOMIPS16 void
test_vec_xor_int (int *__restrict a, int *__restrict b, int *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] ^ b[i];
}
/* { dg-final { scan-assembler "\\tpxor\\t" } } */

/* Test integer NOT loop - vectorized using pnor (128-bit NOR).  */
NOMIPS16 void
test_vec_not_int (int *__restrict a, int *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = ~a[i];
}
/* { dg-final { scan-assembler "\\tpnor\\t" } } */
