/* Test R5900 128-bit callee-saved register preservation using lq/sq */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef int int128_t __attribute__((mode(TI)));
typedef int v4si __attribute__((vector_size(16)));

extern int128_t bar (void);

/* Test that 128-bit values are preserved across function calls.
   The parameter 'a' must be saved to the stack (using sq) before calling bar(),
   and restored (using lq) after bar() returns.  */
NOMIPS16 int128_t
test_callee_saved_ti (int128_t a)
{
  int128_t b = bar ();
  return a + b;
}

extern v4si bar_v4si (void);

/* Same test with vector type */
NOMIPS16 v4si
test_callee_saved_v4si (v4si a)
{
  v4si b = bar_v4si ();
  return a + b;
}

/* Prologue should use sq to save callee-saved registers */
/* { dg-final { scan-assembler "\tsq\t\\\$" } } */
/* Epilogue should use lq to restore callee-saved registers */
/* { dg-final { scan-assembler "\tlq\t\\\$" } } */
