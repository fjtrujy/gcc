/* Test R5900 MMI saturating add/subtract intrinsics */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */

typedef signed char v16qi __attribute__((vector_size(16)));
typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));

/* Test signed saturating add: PADDSB, PADDSH, PADDSW */
v16qi test_paddsb(v16qi a, v16qi b) {
    return __builtin_mmi_paddsb(a, b);
}
/* { dg-final { scan-assembler "\tpaddsb\t" } } */

v8hi test_paddsh(v8hi a, v8hi b) {
    return __builtin_mmi_paddsh(a, b);
}
/* { dg-final { scan-assembler "\tpaddsh\t" } } */

v4si test_paddsw(v4si a, v4si b) {
    return __builtin_mmi_paddsw(a, b);
}
/* { dg-final { scan-assembler "\tpaddsw\t" } } */

/* Test signed saturating subtract: PSUBSB, PSUBSH, PSUBSW */
v16qi test_psubsb(v16qi a, v16qi b) {
    return __builtin_mmi_psubsb(a, b);
}
/* { dg-final { scan-assembler "\tpsubsb\t" } } */

v8hi test_psubsh(v8hi a, v8hi b) {
    return __builtin_mmi_psubsh(a, b);
}
/* { dg-final { scan-assembler "\tpsubsh\t" } } */

v4si test_psubsw(v4si a, v4si b) {
    return __builtin_mmi_psubsw(a, b);
}
/* { dg-final { scan-assembler "\tpsubsw\t" } } */

/* Test unsigned saturating add: PADDUB, PADDUH, PADDUW */
v16qi test_paddub(v16qi a, v16qi b) {
    return __builtin_mmi_paddub(a, b);
}
/* { dg-final { scan-assembler "\tpaddub\t" } } */

v8hi test_padduh(v8hi a, v8hi b) {
    return __builtin_mmi_padduh(a, b);
}
/* { dg-final { scan-assembler "\tpadduh\t" } } */

v4si test_padduw(v4si a, v4si b) {
    return __builtin_mmi_padduw(a, b);
}
/* { dg-final { scan-assembler "\tpadduw\t" } } */

/* Test unsigned saturating subtract: PSUBUB, PSUBUH, PSUBUW */
v16qi test_psubub(v16qi a, v16qi b) {
    return __builtin_mmi_psubub(a, b);
}
/* { dg-final { scan-assembler "\tpsubub\t" } } */

v8hi test_psubuh(v8hi a, v8hi b) {
    return __builtin_mmi_psubuh(a, b);
}
/* { dg-final { scan-assembler "\tpsubuh\t" } } */

v4si test_psubuw(v4si a, v4si b) {
    return __builtin_mmi_psubuw(a, b);
}
/* { dg-final { scan-assembler "\tpsubuw\t" } } */
