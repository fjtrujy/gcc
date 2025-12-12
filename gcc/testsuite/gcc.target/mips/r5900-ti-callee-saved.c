/* Test R5900 128-bit callee-saved register save/restore using lq/sq */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

typedef int int128_t __attribute__((mode(TI)));

extern void external_func(void);
extern int128_t external_ti_func(int128_t, int128_t, int128_t);

/* Test 1: Single TImode value preserved across function call.
   Should save/restore $ra as 128-bit using sq/lq.  */
int128_t test_single_ti_preserved(int128_t *p) {
    int128_t val = *p;
    external_func();
    return val;
}

/* Test 2: Multiple TImode values preserved across function call.
   Forces use of callee-saved registers $s0-$s7 by using all values.  */
int128_t test_multiple_ti_preserved(int128_t *p1, int128_t *p2, int128_t *p3) {
    int128_t a = *p1;
    int128_t b = *p2;
    int128_t c = *p3;
    external_func();
    /* Use all values to ensure they're all live across call */
    return external_ti_func(a, b, c);
}

/* Test 3: TImode parameter preserved across call.
   The parameter itself must be saved since it's passed in caller-saved reg.  */
int128_t test_ti_param_preserved(int128_t val) {
    external_func();
    return val;
}

/* Test 4: Complex control flow with TImode values.
   Tests that callee-saved handling works correctly with branches.  */
int128_t test_ti_with_branches(int128_t *p, int cond) {
    int128_t val = *p;
    if (cond) {
        external_func();
    }
    return val;
}

/* Test 5: Multiple TImode values all used after multiple calls.
   This should force use of callee-saved registers.  */
int128_t test_force_callee_saved(int128_t a, int128_t b, int128_t c) {
    external_func();
    external_func();
    /* All values must survive across both calls */
    return external_ti_func(a, b, c);
}

/* Expected assembly patterns:
   - Return address should be saved/restored with sq/lq (128-bit)
   - TImode values loaded/stored with lq/sq
   - Callee-saved registers ($s0-$s7, $ra) saved with sq, restored with lq */

/* { dg-final { scan-assembler "\tsq\t\\\$31," } } */
/* { dg-final { scan-assembler "\tlq\t\\\$31," } } */
/* { dg-final { scan-assembler "\tsq\t" } } */
/* { dg-final { scan-assembler "\tlq\t" } } */
