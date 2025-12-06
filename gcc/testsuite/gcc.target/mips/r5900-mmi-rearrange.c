/* Test R5900 MMI data rearrangement instructions */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef signed char v16qi __attribute__((vector_size(16)));
typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));
typedef long long v2di __attribute__((vector_size(16)));

/* === Pack Instructions === */

/* Test __builtin_mmi_ppacb */
NOMIPS16 v16qi
test_ppacb (v16qi a, v16qi b)
{
  return __builtin_mmi_ppacb (a, b);
}
/* { dg-final { scan-assembler "\\tppacb\\t" } } */

/* Test __builtin_mmi_ppach */
NOMIPS16 v8hi
test_ppach (v8hi a, v8hi b)
{
  return __builtin_mmi_ppach (a, b);
}
/* { dg-final { scan-assembler "\\tppach\\t" } } */

/* Test __builtin_mmi_ppacw */
NOMIPS16 v4si
test_ppacw (v4si a, v4si b)
{
  return __builtin_mmi_ppacw (a, b);
}
/* { dg-final { scan-assembler "\\tppacw\\t" } } */

/* Test __builtin_mmi_ppac5 */
NOMIPS16 v8hi
test_ppac5 (v4si a)
{
  return __builtin_mmi_ppac5 (a);
}
/* { dg-final { scan-assembler "\\tppac5\\t" } } */

/* === Extend Lower Instructions === */

/* Test __builtin_mmi_pextlb */
NOMIPS16 v16qi
test_pextlb (v16qi a, v16qi b)
{
  return __builtin_mmi_pextlb (a, b);
}
/* { dg-final { scan-assembler "\\tpextlb\\t" } } */

/* Test __builtin_mmi_pextlh */
NOMIPS16 v8hi
test_pextlh (v8hi a, v8hi b)
{
  return __builtin_mmi_pextlh (a, b);
}
/* { dg-final { scan-assembler "\\tpextlh\\t" } } */

/* Test __builtin_mmi_pextlw */
NOMIPS16 v4si
test_pextlw (v4si a, v4si b)
{
  return __builtin_mmi_pextlw (a, b);
}
/* { dg-final { scan-assembler "\\tpextlw\\t" } } */

/* === Extend Upper Instructions === */

/* Test __builtin_mmi_pextub */
NOMIPS16 v16qi
test_pextub (v16qi a, v16qi b)
{
  return __builtin_mmi_pextub (a, b);
}
/* { dg-final { scan-assembler "\\tpextub\\t" } } */

/* Test __builtin_mmi_pextuh */
NOMIPS16 v8hi
test_pextuh (v8hi a, v8hi b)
{
  return __builtin_mmi_pextuh (a, b);
}
/* { dg-final { scan-assembler "\\tpextuh\\t" } } */

/* Test __builtin_mmi_pextuw */
NOMIPS16 v4si
test_pextuw (v4si a, v4si b)
{
  return __builtin_mmi_pextuw (a, b);
}
/* { dg-final { scan-assembler "\\tpextuw\\t" } } */

/* Test __builtin_mmi_pext5 */
NOMIPS16 v4si
test_pext5 (v8hi a)
{
  return __builtin_mmi_pext5 (a);
}
/* { dg-final { scan-assembler "\\tpext5\\t" } } */

/* === Copy Instructions === */

/* Test __builtin_mmi_pcpyh */
NOMIPS16 v8hi
test_pcpyh (v8hi a)
{
  return __builtin_mmi_pcpyh (a);
}
/* { dg-final { scan-assembler "\\tpcpyh\\t" } } */

/* Test __builtin_mmi_pcpyld */
NOMIPS16 v2di
test_pcpyld (v2di a, v2di b)
{
  return __builtin_mmi_pcpyld (a, b);
}
/* { dg-final { scan-assembler "\\tpcpyld\\t" } } */

/* Test __builtin_mmi_pcpyud */
NOMIPS16 v2di
test_pcpyud (v2di a, v2di b)
{
  return __builtin_mmi_pcpyud (a, b);
}
/* { dg-final { scan-assembler "\\tpcpyud\\t" } } */

/* === Exchange Instructions === */

/* Test __builtin_mmi_pexch */
NOMIPS16 v8hi
test_pexch (v8hi a)
{
  return __builtin_mmi_pexch (a);
}
/* { dg-final { scan-assembler "\\tpexch\\t" } } */

/* Test __builtin_mmi_pexcw */
NOMIPS16 v4si
test_pexcw (v4si a)
{
  return __builtin_mmi_pexcw (a);
}
/* { dg-final { scan-assembler "\\tpexcw\\t" } } */

/* Test __builtin_mmi_pexeh */
NOMIPS16 v8hi
test_pexeh (v8hi a)
{
  return __builtin_mmi_pexeh (a);
}
/* { dg-final { scan-assembler "\\tpexeh\\t" } } */

/* Test __builtin_mmi_pexew */
NOMIPS16 v4si
test_pexew (v4si a)
{
  return __builtin_mmi_pexew (a);
}
/* { dg-final { scan-assembler "\\tpexew\\t" } } */

/* Test __builtin_mmi_prevh */
NOMIPS16 v8hi
test_prevh (v8hi a)
{
  return __builtin_mmi_prevh (a);
}
/* { dg-final { scan-assembler "\\tprevh\\t" } } */

/* === Interleave Instructions === */

/* Test __builtin_mmi_pinteh */
NOMIPS16 v8hi
test_pinteh (v8hi a, v8hi b)
{
  return __builtin_mmi_pinteh (a, b);
}
/* { dg-final { scan-assembler "\\tpinteh\\t" } } */

/* Test __builtin_mmi_pinth */
NOMIPS16 v8hi
test_pinth (v8hi a, v8hi b)
{
  return __builtin_mmi_pinth (a, b);
}
/* { dg-final { scan-assembler "\\tpinth\\t" } } */

/* === Rotation Instructions === */

/* Test __builtin_mmi_prot3w */
NOMIPS16 v4si
test_prot3w (v4si a)
{
  return __builtin_mmi_prot3w (a);
}
/* { dg-final { scan-assembler "\\tprot3w\\t" } } */
