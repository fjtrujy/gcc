/* Test R5900 MMI misaligned stores for different vector element sizes.
   The movmisalign pattern handles V4SI, V8HI, and V16QI modes.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -O2" } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef int v4si __attribute__ ((vector_size (16)));
typedef short v8hi __attribute__ ((vector_size (16)));
typedef char v16qi __attribute__ ((vector_size (16)));

/* Structures with vectors at misaligned offsets for each element size.  */
struct misaligned_v4si {
    int header;
    v4si vec __attribute__((packed));
};

struct misaligned_v8hi {
    short header;
    v8hi vec __attribute__((packed));
};

struct misaligned_v16qi {
    char header;
    v16qi vec __attribute__((packed));
};

/* Test V4SI (4x32-bit) misaligned store.  */
NOMIPS16 void
store_v4si_misaligned (struct misaligned_v4si *s, v4si val)
{
    s->vec = val;
}

/* Test V8HI (8x16-bit) misaligned store.  */
NOMIPS16 void
store_v8hi_misaligned (struct misaligned_v8hi *s, v8hi val)
{
    s->vec = val;
}

/* Test V16QI (16x8-bit) misaligned store.  */
NOMIPS16 void
store_v16qi_misaligned (struct misaligned_v16qi *s, v16qi val)
{
    s->vec = val;
}

/* Misaligned stores use sdl/sdr (unaligned doubleword) or sd.  */
/* { dg-final { scan-assembler "\\tsdl\\t|\\tsdr\\t|\\tsd\\t" } } */

