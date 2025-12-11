/* Test R5900 Pipeline 0 scalar multiply/divide intrinsics */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */

/* Multiply operations */
void test_mult(int a, int b) {
    __builtin_mips_mult(a, b);
}

void test_multu(unsigned a, unsigned b) {
    __builtin_mips_multu(a, b);
}

/* Divide operations */
void test_div(int a, int b) {
    __builtin_mips_div(a, b);
}

void test_divu(unsigned a, unsigned b) {
    __builtin_mips_divu(a, b);
}

/* Multiply-add operations */
void test_madd(int a, int b) {
    __builtin_mips_madd(a, b);
}

void test_maddu(unsigned a, unsigned b) {
    __builtin_mips_maddu(a, b);
}

/* Move from HI/LO */
int test_mfhi(void) {
    return __builtin_mips_mfhi();
}

int test_mflo(void) {
    return __builtin_mips_mflo();
}

/* Move to HI/LO */
void test_mthi(int v) {
    __builtin_mips_mthi(v);
}

void test_mtlo(int v) {
    __builtin_mips_mtlo(v);
}

/* Full 64-bit multiply using Pipeline 0 intrinsics */
long long mult0_64(int a, int b) {
    __builtin_mips_mult(a, b);
    int hi = __builtin_mips_mfhi();
    int lo = __builtin_mips_mflo();
    return ((long long)hi << 32) | (unsigned int)lo;
}

/* Division with quotient and remainder via Pipeline 0 */
void div0_full(int a, int b, int *quot, int *rem) {
    __builtin_mips_div(a, b);
    *quot = __builtin_mips_mflo();  /* quotient in LO0 */
    *rem = __builtin_mips_mfhi();   /* remainder in HI0 */
}

/* Multiply-accumulate chain on Pipeline 0 */
int mac0_chain(int a, int b, int c, int d) {
    __builtin_mips_mult(a, b);      /* HI0:LO0 = a * b */
    __builtin_mips_madd(c, d);      /* HI0:LO0 += c * d */
    return __builtin_mips_mflo();   /* return low 32 bits */
}

/* { dg-final { scan-assembler "\tmult\t" } } */
/* { dg-final { scan-assembler "\tmultu\t" } } */
/* { dg-final { scan-assembler "\tdiv\t" } } */
/* { dg-final { scan-assembler "\tdivu\t" } } */
/* { dg-final { scan-assembler "\tmadd\t" } } */
/* { dg-final { scan-assembler "\tmaddu\t" } } */
/* { dg-final { scan-assembler "\tmfhi\t" } } */
/* { dg-final { scan-assembler "\tmflo\t" } } */
/* { dg-final { scan-assembler "\tmthi\t" } } */
/* { dg-final { scan-assembler "\tmtlo\t" } } */
