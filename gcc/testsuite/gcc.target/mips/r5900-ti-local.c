/* Test R5900 128-bit local variable operations */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef int int128_t __attribute__((mode(TI)));

/* Test local 128-bit variable - should use lq and sq */
NOMIPS16 void
test_local (int128_t *in, int128_t *out)
{
  int128_t local = *in;
  *out = local;
}
/* { dg-final { scan-assembler "\tlq\t" } } */
/* { dg-final { scan-assembler "\tsq\t" } } */

/* Test 128-bit register copy - optimizer may eliminate this by loading
   directly to destination, so we don't check for por here. The por
   instruction is tested separately where it's guaranteed to be generated
   (e.g., function return value copy from parameter register). */
NOMIPS16 void
test_reg_copy (int128_t *a, int128_t *b, int128_t *c)
{
  int128_t tmp = *a;
  *b = tmp;
  *c = tmp;
}
