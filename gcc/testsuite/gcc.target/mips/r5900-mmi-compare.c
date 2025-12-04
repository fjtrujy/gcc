/* Test R5900 MMI parallel comparison instructions */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef signed char v16qi __attribute__((vector_size(16)));
typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));

/* Test __builtin_mmi_pceqb */
NOMIPS16 v16qi
test_pceqb (v16qi a, v16qi b)
{
  return __builtin_mmi_pceqb (a, b);
}
/* { dg-final { scan-assembler "\\tpceqb\\t" } } */

/* Test __builtin_mmi_pceqh */
NOMIPS16 v8hi
test_pceqh (v8hi a, v8hi b)
{
  return __builtin_mmi_pceqh (a, b);
}
/* { dg-final { scan-assembler "\\tpceqh\\t" } } */

/* Test __builtin_mmi_pceqw */
NOMIPS16 v4si
test_pceqw (v4si a, v4si b)
{
  return __builtin_mmi_pceqw (a, b);
}
/* { dg-final { scan-assembler "\\tpceqw\\t" } } */

/* Test __builtin_mmi_pcgtb */
NOMIPS16 v16qi
test_pcgtb (v16qi a, v16qi b)
{
  return __builtin_mmi_pcgtb (a, b);
}
/* { dg-final { scan-assembler "\\tpcgtb\\t" } } */

/* Test __builtin_mmi_pcgth */
NOMIPS16 v8hi
test_pcgth (v8hi a, v8hi b)
{
  return __builtin_mmi_pcgth (a, b);
}
/* { dg-final { scan-assembler "\\tpcgth\\t" } } */

/* Test __builtin_mmi_pcgtw */
NOMIPS16 v4si
test_pcgtw (v4si a, v4si b)
{
  return __builtin_mmi_pcgtw (a, b);
}
/* { dg-final { scan-assembler "\\tpcgtw\\t" } } */
