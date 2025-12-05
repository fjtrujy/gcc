/* Test R5900 MMI parallel shift instructions */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));

/* Immediate shifts - halfword */
NOMIPS16 v8hi test_psllh (v8hi a) { return __builtin_mmi_psllh(a, 4); }
/* { dg-final { scan-assembler "\tpsllh\t" } } */

NOMIPS16 v8hi test_psrlh (v8hi a) { return __builtin_mmi_psrlh(a, 4); }
/* { dg-final { scan-assembler "\tpsrlh\t" } } */

NOMIPS16 v8hi test_psrah (v8hi a) { return __builtin_mmi_psrah(a, 4); }
/* { dg-final { scan-assembler "\tpsrah\t" } } */

/* Immediate shifts - word */
NOMIPS16 v4si test_psllw (v4si a) { return __builtin_mmi_psllw(a, 8); }
/* { dg-final { scan-assembler "\tpsllw\t" } } */

NOMIPS16 v4si test_psrlw (v4si a) { return __builtin_mmi_psrlw(a, 8); }
/* { dg-final { scan-assembler "\tpsrlw\t" } } */

NOMIPS16 v4si test_psraw (v4si a) { return __builtin_mmi_psraw(a, 8); }
/* { dg-final { scan-assembler "\tpsraw\t" } } */

/* Variable shifts - word only */
NOMIPS16 v4si test_psllvw (v4si a, v4si b) { return __builtin_mmi_psllvw(a, b); }
/* { dg-final { scan-assembler "\tpsllvw\t" } } */

NOMIPS16 v4si test_psrlvw (v4si a, v4si b) { return __builtin_mmi_psrlvw(a, b); }
/* { dg-final { scan-assembler "\tpsrlvw\t" } } */

NOMIPS16 v4si test_psravw (v4si a, v4si b) { return __builtin_mmi_psravw(a, b); }
/* { dg-final { scan-assembler "\tpsravw\t" } } */
