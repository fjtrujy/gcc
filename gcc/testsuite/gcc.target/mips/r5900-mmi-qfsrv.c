/* Test R5900 MMI QFSRV and SA register instructions (intrinsics) */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */

typedef __int128 ti;

/* Test MTSAB with immediate */
void test_mtsab(int val) {
    __builtin_mmi_mtsab(val, 5);
}
/* { dg-final { scan-assembler "\tmtsab\t" } } */

/* Test MTSAH with immediate */
void test_mtsah(int val) {
    __builtin_mmi_mtsah(val, 3);
}
/* { dg-final { scan-assembler "\tmtsah\t" } } */

/* Test MFSA */
int test_mfsa(void) {
    return __builtin_mmi_mfsa();
}
/* { dg-final { scan-assembler "\tmfsa\t" } } */

/* Test MTSA */
void test_mtsa(int val) {
    __builtin_mmi_mtsa(val);
}
/* { dg-final { scan-assembler "\tmtsa\t" } } */

/* Test QFSRV */
ti test_qfsrv(ti high, ti low) {
    return __builtin_mmi_qfsrv(high, low);
}
/* { dg-final { scan-assembler "\tqfsrv\t" } } */

/* Test 128-bit rotation (same register for both operands) */
ti test_rotate_48(ti data) {
    __builtin_mmi_mtsah(0, 3);  /* 3 halfwords = 48 bits */
    return __builtin_mmi_qfsrv(data, data);
}
