/* Test R5900 3-operand MADD1/MADDU1 instructions (result in GPR) */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mhard-float" } */

/* Test 3-operand signed multiply-add using Pipeline 1 */
int test_madd1_3op (long long acc, int a, int b)
{
  return __builtin_mips_madd1_3op (acc, a, b);
}

/* Test 3-operand unsigned multiply-add using Pipeline 1 */
unsigned int test_maddu1_3op (unsigned long long acc, unsigned int a, unsigned int b)
{
  return __builtin_mips_maddu1_3op (acc, a, b);
}

/* { dg-final { scan-assembler "\tmadd1\t\\\$\[0-9\]+,\\\$\[0-9\]+,\\\$\[0-9\]+" } } */
/* { dg-final { scan-assembler "\tmaddu1\t\\\$\[0-9\]+,\\\$\[0-9\]+,\\\$\[0-9\]+" } } */

