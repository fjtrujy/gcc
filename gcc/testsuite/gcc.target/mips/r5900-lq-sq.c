/* Test R5900 128-bit load/store with lq/sq instructions */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef int int128_t __attribute__((mode(TI)));

/* Test 128-bit load */
NOMIPS16 int128_t
test_load (int128_t *p)
{
  return *p;
}
/* { dg-final { scan-assembler "\tlq\t" } } */

/* Test 128-bit store */
NOMIPS16 void
test_store (int128_t *p, int128_t val)
{
  *p = val;
}
/* { dg-final { scan-assembler "\tsq\t" } } */

/* Test 128-bit memory copy - should use lq to load, sq to store */
NOMIPS16 void
test_copy (int128_t *dst, int128_t *src)
{
  *dst = *src;
}
