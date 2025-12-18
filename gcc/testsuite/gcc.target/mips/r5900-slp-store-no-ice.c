/* Test that SLP vectorized struct stores compile without ICE on R5900.
   The SLP vectorizer can merge consecutive scalar stores to struct
   members into vector stores. On R5900, misaligned 128-bit vector stores
   are decomposed into two 64-bit stores (sd) since the R5900's sq
   instruction silently ignores the lower 4 bits of the address.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -O2 -ftree-slp-vectorize" } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef unsigned int u32;
typedef unsigned long long u64;

/* Structure with aligned(64) field followed by regular int fields,
   which creates misaligned offsets for the subsequent fields when
   accessed as vectors.  */
struct test_struct {
    u32 a, b, c, d;        /* offset 0-15: 16-byte aligned */
    void *aligned_ptr __attribute__ ((aligned (64)));  /* offset 64 */
    int ff;                /* offset 68: NOT 16-byte aligned */
    int gg;                /* offset 72 */
    int hh;                /* offset 76 */
    int ii;                /* offset 80 */
};

void *calloc(u32, u32);

/* The SLP vectorizer will try to merge the consecutive stores to
   ff, gg, hh, ii into a V4SI store. Since offset 68 is not 16-byte
   aligned, the movmisalign pattern will decompose this into two
   sd (store doubleword) instructions.  */
NOMIPS16 struct test_struct *
test_init (void)
{
    struct test_struct *p = calloc(1, sizeof(struct test_struct));
    
    /* These will be vectorized to an aligned sq instruction.  */
    p->a = 2;
    p->b = 1;
    p->c = 0;
    p->d = 32768;
    
    p->aligned_ptr = (void *)0x10000000;
    
    /* These consecutive stores are merged by SLP into a V4SI store,
       but since offset 68 is misaligned, the store is decomposed
       into two sd instructions instead of an incorrect sq.  */
    p->ff = 0;
    p->gg = 0;
    p->hh = 0;
    p->ii = 0;
    
    return p;
}

/* First vector store (a,b,c,d) should use sq (aligned at offset 0).  */
/* { dg-final { scan-assembler "\\tsq\\t" } } */
/* Misaligned store (ff,gg,hh,ii) uses sdl/sdr or sd.  */
/* { dg-final { scan-assembler "\\tsdl\\t|\\tsdr\\t|\\tsd\\t" } } */
