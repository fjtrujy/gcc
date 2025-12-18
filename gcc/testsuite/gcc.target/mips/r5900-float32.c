/* Test that R5900 generates single-precision FP instructions.  */
/* This test runs at all optimization levels including -O0.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mhard-float" } */
/* { dg-final { scan-assembler "\tadd\\.s\t" } } */
/* { dg-final { scan-assembler "\tmul\\.s\t" } } */

NOMIPS16 float
test_fadd (float a, float b)
{
  return a + b;
}

NOMIPS16 float
test_fmul (float a, float b)
{
  return a * b;
}

