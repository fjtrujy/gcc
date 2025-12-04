/* Test R5900 TImode (128-bit) logical operations */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef __int128 int128_t;

/* Test __int128 AND */
NOMIPS16 int128_t
test_and128 (int128_t a, int128_t b)
{
  return a & b;
}
/* { dg-final { scan-assembler "\\tpand\\t" } } */

/* Test __int128 OR */
NOMIPS16 int128_t
test_or128 (int128_t a, int128_t b)
{
  return a | b;
}
/* { dg-final { scan-assembler "\\tpor\\t" } } */

/* Test __int128 XOR */
NOMIPS16 int128_t
test_xor128 (int128_t a, int128_t b)
{
  return a ^ b;
}
/* { dg-final { scan-assembler "\\tpxor\\t" } } */

/* Test __int128 NOT */
NOMIPS16 int128_t
test_not128 (int128_t a)
{
  return ~a;
}
/* { dg-final { scan-assembler "\\tpnor\\t" } } */
