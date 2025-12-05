/* Test R5900 MMI parallel comparison instructions.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "requires optimization" { *-*-* } { "-O0" } { "" } } */

typedef char v16qi __attribute__((vector_size(16)));
typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));

/* Byte comparisons */
v16qi test_eq_byte(v16qi a, v16qi b) { return a == b; }
/* { dg-final { scan-assembler "\tpceqb\t" } } */

v16qi test_gt_byte(v16qi a, v16qi b) { return a > b; }
/* { dg-final { scan-assembler "\tpcgtb\t" } } */

v16qi test_lt_byte(v16qi a, v16qi b) { return a < b; }
/* { dg-final { scan-assembler "\tpcgtb\t.*\t\\\$\[0-9\]+,\\\$\[0-9\]+,\\\$\[0-9\]+" } } */

v16qi test_ne_byte(v16qi a, v16qi b) { return a != b; }
/* NE uses pceqb + pnor */

/* Halfword comparisons */
v8hi test_eq_half(v8hi a, v8hi b) { return a == b; }
/* { dg-final { scan-assembler "\tpceqh\t" } } */

v8hi test_gt_half(v8hi a, v8hi b) { return a > b; }
/* { dg-final { scan-assembler "\tpcgth\t" } } */

/* Word comparisons */
v4si test_eq_word(v4si a, v4si b) { return a == b; }
/* { dg-final { scan-assembler "\tpceqw\t" } } */

v4si test_gt_word(v4si a, v4si b) { return a > b; }
/* { dg-final { scan-assembler "\tpcgtw\t" } } */

v4si test_lt_word(v4si a, v4si b) { return a < b; }
/* LT uses pcgtw with swapped operands */

v4si test_ne_word(v4si a, v4si b) { return a != b; }
/* NE uses pceqw + pnor */

v4si test_le_word(v4si a, v4si b) { return a <= b; }
/* LE uses pcgtw + pnor */
/* { dg-final { scan-assembler "\tpnor\t" } } */

v4si test_ge_word(v4si a, v4si b) { return a >= b; }
/* GE uses pcgtw + pceqw + por */
/* { dg-final { scan-assembler "\tpor\t" } } */
