/* Test R5900 dual pipeline usage - explicit parallel multiply operations */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mhard-float" } */
/* { dg-skip-if "code quality" { *-*-* } { "-O0" } { "" } } */

/* 
 * This test demonstrates EXPLICIT dual-pipeline usage.
 * 
 * Currently, GCC does NOT automatically use Pipeline 1.
 * Users must explicitly call Pipeline 1 builtins to utilize both MACs.
 *
 * For automatic dual-pipeline scheduling, GCC would need:
 * 1. Alternative instruction patterns (mult can use either pipeline)
 * 2. Register allocation support for HI1/LO1 as alternatives to HI/LO
 * 3. Cost modeling to decide when pipeline switching is beneficial
 *
 * The scheduling model (r5900_mac0 vs r5900_mac1) allows the instruction
 * scheduler to order mult and mult1 operations efficiently when both
 * pipelines are used explicitly.
 */

/* Example: Parallel multiply using both pipelines explicitly */
int parallel_multiply (int a, int b, int c, int d)
{
  /* Pipeline 0: 3-operand mult a,b -> result in GPR (R5900 specific) */
  int r0 = a * b;
  
  /* Pipeline 1: mult1 c,d -> HI1:LO1, result in GPR (explicit builtin) */
  int r1 = __builtin_mips_mult1_3op (c, d);
  
  return r0 + r1;
}

/* Verify both mult (Pipeline 0) and mult1 (Pipeline 1) are generated */
/* At -O1 and above, GCC uses 3-operand mult for Pipeline 0 */
/* { dg-final { scan-assembler "\tmult\t\\\$\[0-9\]+,\\\$\[0-9\]+,\\\$\[0-9\]+" } } */
/* { dg-final { scan-assembler "\tmult1\t\\\$\[0-9\]+,\\\$\[0-9\]+,\\\$\[0-9\]+" } } */

