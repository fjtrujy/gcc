/* Test R5900 automatic MULTU selection */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mhard-float" } */
/* Skip -O0 and -O1 as they don't use hardware multiply for widening */
/* { dg-skip-if "requires higher optimization" { *-*-* } { "-O0" "-O1" } { "" } } */

/*
 * GCC automatically selects MULTU (unsigned multiply) vs MULT (signed)
 * based on the operation type:
 *
 * 1. 64-bit unsigned widening: (unsigned long long)a * b
 *    -> Uses MULTU (2-operand) because HI:LO needs full 64-bit unsigned result
 *
 * 2. 32-bit result (signed or unsigned): a * b -> int/unsigned int
 *    -> Uses MULT or MULT1 (3-operand) because low 32 bits are identical
 *       for signed and unsigned multiplication
 *
 * For explicit 3-operand MULTU (when you need zero-extended HI bits AND
 * the low 32 bits in a GPR), use __builtin_mips_multu_3op().
 */

/* 64-bit unsigned widening multiply - automatically uses MULTU */
unsigned long long unsigned_widening (unsigned int a, unsigned int b)
{
  return (unsigned long long)a * b;
}

/* 64-bit signed widening multiply - automatically uses MULT */
long long signed_widening (int a, int b)
{
  return (long long)a * b;
}

/* 32-bit unsigned multiply - uses MULT/MULT1 (low 32 bits same as MULTU) */
unsigned int unsigned_32bit (unsigned int a, unsigned int b)
{
  return a * b;
}

/* 32-bit signed multiply - uses MULT/MULT1 */
int signed_32bit (int a, int b)
{
  return a * b;
}

/* Verify automatic MULTU for 64-bit unsigned widening */
/* { dg-final { scan-assembler "\tmultu\t\\\$\[0-9\]+,\\\$\[0-9\]+" } } */

/* Verify automatic MULT for 64-bit signed widening */
/* { dg-final { scan-assembler "\tmult\t\\\$\[0-9\]+,\\\$\[0-9\]+" } } */

/* Verify 3-operand form for 32-bit results (mult or mult1) */
/* { dg-final { scan-assembler "\tmult1?\t\\\$\[0-9\]+,\\\$\[0-9\]+,\\\$\[0-9\]+" } } */

