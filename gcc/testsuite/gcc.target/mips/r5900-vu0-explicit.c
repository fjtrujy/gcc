/* Test R5900 VU0 explicit intrinsics for automatic operations */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mvu0" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef float v4sf __attribute__((vector_size(16)));

/* Test __builtin_vu0_vadd */
NOMIPS16 v4sf
test_vadd (v4sf a, v4sf b)
{
  return __builtin_vu0_vadd (a, b);
}
/* { dg-final { scan-assembler "\\tvadd\\.xyzw\\t" } } */

/* Test __builtin_vu0_vsub */
NOMIPS16 v4sf
test_vsub (v4sf a, v4sf b)
{
  return __builtin_vu0_vsub (a, b);
}
/* { dg-final { scan-assembler "\\tvsub\\.xyzw\\t" } } */

/* Test __builtin_vu0_vmul */
NOMIPS16 v4sf
test_vmul (v4sf a, v4sf b)
{
  return __builtin_vu0_vmul (a, b);
}
/* { dg-final { scan-assembler "\\tvmul\\.xyzw\\t" } } */

/* Test __builtin_vu0_vabs */
NOMIPS16 v4sf
test_vabs (v4sf a)
{
  return __builtin_vu0_vabs (a);
}
/* { dg-final { scan-assembler "\\tvabs\\.xyzw\\t" } } */

/* Test __builtin_vu0_vmax */
NOMIPS16 v4sf
test_vmax (v4sf a, v4sf b)
{
  return __builtin_vu0_vmax (a, b);
}
/* { dg-final { scan-assembler "\\tvmax\\.xyzw\\t" } } */

/* Test __builtin_vu0_vmini */
NOMIPS16 v4sf
test_vmini (v4sf a, v4sf b)
{
  return __builtin_vu0_vmini (a, b);
}
/* { dg-final { scan-assembler "\\tvmini\\.xyzw\\t" } } */

/* Test __builtin_vu0_vmove */
NOMIPS16 v4sf
test_vmove (v4sf a)
{
  return __builtin_vu0_vmove (a);
}
/* { dg-final { scan-assembler "\\tvmove\\.xyzw\\t" } } */
