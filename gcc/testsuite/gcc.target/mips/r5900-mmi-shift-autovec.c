/* Test R5900 MMI parallel shift autovectorization */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "autovectorization requires -O3" { *-*-* } { "-O0" "-O1" "-O2" "-Os" "-flto" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* Autovectorizable left shift - halfword */
NOMIPS16 void
shl_hi (short *a, short *b, int n)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] << 4;
}
/* { dg-final { scan-assembler "\tpsllh\t" } } */

/* Autovectorizable left shift - word */
NOMIPS16 void
shl_si (int *a, int *b, int n)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] << 8;
}
/* { dg-final { scan-assembler "\tpsllw\t" } } */

/* Autovectorizable right shift (logical) - halfword */
NOMIPS16 void
shr_hi (unsigned short *a, unsigned short *b, int n)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] >> 4;
}
/* { dg-final { scan-assembler "\tpsrlh\t" } } */

/* Autovectorizable right shift (logical) - word */
NOMIPS16 void
shr_si (unsigned int *a, unsigned int *b, int n)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] >> 8;
}
/* { dg-final { scan-assembler "\tpsrlw\t" } } */

/* Autovectorizable right shift (arithmetic) - halfword */
NOMIPS16 void
ashr_hi (short *a, short *b, int n)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] >> 4;
}
/* { dg-final { scan-assembler "\tpsrah\t" } } */

/* Autovectorizable right shift (arithmetic) - word */
NOMIPS16 void
ashr_si (int *a, int *b, int n)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] >> 8;
}
/* { dg-final { scan-assembler "\tpsraw\t" } } */
