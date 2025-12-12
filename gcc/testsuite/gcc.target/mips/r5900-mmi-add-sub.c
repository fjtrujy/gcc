/* Test R5900 MMI basic parallel add/subtract intrinsics */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */

typedef signed char v16qi __attribute__((vector_size(16)));
typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));

/* Test PADDB - parallel add bytes */
v16qi test_paddb(v16qi a, v16qi b) {
    return __builtin_mmi_paddb(a, b);
}
/* { dg-final { scan-assembler "\tpaddb\t" } } */

/* Test PADDH - parallel add halfwords */
v8hi test_paddh(v8hi a, v8hi b) {
    return __builtin_mmi_paddh(a, b);
}
/* { dg-final { scan-assembler "\tpaddh\t" } } */

/* Test PADDW - parallel add words */
v4si test_paddw(v4si a, v4si b) {
    return __builtin_mmi_paddw(a, b);
}
/* { dg-final { scan-assembler "\tpaddw\t" } } */

/* Test PSUBB - parallel subtract bytes */
v16qi test_psubb(v16qi a, v16qi b) {
    return __builtin_mmi_psubb(a, b);
}
/* { dg-final { scan-assembler "\tpsubb\t" } } */

/* Test PSUBH - parallel subtract halfwords */
v8hi test_psubh(v8hi a, v8hi b) {
    return __builtin_mmi_psubh(a, b);
}
/* { dg-final { scan-assembler "\tpsubh\t" } } */

/* Test PSUBW - parallel subtract words */
v4si test_psubw(v4si a, v4si b) {
    return __builtin_mmi_psubw(a, b);
}
/* { dg-final { scan-assembler "\tpsubw\t" } } */
