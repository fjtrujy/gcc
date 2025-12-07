/* Test that MMI operations properly clobber HI1/LO1 registers */
/* This test verifies that MMI parallel multiply/divide instructions
   are modeled as clobbering both HI0/LO0 and HI1/LO1, ensuring correct
   code generation when mixing MMI and Pipeline 1 scalar operations.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */

typedef __int128 ti;
typedef int v4si __attribute__((vector_size(16)));
typedef long long v2di __attribute__((vector_size(16)));

/* Test that PMULTW clobbers both HI0/LO0 and HI1/LO1 */
/* The MULT1 result in HI1/LO1 should be consumed before PMULTW */
int test_pmultw_after_mult1(int a, int b, v4si x, v4si y) {
    __builtin_mips_mult1(a, b);
    int hi1_before = __builtin_mips_mfhi1();  /* Must happen before PMULTW */
    v2di result = __builtin_mmi_pmultw(x, y);  /* Clobbers HI1/LO1 */
    return hi1_before;
}
/* { dg-final { scan-assembler "mult1" } } */
/* { dg-final { scan-assembler "mfhi1" } } */
/* { dg-final { scan-assembler "pmultw" } } */

/* Test that PMADDW clobbers HI1/LO1 */
v2di test_pmaddw_clobbers(v4si x, v4si y) {
    return __builtin_mmi_pmaddw(x, y);
}
/* { dg-final { scan-assembler "pmaddw" } } */

/* Test that PDIVW clobbers HI1/LO1 */
void test_pdivw_clobbers(v4si x, v4si y) {
    __builtin_mmi_pdivw(x, y);
}
/* { dg-final { scan-assembler "pdivw" } } */
