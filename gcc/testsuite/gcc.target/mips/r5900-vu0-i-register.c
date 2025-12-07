/* Test R5900 VU0 I register operations (CTC2_I load and I broadcast) */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mvu0" } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef float v4sf __attribute__((vector_size(16)));

/* Test __builtin_vu0_ctc2_i: Load I register via CTC2 */
NOMIPS16 void
test_ctc2_i (int val)
{
  __builtin_vu0_ctc2_i (val);
}
/* { dg-final { scan-assembler "\\tctc2\\t\[^,\]+,\\\$21" } } */

/* Test __builtin_vu0_vaddi: dest = src + I */
NOMIPS16 v4sf
test_vaddi (v4sf a)
{
  return __builtin_vu0_vaddi (a);
}
/* { dg-final { scan-assembler "\\tvaddi\\.xyzw\\t" } } */

/* Test __builtin_vu0_vsubi: dest = src - I */
NOMIPS16 v4sf
test_vsubi (v4sf a)
{
  return __builtin_vu0_vsubi (a);
}
/* { dg-final { scan-assembler "\\tvsubi\\.xyzw\\t" } } */

/* Test __builtin_vu0_vmuli: dest = src * I */
NOMIPS16 v4sf
test_vmuli (v4sf a)
{
  return __builtin_vu0_vmuli (a);
}
/* { dg-final { scan-assembler "\\tvmuli\\.xyzw\\t" } } */

/* Test __builtin_vu0_vmaxi: dest = max(src, I) */
NOMIPS16 v4sf
test_vmaxi (v4sf a)
{
  return __builtin_vu0_vmaxi (a);
}
/* { dg-final { scan-assembler "\\tvmaxi\\.xyzw\\t" } } */

/* Test __builtin_vu0_vminii: dest = min(src, I) */
NOMIPS16 v4sf
test_vminii (v4sf a)
{
  return __builtin_vu0_vminii (a);
}
/* { dg-final { scan-assembler "\\tvminii\\.xyzw\\t" } } */

/* Test __builtin_vu0_vaddai: ACC = src + I */
NOMIPS16 void
test_vaddai (v4sf a)
{
  __builtin_vu0_vaddai (a);
}
/* { dg-final { scan-assembler "\\tvaddai\\.xyzw\\tACC," } } */

/* Test __builtin_vu0_vsubai: ACC = src - I */
NOMIPS16 void
test_vsubai (v4sf a)
{
  __builtin_vu0_vsubai (a);
}
/* { dg-final { scan-assembler "\\tvsubai\\.xyzw\\tACC," } } */

/* Test __builtin_vu0_vmulai: ACC = src * I */
NOMIPS16 void
test_vmulai (v4sf a)
{
  __builtin_vu0_vmulai (a);
}
/* { dg-final { scan-assembler "\\tvmulai\\.xyzw\\tACC," } } */

/* Test __builtin_vu0_vmaddi: dest = ACC + src * I */
NOMIPS16 v4sf
test_vmaddi (v4sf a)
{
  return __builtin_vu0_vmaddi (a);
}
/* { dg-final { scan-assembler "\\tvmaddi\\.xyzw\\t" } } */

/* Test __builtin_vu0_vmsubi: dest = ACC - src * I */
NOMIPS16 v4sf
test_vmsubi (v4sf a)
{
  return __builtin_vu0_vmsubi (a);
}
/* { dg-final { scan-assembler "\\tvmsubi\\.xyzw\\t" } } */

/* Test __builtin_vu0_vmaddai: ACC = ACC + src * I */
NOMIPS16 void
test_vmaddai (v4sf a)
{
  __builtin_vu0_vmaddai (a);
}
/* { dg-final { scan-assembler "\\tvmaddai\\.xyzw\\tACC," } } */

/* Test __builtin_vu0_vmsubai: ACC = ACC - src * I */
NOMIPS16 void
test_vmsubai (v4sf a)
{
  __builtin_vu0_vmsubai (a);
}
/* { dg-final { scan-assembler "\\tvmsubai\\.xyzw\\tACC," } } */
