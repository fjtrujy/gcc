/* Test multiply instruction patterns for R5900.  */
/* This test runs at all optimization levels including -O0.  */
/* With automatic pipeline selection, GCC may choose mult or mult1.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-final { scan-assembler "\tmultu?1?\t" } } */

NOMIPS16 int
test_mult (int a, int b)
{
  return a * b;
}

