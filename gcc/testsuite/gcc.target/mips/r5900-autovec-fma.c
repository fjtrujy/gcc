/* Test VU0 FMA autovectorization */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mvu0 -ftree-vectorize -ffast-math" } */
/* { dg-skip-if "requires optimization" { *-*-* } { "-O0" "-O1" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* Test FMA: d = a * b + c
   This should vectorize and use the fmav4sf4 pattern which generates:
   vmulaw.xyzw ACC, c, $vf0  (load c into accumulator via c * 1.0)
   vmadd.xyzw  d, a, b       (d = ACC + a * b = c + a*b) */
NOMIPS16 void
test_fma (float * __restrict d, float * __restrict a,
          float * __restrict b, float * __restrict c)
{
  for (int i = 0; i < 4; i++)
    d[i] = a[i] * b[i] + c[i];
}
/* { dg-final { scan-assembler "\\tvmulaw\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmadd\\.xyzw\\t" } } */

/* Test FMA with vector types directly */
typedef float v4sf __attribute__((vector_size(16)));

NOMIPS16 v4sf
test_fma_vec (v4sf a, v4sf b, v4sf c)
{
  return a * b + c;
}
/* The vector version should also use FMA */
