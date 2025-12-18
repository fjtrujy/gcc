/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-final { scan-assembler "\tmult\t\[^\n\]*,\[^\n\]*," } } */

NOMIPS16 int
f (int a, int b)
{
  return a * b;
}

