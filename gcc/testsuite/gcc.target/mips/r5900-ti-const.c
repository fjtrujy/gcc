/* Test R5900 128-bit constant handling */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef int int128_t __attribute__((mode(TI)));

/* Test storing zero constant - should use sq $0 */
NOMIPS16 void
test_store_zero (int128_t *p)
{
  *p = 0;
}
/* { dg-final { scan-assembler "\tsq\t\\\$0," } } */

/* Test storing non-zero constant - should use lq from constant pool then sq */
NOMIPS16 void
test_store_nonzero (int128_t *p)
{
  *p = 0x12345678;
}
/* { dg-final { scan-assembler "\tlq\t.*\n.*\tsq\t" } } */

/* Test returning zero constant - should use por $0,$0 */
NOMIPS16 int128_t
test_return_zero (void)
{
  return 0;
}
/* { dg-final { scan-assembler "\tpor\t\\\$2,\\\$0,\\\$0" } } */

/* Test returning non-zero constant - should use lq from constant pool */
NOMIPS16 int128_t
test_return_nonzero (void)
{
  return 0x12345678;
}
/* { dg-final { scan-assembler-times "\tlq\t\\\$2," 2 } } */
