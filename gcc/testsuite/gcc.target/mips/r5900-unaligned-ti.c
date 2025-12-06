/* Test R5900 automatic unaligned 128-bit access using QFSRV.
   GCC uses QFSRV for unaligned loads, regular moves for stores.
   This applies to all 128-bit types: TI, V16QI, V8HI, V4SI, V4SF.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "requires optimization" { *-*-* } { "-O0" } { "" } } */

/* 128-bit scalar integer type */
typedef __int128 ti;

/* MMI vector types (128-bit) */
typedef signed char v16qi __attribute__((vector_size(16)));
typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));

/* VU0 vector type (128-bit) */
typedef float v4sf __attribute__((vector_size(16)));

/* Unaligned types - GCC knows accesses through these are misaligned.
   This triggers the movmisalign patterns which use QFSRV for loads.  */
typedef ti unaligned_ti __attribute__((aligned(1)));
typedef v16qi unaligned_v16qi __attribute__((aligned(1)));
typedef v8hi unaligned_v8hi __attribute__((aligned(1)));
typedef v4si unaligned_v4si __attribute__((aligned(1)));
typedef v4sf unaligned_v4sf __attribute__((aligned(1)));

/* Automatic unaligned 128-bit load for TI (scalar integer).
   GCC generates LQ+LQ+MTSAB+QFSRV sequence.  */
ti load_unaligned_ti(unaligned_ti *ptr) {
    return *ptr;
}

/* Automatic unaligned 128-bit load for v16qi (16 bytes).  */
v16qi load_unaligned_v16qi(unaligned_v16qi *ptr) {
    return *ptr;
}

/* Automatic unaligned 128-bit load for v8hi (8 halfwords).  */
v8hi load_unaligned_v8hi(unaligned_v8hi *ptr) {
    return *ptr;
}

/* Automatic unaligned 128-bit load for v4si (4 words).  */
v4si load_unaligned_v4si(unaligned_v4si *ptr) {
    return *ptr;
}

/* Automatic unaligned 128-bit load for v4sf (4 floats - VU0).  */
v4sf load_unaligned_v4sf(unaligned_v4sf *ptr) {
    return *ptr;
}

/* ========== Loop-based unaligned access tests ========== */

/* Unaligned load loop - reads from unaligned source, writes to aligned dest.
   Each iteration should use QFSRV for the unaligned load.  */
void load_loop_ti(ti *__restrict dst, unaligned_ti *__restrict src, int n) {
    for (int i = 0; i < n; i++)
        dst[i] = src[i];
}

/* Unaligned store loop - reads from aligned source, writes to unaligned dest.
   Stores use regular moves (may trap on truly misaligned access).  */
void store_loop_ti(unaligned_ti *__restrict dst, ti *__restrict src, int n) {
    for (int i = 0; i < n; i++)
        dst[i] = src[i];
}

/* Unaligned copy loop - both source and dest are unaligned.
   Loads use QFSRV, stores use regular moves.  */
void copy_loop_ti(unaligned_ti *__restrict dst, unaligned_ti *__restrict src, int n) {
    for (int i = 0; i < n; i++)
        dst[i] = src[i];
}

/* Unaligned load from byte offset - common pattern for parsing data.
   Loads from arbitrary byte positions in a buffer.  */
void load_from_bytes(ti *dst, char *src, int n) {
    for (int i = 0; i < n; i++)
        dst[i] = *(unaligned_ti *)(src + i * 16);
}

/* Unaligned store to byte offset - writes to arbitrary byte positions.  */
void store_to_bytes(char *dst, ti *src, int n) {
    for (int i = 0; i < n; i++)
        *(unaligned_ti *)(dst + i * 16) = src[i];
}

/* Unaligned load with stride - non-contiguous access pattern.  */
void load_strided_ti(ti *dst, unaligned_ti *src, int n, int stride) {
    for (int i = 0; i < n; i++)
        dst[i] = src[i * stride];
}

/* Accumulate from unaligned source - reduction pattern.  */
ti accumulate_unaligned(unaligned_ti *src, int n) {
    ti acc = 0;
    for (int i = 0; i < n; i++)
        acc += src[i];
    return acc;
}

/* { dg-final { scan-assembler "\tmtsab\t" } } */
/* { dg-final { scan-assembler "\tqfsrv\t" } } */
