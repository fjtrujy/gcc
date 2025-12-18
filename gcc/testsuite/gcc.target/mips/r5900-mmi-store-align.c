/* Test R5900 MMI vector store alignment safety.
   R5900's sq (store quadword) instruction requires 16-byte alignment.
   It silently masks off the lower 4 address bits, causing data corruption
   when storing to misaligned addresses.

   The compiler uses:
   - sq (fast 128-bit store) for stack-relative addresses (known 16-byte aligned)
   - sd (safe 64-bit stores) for heap/global data (alignment unknown)  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -ftree-vectorize -O3" } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* Test vectorized loop stores use sd (safe) instead of sq (unsafe).
   The loop writes to an array parameter whose alignment is unknown
   at compile time - must use sd for safety.  */
NOMIPS16 void
test_vec_store_int (int *__restrict a, int *__restrict b, int *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] + b[i];
}
/* Should vectorize using paddw.  */
/* { dg-final { scan-assembler "\\tpaddw\\t" } } */
/* Should use sd (store doubleword) for safe 64-bit stores to heap data.  */
/* { dg-final { scan-assembler "\\tsd\\t" } } */

/* Test structure member store - alignment unknown, must use sd.  */
struct data {
  int values[64];
};

NOMIPS16 void
test_struct_store (struct data *d, int *src, int n)
{
  for (int i = 0; i < n && i < 64; i++)
    d->values[i] = src[i];
}

/* Test short vector store - uses paddh and safe stores.  */
NOMIPS16 void
test_vec_store_short (short *__restrict a, short *__restrict b, short *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    c[i] = a[i] + b[i];
}
/* { dg-final { scan-assembler "\\tpaddh\\t" } } */

/* Test stack-local array - can use sq since stack is 16-byte aligned.  */
NOMIPS16 void
test_stack_store (int *src, int n)
{
  int local[16] __attribute__((aligned(16)));
  for (int i = 0; i < 16 && i < n; i++)
    local[i] = src[i];
  /* Use local to prevent optimization */
  asm volatile ("" : : "r"(local[0]));
}
/* Stack stores can use sq for performance.  */
/* { dg-final { scan-assembler "\\tsq\\t.*\\(\\$sp\\)" } } */

