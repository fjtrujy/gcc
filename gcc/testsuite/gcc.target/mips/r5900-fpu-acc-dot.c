/* Test R5900 FPU ACC with dot product using explicit builtin chains */
/* { dg-do compile } */
/* { dg-options "-march=r5900" } */
/* { dg-skip-if "requires optimization" { *-*-* } { "-O0" "-O1" } { "" } } */

/* FPU ACC is a fixed register - use intrinsics for explicit control.
   These builtin chains are useful for dot products and reductions. */

/* 3-element dot product using explicit ACC chain */
float dot3_builtin(float a0, float a1, float a2,
                   float b0, float b1, float b2) {
  __builtin_mips_mula_s(a0, b0);     /* ACC = a0*b0 */
  __builtin_mips_madda_s(a1, b1);    /* ACC += a1*b1 */
  return __builtin_mips_madd_s(a2, b2);  /* return ACC + a2*b2 */
}
/* { dg-final { scan-assembler "\tmula\\.s\t" } } */
/* { dg-final { scan-assembler "\tmadda\\.s\t" } } */
/* { dg-final { scan-assembler "\tmadd\\.s\t" } } */

/* 4-element dot product - ideal ACC chain pattern:
   mula.s ACC, a[0], b[0]
   madda.s ACC, a[1], b[1]
   madda.s ACC, a[2], b[2]
   madd.s result, a[3], b[3]
*/
float dot4_builtin(float a0, float a1, float a2, float a3,
                   float b0, float b1, float b2, float b3) {
  __builtin_mips_mula_s(a0, b0);     /* ACC = a0*b0 */
  __builtin_mips_madda_s(a1, b1);    /* ACC += a1*b1 */
  __builtin_mips_madda_s(a2, b2);    /* ACC += a2*b2 */
  return __builtin_mips_madd_s(a3, b3);  /* return ACC + a3*b3 */
}
