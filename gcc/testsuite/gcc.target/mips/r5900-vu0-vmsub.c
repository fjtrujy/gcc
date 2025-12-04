/* Test R5900 VU0 multiply-subtract builtins */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mvu0" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef float v4sf __attribute__((vector_size(16)));

/* Test basic accumulator operations: vmula, vmsuba, vmsub
   Computes: ACC = a * b; ACC -= c * d; result = ACC - e * f */
NOMIPS16 void
test_msub_accumulator (v4sf *result, v4sf a, v4sf b, v4sf c, v4sf d, v4sf e, v4sf f)
{
  __builtin_vu0_vmula (a, b);    /* ACC = a * b */
  __builtin_vu0_vmsuba (c, d);   /* ACC -= c * d */
  *result = __builtin_vu0_vmsub (e, f);  /* result = ACC - e * f */
}
/* { dg-final { scan-assembler "\\tvmula\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmsuba\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmsub\\.xyzw\\t" } } */

/* Test broadcast multiply-subtract to accumulator: vmsubax/y/z/w
   Computes: ACC = a * b; ACC -= c * d.x; ACC -= e * f.y */
NOMIPS16 void
test_msuba_broadcast (v4sf *result, v4sf a, v4sf b, v4sf c, v4sf d, v4sf e, v4sf f)
{
  __builtin_vu0_vmula (a, b);      /* ACC = a * b */
  __builtin_vu0_vmsubax (c, d);    /* ACC -= c * d.x */
  __builtin_vu0_vmsubay (c, d);    /* ACC -= c * d.y */
  __builtin_vu0_vmsubaz (e, f);    /* ACC -= e * f.z */
  __builtin_vu0_vmsubaw (e, f);    /* ACC -= e * f.w */
  *result = __builtin_vu0_vmadd (a, a);  /* result = ACC + a * a */
}
/* { dg-final { scan-assembler "\\tvmsubax\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmsubay\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmsubaz\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmsubaw\\.xyzw\\t" } } */

/* Test broadcast multiply-subtract with register result: vmsubx/y/z/w
   Computes: ACC = a * b; result = ACC - c * d.x/y/z/w */
NOMIPS16 void
test_msub_broadcast (v4sf *dst, v4sf a, v4sf b, v4sf c, v4sf d)
{
  __builtin_vu0_vmula (a, b);
  dst[0] = __builtin_vu0_vmsubx (c, d);  /* ACC - c * d.x */
  __builtin_vu0_vmula (a, b);
  dst[1] = __builtin_vu0_vmsuby (c, d);  /* ACC - c * d.y */
  __builtin_vu0_vmula (a, b);
  dst[2] = __builtin_vu0_vmsubz (c, d);  /* ACC - c * d.z */
  __builtin_vu0_vmula (a, b);
  dst[3] = __builtin_vu0_vmsubw (c, d);  /* ACC - c * d.w */
}
/* { dg-final { scan-assembler "\\tvmsubx\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmsuby\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmsubz\\.xyzw\\t" } } */
/* { dg-final { scan-assembler "\\tvmsubw\\.xyzw\\t" } } */
