/* Test R5900 MMI aligned vs misaligned vector store instruction selection.
   Aligned stores should use sq, misaligned stores should use sd.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -O2" } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef int v4si __attribute__ ((vector_size (16)));

/* Aligned vector - will use sq.  */
struct aligned_struct {
    v4si vec __attribute__ ((aligned (16)));
};

/* Misaligned vector - will use sd (decomposed).  */  
struct misaligned_struct {
    int header;
    v4si vec __attribute__((packed));
};

/* Mixed struct - has both aligned and misaligned access patterns.  */
struct mixed_struct {
    v4si aligned_vec __attribute__ ((aligned (16)));  /* offset 0: aligned */
    int spacer;                                        /* offset 16 */
    v4si misaligned_vec __attribute__((packed));      /* offset 20: misaligned */
};

/* Store to aligned vector member - should use sq.  */
NOMIPS16 void
store_aligned (struct aligned_struct *s, v4si val)
{
    s->vec = val;
}

/* Store to misaligned vector member - should decompose to sd.  */
NOMIPS16 void
store_misaligned (struct misaligned_struct *s, v4si val)
{
    s->vec = val;
}

/* Store to both aligned and misaligned in same function.
   Tests that the compiler correctly distinguishes between them.  */
NOMIPS16 void
store_mixed (struct mixed_struct *s, v4si val1, v4si val2)
{
    s->aligned_vec = val1;     /* Should use sq */
    s->misaligned_vec = val2;  /* Should use sd */
}

/* Aligned stores use sq (store quadword).  */
/* { dg-final { scan-assembler "\\tsq\\t" } } */
/* Misaligned stores use sdl/sdr (unaligned doubleword) or sd.  */
/* { dg-final { scan-assembler "\\tsdl\\t|\\tsdr\\t|\\tsd\\t" } } */

