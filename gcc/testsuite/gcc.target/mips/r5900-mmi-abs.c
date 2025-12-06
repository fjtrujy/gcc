/* Test R5900 MMI parallel absolute value instructions.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "requires optimization" { *-*-* } { "-O0" } { "" } } */

typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));

v8hi test_pabsh(v8hi a) { return __builtin_mmi_pabsh(a); }
/* { dg-final { scan-assembler "\\tpabsh\\t" } } */

v4si test_pabsw(v4si a) { return __builtin_mmi_pabsw(a); }
/* { dg-final { scan-assembler "\\tpabsw\\t" } } */
