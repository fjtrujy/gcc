/* Test R5900 VU0 CTC2/CFC2 control register transfer operations */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mvu0" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* Test __builtin_vu0_cfc2: Read VU0 control register to GP register */
NOMIPS16 int
test_cfc2_status (void)
{
  return __builtin_vu0_cfc2 (16);  /* Read Status register (id=16) */
}
/* { dg-final { scan-assembler "\\tcfc2\\t\[^,\]+,\\\$16" } } */

NOMIPS16 int
test_cfc2_i (void)
{
  return __builtin_vu0_cfc2 (21);  /* Read I register (id=21) */
}
/* { dg-final { scan-assembler "\\tcfc2\\t\[^,\]+,\\\$21" } } */

NOMIPS16 int
test_cfc2_q (void)
{
  return __builtin_vu0_cfc2 (22);  /* Read Q register (id=22) */
}
/* { dg-final { scan-assembler "\\tcfc2\\t\[^,\]+,\\\$22" } } */

/* Test __builtin_vu0_ctc2: Write GP register to VU0 control register */
NOMIPS16 void
test_ctc2_status (int val)
{
  __builtin_vu0_ctc2 (val, 16);  /* Write to Status register */
}
/* { dg-final { scan-assembler "\\tctc2\\t\[^,\]+,\\\$16" } } */

NOMIPS16 void
test_ctc2_r (int val)
{
  __builtin_vu0_ctc2 (val, 20);  /* Write to R register (random number) */
}
/* { dg-final { scan-assembler "\\tctc2\\t\[^,\]+,\\\$20" } } */

NOMIPS16 void
test_ctc2_cmsar0 (int val)
{
  __builtin_vu0_ctc2 (val, 27);  /* Write to CMSAR0 (VU0 micro start addr) */
}
/* { dg-final { scan-assembler "\\tctc2\\t\[^,\]+,\\\$27" } } */
