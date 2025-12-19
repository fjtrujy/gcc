/* Test R5900 Pipeline 1 divide instructions (div1/divu1) via builtin.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */

long long
test_div1 (int a, int b)
{
  return __builtin_mips_div1 (a, b);
}

unsigned long long
test_divu1 (unsigned int a, unsigned int b)
{
  return __builtin_mips_divu1 (a, b);
}

/* { dg-final { scan-assembler "\tdiv1\t" } } */
/* { dg-final { scan-assembler "\tdivu1\t" } } */

