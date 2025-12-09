/* Test that vector modes don't crash the conditional move expander */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -O2" } */

typedef int v4si __attribute__((vector_size(16)));
typedef unsigned int uint32_t;

/* This was causing ICE before the fix - scalar conditional in vectorized loop */
void test_scalar_loop(uint32_t *buf, uint32_t a, uint32_t b) {
    for (int i = 0; i < 320; i++) {
        unsigned int sel = i & 1;
        buf[i] = sel ? a : b;
    }
}

/* Reproduce the original libretro-test.c pattern */
void test_checkered(uint32_t *buf, unsigned x_coord, unsigned y_coord) {
    uint32_t color_r = 0xff << 16;
    uint32_t color_g = 0xff << 8;

    for (unsigned y = 0; y < 240; y++) {
        unsigned index_y = ((y - y_coord) >> 4) & 1;
        for (unsigned x = 0; x < 320; x++) {
            unsigned index_x = ((x - x_coord) >> 4) & 1;
            buf[y * 320 + x] = (index_y ^ index_x) ? color_r : color_g;
        }
    }
}
