/* Test R5900 FPU ACC accumulator instruction builtins */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O1" "-O2" "-O3" "-Os" "-flto" } { "" } } */

/* Note: These builtins provide explicit control over ACC instructions.
   For most code, plain C arithmetic like a*b + c*d will automatically
   use ACC instructions when beneficial (see r5900-fpu-acc-auto.c).

   ACC-writing builtins (adda, suba, mula, madda, msuba) are tested
   at -O0 only because they have no visible side effects and get optimized
   away at higher optimization levels without a matching ACC-reading op. */

void test_adda(float a, float b) {
  __builtin_mips_adda_s(a, b);
}
/* { dg-final { scan-assembler "\tadda\\.s\t" } } */

void test_suba(float a, float b) {
  __builtin_mips_suba_s(a, b);
}
/* { dg-final { scan-assembler "\tsuba\\.s\t" } } */

void test_mula(float a, float b) {
  __builtin_mips_mula_s(a, b);
}
/* { dg-final { scan-assembler "\tmula\\.s\t" } } */

void test_madda(float a, float b) {
  __builtin_mips_madda_s(a, b);
}
/* { dg-final { scan-assembler "\tmadda\\.s\t" } } */

void test_msuba(float a, float b) {
  __builtin_mips_msuba_s(a, b);
}
/* { dg-final { scan-assembler "\tmsuba\\.s\t" } } */

float test_madd(float a, float b) {
  return __builtin_mips_madd_s(a, b);
}
/* { dg-final { scan-assembler "\tmadd\\.s\t" } } */

float test_msub(float a, float b) {
  return __builtin_mips_msub_s(a, b);
}
/* { dg-final { scan-assembler "\tmsub\\.s\t" } } */
