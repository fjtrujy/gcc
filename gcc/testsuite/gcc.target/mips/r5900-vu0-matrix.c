/* Test R5900 VU0 matrix multiplication builtins */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mvu0" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef float v4sf __attribute__((vector_size(16)));

/* Matrix * Vector multiplication using VU0 accumulator and broadcast ops.
   Classic PS2 approach:
     result = mat[0] * vec.x + mat[1] * vec.y + mat[2] * vec.z + mat[3] * vec.w

   Using VU0 instructions:
     vmulax  ACC, mat[0], vec    // ACC = mat[0] * vec.x
     vmadday ACC, mat[1], vec    // ACC += mat[1] * vec.y
     vmaddaz ACC, mat[2], vec    // ACC += mat[2] * vec.z
     vmaddw  result, mat[3], vec // result = ACC + mat[3] * vec.w
*/
NOMIPS16 void
test_matvec (v4sf *result, v4sf *mat, v4sf vec)
{
  __builtin_vu0_vmulax (mat[0], vec);
  __builtin_vu0_vmadday (mat[1], vec);
  __builtin_vu0_vmaddaz (mat[2], vec);
  *result = __builtin_vu0_vmaddw (mat[3], vec);
}
/* { dg-final { scan-assembler "\\tvmulax\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmadday\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmaddaz\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmaddw\\.xyzw\\t" } } */

/* Test basic accumulator operations: vmula, vmadda, vmadd */
NOMIPS16 void
test_accumulator (v4sf *result, v4sf a, v4sf b, v4sf c, v4sf d)
{
  __builtin_vu0_vmula (a, b);    /* ACC = a * b */
  __builtin_vu0_vmadda (c, d);   /* ACC += c * d */
  *result = __builtin_vu0_vmadd (a, c);  /* result = ACC + a * c */
}
/* { dg-final { scan-assembler "\\tvmula\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmadda\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmadd\\.xyzw\\t" } } */

/* Test broadcast multiply operations: vmulx, vmuly, vmulz, vmulw */
NOMIPS16 void
test_broadcast_mul (v4sf *dst, v4sf a, v4sf b)
{
  dst[0] = __builtin_vu0_vmulx (a, b);  /* a * b.x */
  dst[1] = __builtin_vu0_vmuly (a, b);  /* a * b.y */
  dst[2] = __builtin_vu0_vmulz (a, b);  /* a * b.z */
  dst[3] = __builtin_vu0_vmulw (a, b);  /* a * b.w */
}
/* { dg-final { scan-assembler "\\tvmulx\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmuly\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmulz\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmulw\\.xyzw\\t" } } */

/* Test cross product using outer product operations */
NOMIPS16 void
test_cross_product (v4sf *result, v4sf a, v4sf b)
{
  /* Cross product: a x b
     vopmula computes: ACC.xyz = a.yzx * b.zxy
     vopmsub computes: result.xyz = ACC.xyz - a.zxy * b.yzx
     This gives: a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x */
  __builtin_vu0_vopmula (a, b);
  *result = __builtin_vu0_vopmsub (a, b);
}
/* { dg-final { scan-assembler "\\tvopmula\\.xyz\\t" } } */
/* { dg-final { scan-assembler "\\tvopmsub\\.xyz\\t" } } */
