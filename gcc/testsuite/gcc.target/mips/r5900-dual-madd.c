/* Test R5900 dual pipeline usage with both mult+madd and mult1+madd1 */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mhard-float" } */

/* 
 * This test demonstrates using BOTH multiply-accumulate pipelines
 * simultaneously on the R5900:
 * - Pipeline 0: mult + madd using HI:LO (via inline asm)
 * - Pipeline 1: mult1 + madd1 using HI1:LO1 (via builtins)
 *
 * This allows two independent accumulator chains to run in parallel,
 * effectively doubling multiply-accumulate throughput.
 */

/* Explicit dual-pipeline accumulator chains */
long long dual_pipeline_madd (int a, int b, int c, int d,
                              int e, int f, int g, int h)
{
  long long acc0, acc1;
  
  /* Pipeline 0: HI:LO accumulator chain (inline asm) */
  __asm__ __volatile__ (
    "mult\t%1, %2\n\t"      /* mult a,b -> HI:LO */
    "madd\t%3, %4\n\t"      /* madd c,d -> HI:LO += c*d */
    "mflo\t%L0\n\t"         /* get low part */
    "mfhi\t%M0"             /* get high part */
    : "=d" (acc0)
    : "d" (a), "d" (b), "d" (c), "d" (d)
    : "hi", "lo"
  );
  
  /* Pipeline 1: HI1:LO1 accumulator chain (builtins) */
  acc1 = __builtin_mips_mult1 (e, f);     /* mult1 e,f -> HI1:LO1 */
  acc1 = __builtin_mips_madd1 (acc1, g, h); /* madd1 g,h -> HI1:LO1 += g*h */
  
  /* Combine results from both pipelines */
  return acc0 + acc1;
}

/* 3-operand form of dual-pipeline multiply-add */
int dual_pipeline_madd_3op (int a, int b, int c, int d,
                            int e, int f, int g, int h)
{
  /* Pipeline 0: Use standard 3-operand multiply (automatic) */
  int r0 = a * b;
  int r1 = c * d;
  
  /* Pipeline 1: Use 3-operand builtins */
  int r2 = __builtin_mips_mult1_3op (e, f);
  int r3 = __builtin_mips_mult1_3op (g, h);
  
  return r0 + r1 + r2 + r3;
}

/* Interleaved dual-pipeline usage - shows both running "together" */
int interleaved_pipelines (int a, int b, int c, int d,
                           int e, int f, int g, int h)
{
  int lo0, lo1;
  
  /* Interleaved Pipeline 0 and Pipeline 1 operations */
  __asm__ __volatile__ (
    "mult\t%2, %3\n\t"      /* Pipeline 0: start multiply */
    "mult1\t%4, %5\n\t"     /* Pipeline 1: start multiply (parallel!) */
    "madd\t%6, %7\n\t"      /* Pipeline 0: accumulate */
    "madd1\t%8, %9\n\t"     /* Pipeline 1: accumulate (parallel!) */
    "mflo\t%0\n\t"          /* Pipeline 0: get result */
    "mflo1\t%1"             /* Pipeline 1: get result */
    : "=d" (lo0), "=d" (lo1)
    : "d" (a), "d" (b), "d" (c), "d" (d),
      "d" (e), "d" (f), "d" (g), "d" (h)
    : "hi", "lo", "$hi1", "$lo1"
  );
  
  return lo0 + lo1;
}

/* Verify both pipelines generate their respective instructions */

/* Pipeline 0: Standard multiply and multiply-add */
/* { dg-final { scan-assembler "\tmult\t" } } */
/* { dg-final { scan-assembler "\tmadd\t" } } */
/* { dg-final { scan-assembler "\tmflo\t" } } */

/* Pipeline 1: R5900-specific multiply and multiply-add */
/* { dg-final { scan-assembler "\tmult1\t" } } */
/* { dg-final { scan-assembler "\tmadd1\t" } } */
/* { dg-final { scan-assembler "\tmflo1\t" } } */

