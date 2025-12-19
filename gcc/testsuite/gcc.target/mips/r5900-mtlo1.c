/* Test R5900 Pipeline 1 move to LO1 instruction via builtin.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */

long long
test_mtlo1 (int val)
{
  return __builtin_mips_mtlo1 (val);
}

/* { dg-final { scan-assembler "\tmtlo1\t" } } */

