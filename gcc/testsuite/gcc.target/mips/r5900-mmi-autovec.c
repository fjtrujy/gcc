/* Test R5900 MMI autovectorization */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -ftree-vectorize" } */
/* { dg-skip-if "vectorization requires -O3" { *-*-* } { "-O0" "-O1" "-O2" "-Os" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* Test basic integer add loop */
NOMIPS16 void
test_vec_add_int (int *__restrict a, int *__restrict b, int *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] + b[i];
}
/* { dg-final { scan-assembler "\\tpaddw\\t" } } */

/* Test basic integer sub loop */
NOMIPS16 void
test_vec_sub_int (int *__restrict a, int *__restrict b, int *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] - b[i];
}
/* { dg-final { scan-assembler "\\tpsubw\\t" } } */

/* Test short add loop */
NOMIPS16 void
test_vec_add_short (short *__restrict a, short *__restrict b, short *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] + b[i];
}
/* { dg-final { scan-assembler "\\tpaddh\\t" } } */

/* Test byte add loop */
NOMIPS16 void
test_vec_add_byte (signed char *__restrict a, signed char *__restrict b, signed char *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] + b[i];
}
/* { dg-final { scan-assembler "\\tpaddb\\t" } } */

/* Test integer max loop */
NOMIPS16 void
test_vec_max_int (int *__restrict a, int *__restrict b, int *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] > b[i] ? a[i] : b[i];
}
/* { dg-final { scan-assembler "\\tpmaxw\\t" } } */

/* Test integer min loop */
NOMIPS16 void
test_vec_min_int (int *__restrict a, int *__restrict b, int *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] < b[i] ? a[i] : b[i];
}
/* { dg-final { scan-assembler "\\tpminw\\t" } } */

/* Test short max loop */
NOMIPS16 void
test_vec_max_short (short *__restrict a, short *__restrict b, short *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] > b[i] ? a[i] : b[i];
}
/* { dg-final { scan-assembler "\\tpmaxh\\t" } } */

/* Test short min loop */
NOMIPS16 void
test_vec_min_short (short *__restrict a, short *__restrict b, short *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] < b[i] ? a[i] : b[i];
}
/* { dg-final { scan-assembler "\\tpminh\\t" } } */
