/* Test 64-bit multiplication for R5900.  */
/* The R5900 requires a custom __umulsidi3 to prevent recursive calls.  */
/* This test runs at all optimization levels including -O0.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mgp32" } */
/* { dg-final { scan-assembler "\tmultu\t" } } */

NOMIPS16 unsigned long long
test_muldi (unsigned int a, unsigned int b)
{
  return (unsigned long long) a * b;
}

