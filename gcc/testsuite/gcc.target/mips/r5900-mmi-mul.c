/* Test R5900 MMI multiply/divide builtins */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */

typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));
typedef long long v2di __attribute__((vector_size(16)));
typedef __int128 ti;

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* === HI/LO Register Operations === */

NOMIPS16 ti
test_pmfhi (void)
{
  return __builtin_mmi_pmfhi ();
}
/* { dg-final { scan-assembler "\\tpmfhi\\t" } } */

NOMIPS16 ti
test_pmflo (void)
{
  return __builtin_mmi_pmflo ();
}
/* { dg-final { scan-assembler "\\tpmflo\\t" } } */

NOMIPS16 void
test_pmthi (ti a)
{
  __builtin_mmi_pmthi (a);
}
/* { dg-final { scan-assembler "\\tpmthi\\t" } } */

NOMIPS16 void
test_pmtlo (ti a)
{
  __builtin_mmi_pmtlo (a);
}
/* { dg-final { scan-assembler "\\tpmtlo\\t" } } */

NOMIPS16 v2di
test_pmfhl_lw (void)
{
  return __builtin_mmi_pmfhl_lw ();
}
/* { dg-final { scan-assembler "\\tpmfhl.lw\\t" } } */

NOMIPS16 v2di
test_pmfhl_uw (void)
{
  return __builtin_mmi_pmfhl_uw ();
}
/* { dg-final { scan-assembler "\\tpmfhl.uw\\t" } } */

NOMIPS16 v2di
test_pmfhl_slw (void)
{
  return __builtin_mmi_pmfhl_slw ();
}
/* { dg-final { scan-assembler "\\tpmfhl.slw\\t" } } */

NOMIPS16 v8hi
test_pmfhl_lh (void)
{
  return __builtin_mmi_pmfhl_lh ();
}
/* { dg-final { scan-assembler "\\tpmfhl.lh\\t" } } */

NOMIPS16 v8hi
test_pmfhl_sh (void)
{
  return __builtin_mmi_pmfhl_sh ();
}
/* { dg-final { scan-assembler "\\tpmfhl.sh\\t" } } */

NOMIPS16 void
test_pmthl_lw (v4si a)
{
  __builtin_mmi_pmthl_lw (a);
}
/* { dg-final { scan-assembler "\\tpmthl.lw\\t" } } */

/* === Multiply Operations === */

NOMIPS16 v2di
test_pmultw (v4si a, v4si b)
{
  return __builtin_mmi_pmultw (a, b);
}
/* { dg-final { scan-assembler "\\tpmultw\\t" } } */

NOMIPS16 v2di
test_pmultuw (v4si a, v4si b)
{
  return __builtin_mmi_pmultuw (a, b);
}
/* { dg-final { scan-assembler "\\tpmultuw\\t" } } */

NOMIPS16 v4si
test_pmulth (v8hi a, v8hi b)
{
  return __builtin_mmi_pmulth (a, b);
}
/* { dg-final { scan-assembler "\\tpmulth\\t" } } */

/* === Multiply-Add/Subtract Operations === */

NOMIPS16 v2di
test_pmaddw (v4si a, v4si b)
{
  return __builtin_mmi_pmaddw (a, b);
}
/* { dg-final { scan-assembler "\\tpmaddw\\t" } } */

NOMIPS16 v2di
test_pmadduw (v4si a, v4si b)
{
  return __builtin_mmi_pmadduw (a, b);
}
/* { dg-final { scan-assembler "\\tpmadduw\\t" } } */

NOMIPS16 v4si
test_pmaddh (v8hi a, v8hi b)
{
  return __builtin_mmi_pmaddh (a, b);
}
/* { dg-final { scan-assembler "\\tpmaddh\\t" } } */

NOMIPS16 v2di
test_pmsubw (v4si a, v4si b)
{
  return __builtin_mmi_pmsubw (a, b);
}
/* { dg-final { scan-assembler "\\tpmsubw\\t" } } */

NOMIPS16 v4si
test_pmsubh (v8hi a, v8hi b)
{
  return __builtin_mmi_pmsubh (a, b);
}
/* { dg-final { scan-assembler "\\tpmsubh\\t" } } */

/* === Horizontal Multiply Operations === */

NOMIPS16 v4si
test_phmadh (v8hi a, v8hi b)
{
  return __builtin_mmi_phmadh (a, b);
}
/* { dg-final { scan-assembler "\\tphmadh\\t" } } */

NOMIPS16 v4si
test_phmsbh (v8hi a, v8hi b)
{
  return __builtin_mmi_phmsbh (a, b);
}
/* { dg-final { scan-assembler "\\tphmsbh\\t" } } */

/* === Divide Operations === */

NOMIPS16 void
test_pdivw (v4si a, v4si b)
{
  __builtin_mmi_pdivw (a, b);
}
/* { dg-final { scan-assembler "\\tpdivw\\t" } } */

NOMIPS16 void
test_pdivuw (v4si a, v4si b)
{
  __builtin_mmi_pdivuw (a, b);
}
/* { dg-final { scan-assembler "\\tpdivuw\\t" } } */

NOMIPS16 void
test_pdivbw (v4si a, v8hi b)
{
  __builtin_mmi_pdivbw (a, b);
}
/* { dg-final { scan-assembler "\\tpdivbw\\t" } } */
