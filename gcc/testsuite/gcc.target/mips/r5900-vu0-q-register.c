/* Test R5900 VU0 Q register operations (division/sqrt and Q broadcast) */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mvu0" } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef float v4sf __attribute__((vector_size(16)));

/* Test __builtin_vu0_vdiv: Q = fs.bc / ft.bc */
NOMIPS16 void
test_vdiv (v4sf fs, v4sf ft)
{
  __builtin_vu0_vdiv (fs, 0, ft, 1);  /* Q = fs.x / ft.y */
}
/* { dg-final { scan-assembler "\\tvdiv\\t\\\$Q,.*x,.*y" } } */

/* Test __builtin_vu0_vsqrt: Q = sqrt(ft.bc) */
NOMIPS16 void
test_vsqrt (v4sf ft)
{
  __builtin_vu0_vsqrt (ft, 2);  /* Q = sqrt(ft.z) */
}
/* { dg-final { scan-assembler "\\tvsqrt\\t\\\$Q,.*z" } } */

/* Test __builtin_vu0_vrsqrt: Q = fs.bc / sqrt(ft.bc) */
NOMIPS16 void
test_vrsqrt (v4sf fs, v4sf ft)
{
  __builtin_vu0_vrsqrt (fs, 3, ft, 0);  /* Q = fs.w / sqrt(ft.x) */
}
/* { dg-final { scan-assembler "\\tvrsqrt\\t\\\$Q,.*w,.*x" } } */

/* Test __builtin_vu0_vwaitq */
NOMIPS16 void
test_vwaitq (void)
{
  __builtin_vu0_vwaitq ();
}
/* { dg-final { scan-assembler "\\tvwaitq" } } */

/* Test __builtin_vu0_vaddq: dest = src + Q */
NOMIPS16 v4sf
test_vaddq (v4sf a)
{
  return __builtin_vu0_vaddq (a);
}
/* { dg-final { scan-assembler "\\tvaddq\\.xyzw\\t" } } */

/* Test __builtin_vu0_vsubq: dest = src - Q */
NOMIPS16 v4sf
test_vsubq (v4sf a)
{
  return __builtin_vu0_vsubq (a);
}
/* { dg-final { scan-assembler "\\tvsubq\\.xyzw\\t" } } */

/* Test __builtin_vu0_vmulq: dest = src * Q */
NOMIPS16 v4sf
test_vmulq (v4sf a)
{
  return __builtin_vu0_vmulq (a);
}
/* { dg-final { scan-assembler "\\tvmulq\\.xyzw\\t" } } */

/* Test __builtin_vu0_vmaxq: dest = max(src, Q) */
NOMIPS16 v4sf
test_vmaxq (v4sf a)
{
  return __builtin_vu0_vmaxq (a);
}
/* { dg-final { scan-assembler "\\tvmaxq\\.xyzw\\t" } } */

/* Test __builtin_vu0_vminiq: dest = min(src, Q) */
NOMIPS16 v4sf
test_vminiq (v4sf a)
{
  return __builtin_vu0_vminiq (a);
}
/* { dg-final { scan-assembler "\\tvminiq\\.xyzw\\t" } } */

/* Test __builtin_vu0_vaddaq: ACC = src + Q */
NOMIPS16 void
test_vaddaq (v4sf a)
{
  __builtin_vu0_vaddaq (a);
}
/* { dg-final { scan-assembler "\\tvaddaq\\.xyzw\\t\\\$ACC," } } */

/* Test __builtin_vu0_vsubaq: ACC = src - Q */
NOMIPS16 void
test_vsubaq (v4sf a)
{
  __builtin_vu0_vsubaq (a);
}
/* { dg-final { scan-assembler "\\tvsubaq\\.xyzw\\t\\\$ACC," } } */

/* Test __builtin_vu0_vmulaq: ACC = src * Q */
NOMIPS16 void
test_vmulaq (v4sf a)
{
  __builtin_vu0_vmulaq (a);
}
/* { dg-final { scan-assembler "\\tvmulaq\\.xyzw\\t\\\$ACC," } } */

/* Test __builtin_vu0_vmaddq: dest = ACC + src * Q */
NOMIPS16 v4sf
test_vmaddq (v4sf a)
{
  return __builtin_vu0_vmaddq (a);
}
/* { dg-final { scan-assembler "\\tvmaddq\\.xyzw\\t" } } */

/* Test __builtin_vu0_vmsubq: dest = ACC - src * Q */
NOMIPS16 v4sf
test_vmsubq (v4sf a)
{
  return __builtin_vu0_vmsubq (a);
}
/* { dg-final { scan-assembler "\\tvmsubq\\.xyzw\\t" } } */

/* Test __builtin_vu0_vmaddaq: ACC = ACC + src * Q */
NOMIPS16 void
test_vmaddaq (v4sf a)
{
  __builtin_vu0_vmaddaq (a);
}
/* { dg-final { scan-assembler "\\tvmaddaq\\.xyzw\\t\\\$ACC," } } */

/* Test __builtin_vu0_vmsubaq: ACC = ACC - src * Q */
NOMIPS16 void
test_vmsubaq (v4sf a)
{
  __builtin_vu0_vmsubaq (a);
}
/* { dg-final { scan-assembler "\\tvmsubaq\\.xyzw\\t\\\$ACC," } } */
