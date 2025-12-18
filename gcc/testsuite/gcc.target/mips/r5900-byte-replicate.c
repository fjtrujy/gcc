/* Test that byte replication pattern is correctly vectorized for R5900 MMI.
   The pattern (j << 24) | (j << 16) | (j << 8) | j should produce a word
   with the same byte value in all 4 positions. */

/* { dg-do run } */
/* { dg-options "-march=r5900 -O2" } */

#include <stdlib.h>

/* This pattern replicates a byte to all 4 byte positions of a 32-bit word */
unsigned int __attribute__((noinline))
byte_replicate(unsigned int j) {
    return (j << 24) | (j << 16) | (j << 8) | j;
}

/* Vectorized version - processes array */
void __attribute__((noinline))
fill_replicated(unsigned int *out, int count, int offset) {
    for (int i = 0; i < count; ++i) {
        unsigned int j = (i + offset) & 0xff;
        out[i] = (j << 24) | (j << 16) | (j << 8) | j;
    }
}

int main(void) {
    unsigned int results[256];
    
    /* Test scalar version */
    for (int i = 0; i < 256; i++) {
        unsigned int expected = (i << 24) | (i << 16) | (i << 8) | i;
        unsigned int got = byte_replicate(i);
        if (got != expected) {
            return 1;  /* FAIL */
        }
    }
    
    /* Test vectorized version with offset=0 */
    fill_replicated(results, 256, 0);
    for (int i = 0; i < 256; i++) {
        unsigned int expected = (i << 24) | (i << 16) | (i << 8) | i;
        if (results[i] != expected) {
            return 2;  /* FAIL - vectorized version wrong */
        }
    }
    
    /* Test vectorized version with offset=100 */
    fill_replicated(results, 256, 100);
    for (int i = 0; i < 256; i++) {
        unsigned int j = (i + 100) & 0xff;
        unsigned int expected = (j << 24) | (j << 16) | (j << 8) | j;
        if (results[i] != expected) {
            return 3;  /* FAIL - vectorized version with offset wrong */
        }
    }
    
    return 0;  /* PASS */
}
