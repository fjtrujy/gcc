/* Test R5900 3-operand multiply instruction (mult or mult1) */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* GCC may choose mult (Pipeline 0) or mult1 (Pipeline 1) automatically */
/* { dg-final { scan-assembler "\tmultu?1?\t\[^\n\]*,\[^\n\]*," } } */

NOMIPS16 int
f (int a, int b)
{
  return a * b;
}

