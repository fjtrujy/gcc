/* Test R5900 128-bit parameter passing in registers (n32 ABI) */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mabi=n32" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef int int128_t __attribute__((mode(TI)));

/* External function that takes 128-bit parameter */
extern void use_ti (int128_t val);

/* Test passing 128-bit as parameter - should use register, not stack */
NOMIPS16 void
test_pass_param (int128_t *p)
{
  use_ti (*p);
}

/* Test returning 128-bit value - should use register */
NOMIPS16 int128_t
test_return (int128_t *p)
{
  return *p;
}
/* { dg-final { scan-assembler "\tlq\t" } } */

/* Test returning 128-bit parameter - uses move to copy from $4 to $2 */
NOMIPS16 int128_t
test_return_param (int128_t val)
{
  return val;
}
/* { dg-final { scan-assembler "\tmove\t" } } */
