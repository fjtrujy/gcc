/* Test R5900 MMI misaligned 128-bit vector stores.
   Misaligned stores are decomposed into two 64-bit sd instructions
   since R5900's sq instruction silently ignores the lower 4 address bits.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -O2 -ftree-slp-vectorize" } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef int v4si __attribute__ ((vector_size (16)));

/* Structure with vector at misaligned offset.  */
struct misaligned_vec {
    int header;                         /* offset 0 */
    v4si vec __attribute__((packed));   /* offset 4: NOT 16-byte aligned */
};

/* Store to known misaligned struct member - should decompose to sd.  */
NOMIPS16 void
store_misaligned_member (struct misaligned_vec *s, v4si val)
{
    s->vec = val;
}

/* Store zero vector to misaligned address - tests CONST_VECTOR handling.  */
NOMIPS16 void
store_zero_misaligned (struct misaligned_vec *s)
{
    s->vec = (v4si){0, 0, 0, 0};
}

/* Store non-zero constant vector to misaligned address.  */
NOMIPS16 void
store_const_misaligned (struct misaligned_vec *s)
{
    s->vec = (v4si){1, 2, 3, 4};
}

/* Misaligned stores use sdl/sdr (unaligned doubleword store) or sd.  */
/* { dg-final { scan-assembler "\\tsdl\\t|\\tsdr\\t|\\tsd\\t" } } */

