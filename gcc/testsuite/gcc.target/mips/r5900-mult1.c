/* Test R5900 Pipeline 1 multiply instruction (mult1) via builtin.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */

long long
test_mult1 (int a, int b)
{
  return __builtin_mips_mult1 (a, b);
}

unsigned long long
test_multu1 (unsigned int a, unsigned int b)
{
  return __builtin_mips_multu1 (a, b);
}

/* { dg-final { scan-assembler "\tmult1\t" } } */
/* { dg-final { scan-assembler "\tmultu1\t" } } */

