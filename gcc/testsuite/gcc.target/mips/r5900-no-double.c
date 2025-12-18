/* Test that R5900 does NOT generate double-precision FP instructions.  */
/* R5900 FPU only supports single-precision.  */
/* This test runs at all optimization levels including -O0.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mhard-float" } */
/* { dg-final { scan-assembler-not "\tadd\\.d\t" } } */
/* { dg-final { scan-assembler-not "\tmul\\.d\t" } } */
/* { dg-final { scan-assembler-not "\tsub\\.d\t" } } */
/* { dg-final { scan-assembler-not "\tdiv\\.d\t" } } */
/* { dg-final { scan-assembler-not "\tldc1\t" } } */
/* { dg-final { scan-assembler-not "\tsdc1\t" } } */

/* Double operations should be emulated in software.  */

NOMIPS16 double
test_dadd (double a, double b)
{
  return a + b;
}

NOMIPS16 double
test_dmul (double a, double b)
{
  return a * b;
}

