/* Test that LL/SC (atomic) instructions are NOT generated for R5900.  */
/* R5900 does not support LL/SC atomic instructions.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler-not "\tll\t" } } */
/* { dg-final { scan-assembler-not "\tsc\t" } } */

NOMIPS16 int
test_atomic_load (int *p)
{
  return __atomic_load_n (p, __ATOMIC_SEQ_CST);
}

