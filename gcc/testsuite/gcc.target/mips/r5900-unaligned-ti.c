/* Test R5900 automatic unaligned 128-bit access using QFSRV.
   GCC automatically generates LQ+LQ+MTSAB+QFSRV for unaligned TI loads.

   Note: Only TI mode (scalar __int128) uses QFSRV for unaligned access.
   Vector modes (V16QI, V8HI, V4SI, V4SF) don't have movmisalign patterns
   for MMI because those patterns would interfere with autovectorization
   (which expects aligned accesses). Vector unaligned access falls back to
   ldl/ldr sequences.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "requires optimization" { *-*-* } { "-O0" } { "" } } */

/* 128-bit scalar integer type */
typedef __int128 ti;

/* Unaligned type - GCC knows accesses through this are misaligned.
   This triggers the movmisalignti pattern which uses QFSRV.  */
typedef ti unaligned_ti __attribute__((aligned(1)));

/* Automatic unaligned 128-bit load for TI (scalar integer).
   GCC generates LQ+LQ+MTSAB+QFSRV sequence.  */
ti load_unaligned_ti(unaligned_ti *ptr) {
    return *ptr;
}

/* { dg-final { scan-assembler "\tmtsab\t" } } */
/* { dg-final { scan-assembler "\tqfsrv\t" } } */
