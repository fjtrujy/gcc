/* Test R5900 MMI V8HI shift autovectorization */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "autovectorization requires -O3" { *-*-* } { "-O0" "-O1" "-O2" "-Os" "-flto" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* Autovectorizable left shift - halfword with constant */
NOMIPS16 void
shl_hi_const (short *a, short *b, int n)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] << 4;
}
/* { dg-final { scan-assembler "\tpsllh\t" } } */

/* Autovectorizable right shift (logical) - halfword with constant */
NOMIPS16 void
shr_hi_const (unsigned short *a, unsigned short *b, int n)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] >> 4;
}
/* { dg-final { scan-assembler "\tpsrlh\t" } } */

/* Autovectorizable right shift (arithmetic) - halfword with constant */
NOMIPS16 void
ashr_hi_const (short *a, short *b, int n)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] >> 4;
}
/* { dg-final { scan-assembler "\tpsrah\t" } } */

/* Variable shift - should use scalar code, not ICE.
   R5900 has no psllvh instruction.  */
NOMIPS16 void
shl_hi_var (short *a, short *b, int n, int shift)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] << shift;
}
/* { dg-final { scan-assembler-not "\tpsllvh\t" } } */
