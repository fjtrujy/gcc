/* Test R5900 3-operand MULT1/MULTU1 instructions (result in GPR) */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mhard-float" } */

/* Test 3-operand signed multiply using Pipeline 1 */
int test_mult1_3op (int a, int b)
{
  return __builtin_mips_mult1_3op (a, b);
}

/* Test 3-operand unsigned multiply using Pipeline 1 */
unsigned int test_multu1_3op (unsigned int a, unsigned int b)
{
  return __builtin_mips_multu1_3op (a, b);
}

/* { dg-final { scan-assembler "\tmult1\t\\\$\[0-9\]+,\\\$\[0-9\]+,\\\$\[0-9\]+" } } */
/* { dg-final { scan-assembler "\tmultu1\t\\\$\[0-9\]+,\\\$\[0-9\]+,\\\$\[0-9\]+" } } */

