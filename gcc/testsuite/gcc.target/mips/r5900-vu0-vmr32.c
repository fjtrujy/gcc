/* Test R5900 VU0 vector rotate builtin */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mvu0" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef float v4sf __attribute__((vector_size(16)));

/* Test vector rotate by 32 bits (1 element):
   vmr32: dest.xyzw = src.yzwx
   This rotates all elements left by one position. */
NOMIPS16 v4sf
test_vmr32 (v4sf src)
{
  return __builtin_vu0_vmr32 (src);
}
/* { dg-final { scan-assembler "\\tvmr32\\.xyzw\\t" } } */

/* Test multiple rotations (rotate by 2, 3, 4 elements) */
NOMIPS16 void
test_multi_rotate (v4sf *dst, v4sf src)
{
  /* Rotate by 1: xyzw -> yzwx */
  dst[0] = __builtin_vu0_vmr32 (src);
  /* Rotate by 2: xyzw -> zwxy */
  dst[1] = __builtin_vu0_vmr32 (dst[0]);
  /* Rotate by 3: xyzw -> wxyz */
  dst[2] = __builtin_vu0_vmr32 (dst[1]);
  /* Rotate by 4 (full circle): xyzw -> xyzw */
  dst[3] = __builtin_vu0_vmr32 (dst[2]);
}
