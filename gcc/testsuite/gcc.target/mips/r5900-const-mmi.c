/* Test R5900 128-bit constant loading for TI and MMI vector modes */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef int int128_t __attribute__((mode(TI)));
typedef int v4si __attribute__((vector_size(16)));
typedef short v8hi __attribute__((vector_size(16)));
typedef signed char v16qi __attribute__((vector_size(16)));
typedef long long v2di __attribute__((vector_size(16)));

/* =========================================================================
   TImode (128-bit scalar integer) constant loading
   ========================================================================= */

/* Zero store: should use sq $0,mem directly */
NOMIPS16 void
test_ti_store_zero (int128_t *p)
{
  *p = 0;
}
/* { dg-final { scan-assembler "\tsq\t\\\$0," } } */

/* Zero return: should use por $2,$0,$0 to create zero in return register */
NOMIPS16 int128_t
test_ti_return_zero (void)
{
  return 0;
}
/* { dg-final { scan-assembler "\tpor\t\\\$2,\\\$0,\\\$0" } } */

/* Non-zero store: should use lq from constant pool then sq */
NOMIPS16 void
test_ti_store_nonzero (int128_t *p)
{
  *p = 0x12345678;
}

/* Non-zero return: should use lq from constant pool */
NOMIPS16 int128_t
test_ti_return_nonzero (void)
{
  return 0x12345678;
}

/* =========================================================================
   V4SI (4x32-bit integer vector) constant loading
   ========================================================================= */

/* Zero store: should use sq $0,mem */
NOMIPS16 void
test_v4si_store_zero (v4si *p)
{
  *p = (v4si){0, 0, 0, 0};
}

/* Zero return: should use por $2,$0,$0 */
NOMIPS16 v4si
test_v4si_return_zero (void)
{
  return (v4si){0, 0, 0, 0};
}

/* Non-zero return: should use lq from constant pool */
NOMIPS16 v4si
test_v4si_return_nonzero (void)
{
  return (v4si){1, 2, 3, 4};
}

/* Splat pattern (all elements same): should use lq from constant pool */
NOMIPS16 v4si
test_v4si_splat (void)
{
  return (v4si){42, 42, 42, 42};
}

/* =========================================================================
   V8HI (8x16-bit integer vector) constant loading
   ========================================================================= */

/* Zero return: should use por $2,$0,$0 */
NOMIPS16 v8hi
test_v8hi_return_zero (void)
{
  return (v8hi){0, 0, 0, 0, 0, 0, 0, 0};
}

/* Non-zero return: should use lq from constant pool */
NOMIPS16 v8hi
test_v8hi_return_nonzero (void)
{
  return (v8hi){1, 2, 3, 4, 5, 6, 7, 8};
}

/* Splat pattern: should use lq from constant pool */
NOMIPS16 v8hi
test_v8hi_splat (void)
{
  return (v8hi){100, 100, 100, 100, 100, 100, 100, 100};
}

/* =========================================================================
   V16QI (16x8-bit integer vector) constant loading
   ========================================================================= */

/* Zero return: should use por $2,$0,$0 */
NOMIPS16 v16qi
test_v16qi_return_zero (void)
{
  return (v16qi){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
}

/* Non-zero return: should use lq from constant pool */
NOMIPS16 v16qi
test_v16qi_return_nonzero (void)
{
  return (v16qi){1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
}

/* Splat pattern: should use lq from constant pool */
NOMIPS16 v16qi
test_v16qi_splat (void)
{
  return (v16qi){0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
                 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};
}

/* =========================================================================
   V2DI (2x64-bit integer vector) constant loading
   ========================================================================= */

/* Zero return: should use por $2,$0,$0 */
NOMIPS16 v2di
test_v2di_return_zero (void)
{
  return (v2di){0, 0};
}

/* Non-zero return: should use lq from constant pool */
NOMIPS16 v2di
test_v2di_return_nonzero (void)
{
  return (v2di){0x123456789ABCDEF0LL, 0xFEDCBA9876543210LL};
}

/* Splat pattern: should use lq from constant pool */
NOMIPS16 v2di
test_v2di_splat (void)
{
  return (v2di){0xDEADBEEFCAFEBABELL, 0xDEADBEEFCAFEBABELL};
}

/* =========================================================================
   Verification: Zero constants should use por, non-zero should use lq
   ========================================================================= */

/* Count por $2,$0,$0 patterns for zero returns:
   - test_ti_return_zero
   - test_v4si_return_zero
   - test_v8hi_return_zero
   - test_v16qi_return_zero
   - test_v2di_return_zero
   Total: 5 occurrences */
/* { dg-final { scan-assembler-times "\tpor\t\\\$2,\\\$0,\\\$0" 5 } } */

/* Count sq $0 patterns for zero stores:
   - test_ti_store_zero
   - test_v4si_store_zero
   Total: 2 occurrences */
/* { dg-final { scan-assembler-times "\tsq\t\\\$0," 2 } } */

/* Verify lq is used for non-zero constants (at least some) */
/* { dg-final { scan-assembler "\tlq\t" } } */
