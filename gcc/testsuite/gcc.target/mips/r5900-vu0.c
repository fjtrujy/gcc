/* Test R5900 VU0 128-bit vector float instructions */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mvu0" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" "-O1" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef float v4sf __attribute__((vector_size(16)));

/* Test direct VU0 load/store - lqc2/sqc2 when values stay in VU0 registers */
NOMIPS16 void
test_add_mem (v4sf *dst, v4sf *a, v4sf *b)
{
  *dst = *a + *b;
}
/* { dg-final { scan-assembler "\tlqc2\t" } } */
/* { dg-final { scan-assembler "\tsqc2\t" } } */

/* Test VU0 vector addition (vadd.xyzw) */
/* { dg-final { scan-assembler "\tvadd\\.xyzw\t" } } */

/* Test VU0 vector subtraction (vsub.xyzw) */
NOMIPS16 void
test_sub_mem (v4sf *dst, v4sf *a, v4sf *b)
{
  *dst = *a - *b;
}
/* { dg-final { scan-assembler "\tvsub\\.xyzw\t" } } */

/* Test VU0 vector multiplication (vmul.xyzw) */
NOMIPS16 void
test_mul_mem (v4sf *dst, v4sf *a, v4sf *b)
{
  *dst = *a * *b;
}
/* { dg-final { scan-assembler "\tvmul\\.xyzw\t" } } */

/* Test VU0 memory copy using lqc2/sqc2 */
NOMIPS16 void
test_copy (v4sf *dst, v4sf *src)
{
  *dst = *src;
}

/* Test GP to GP register copy (por) - when V4SF passed/returned via GP regs */
NOMIPS16 v4sf
test_return_param (v4sf a)
{
  return a;
}
/* { dg-final { scan-assembler "\tpor\t" } } */

/* Test VU0 vector*scalar autovectorization (vmulx.xyzw) */
NOMIPS16 v4sf
test_mul_scalar (v4sf v, float s)
{
  return v * s;
}
/* { dg-final { scan-assembler "\tvmulx\\.xyzw\t" } } */

/* Test VU0 vector+scalar autovectorization (vaddx.xyzw) */
NOMIPS16 v4sf
test_add_scalar (v4sf v, float s)
{
  return v + s;
}
/* { dg-final { scan-assembler "\tvaddx\\.xyzw\t" } } */

/* Test VU0 vector-scalar autovectorization (vsubx.xyzw) */
NOMIPS16 v4sf
test_sub_scalar (v4sf v, float s)
{
  return v - s;
}
/* { dg-final { scan-assembler "\tvsubx\\.xyzw\t" } } */
