/* Test R5900 MMI parallel min/max instructions */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));

/* Test __builtin_mmi_pmaxh */
NOMIPS16 v8hi
test_pmaxh (v8hi a, v8hi b)
{
  return __builtin_mmi_pmaxh (a, b);
}
/* { dg-final { scan-assembler "\\tpmaxh\\t" } } */

/* Test __builtin_mmi_pmaxw */
NOMIPS16 v4si
test_pmaxw (v4si a, v4si b)
{
  return __builtin_mmi_pmaxw (a, b);
}
/* { dg-final { scan-assembler "\\tpmaxw\\t" } } */

/* Test __builtin_mmi_pminh */
NOMIPS16 v8hi
test_pminh (v8hi a, v8hi b)
{
  return __builtin_mmi_pminh (a, b);
}
/* { dg-final { scan-assembler "\\tpminh\\t" } } */

/* Test __builtin_mmi_pminw */
NOMIPS16 v4si
test_pminw (v4si a, v4si b)
{
  return __builtin_mmi_pminw (a, b);
}
/* { dg-final { scan-assembler "\\tpminw\\t" } } */
