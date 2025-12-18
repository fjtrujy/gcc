/* Test that 3-operand MULT instruction is generated for R5900.  */
/* The R5900 supports the 3-operand multiply like the R3900.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-final { scan-assembler "\tmult\t\[^\n\]*,\[^\n\]*," } } */

NOMIPS16 int
test_mult3 (int a, int b)
{
  return a * b;
}

