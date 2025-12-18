/* Test that min.s and max.s instructions are generated for R5900.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mhard-float" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler "\tmin\\.s\t" } } */
/* { dg-final { scan-assembler "\tmax\\.s\t" } } */

NOMIPS16 float
test_minf (float x, float y)
{
  return __builtin_fminf (x, y);
}

NOMIPS16 float
test_maxf (float x, float y)
{
  return __builtin_fmaxf (x, y);
}

