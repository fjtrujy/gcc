/* Test that CLZ instruction is NOT generated for R5900.  */
/* The R5900 does not properly support CLZ/CLO instructions.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler-not "\tclz\t" } } */
/* { dg-final { scan-assembler-not "\tclo\t" } } */

NOMIPS16 unsigned int
test_clz (unsigned int x)
{
  return __builtin_clz (x);
}

