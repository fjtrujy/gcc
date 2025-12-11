/* Test that scalar shifts compile without ICE on R5900.
   This pattern from libmad layer3.c caused an ICE in expand_shift_1.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -O3" } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

struct channel {
    unsigned char scalefac[22];
};

static const unsigned char pretab[22] = {0};

/* This pattern caused the ICE in expand_shift_1.
   From libmad layer3.c III_exponents function.  */
NOMIPS16 int
test_shift_ice(struct channel *channel, int sfbi, int scalefac_multiplier)
{
    return (signed int) ((channel->scalefac[sfbi] + pretab[sfbi]) << scalefac_multiplier);
}

/* Simpler variant with just the shift.  */
NOMIPS16 int
test_shift_simple(unsigned char a, unsigned char b, int shift)
{
    return (signed int) ((a + b) << shift);
}

/* Multiple shift operations in a loop (for autovectorization).  */
NOMIPS16 void
test_shift_loop(unsigned char *src, int *dst, int n, int shift)
{
    for (int i = 0; i < n; i++)
        dst[i] = (signed int) (src[i] << shift);
}

/* Test case with inlined function - similar to III_exponents inlining.  */
static inline int
compute_exponent(unsigned char sf, unsigned char pt, int mult)
{
    return (signed int) ((sf + pt) << mult);
}

NOMIPS16 int
test_inline_shift(struct channel *channel, int n, int mult)
{
    int result = 0;
    for (int i = 0; i < n && i < 22; i++)
        result += compute_exponent(channel->scalefac[i], pretab[i], mult);
    return result;
}
