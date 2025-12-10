/* Test R5900 VU0 128-bit constant loading for V4SF mode */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mvu0" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef float v4sf __attribute__((vector_size(16)));

/* =========================================================================
   V4SF (4x32-bit float vector) constant loading

   Key points:
   - Zero vectors use vsub.xyzw $vfN,$vf0,$vf0 (NOT vmove from $vf0)
   - $vf0 is hardcoded to {0.0, 0.0, 0.0, 1.0}, so vmove would corrupt W
   - Non-zero constants load from memory via lqc2
   - V4SF uses VU0/COP2 registers exclusively (not GP registers)
   ========================================================================= */

/* Zero vector return: should use vsub.xyzw to compute true {0,0,0,0} */
NOMIPS16 v4sf
test_v4sf_return_zero (void)
{
  return (v4sf){0.0f, 0.0f, 0.0f, 0.0f};
}

/* Zero vector store: should use vsub.xyzw then sqc2 */
NOMIPS16 void
test_v4sf_store_zero (v4sf *p)
{
  *p = (v4sf){0.0f, 0.0f, 0.0f, 0.0f};
}

/* Non-zero constant return: should use lqc2 from constant pool */
NOMIPS16 v4sf
test_v4sf_return_nonzero (void)
{
  return (v4sf){1.0f, 2.0f, 3.0f, 4.0f};
}

/* Non-zero constant store: should use lqc2 then sqc2 */
NOMIPS16 void
test_v4sf_store_nonzero (v4sf *p)
{
  *p = (v4sf){1.0f, 2.0f, 3.0f, 4.0f};
}

/* Splat float constant: should use lqc2 from constant pool */
NOMIPS16 v4sf
test_v4sf_splat (void)
{
  return (v4sf){3.14159f, 3.14159f, 3.14159f, 3.14159f};
}

/* Unit vector patterns - common in graphics */
NOMIPS16 v4sf
test_v4sf_unit_x (void)
{
  return (v4sf){1.0f, 0.0f, 0.0f, 0.0f};
}

NOMIPS16 v4sf
test_v4sf_unit_w (void)
{
  /* This is NOT the same as $vf0 which is {0,0,0,1} -
     the element order may differ based on endianness */
  return (v4sf){0.0f, 0.0f, 0.0f, 1.0f};
}

/* =========================================================================
   Zero vector usage in operations

   When zero vectors are used in operations like vmax/vmini, the zero
   must be properly synthesized (not use $vf0 directly, which has W=1.0)
   ========================================================================= */

/* Zero in vmax: clamp minimum to zero */
NOMIPS16 v4sf
test_v4sf_clamp_min_zero (v4sf input)
{
  v4sf zero = (v4sf){0.0f, 0.0f, 0.0f, 0.0f};
  return __builtin_vu0_vmax (input, zero);
}

/* Zero in vmini: clamp maximum to zero */
NOMIPS16 v4sf
test_v4sf_clamp_max_zero (v4sf input)
{
  v4sf zero = (v4sf){0.0f, 0.0f, 0.0f, 0.0f};
  return __builtin_vu0_vmini (input, zero);
}

/* =========================================================================
   Verification patterns
   ========================================================================= */

/* Zero vectors should use vsub.xyzw $vfN,$vf0,$vf0 */
/* { dg-final { scan-assembler "\tvsub\\.xyzw\t\\\$vf\[0-9\]+,\\\$vf0,\\\$vf0" } } */

/* Non-zero constants should load from memory via lqc2 */
/* { dg-final { scan-assembler "\tlqc2\t" } } */

/* Stores should use sqc2 */
/* { dg-final { scan-assembler "\tsqc2\t" } } */

/* CRITICAL: Should NOT see vmove from $vf0 for zero vectors
   vmove.xyzw $vfN,$vf0 would give {0,0,0,1} not {0,0,0,0} */
/* { dg-final { scan-assembler-not "\tvmove\\.xyzw\t\\\$vf\[0-9\]+,\\\$vf0" } } */

/* V4SF should use VU0 registers, not GP register transfers
   No qmtc2/qmfc2 for V4SF constant handling */
/* { dg-final { scan-assembler-not "\tqmtc2\t" } } */
/* { dg-final { scan-assembler-not "\tqmfc2\t" } } */
