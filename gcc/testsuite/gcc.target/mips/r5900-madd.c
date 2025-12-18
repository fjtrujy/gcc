/* Test multiply instruction patterns for R5900.  */
/* This test runs at all optimization levels including -O0.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-final { scan-assembler "\tmult\t" } } */

NOMIPS16 int
test_mult (int a, int b)
{
  return a * b;
}

