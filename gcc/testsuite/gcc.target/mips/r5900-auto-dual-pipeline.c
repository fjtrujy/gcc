/* Test R5900 automatic dual pipeline usage - pure C code */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mhard-float" } */
/* Skip -O0, -O1, -Os as they don't enable the scheduling optimizations needed */
/* { dg-skip-if "requires scheduling" { *-*-* } { "-O0" "-O1" "-Os" } { "" } } */

/* 
 * This test demonstrates GCC AUTOMATICALLY using both pipelines
 * for multiply operations without any builtins or inline assembly.
 *
 * When there are multiple independent multiply operations, GCC will:
 * - Use Pipeline 0 (mult + madd) for accumulator chains
 * - Use Pipeline 1 (mult1) for independent multiplies when beneficial
 *
 * This automatic selection improves performance by utilizing both
 * of the R5900's multiply units in parallel.
 */

/* Simple accumulator pattern - uses Pipeline 0 (mult + madd) */
int single_accum (int a, int b, int c, int d)
{
  return a * b + c * d;
}

/* Two independent results - GCC automatically uses both pipelines */
int dual_result (int a, int b, int c, int d,
                 int e, int f, int g, int h)
{
  int sum1 = a * b + c * d;  /* Pipeline 0: mult + madd */
  int sum2 = e * f + g * h;  /* Pipeline 1: mult1 for independent muls */
  return sum1 + sum2;
}

/* Four independent accumulator patterns - both pipelines utilized */
int four_accums (int a, int b, int c, int d,
                 int e, int f, int g, int h,
                 int i, int j, int k, int l,
                 int m, int n, int o, int p)
{
  int sum1 = a * b + c * d;
  int sum2 = e * f + g * h;
  int sum3 = i * j + k * l;
  int sum4 = m * n + o * p;
  return sum1 + sum2 + sum3 + sum4;
}

/* 
 * Verify automatic dual pipeline usage:
 * - mult + madd: Pipeline 0 accumulator chain
 * - mult1: Pipeline 1 for additional multiplies (automatic!)
 */

/* Pipeline 0: Accumulator pattern */
/* { dg-final { scan-assembler "\tmult\t\\\$\[0-9\]+,\\\$\[0-9\]+" } } */
/* { dg-final { scan-assembler "\tmadd\t" } } */

/* Pipeline 1: Automatic selection for independent multiplies */
/* { dg-final { scan-assembler "\tmult1\t" } } */

