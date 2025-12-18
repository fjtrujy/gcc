/* Test R5900 MMI (Multimedia Instructions) intrinsics.
   These intrinsics provide direct access to the 128-bit integer SIMD
   operations available on the R5900 Emotion Engine.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -O2" } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef int v4si __attribute__ ((vector_size (16)));
typedef short v8hi __attribute__ ((vector_size (16)));

/* Test parallel add (paddw - 4x32-bit).  */
NOMIPS16 v4si
test_paddw (v4si a, v4si b)
{
    return a + b;
}

/* Test parallel subtract (psubw - 4x32-bit).  */
NOMIPS16 v4si  
test_psubw (v4si a, v4si b)
{
    return a - b;
}

/* Test parallel and (pand - 128-bit).  */
NOMIPS16 v4si
test_pand (v4si a, v4si b)
{
    return a & b;
}

/* Test parallel or (por - 128-bit).  */
NOMIPS16 v4si
test_por (v4si a, v4si b)
{
    return a | b;
}

/* Test parallel xor (pxor - 128-bit).  */
NOMIPS16 v4si
test_pxor (v4si a, v4si b)
{
    return a ^ b;
}

/* Verify MMI instructions are generated.  */
/* { dg-final { scan-assembler "\\tpaddw\\t" } } */
/* { dg-final { scan-assembler "\\tpsubw\\t" } } */
/* { dg-final { scan-assembler "\\tpand\\t" } } */
/* { dg-final { scan-assembler "\\tpor\\t" } } */
/* { dg-final { scan-assembler "\\tpxor\\t" } } */

