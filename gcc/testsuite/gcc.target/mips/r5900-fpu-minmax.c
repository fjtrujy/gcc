/* Test R5900 FPU min/max explicit builtins */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* Test __builtin_mips_min_s */
NOMIPS16 float
test_min (float a, float b)
{
  return __builtin_mips_min_s (a, b);
}
/* { dg-final { scan-assembler "\\tmin\\.s\\t" } } */

/* Test __builtin_mips_max_s */
NOMIPS16 float
test_max (float a, float b)
{
  return __builtin_mips_max_s (a, b);
}
/* { dg-final { scan-assembler "\\tmax\\.s\\t" } } */
