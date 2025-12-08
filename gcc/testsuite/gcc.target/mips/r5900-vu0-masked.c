/* Test R5900 VU0 masked intrinsics.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mvu0" } */

typedef float v4sf __attribute__((vector_size(16)));

/* Mask constants */
#define VU0_DEST_W    0x1
#define VU0_DEST_Z    0x2
#define VU0_DEST_ZW   0x3
#define VU0_DEST_Y    0x4
#define VU0_DEST_YW   0x5
#define VU0_DEST_YZ   0x6
#define VU0_DEST_YZW  0x7
#define VU0_DEST_X    0x8
#define VU0_DEST_XW   0x9
#define VU0_DEST_XZ   0xA
#define VU0_DEST_XZW  0xB
#define VU0_DEST_XY   0xC
#define VU0_DEST_XYW  0xD
#define VU0_DEST_XYZ  0xE
#define VU0_DEST_XYZW 0xF

/* Test vadd with xyz mask */
v4sf test_vadd_xyz (v4sf a, v4sf b)
{
  return __builtin_vu0_vadd_m (a, b, VU0_DEST_XYZ);
}
/* { dg-final { scan-assembler "\\tvadd\\.xyz\\t" } } */

/* Test vadd with w mask */
v4sf test_vadd_w (v4sf a, v4sf b)
{
  return __builtin_vu0_vadd_m (a, b, VU0_DEST_W);
}
/* { dg-final { scan-assembler "\\tvadd\\.w\\t" } } */

/* Test vsub with xyzw mask */
v4sf test_vsub_xyzw (v4sf a, v4sf b)
{
  return __builtin_vu0_vsub_m (a, b, VU0_DEST_XYZW);
}
/* { dg-final { scan-assembler "\\tvsub\\.xyzw\\t" } } */

/* Test vmul with xy mask */
v4sf test_vmul_xy (v4sf a, v4sf b)
{
  return __builtin_vu0_vmul_m (a, b, VU0_DEST_XY);
}
/* { dg-final { scan-assembler "\\tvmul\\.xy\\t" } } */

/* Test vabs with yzw mask */
v4sf test_vabs_yzw (v4sf a)
{
  return __builtin_vu0_vabs_m (a, VU0_DEST_YZW);
}
/* { dg-final { scan-assembler "\\tvabs\\.yzw\\t" } } */

/* Test vmax with z mask */
v4sf test_vmax_z (v4sf a, v4sf b)
{
  return __builtin_vu0_vmax_m (a, b, VU0_DEST_Z);
}
/* { dg-final { scan-assembler "\\tvmax\\.z\\t" } } */

/* Test vmini with xw mask */
v4sf test_vmini_xw (v4sf a, v4sf b)
{
  return __builtin_vu0_vmini_m (a, b, VU0_DEST_XW);
}
/* { dg-final { scan-assembler "\\tvmini\\.xw\\t" } } */

/* Test vmove with x mask */
v4sf test_vmove_x (v4sf a)
{
  return __builtin_vu0_vmove_m (a, VU0_DEST_X);
}
/* { dg-final { scan-assembler "\\tvmove\\.x\\t" } } */

/* Test vmadd with xyz mask (reads ACC) */
v4sf test_vmadd_xyz (v4sf a, v4sf b)
{
  return __builtin_vu0_vmadd_m (a, b, VU0_DEST_XYZ);
}
/* { dg-final { scan-assembler "\\tvmadd\\.xyz\\t" } } */

/* Test vmsub with yz mask (reads ACC) */
v4sf test_vmsub_yz (v4sf a, v4sf b)
{
  return __builtin_vu0_vmsub_m (a, b, VU0_DEST_YZ);
}
/* { dg-final { scan-assembler "\\tvmsub\\.yz\\t" } } */

/* Test vmula with xyzw mask (writes ACC), followed by vmadd to use result */
v4sf test_vmula_xyzw (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vmula_m (a, b, VU0_DEST_XYZW);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvmula\\.xyzw\\t" } } */

/* Test vmadda with xyz mask (reads/writes ACC) */
v4sf test_vmadda_xyz (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vmula (a, b);
  __builtin_vu0_vmadda_m (a, b, VU0_DEST_XYZ);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvmadda\\.xyz\\t" } } */

/* Test vmsuba with w mask (reads/writes ACC) */
v4sf test_vmsuba_w (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vmula (a, b);
  __builtin_vu0_vmsuba_m (a, b, VU0_DEST_W);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvmsuba\\.w\\t" } } */

/* Test vadda with xy mask (writes ACC) */
v4sf test_vadda_xy (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vadda_m (a, b, VU0_DEST_XY);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvadda\\.xy\\t" } } */

/* Test vsuba with zw mask (writes ACC) */
v4sf test_vsuba_zw (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vsuba_m (a, b, VU0_DEST_ZW);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvsuba\\.zw\\t" } } */

/* =================================================================
   Broadcast masked operations (BC variants)
   ================================================================= */

/* Test vaddx with xyz mask */
v4sf test_vaddx_xyz (v4sf a, v4sf b)
{
  return __builtin_vu0_vaddx_m (a, b, VU0_DEST_XYZ);
}
/* { dg-final { scan-assembler "\\tvaddx\\.xyz\\t" } } */

/* Test vaddy with xy mask */
v4sf test_vaddy_xy (v4sf a, v4sf b)
{
  return __builtin_vu0_vaddy_m (a, b, VU0_DEST_XY);
}
/* { dg-final { scan-assembler "\\tvaddy\\.xy\\t" } } */

/* Test vaddz with z mask */
v4sf test_vaddz_z (v4sf a, v4sf b)
{
  return __builtin_vu0_vaddz_m (a, b, VU0_DEST_Z);
}
/* { dg-final { scan-assembler "\\tvaddz\\.z\\t" } } */

/* Test vaddw with w mask */
v4sf test_vaddw_w (v4sf a, v4sf b)
{
  return __builtin_vu0_vaddw_m (a, b, VU0_DEST_W);
}
/* { dg-final { scan-assembler "\\tvaddw\\.w\\t" } } */

/* Test vsubx with xyzw mask */
v4sf test_vsubx_xyzw (v4sf a, v4sf b)
{
  return __builtin_vu0_vsubx_m (a, b, VU0_DEST_XYZW);
}
/* { dg-final { scan-assembler "\\tvsubx\\.xyzw\\t" } } */

/* Test vmulx with xz mask */
v4sf test_vmulx_xz (v4sf a, v4sf b)
{
  return __builtin_vu0_vmulx_m (a, b, VU0_DEST_XZ);
}
/* { dg-final { scan-assembler "\\tvmulx\\.xz\\t" } } */

/* Test vmaddx with yz mask (reads ACC) */
v4sf test_vmaddx_yz (v4sf a, v4sf b)
{
  return __builtin_vu0_vmaddx_m (a, b, VU0_DEST_YZ);
}
/* { dg-final { scan-assembler "\\tvmaddx\\.yz\\t" } } */

/* Test vmsubw with xw mask (reads ACC) */
v4sf test_vmsubw_xw (v4sf a, v4sf b)
{
  return __builtin_vu0_vmsubw_m (a, b, VU0_DEST_XW);
}
/* { dg-final { scan-assembler "\\tvmsubw\\.xw\\t" } } */

/* =================================================================
   Broadcast ACC masked operations
   ================================================================= */

/* Test vmulax with xyz mask (writes ACC) */
v4sf test_vmulax_xyz (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vmulax_m (a, b, VU0_DEST_XYZ);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvmulax\\.xyz\\t" } } */

/* Test vmulaw with w mask (writes ACC) */
v4sf test_vmulaw_w (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vmulaw_m (a, b, VU0_DEST_W);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvmulaw\\.w\\t" } } */

/* Test vmaddax with xy mask (reads/writes ACC) */
v4sf test_vmaddax_xy (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vmula (a, b);
  __builtin_vu0_vmaddax_m (a, b, VU0_DEST_XY);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvmaddax\\.xy\\t" } } */

/* Test vmsubaz with z mask (reads/writes ACC) */
v4sf test_vmsubaz_z (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vmula (a, b);
  __builtin_vu0_vmsubaz_m (a, b, VU0_DEST_Z);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvmsubaz\\.z\\t" } } */

/* Test vaddax with xyzw mask (writes ACC) */
v4sf test_vaddax_xyzw (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vaddax_m (a, b, VU0_DEST_XYZW);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvaddax\\.xyzw\\t" } } */

/* Test vsubaw with yw mask (writes ACC) */
v4sf test_vsubaw_yw (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vsubaw_m (a, b, VU0_DEST_YW);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvsubaw\\.yw\\t" } } */

/* =================================================================
   Q register masked operations
   ================================================================= */

/* Test vaddq with xyz mask */
v4sf test_vaddq_xyz (v4sf a)
{
  return __builtin_vu0_vaddq_m (a, VU0_DEST_XYZ);
}
/* { dg-final { scan-assembler "\\tvaddq\\.xyz\\t" } } */

/* Test vsubq with x mask */
v4sf test_vsubq_x (v4sf a)
{
  return __builtin_vu0_vsubq_m (a, VU0_DEST_X);
}
/* { dg-final { scan-assembler "\\tvsubq\\.x\\t" } } */

/* Test vmulq with xyzw mask */
v4sf test_vmulq_xyzw (v4sf a)
{
  return __builtin_vu0_vmulq_m (a, VU0_DEST_XYZW);
}
/* { dg-final { scan-assembler "\\tvmulq\\.xyzw\\t" } } */

/* Test vmaddq with yz mask (reads ACC) */
v4sf test_vmaddq_yz (v4sf a)
{
  return __builtin_vu0_vmaddq_m (a, VU0_DEST_YZ);
}
/* { dg-final { scan-assembler "\\tvmaddq\\.yz\\t" } } */

/* Test vmsubq with w mask (reads ACC) */
v4sf test_vmsubq_w (v4sf a)
{
  return __builtin_vu0_vmsubq_m (a, VU0_DEST_W);
}
/* { dg-final { scan-assembler "\\tvmsubq\\.w\\t" } } */

/* Test vaddaq with xy mask (writes ACC) */
v4sf test_vaddaq_xy (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vaddaq_m (a, VU0_DEST_XY);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvaddaq\\.xy\\t" } } */

/* Test vmulaq with xzw mask (writes ACC) */
v4sf test_vmulaq_xzw (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vmulaq_m (a, VU0_DEST_XZW);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvmulaq\\.xzw\\t" } } */

/* Test vmaddaq with z mask (reads/writes ACC) */
v4sf test_vmaddaq_z (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vmula (a, b);
  __builtin_vu0_vmaddaq_m (a, VU0_DEST_Z);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvmaddaq\\.z\\t" } } */

/* =================================================================
   I register masked operations
   ================================================================= */

/* Test vaddi with xyzw mask */
v4sf test_vaddi_xyzw (v4sf a)
{
  return __builtin_vu0_vaddi_m (a, VU0_DEST_XYZW);
}
/* { dg-final { scan-assembler "\\tvaddi\\.xyzw\\t" } } */

/* Test vsubi with xy mask */
v4sf test_vsubi_xy (v4sf a)
{
  return __builtin_vu0_vsubi_m (a, VU0_DEST_XY);
}
/* { dg-final { scan-assembler "\\tvsubi\\.xy\\t" } } */

/* Test vmuli with z mask */
v4sf test_vmuli_z (v4sf a)
{
  return __builtin_vu0_vmuli_m (a, VU0_DEST_Z);
}
/* { dg-final { scan-assembler "\\tvmuli\\.z\\t" } } */

/* Test vmaxi with xyz mask */
v4sf test_vmaxi_xyz (v4sf a)
{
  return __builtin_vu0_vmaxi_m (a, VU0_DEST_XYZ);
}
/* { dg-final { scan-assembler "\\tvmaxi\\.xyz\\t" } } */

/* Test vminii with w mask */
v4sf test_vminii_w (v4sf a)
{
  return __builtin_vu0_vminii_m (a, VU0_DEST_W);
}
/* { dg-final { scan-assembler "\\tvminii\\.w\\t" } } */

/* Test vmaddi with yw mask (reads ACC) */
v4sf test_vmaddi_yw (v4sf a)
{
  return __builtin_vu0_vmaddi_m (a, VU0_DEST_YW);
}
/* { dg-final { scan-assembler "\\tvmaddi\\.yw\\t" } } */

/* Test vmsubi with xz mask (reads ACC) */
v4sf test_vmsubi_xz (v4sf a)
{
  return __builtin_vu0_vmsubi_m (a, VU0_DEST_XZ);
}
/* { dg-final { scan-assembler "\\tvmsubi\\.xz\\t" } } */

/* Test vaddai with xyzw mask (writes ACC) */
v4sf test_vaddai_xyzw (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vaddai_m (a, VU0_DEST_XYZW);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvaddai\\.xyzw\\t" } } */

/* Test vmulai with zw mask (writes ACC) */
v4sf test_vmulai_zw (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vmulai_m (a, VU0_DEST_ZW);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvmulai\\.zw\\t" } } */

/* Test vmaddai with x mask (reads/writes ACC) */
v4sf test_vmaddai_x (v4sf a, v4sf b, v4sf c)
{
  __builtin_vu0_vmula (a, b);
  __builtin_vu0_vmaddai_m (a, VU0_DEST_X);
  return __builtin_vu0_vmadd (b, c);
}
/* { dg-final { scan-assembler "\\tvmaddai\\.x\\t" } } */

/* =================================================================
   Conversion masked operations
   ================================================================= */

/* Test vftoi0 with xyz mask */
v4sf test_vftoi0_xyz (v4sf a)
{
  return __builtin_vu0_vftoi0_m (a, VU0_DEST_XYZ);
}
/* { dg-final { scan-assembler "\\tvftoi0\\.xyz\\t" } } */

/* Test vftoi4 with x mask */
v4sf test_vftoi4_x (v4sf a)
{
  return __builtin_vu0_vftoi4_m (a, VU0_DEST_X);
}
/* { dg-final { scan-assembler "\\tvftoi4\\.x\\t" } } */

/* Test vftoi12 with yw mask */
v4sf test_vftoi12_yw (v4sf a)
{
  return __builtin_vu0_vftoi12_m (a, VU0_DEST_YW);
}
/* { dg-final { scan-assembler "\\tvftoi12\\.yw\\t" } } */

/* Test vftoi15 with zw mask */
v4sf test_vftoi15_zw (v4sf a)
{
  return __builtin_vu0_vftoi15_m (a, VU0_DEST_ZW);
}
/* { dg-final { scan-assembler "\\tvftoi15\\.zw\\t" } } */

/* Test vitof0 with xyzw mask */
v4sf test_vitof0_xyzw (v4sf a)
{
  return __builtin_vu0_vitof0_m (a, VU0_DEST_XYZW);
}
/* { dg-final { scan-assembler "\\tvitof0\\.xyzw\\t" } } */

/* Test vitof4 with y mask */
v4sf test_vitof4_y (v4sf a)
{
  return __builtin_vu0_vitof4_m (a, VU0_DEST_Y);
}
/* { dg-final { scan-assembler "\\tvitof4\\.y\\t" } } */

/* Test vitof12 with xz mask */
v4sf test_vitof12_xz (v4sf a)
{
  return __builtin_vu0_vitof12_m (a, VU0_DEST_XZ);
}
/* { dg-final { scan-assembler "\\tvitof12\\.xz\\t" } } */

/* Test vitof15 with w mask */
v4sf test_vitof15_w (v4sf a)
{
  return __builtin_vu0_vitof15_m (a, VU0_DEST_W);
}
/* { dg-final { scan-assembler "\\tvitof15\\.w\\t" } } */

/* =================================================================
   Data movement masked operations
   ================================================================= */

/* Test vmr32 with xyz mask */
v4sf test_vmr32_xyz (v4sf a)
{
  return __builtin_vu0_vmr32_m (a, VU0_DEST_XYZ);
}
/* { dg-final { scan-assembler "\\tvmr32\\.xyz\\t" } } */

/* Test vmr32 with xw mask */
v4sf test_vmr32_xw (v4sf a)
{
  return __builtin_vu0_vmr32_m (a, VU0_DEST_XW);
}
/* { dg-final { scan-assembler "\\tvmr32\\.xw\\t" } } */
