/* Test R5900 MMI parallel comparison autovectorization.
   Autovectorization requires vcond pattern with unconditional loads.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "autovectorization requires -O3" { *-*-* } { "-O0" "-O1" "-O2" "-Os" "-flto" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* Test vcond with GT comparison and selection between array values.
   Requires unconditional loads for if-convert to work.  */
NOMIPS16 void
test_gt_vcond (int * __restrict a, int * __restrict b,
               int * __restrict c, int * __restrict d,
               int * __restrict e, int n)
{
  for (int i = 0; i < n; i++)
    {
      int av = a[i];
      int bv = b[i];
      int cv = c[i];
      int dv = d[i];
      e[i] = (av > bv) ? cv : dv;
    }
}
/* { dg-final { scan-assembler "\tpcgtw\t" } } */
/* { dg-final { scan-assembler "\tpand\t" } } */
/* { dg-final { scan-assembler "\tpor\t" } } */

/* Test vcond with EQ comparison.  */
NOMIPS16 void
test_eq_vcond (int * __restrict a, int * __restrict b,
               int * __restrict c, int * __restrict d,
               int * __restrict e, int n)
{
  for (int i = 0; i < n; i++)
    {
      int av = a[i];
      int bv = b[i];
      int cv = c[i];
      int dv = d[i];
      e[i] = (av == bv) ? cv : dv;
    }
}
/* { dg-final { scan-assembler "\tpceqw\t" } } */

/* Test vcond with halfword elements.  */
NOMIPS16 void
test_gt_vcond_h (short * __restrict a, short * __restrict b,
                 short * __restrict c, short * __restrict d,
                 short * __restrict e, int n)
{
  for (int i = 0; i < n; i++)
    {
      short av = a[i];
      short bv = b[i];
      short cv = c[i];
      short dv = d[i];
      e[i] = (av > bv) ? cv : dv;
    }
}
/* { dg-final { scan-assembler "\tpcgth\t" } } */
