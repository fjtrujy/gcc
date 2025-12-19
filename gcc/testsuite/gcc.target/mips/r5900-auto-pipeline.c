/* Test R5900 automatic dual pipeline selection for multiply operations */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mhard-float" } */
/* { dg-skip-if "code quality" { *-*-* } { "-O0" } { "" } } */

/* 
 * This test verifies that GCC automatically selects between
 * Pipeline 0 (mult) and Pipeline 1 (mult1).
 *
 * The mul<mode>3_mul3 pattern has four alternatives for R5900:
 * - Alt 0: Pipeline 0, result in GPR (mult rd,rs,rt) - R5900/R3900 specific
 * - Alt 1: Pipeline 0, result in LO (mult rs,rt) - 2-operand form
 * - Alt 2: Pipeline 1, result in GPR (mult1 rd,rs,rt) - R5900 specific
 * - Alt 3: Pipeline 1, result in LO1 (mult1 rs,rt) - 2-operand form, R5900 specific
 *
 * Note: The 3-operand form "mult rd,rs,rt" is R5900/R3900 specific.
 * Standard MIPS uses "mul" for 3-operand multiply.
 *
 * GCC automatic selection:
 * - For low part only: 3-operand form (Pipeline 1 preferred)
 * - For accumulator patterns (madd): 2-operand form (Pipeline 0, needs HI:LO)
 *
 * Limitations:
 * - The 2-operand mult1 (Alt 3) is not auto-selected by GCC.
 * - Accumulator chains always use Pipeline 0 (mult+madd, not mult1+madd1).
 *   This is because the combiner creates chains around HI:LO, not HI1:LO1.
 * - To use Pipeline 1 for accumulation, use built-ins:
 *   __builtin_mips_mult1(), __builtin_mips_madd1(), __builtin_mips_madd1_3op()
 */

/* Independent multiplies (low part only) - GCC uses 3-operand Pipeline 1 */
void independent_multiplies (int a, int b, int c, int d,
                             int *r1, int *r2)
{
  *r1 = a * b;
  *r2 = c * d;
}

/* Accumulator pattern - GCC uses 2-operand Pipeline 0 (mult + madd) */
int accumulator_multiply (int a, int b, int c, int d)
{
  /* This pattern is recognized as: (a*b) + (c*d) using mult+madd */
  return a * b + c * d;
}

/* 
 * Verify different pipeline/operand combinations:
 * - mult1 rd,rs,rt: 3-operand Pipeline 1 (for low part only)
 * - mult rs,rt: 2-operand Pipeline 0 (for accumulator patterns)
 * - madd: Accumulator multiply-add (Pipeline 0)
 */

/* Pipeline 1: 3-operand multiply (mult1 rd,rs,rt) */
/* { dg-final { scan-assembler "\tmult1\t\\\$\[0-9\]+,\\\$\[0-9\]+,\\\$\[0-9\]+" } } */

/* Pipeline 0: 2-operand multiply (mult rs,rt) - used for accumulator patterns */
/* { dg-final { scan-assembler "\tmult\t\\\$\[0-9\]+,\\\$\[0-9\]+" } } */

/* Pipeline 0: Accumulator add (demonstrates 2-operand mult is used) */
/* { dg-final { scan-assembler "\tmadd\t" } } */

