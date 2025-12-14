/* Test R5900 MMI parallel add/subtract instructions */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef signed char v16qi __attribute__((vector_size(16)));
typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));

/* Test __builtin_mmi_paddb */
NOMIPS16 v16qi
test_paddb (v16qi a, v16qi b)
{
  return __builtin_mmi_paddb (a, b);
}
/* { dg-final { scan-assembler "\\tpaddb\\t" } } */

/* Test __builtin_mmi_paddh */
NOMIPS16 v8hi
test_paddh (v8hi a, v8hi b)
{
  return __builtin_mmi_paddh (a, b);
}
/* { dg-final { scan-assembler "\\tpaddh\\t" } } */

/* Test __builtin_mmi_paddw */
NOMIPS16 v4si
test_paddw (v4si a, v4si b)
{
  return __builtin_mmi_paddw (a, b);
}
/* { dg-final { scan-assembler "\\tpaddw\\t" } } */

/* Test __builtin_mmi_padsbh */
NOMIPS16 v8hi
test_padsbh (v8hi a, v8hi b)
{
  return __builtin_mmi_padsbh (a, b);
}
/* { dg-final { scan-assembler "\\tpadsbh\\t" } } */

/* Test __builtin_mmi_psubb */
NOMIPS16 v16qi
test_psubb (v16qi a, v16qi b)
{
  return __builtin_mmi_psubb (a, b);
}
/* { dg-final { scan-assembler "\\tpsubb\\t" } } */

/* Test __builtin_mmi_psubh */
NOMIPS16 v8hi
test_psubh (v8hi a, v8hi b)
{
  return __builtin_mmi_psubh (a, b);
}
/* { dg-final { scan-assembler "\\tpsubh\\t" } } */

/* Test __builtin_mmi_psubw */
NOMIPS16 v4si
test_psubw (v4si a, v4si b)
{
  return __builtin_mmi_psubw (a, b);
}
/* { dg-final { scan-assembler "\\tpsubw\\t" } } */
