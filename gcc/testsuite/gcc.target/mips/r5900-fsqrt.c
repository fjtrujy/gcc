/* Test that sqrt.s instruction is generated for R5900.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mhard-float -ffast-math" } */
/* Skip -O0: sqrt.s requires optimization to inline sqrtf builtin.  */
/* { dg-skip-if "requires optimization" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler "\tsqrt\\.s\t" } } */

extern float sqrtf (float);

NOMIPS16 float
test_sqrt (float x)
{
  return sqrtf (x);
}

