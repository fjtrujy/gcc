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

/* Unsigned vector types for testing unsigned comparisons.
   MMI lacks PCGTU instruction, so unsigned comparisons are synthesized
   using XOR with sign bit followed by signed comparison.  */
typedef unsigned char v16uqi __attribute__((vector_size(16)));
typedef unsigned short v8uhi __attribute__((vector_size(16)));
typedef unsigned int v4usi __attribute__((vector_size(16)));

/* Test unsigned greater-than (GTU) - uses pxor + pcgt */
NOMIPS16 v16uqi
test_gtub (v16uqi a, v16uqi b)
{
  return a > b;
}
/* { dg-final { scan-assembler "test_gtub:.*pxor.*pxor.*pcgtb" } } */

NOMIPS16 v8uhi
test_gtuh (v8uhi a, v8uhi b)
{
  return a > b;
}
/* { dg-final { scan-assembler "test_gtuh:.*pxor.*pxor.*pcgth" } } */

NOMIPS16 v4usi
test_gtuw (v4usi a, v4usi b)
{
  return a > b;
}
/* { dg-final { scan-assembler "test_gtuw:.*pxor.*pxor.*pcgtw" } } */

/* Test unsigned less-than (LTU) - uses pxor + pcgt with swapped operands */
NOMIPS16 v16uqi
test_ltub (v16uqi a, v16uqi b)
{
  return a < b;
}
/* { dg-final { scan-assembler "test_ltub:.*pxor.*pxor.*pcgtb" } } */

NOMIPS16 v8uhi
test_ltuh (v8uhi a, v8uhi b)
{
  return a < b;
}
/* { dg-final { scan-assembler "test_ltuh:.*pxor.*pxor.*pcgth" } } */

NOMIPS16 v4usi
test_ltuw (v4usi a, v4usi b)
{
  return a < b;
}
/* { dg-final { scan-assembler "test_ltuw:.*pxor.*pxor.*pcgtw" } } */

/* Test unsigned greater-or-equal (GEU) - uses pxor + pcgt + pceq + por */
NOMIPS16 v16uqi
test_geub (v16uqi a, v16uqi b)
{
  return a >= b;
}
/* { dg-final { scan-assembler "test_geub:.*pceqb.*pxor.*pxor.*pcgtb.*por" } } */

NOMIPS16 v8uhi
test_geuh (v8uhi a, v8uhi b)
{
  return a >= b;
}
/* { dg-final { scan-assembler "test_geuh:.*pceqh.*pxor.*pxor.*pcgth.*por" } } */

NOMIPS16 v4usi
test_geuw (v4usi a, v4usi b)
{
  return a >= b;
}
/* { dg-final { scan-assembler "test_geuw:.*pceqw.*pxor.*pxor.*pcgtw.*por" } } */

/* Test unsigned less-or-equal (LEU) - uses pxor + pcgt + pnor */
NOMIPS16 v16uqi
test_leub (v16uqi a, v16uqi b)
{
  return a <= b;
}
/* { dg-final { scan-assembler "test_leub:.*pxor.*pxor.*pcgtb.*pnor" } } */

NOMIPS16 v8uhi
test_leuh (v8uhi a, v8uhi b)
{
  return a <= b;
}
/* { dg-final { scan-assembler "test_leuh:.*pxor.*pxor.*pcgth.*pnor" } } */

NOMIPS16 v4usi
test_leuw (v4usi a, v4usi b)
{
  return a <= b;
}
/* { dg-final { scan-assembler "test_leuw:.*pxor.*pxor.*pcgtw.*pnor" } } */
