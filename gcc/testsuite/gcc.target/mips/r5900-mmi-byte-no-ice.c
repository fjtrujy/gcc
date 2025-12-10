/* Test that byte (V16QI) operations compile without ICE.
   R5900 MMI doesn't have byte-level SIMD for shifts or abs,
   so these should fall back to scalar code without crashing.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "ICE only occurs at -O3" { *-*-* } { "-O0" "-O1" "-O2" "-Os" "-flto" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef signed char int8_t;
typedef unsigned char uint8_t;

/* Byte abs - should compile without ICE.
   R5900 has no pabsb instruction, so this should use scalar code.  */
NOMIPS16 void
abs_byte (int8_t *a, int8_t *b, int n)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] < 0 ? -a[i] : a[i];
}
/* { dg-final { scan-assembler-not "\tpabsb\t" } } */

/* Byte left shift - should compile without ICE.
   R5900 has no psllb instruction, so this should use scalar code.  */
NOMIPS16 void
shl_byte (uint8_t *a, uint8_t *b, int n)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] << 2;
}
/* { dg-final { scan-assembler-not "\tpsllb\t" } } */

/* Byte right shift (logical) - should compile without ICE.
   R5900 has no psrlb instruction.  */
NOMIPS16 void
shr_byte (uint8_t *a, uint8_t *b, int n)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] >> 2;
}
/* { dg-final { scan-assembler-not "\tpsrlb\t" } } */

/* Byte right shift (arithmetic) - should compile without ICE.
   R5900 has no psrab instruction.  */
NOMIPS16 void
ashr_byte (int8_t *a, int8_t *b, int n)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] >> 2;
}
/* { dg-final { scan-assembler-not "\tpsrab\t" } } */

/* Byte variable shift - should compile without ICE.  */
NOMIPS16 void
shl_byte_var (uint8_t *a, uint8_t *b, int n, int shift)
{
  for (int i = 0; i < n; i++)
    b[i] = a[i] << shift;
}
/* { dg-final { scan-assembler-not "\tpsllvb\t" } } */
