/* Test rsqrt.s instruction generation for R5900.  */
/* The R5900 has a hardware rsqrt.s instruction that computes a/sqrt(b).  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mhard-float -ffast-math" } */
/* Skip -O0: rsqrt.s requires optimization to combine div and sqrt.  */
/* { dg-skip-if "requires optimization" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler "\trsqrt\\.s\t" } } */

extern float sqrtf (float);

NOMIPS16 float
test_rsqrt (float a, float b)
{
  return a / sqrtf (b);
}

