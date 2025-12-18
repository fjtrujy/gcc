/* Test for short-loop bug fix on R5900.  */
/* The R5900 has a hardware bug with branch-likely delay slots.  */
/* -mfix-r5900 is automatically enabled for -march=r5900.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

int array[100];

NOMIPS16 int
test_loop (int n)
{
  int sum = 0;
  int i;
  for (i = 0; i < n; i++)
    sum += array[i];
  return sum;
}

