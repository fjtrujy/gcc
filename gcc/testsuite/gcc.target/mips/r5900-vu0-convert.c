/* Test R5900 VU0 float/fixed-point conversion builtins */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mvu0" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef float v4sf __attribute__((vector_size(16)));

/* Test float to fixed-point conversions:
   vftoi0:  Convert to 32-bit signed integer (no fractional bits)
   vftoi4:  Convert to 28.4 fixed-point (4 fractional bits)
   vftoi12: Convert to 20.12 fixed-point (12 fractional bits)
   vftoi15: Convert to 17.15 fixed-point (15 fractional bits) */
NOMIPS16 void
test_float_to_fixed (v4sf *dst, v4sf src)
{
  dst[0] = __builtin_vu0_vftoi0 (src);
  dst[1] = __builtin_vu0_vftoi4 (src);
  dst[2] = __builtin_vu0_vftoi12 (src);
  dst[3] = __builtin_vu0_vftoi15 (src);
}
/* { dg-final { scan-assembler "\\tvftoi0\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvftoi4\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvftoi12\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvftoi15\\.xyzw\\t" } } */

/* Test fixed-point to float conversions:
   vitof0:  Convert from 32-bit signed integer (no fractional bits)
   vitof4:  Convert from 28.4 fixed-point (4 fractional bits)
   vitof12: Convert from 20.12 fixed-point (12 fractional bits)
   vitof15: Convert from 17.15 fixed-point (15 fractional bits) */
NOMIPS16 void
test_fixed_to_float (v4sf *dst, v4sf src)
{
  dst[0] = __builtin_vu0_vitof0 (src);
  dst[1] = __builtin_vu0_vitof4 (src);
  dst[2] = __builtin_vu0_vitof12 (src);
  dst[3] = __builtin_vu0_vitof15 (src);
}
/* { dg-final { scan-assembler "\\tvitof0\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvitof4\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvitof12\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvitof15\\.xyzw\\t" } } */

/* Test round-trip conversion (common pattern in PS2 graphics) */
NOMIPS16 v4sf
test_round_trip (v4sf src)
{
  /* Convert to fixed, then back to float */
  v4sf fixed = __builtin_vu0_vftoi4 (src);
  return __builtin_vu0_vitof4 (fixed);
}
