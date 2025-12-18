/* Test R5900 MMI aligned vector loads and stores.
   When vectors are properly 16-byte aligned, R5900 can use the native
   lq/sq instructions for optimal performance.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -O2 -ftree-vectorize" } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* Properly aligned structure - all vector accesses will use sq/lq.  */
struct aligned_vec {
    int data[4] __attribute__ ((aligned (16)));
};

/* Copy between aligned vectors should use lq/sq.  */
NOMIPS16 void
copy_aligned_vec (struct aligned_vec *dst, struct aligned_vec *src)
{
    for (int i = 0; i < 4; i++)
        dst->data[i] = src->data[i];
}

/* Initialize aligned vector should use sq.  */
NOMIPS16 void
init_aligned_vec (struct aligned_vec *v)
{
    v->data[0] = 1;
    v->data[1] = 2;
    v->data[2] = 3;
    v->data[3] = 4;
}

/* Aligned loads should use lq.  */
/* { dg-final { scan-assembler "\\tlq\\t" } } */
/* Aligned stores should use sq.  */
/* { dg-final { scan-assembler "\\tsq\\t" } } */

