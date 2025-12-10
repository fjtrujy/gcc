/* Test R5900 MMI parallel shift by scalar variable autovectorization.
   This tests the ashlv4si3, lshrv4si3, ashrv4si3 expanders that convert
   vector<<scalar to vector<<vector using broadcast.

   V4SI (word): Uses psllvw, psrlvw, psravw directly.
   V8HI (halfword): Emulated by extending to words, shifting with psllvw/
   psrlvw/psravw, then packing back with ppach.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "autovectorization requires -O3" { *-*-* } { "-O0" "-O1" "-O2" "-Os" "-flto" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* Variable scalar left shift - word.
   Should use psllvw (broadcasts scalar, then vector shift).  */
NOMIPS16 void
shl_var_si (int *a, int *b, int n, int shift)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] << shift;
}
/* { dg-final { scan-assembler "\tpsllvw\t" } } */

/* Variable scalar left shift - halfword.
   No psllvh instruction exists, so GCC emulates with:
   pextlh/pextuh (extend to words), psllvw (shift), ppach (pack back).  */
NOMIPS16 void
shl_var_hi (short *a, short *b, int n, int shift)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] << shift;
}
/* { dg-final { scan-assembler "\tppach\t" } } */

/* Variable scalar right logical shift - word.
   Should use psrlvw (broadcasts scalar, then vector shift).  */
NOMIPS16 void
shr_var_si (unsigned int *a, unsigned int *b, int n, int shift)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] >> shift;
}
/* { dg-final { scan-assembler "\tpsrlvw\t" } } */

/* Variable scalar right logical shift - halfword.
   Emulated like left shift: extend to words, psrlvw, ppach.  */
NOMIPS16 void
shr_var_hi (unsigned short *a, unsigned short *b, int n, int shift)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] >> shift;
}

/* Variable scalar right arithmetic shift - word.
   Should use psravw (broadcasts scalar, then vector shift).  */
NOMIPS16 void
ashr_var_si (int *a, int *b, int n, int shift)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] >> shift;
}
/* { dg-final { scan-assembler "\tpsravw\t" } } */

/* Variable scalar right arithmetic shift - halfword.
   Emulated like left shift: extend to words, psravw, ppach.  */
NOMIPS16 void
ashr_var_hi (short *a, short *b, int n, int shift)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] >> shift;
}
