/* Test R5900 MMI parallel logical instructions */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef long long v2di __attribute__((vector_size(16)));

/* Test __builtin_mmi_pand */
NOMIPS16 v2di
test_pand (v2di a, v2di b)
{
  return __builtin_mmi_pand (a, b);
}
/* { dg-final { scan-assembler "\\tpand\\t" } } */

/* Test __builtin_mmi_por */
NOMIPS16 v2di
test_por (v2di a, v2di b)
{
  return __builtin_mmi_por (a, b);
}
/* { dg-final { scan-assembler "\\tpor\\t" } } */

/* Test __builtin_mmi_pxor */
NOMIPS16 v2di
test_pxor (v2di a, v2di b)
{
  return __builtin_mmi_pxor (a, b);
}
/* { dg-final { scan-assembler "\\tpxor\\t" } } */

/* Test __builtin_mmi_pnor */
NOMIPS16 v2di
test_pnor (v2di a, v2di b)
{
  return __builtin_mmi_pnor (a, b);
}
/* { dg-final { scan-assembler "\\tpnor\\t" } } */
