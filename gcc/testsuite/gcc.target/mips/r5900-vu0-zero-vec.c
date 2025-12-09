/* Test R5900 VU0 zero vector loading uses vsub, not vmove from vf0 */
/* The VF0 register is {0,0,0,1}, so vmove from vf0 would corrupt W component */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mvu0" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef float v4sf __attribute__((vector_size(16)));

/* Loading zero vector should use vsub $vf0,$vf0 to get true {0,0,0,0} */
NOMIPS16 v4sf
test_zero_vector (void)
{
  return (v4sf){0.0f, 0.0f, 0.0f, 0.0f};
}

/* Using zero vector in vmax should preserve W component */
NOMIPS16 v4sf
test_clamp_min_zero (v4sf input)
{
  v4sf zero = (v4sf){0.0f, 0.0f, 0.0f, 0.0f};
  return __builtin_vu0_vmax(input, zero);
}

/* Using zero vector in vmini should preserve W component */
NOMIPS16 v4sf
test_clamp_max_zero (v4sf input)
{
  v4sf zero = (v4sf){0.0f, 0.0f, 0.0f, 0.0f};
  return __builtin_vu0_vmini(input, zero);
}

/* Zero vector load should use vsub.xyzw $vfN,$vf0,$vf0 */
/* { dg-final { scan-assembler "\tvsub\\.xyzw\t\\\$vf\[0-9\]+,\\\$vf0,\\\$vf0" } } */

/* Should NOT see vmove from $vf0 for zero vectors (that gives W=1.0) */
/* { dg-final { scan-assembler-not "\tvmove\\.xyzw\t\\\$vf\[0-9\]+,\\\$vf0" } } */
