/* Test R5900 Pipeline 1 multiply-add instructions (madd1/maddu1) via builtin.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */

long long
test_madd1 (long long acc, int a, int b)
{
  return __builtin_mips_madd1 (acc, a, b);
}

unsigned long long
test_maddu1 (unsigned long long acc, unsigned int a, unsigned int b)
{
  return __builtin_mips_maddu1 (acc, a, b);
}

/* { dg-final { scan-assembler "\tmadd1\t" } } */
/* { dg-final { scan-assembler "\tmaddu1\t" } } */

