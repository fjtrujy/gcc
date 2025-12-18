/* Test basic R5900 target support.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */

NOMIPS16 int
test (void)
{
  return 1;
}

