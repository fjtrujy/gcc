/* Test R5900 Pipeline 1 move from HI1/LO1 instructions via builtin.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */

int
test_mfhi1 (long long acc)
{
  return __builtin_mips_mfhi1 (acc);
}

int
test_mflo1 (long long acc)
{
  return __builtin_mips_mflo1 (acc);
}

/* { dg-final { scan-assembler "\tmfhi1\t" } } */
/* { dg-final { scan-assembler "\tmflo1\t" } } */

