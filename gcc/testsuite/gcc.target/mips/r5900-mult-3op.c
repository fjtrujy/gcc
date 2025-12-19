/* Test R5900 Pipeline 0 3-operand multiply builtins */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mhard-float" } */

/*
 * R5900 supports 3-operand multiply on Pipeline 0.
 * These forms write the low 32 bits of the result to both
 * a GPR and the LO register (HI gets the high 32 bits).
 *
 * Available builtins:
 * - __builtin_mips_mult_3op(a, b)   -> mult rd,rs,rt (signed)
 * - __builtin_mips_multu_3op(a, b)  -> multu rd,rs,rt (unsigned)
 */

/* Test signed 3-operand multiply */
int test_mult_3op (int a, int b)
{
  return __builtin_mips_mult_3op (a, b);
}

/* Test unsigned 3-operand multiply */
unsigned int test_multu_3op (unsigned int a, unsigned int b)
{
  return __builtin_mips_multu_3op (a, b);
}

/* { dg-final { scan-assembler "\tmult\t\\\$\[0-9\]+,\\\$\[0-9\]+,\\\$\[0-9\]+" } } */
/* { dg-final { scan-assembler "\tmultu\t\\\$\[0-9\]+,\\\$\[0-9\]+,\\\$\[0-9\]+" } } */

