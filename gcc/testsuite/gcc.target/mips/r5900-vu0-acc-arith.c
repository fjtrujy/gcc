/* Test R5900 VU0 accumulator add/subtract builtins */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mvu0" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef float v4sf __attribute__((vector_size(16)));

/* Test vadda: ACC = a + b */
NOMIPS16 v4sf
test_vadda (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vadda (a, b);
  return __builtin_vu0_vmadd (c, c);
}
/* { dg-final { scan-assembler "\\tvadda\\.xyzw\\t" } } */

/* Test vsuba: ACC = a - b */
NOMIPS16 v4sf
test_vsuba (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vsuba (a, b);
  return __builtin_vu0_vmadd (c, c);
}
/* { dg-final { scan-assembler "\\tvsuba\\.xyzw\\t" } } */

/* Test vaddax: ACC = a + b.x */
NOMIPS16 v4sf
test_vaddax (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vaddax (a, b);
  return __builtin_vu0_vmadd (c, c);
}
/* { dg-final { scan-assembler "\\tvaddax\\.xyzw\\t" } } */

/* Test vadday: ACC = a + b.y */
NOMIPS16 v4sf
test_vadday (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vadday (a, b);
  return __builtin_vu0_vmadd (c, c);
}
/* { dg-final { scan-assembler "\\tvadday\\.xyzw\\t" } } */

/* Test vaddaz: ACC = a + b.z */
NOMIPS16 v4sf
test_vaddaz (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vaddaz (a, b);
  return __builtin_vu0_vmadd (c, c);
}
/* { dg-final { scan-assembler "\\tvaddaz\\.xyzw\\t" } } */

/* Test vaddaw: ACC = a + b.w */
NOMIPS16 v4sf
test_vaddaw (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vaddaw (a, b);
  return __builtin_vu0_vmadd (c, c);
}
/* { dg-final { scan-assembler "\\tvaddaw\\.xyzw\\t" } } */

/* Test vsubax: ACC = a - b.x */
NOMIPS16 v4sf
test_vsubax (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vsubax (a, b);
  return __builtin_vu0_vmadd (c, c);
}
/* { dg-final { scan-assembler "\\tvsubax\\.xyzw\\t" } } */

/* Test vsubay: ACC = a - b.y */
NOMIPS16 v4sf
test_vsubay (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vsubay (a, b);
  return __builtin_vu0_vmadd (c, c);
}
/* { dg-final { scan-assembler "\\tvsubay\\.xyzw\\t" } } */

/* Test vsubaz: ACC = a - b.z */
NOMIPS16 v4sf
test_vsubaz (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vsubaz (a, b);
  return __builtin_vu0_vmadd (c, c);
}
/* { dg-final { scan-assembler "\\tvsubaz\\.xyzw\\t" } } */

/* Test vsubaw: ACC = a - b.w */
NOMIPS16 v4sf
test_vsubaw (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vsubaw (a, b);
  return __builtin_vu0_vmadd (c, c);
}
/* { dg-final { scan-assembler "\\tvsubaw\\.xyzw\\t" } } */
