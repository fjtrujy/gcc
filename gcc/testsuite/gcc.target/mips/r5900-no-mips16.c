/* Test that MIPS16 is rejected for R5900.  */
/* The R5900 does not support MIPS16 instruction set.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mips16" } */
/* { dg-error "unsupported combination" "" { target *-*-* } 0 } */

int
test (void)
{
  return 1;
}

