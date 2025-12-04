/* Test R5900 VU0 broadcast add/subtract builtins */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mvu0" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef float v4sf __attribute__((vector_size(16)));

/* Test broadcast add: dest = src1 + src2.x/y/z/w */
NOMIPS16 void
test_broadcast_add (v4sf *dst, v4sf a, v4sf b)
{
  dst[0] = __builtin_vu0_vaddx (a, b);  /* a + b.x */
  dst[1] = __builtin_vu0_vaddy (a, b);  /* a + b.y */
  dst[2] = __builtin_vu0_vaddz (a, b);  /* a + b.z */
  dst[3] = __builtin_vu0_vaddw (a, b);  /* a + b.w */
}
/* { dg-final { scan-assembler "\\tvaddx\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvaddy\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvaddz\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvaddw\\.xyzw\\t" } } */

/* Test broadcast subtract: dest = src1 - src2.x/y/z/w */
NOMIPS16 void
test_broadcast_sub (v4sf *dst, v4sf a, v4sf b)
{
  dst[0] = __builtin_vu0_vsubx (a, b);  /* a - b.x */
  dst[1] = __builtin_vu0_vsuby (a, b);  /* a - b.y */
  dst[2] = __builtin_vu0_vsubz (a, b);  /* a - b.z */
  dst[3] = __builtin_vu0_vsubw (a, b);  /* a - b.w */
}
/* { dg-final { scan-assembler "\\tvsubx\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvsuby\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvsubz\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvsubw\\.xyzw\\t" } } */
