/* Test R5900 Pipeline 1 multiply/divide instructions (intrinsics) */
/* These instructions use the second MAC unit (MAC1) with HI1/LO1 registers,
   enabling parallel execution with Pipeline 0 for improved throughput.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */

/* Test MULT1 - Multiply Pipeline 1 (signed) */
void test_mult1(int a, int b) {
    __builtin_mips_mult1(a, b);
}
/* { dg-final { scan-assembler "\tmult1\t" } } */

/* Test MULTU1 - Multiply Unsigned Pipeline 1 */
void test_multu1(unsigned int a, unsigned int b) {
    __builtin_mips_multu1(a, b);
}
/* { dg-final { scan-assembler "\tmultu1\t" } } */

/* Test MADD1 - Multiply-Add Pipeline 1 (signed) */
void test_madd1(int a, int b, int c, int d) {
    __builtin_mips_mult1(a, b);
    __builtin_mips_madd1(c, d);
}
/* { dg-final { scan-assembler "\tmadd1\t" } } */

/* Test MADDU1 - Multiply-Add Unsigned Pipeline 1 */
void test_maddu1(unsigned int a, unsigned int b, unsigned int c, unsigned int d) {
    __builtin_mips_multu1(a, b);
    __builtin_mips_maddu1(c, d);
}
/* { dg-final { scan-assembler "\tmaddu1\t" } } */

/* Test DIV1 - Divide Pipeline 1 (signed) */
void test_div1(int a, int b) {
    __builtin_mips_div1(a, b);
}
/* { dg-final { scan-assembler "\tdiv1\t" } } */

/* Test DIVU1 - Divide Unsigned Pipeline 1 */
void test_divu1(unsigned int a, unsigned int b) {
    __builtin_mips_divu1(a, b);
}
/* { dg-final { scan-assembler "\tdivu1\t" } } */

/* Test MFHI1 - Move From HI1 Register */
int test_mfhi1(int a, int b) {
    __builtin_mips_mult1(a, b);
    return __builtin_mips_mfhi1();
}
/* { dg-final { scan-assembler "\tmfhi1\t" } } */

/* Test MFLO1 - Move From LO1 Register */
int test_mflo1(int a, int b) {
    __builtin_mips_mult1(a, b);
    return __builtin_mips_mflo1();
}
/* { dg-final { scan-assembler "\tmflo1\t" } } */

/* Test MTHI1 - Move To HI1 Register */
void test_mthi1(int val) {
    __builtin_mips_mthi1(val);
}
/* { dg-final { scan-assembler "\tmthi1\t" } } */

/* Test MTLO1 - Move To LO1 Register */
void test_mtlo1(int val) {
    __builtin_mips_mtlo1(val);
}
/* { dg-final { scan-assembler "\tmtlo1\t" } } */

/* Test complete 64-bit multiply result using Pipeline 1 */
long long test_mult1_full(int a, int b) {
    __builtin_mips_mult1(a, b);
    int hi = __builtin_mips_mfhi1();
    int lo = __builtin_mips_mflo1();
    return ((long long)hi << 32) | (unsigned int)lo;
}

/* Test complete signed divide with quotient and remainder */
void test_div1_complete(int a, int b, int *quot, int *rem) {
    __builtin_mips_div1(a, b);
    *quot = __builtin_mips_mflo1();
    *rem = __builtin_mips_mfhi1();
}

/* Test dual pipeline usage - both MAC0 and MAC1 can work in parallel */
void test_dual_pipeline(int a, int b, int c, int d,
                        int *result0, int *result1) {
    /* MAC0 (Pipeline 0) uses regular mult/mflo */
    /* MAC1 (Pipeline 1) uses mult1/mflo1 */
    /* In real usage, these would be scheduled for parallel execution */
    __builtin_mips_mult1(c, d);  /* Start mult on Pipeline 1 */
    *result0 = a * b;            /* Uses Pipeline 0 */
    *result1 = __builtin_mips_mflo1();  /* Get result from Pipeline 1 */
}
