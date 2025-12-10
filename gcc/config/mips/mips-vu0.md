;; VU0 128-bit vector float patterns for PS2 R5900
;; Also provides V4SF patterns for MSA compatibility
;; Copyright (C) 2025 Free Software Foundation, Inc.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 3, or (at your
;; option) any later version.
;;
;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; VU0 operates on 4x32-bit single-precision floats (V4SF mode)
;; using COP2 registers (vf0-vf31)
;; MSA also supports V4SF using FP registers

;; VU0 register constants
(define_constants
  [(VU0_ACC_REGNUM		188)
   (VU0_Q_REGNUM		192)
   (VU0_I_REGNUM		193)])

;; VU0 UNSPEC constants
(define_c_enum "unspec" [
  ;; Basic accumulator operations
  UNSPEC_VU0_VMULA      ;; ACC = a * b
  UNSPEC_VU0_VMADDA     ;; ACC = ACC + a * b
  UNSPEC_VU0_VMADD      ;; dest = ACC + a * b
  UNSPEC_VU0_VMSUBA     ;; ACC = ACC - a * b
  UNSPEC_VU0_VMSUB      ;; dest = ACC - a * b
  ;; Broadcast multiply operations
  UNSPEC_VU0_VMULX
  UNSPEC_VU0_VMULY
  UNSPEC_VU0_VMULZ
  UNSPEC_VU0_VMULW
  ;; Broadcast multiply to accumulator
  UNSPEC_VU0_VMULAX
  UNSPEC_VU0_VMULAY
  UNSPEC_VU0_VMULAZ
  UNSPEC_VU0_VMULAW
  ;; Broadcast multiply-add to accumulator
  UNSPEC_VU0_VMADDAX
  UNSPEC_VU0_VMADDAY
  UNSPEC_VU0_VMADDAZ
  UNSPEC_VU0_VMADDAW
  ;; Broadcast multiply-add with register result
  UNSPEC_VU0_VMADDX
  UNSPEC_VU0_VMADDY
  UNSPEC_VU0_VMADDZ
  UNSPEC_VU0_VMADDW
  ;; Broadcast multiply-subtract to accumulator
  UNSPEC_VU0_VMSUBAX
  UNSPEC_VU0_VMSUBAY
  UNSPEC_VU0_VMSUBAZ
  UNSPEC_VU0_VMSUBAW
  ;; Broadcast multiply-subtract with register result
  UNSPEC_VU0_VMSUBX
  UNSPEC_VU0_VMSUBY
  UNSPEC_VU0_VMSUBZ
  UNSPEC_VU0_VMSUBW
  ;; Outer product operations
  UNSPEC_VU0_VOPMULA
  UNSPEC_VU0_VOPMSUB
  ;; Broadcast add operations
  UNSPEC_VU0_VADDX
  UNSPEC_VU0_VADDY
  UNSPEC_VU0_VADDZ
  UNSPEC_VU0_VADDW
  UNSPEC_VU0_VADDW_XYZ  ;; vaddw.xyz - only affects xyz, w unchanged
  ;; Broadcast subtract operations
  UNSPEC_VU0_VSUBX
  UNSPEC_VU0_VSUBY
  UNSPEC_VU0_VSUBZ
  UNSPEC_VU0_VSUBW
  ;; Accumulator add/subtract
  UNSPEC_VU0_VADDA
  UNSPEC_VU0_VSUBA
  ;; Broadcast add to accumulator
  UNSPEC_VU0_VADDAX
  UNSPEC_VU0_VADDAY
  UNSPEC_VU0_VADDAZ
  UNSPEC_VU0_VADDAW
  ;; Broadcast subtract from accumulator
  UNSPEC_VU0_VSUBAX
  UNSPEC_VU0_VSUBAY
  UNSPEC_VU0_VSUBAZ
  UNSPEC_VU0_VSUBAW
  ;; Float to fixed-point conversions
  UNSPEC_VU0_VFTOI0
  UNSPEC_VU0_VFTOI4
  UNSPEC_VU0_VFTOI12
  UNSPEC_VU0_VFTOI15
  ;; Fixed-point to float conversions
  UNSPEC_VU0_VITOF0
  UNSPEC_VU0_VITOF4
  UNSPEC_VU0_VITOF12
  UNSPEC_VU0_VITOF15
  ;; Vector rotate
  UNSPEC_VU0_VMR32
  ;; Explicit intrinsic operations
  UNSPEC_VU0_VMAX
  UNSPEC_VU0_VMINI
  UNSPEC_VU0_VMOVE
  ;; Q register operations (division/sqrt)
  UNSPEC_VU0_VDIV          ;; Q = fs.bc / ft.bc
  UNSPEC_VU0_VSQRT         ;; Q = sqrt(ft.bc)
  UNSPEC_VU0_VRSQRT        ;; Q = fs.bc / sqrt(ft.bc)
  UNSPEC_VU0_WAITQ         ;; Wait for Q ready
  ;; Q broadcast operations
  UNSPEC_VU0_VADDQ         ;; dest = src + Q
  UNSPEC_VU0_VSUBQ         ;; dest = src - Q
  UNSPEC_VU0_VMULQ         ;; dest = src * Q
  UNSPEC_VU0_VADDQA        ;; ACC = src + Q
  UNSPEC_VU0_VSUBQA        ;; ACC = src - Q
  UNSPEC_VU0_VMULQA        ;; ACC = src * Q (vmulaQ)
  UNSPEC_VU0_VMADDQ        ;; dest = ACC + src * Q
  UNSPEC_VU0_VMSUBQ        ;; dest = ACC - src * Q
  UNSPEC_VU0_VMADDQA       ;; ACC = ACC + src * Q
  UNSPEC_VU0_VMSUBQA       ;; ACC = ACC - src * Q
  UNSPEC_VU0_VMAXQ         ;; dest = max(src, Q)
  UNSPEC_VU0_VMINIQ        ;; dest = min(src, Q)
  ;; I register operations
  UNSPEC_VU0_CTC2_I        ;; Load I register via CTC2
  ;; I broadcast operations
  UNSPEC_VU0_VADDI         ;; dest = src + I
  UNSPEC_VU0_VSUBI         ;; dest = src - I
  UNSPEC_VU0_VMULI         ;; dest = src * I
  UNSPEC_VU0_VADDAI        ;; ACC = src + I
  UNSPEC_VU0_VSUBAI        ;; ACC = src - I
  UNSPEC_VU0_VMULAI        ;; ACC = src * I (vmulaI)
  UNSPEC_VU0_VMADDI        ;; dest = ACC + src * I
  UNSPEC_VU0_VMSUBI        ;; dest = ACC - src * I
  UNSPEC_VU0_VMADDAI       ;; ACC = ACC + src * I
  UNSPEC_VU0_VMSUBAI       ;; ACC = ACC - src * I
  UNSPEC_VU0_VMAXI         ;; dest = max(src, I)
  UNSPEC_VU0_VMINII        ;; dest = min(src, I)
  ;; CTC2/CFC2 generic control register transfer
  UNSPEC_VU0_CTC2          ;; Write GP to VU0 control register
  UNSPEC_VU0_CFC2          ;; Read VU0 control register to GP
  ;; Helper for autovectorization
  UNSPEC_MFC1_VU0          ;; Move FP bits to GP for CTC2
  UNSPEC_VU0_QMTC2_SCALAR  ;; Transfer scalar (SI) to VU0 reg x component
  ;; Masked operations (arbitrary dest field)
  UNSPEC_VU0_VADD_M        ;; vadd with dest mask
  UNSPEC_VU0_VSUB_M        ;; vsub with dest mask
  UNSPEC_VU0_VMUL_M        ;; vmul with dest mask
  UNSPEC_VU0_VABS_M        ;; vabs with dest mask
  UNSPEC_VU0_VMAX_M        ;; vmax with dest mask
  UNSPEC_VU0_VMINI_M       ;; vmini with dest mask
  UNSPEC_VU0_VMOVE_M       ;; vmove with dest mask
  UNSPEC_VU0_VMADD_M       ;; vmadd with dest mask
  UNSPEC_VU0_VMSUB_M       ;; vmsub with dest mask
  UNSPEC_VU0_VMULA_M       ;; vmula with dest mask
  UNSPEC_VU0_VMADDA_M      ;; vmadda with dest mask
  UNSPEC_VU0_VMSUBA_M      ;; vmsuba with dest mask
  UNSPEC_VU0_VADDA_M       ;; vadda with dest mask
  UNSPEC_VU0_VSUBA_M       ;; vsuba with dest mask
  ;; Broadcast masked operations (BC variants)
  UNSPEC_VU0_VADDX_M       ;; vaddx with dest mask
  UNSPEC_VU0_VADDY_M       ;; vaddy with dest mask
  UNSPEC_VU0_VADDZ_M       ;; vaddz with dest mask
  UNSPEC_VU0_VADDW_M       ;; vaddw with dest mask
  UNSPEC_VU0_VSUBX_M       ;; vsubx with dest mask
  UNSPEC_VU0_VSUBY_M       ;; vsuby with dest mask
  UNSPEC_VU0_VSUBZ_M       ;; vsubz with dest mask
  UNSPEC_VU0_VSUBW_M       ;; vsubw with dest mask
  UNSPEC_VU0_VMULX_M       ;; vmulx with dest mask
  UNSPEC_VU0_VMULY_M       ;; vmuly with dest mask
  UNSPEC_VU0_VMULZ_M       ;; vmulz with dest mask
  UNSPEC_VU0_VMULW_M       ;; vmulw with dest mask
  UNSPEC_VU0_VMADDX_M      ;; vmaddx with dest mask
  UNSPEC_VU0_VMADDY_M      ;; vmaddy with dest mask
  UNSPEC_VU0_VMADDZ_M      ;; vmaddz with dest mask
  UNSPEC_VU0_VMADDW_M      ;; vmaddw with dest mask
  UNSPEC_VU0_VMSUBX_M      ;; vmsubx with dest mask
  UNSPEC_VU0_VMSUBY_M      ;; vmsuby with dest mask
  UNSPEC_VU0_VMSUBZ_M      ;; vmsubz with dest mask
  UNSPEC_VU0_VMSUBW_M      ;; vmsubw with dest mask
  ;; Broadcast ACC masked operations
  UNSPEC_VU0_VMULAX_M      ;; vmulax with dest mask
  UNSPEC_VU0_VMULAY_M      ;; vmulay with dest mask
  UNSPEC_VU0_VMULAZ_M      ;; vmulaz with dest mask
  UNSPEC_VU0_VMULAW_M      ;; vmulaw with dest mask
  UNSPEC_VU0_VMADDAX_M     ;; vmaddax with dest mask
  UNSPEC_VU0_VMADDAY_M     ;; vmadday with dest mask
  UNSPEC_VU0_VMADDAZ_M     ;; vmaddaz with dest mask
  UNSPEC_VU0_VMADDAW_M     ;; vmaddaw with dest mask
  UNSPEC_VU0_VMSUBAX_M     ;; vmsubax with dest mask
  UNSPEC_VU0_VMSUBAY_M     ;; vmsubay with dest mask
  UNSPEC_VU0_VMSUBAZ_M     ;; vmsubaz with dest mask
  UNSPEC_VU0_VMSUBAW_M     ;; vmsubaw with dest mask
  UNSPEC_VU0_VADDAX_M      ;; vaddax with dest mask
  UNSPEC_VU0_VADDAY_M      ;; vadday with dest mask
  UNSPEC_VU0_VADDAZ_M      ;; vaddaz with dest mask
  UNSPEC_VU0_VADDAW_M      ;; vaddaw with dest mask
  UNSPEC_VU0_VSUBAX_M      ;; vsubax with dest mask
  UNSPEC_VU0_VSUBAY_M      ;; vsubay with dest mask
  UNSPEC_VU0_VSUBAZ_M      ;; vsubaz with dest mask
  UNSPEC_VU0_VSUBAW_M      ;; vsubaw with dest mask
  ;; Q register masked operations
  UNSPEC_VU0_VADDQ_M       ;; vaddq with dest mask
  UNSPEC_VU0_VSUBQ_M       ;; vsubq with dest mask
  UNSPEC_VU0_VMULQ_M       ;; vmulq with dest mask
  UNSPEC_VU0_VMAXQ_M       ;; vmaxq with dest mask (not valid, but for completeness)
  UNSPEC_VU0_VMINIQ_M      ;; vminiq with dest mask (not valid, but for completeness)
  UNSPEC_VU0_VMADDQ_M      ;; vmaddq with dest mask
  UNSPEC_VU0_VMSUBQ_M      ;; vmsubq with dest mask
  UNSPEC_VU0_VADDAQ_M      ;; vaddaq with dest mask
  UNSPEC_VU0_VSUBAQ_M      ;; vsubaq with dest mask
  UNSPEC_VU0_VMULAQ_M      ;; vmulaq with dest mask
  UNSPEC_VU0_VMADDAQ_M     ;; vmaddaq with dest mask
  UNSPEC_VU0_VMSUBAQ_M     ;; vmsubaq with dest mask
  ;; I register masked operations
  UNSPEC_VU0_VADDI_M       ;; vaddi with dest mask
  UNSPEC_VU0_VSUBI_M       ;; vsubi with dest mask
  UNSPEC_VU0_VMULI_M       ;; vmuli with dest mask
  UNSPEC_VU0_VMAXI_M       ;; vmaxi with dest mask (not valid, but for completeness)
  UNSPEC_VU0_VMINII_M      ;; vminii with dest mask (not valid, but for completeness)
  UNSPEC_VU0_VMADDI_M      ;; vmaddi with dest mask
  UNSPEC_VU0_VMSUBI_M      ;; vmsubi with dest mask
  UNSPEC_VU0_VADDAI_M      ;; vaddai with dest mask
  UNSPEC_VU0_VSUBAI_M      ;; vsubai with dest mask
  UNSPEC_VU0_VMULAI_M      ;; vmulai with dest mask
  UNSPEC_VU0_VMADDAI_M     ;; vmaddai with dest mask
  UNSPEC_VU0_VMSUBAI_M     ;; vmsubai with dest mask
  ;; Conversion masked operations
  UNSPEC_VU0_VFTOI0_M      ;; vftoi0 with dest mask
  UNSPEC_VU0_VFTOI4_M      ;; vftoi4 with dest mask
  UNSPEC_VU0_VFTOI12_M     ;; vftoi12 with dest mask
  UNSPEC_VU0_VFTOI15_M     ;; vftoi15 with dest mask
  UNSPEC_VU0_VITOF0_M      ;; vitof0 with dest mask
  UNSPEC_VU0_VITOF4_M      ;; vitof4 with dest mask
  UNSPEC_VU0_VITOF12_M     ;; vitof12 with dest mask
  UNSPEC_VU0_VITOF15_M     ;; vitof15 with dest mask
  ;; Data movement masked operations
  UNSPEC_VU0_VMR32_M       ;; vmr32 with dest mask
])

;; -------------------------------------------------------------------------
;; Vector Move (required for autovectorization)
;; -------------------------------------------------------------------------

;; V4SF move expand - handles both VU0 and MSA
(define_expand "movv4sf"
  [(set (match_operand:V4SF 0)
        (match_operand:V4SF 1))]
  "ISA_HAS_VU0 || ISA_HAS_MSA"
{
  if (mips_legitimize_move (V4SFmode, operands[0], operands[1]))
    DONE;
})

;; VU0/R5900: V4SF move pattern using COP2 registers and memory only.
;; V4SF does not use GPRs - spills go to memory via sqc2/lqc2.
;; Alternatives:
;;   0: C,YG -> vsub $vf0,$vf0 (vector zero constant: vf0={0,0,0,1} so vf0-vf0={0,0,0,0})
;;   1: C,C  -> vmove.xyzw (COP2 to COP2)
;;   2: C,m  -> lqc2 (memory to COP2)
;;   3: m,C  -> sqc2 (COP2 to memory)
(define_insn "*movv4sf_vu0"
  [(set (match_operand:V4SF 0 "nonimmediate_operand" "=C,C,C,m")
        (match_operand:V4SF 1 "move_operand"          "YG,C,m,C"))]
  "ISA_HAS_VU0"
  "@
   vsub.xyzw\t%0,$vf0,$vf0
   vmove.xyzw\t%0,%1
   lqc2\t%0,%1
   sqc2\t%1,%0"
  [(set_attr "type" "fadd,fmove,fpload,fpstore")
   (set_attr "mode" "V4SF")])

;; VU0 vec_extract: Extract scalar from V4SF by going through memory.
;; VU0 has no direct element extraction instruction, so we store to memory
;; and load the individual element.
(define_expand "vec_extractv4sfsf"
  [(match_operand:SF 0 "register_operand")
   (match_operand:V4SF 1 "register_operand")
   (match_operand 2 "const_0_to_3_operand")]
  "ISA_HAS_VU0"
{
  rtx mem = assign_stack_temp (V4SFmode, 16);
  emit_move_insn (mem, operands[1]);
  rtx elem_mem = adjust_address (mem, SFmode, INTVAL (operands[2]) * 4);
  emit_move_insn (operands[0], elem_mem);
  DONE;
})

;; MSA: V4SF move using FP registers
(define_insn "*movv4sf_msa"
  [(set (match_operand:V4SF 0 "nonimmediate_operand" "=f,f,R,*d,*f")
	(match_operand:V4SF 1 "move_operand" "fYGYI,R,f,*f,*d"))]
  "ISA_HAS_MSA"
  { return mips_output_move (operands[0], operands[1]); }
  [(set_attr "type" "simd_move,simd_load,simd_store,simd_copy,simd_insert")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; Vector Arithmetic (standard optab names for autovectorization)
;; These patterns handle both VU0 and MSA for V4SF mode
;; -------------------------------------------------------------------------

;; Vector addition
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for VU0 operands.
(define_insn "addv4sf3"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C,f")
        (plus:V4SF (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv,f")
                   (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv,f")))]
  "ISA_HAS_VU0 || ISA_HAS_MSA"
  "@
   vadd.xyzw\t%0,%u1,%u2
   vadd.xyzw\t%0,%u1,%u2
   vadd.xyzw\t%0,%u1,%u2
   vadd.xyzw\t%0,%u1,%u2
   fadd.w\t%w0,%w1,%w2"
  [(set_attr "type" "fadd,fadd,fadd,fadd,simd_fadd")
   (set_attr "mode" "V4SF")])

;; Vector subtraction
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for VU0 operands.
(define_insn "subv4sf3"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C,f")
        (minus:V4SF (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv,f")
                    (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv,f")))]
  "ISA_HAS_VU0 || ISA_HAS_MSA"
  "@
   vsub.xyzw\t%0,%u1,%u2
   vsub.xyzw\t%0,%u1,%u2
   vsub.xyzw\t%0,%u1,%u2
   vsub.xyzw\t%0,%u1,%u2
   fsub.w\t%w0,%w1,%w2"
  [(set_attr "type" "fadd,fadd,fadd,fadd,simd_fadd")
   (set_attr "mode" "V4SF")])

;; Vector multiplication
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for VU0 operands.
(define_insn "mulv4sf3"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C,f")
        (mult:V4SF (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv,f")
                   (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv,f")))]
  "ISA_HAS_VU0 || ISA_HAS_MSA"
  "@
   vmul.xyzw\t%0,%u1,%u2
   vmul.xyzw\t%0,%u1,%u2
   vmul.xyzw\t%0,%u1,%u2
   vmul.xyzw\t%0,%u1,%u2
   fmul.w\t%w0,%w1,%w2"
  [(set_attr "type" "fmul,fmul,fmul,fmul,simd_fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; Vector Min/Max/Abs (for general vectorization)
;; -------------------------------------------------------------------------

;; Vector absolute value
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "absv4sf2"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (abs:V4SF (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")))]
  "ISA_HAS_VU0"
  "vabs.xyzw\t%0,%u1"
  [(set_attr "type" "fabs")
   (set_attr "mode" "V4SF")])

;; Vector maximum - VU0 only (MSA provides its own smaxv4sf3)
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "*smaxv4sf3_vu0"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (smax:V4SF (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                   (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")))]
  "ISA_HAS_VU0"
  "vmax.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; Vector minimum - VU0 only (VU0 uses vmini, MSA provides its own sminv4sf3)
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "*sminv4sf3_vu0"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (smin:V4SF (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                   (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")))]
  "ISA_HAS_VU0"
  "vmini.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Accumulator Operations (for matrix multiplication)
;; These use the VU0 accumulator register for efficient FMA chains.
;; The accumulator is implicit - the programmer must sequence these correctly.
;; -------------------------------------------------------------------------

;; vmula: ACC = src1 * src2 (stores to implicit accumulator)
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmula"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMULA))]
  "ISA_HAS_VU0"
  "vmula.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmadda: ACC = ACC + src1 * src2 (accumulates to accumulator)
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmadda"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMADDA))]
  "ISA_HAS_VU0"
  "vmadda.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmadd: dest = ACC + src1 * src2 (reads from accumulator)
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmadd"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMADD))]
  "ISA_HAS_VU0"
  "vmadd.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmsuba: ACC = ACC - src1 * src2 (subtracts from accumulator)
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmsuba"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMSUBA))]
  "ISA_HAS_VU0"
  "vmsuba.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmsub: dest = ACC - src1 * src2 (reads from accumulator)
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmsub"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMSUB))]
  "ISA_HAS_VU0"
  "vmsub.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Fused Multiply-Add (for autovectorization)
;; Standard FMA pattern using VU0 accumulator: dest = (a * b) + c
;; Uses vmulaw to load c into ACC (c * $vf0.w = c * 1.0), then vmadd.
;; Note: $vf0.w is always 1.0 on VU0.
;; -------------------------------------------------------------------------

;; Note: This pattern uses ACC internally but doesn't expose it since both
;; vmulaw and vmadd are in the same insn - the ACC usage is self-contained.
(define_insn "*fmav4sf4_vu0"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (fma:V4SF (match_operand:V4SF 1 "register_operand" "C")
                  (match_operand:V4SF 2 "register_operand" "C")
                  (match_operand:V4SF 3 "register_operand" "C")))]
  "ISA_HAS_VU0"
  "vmulaw.xyzw\t$ACC,%3,$vf0\;vmadd.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")
   (set_attr "length" "8")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Operations (scalar * vector)
;; These multiply a vector by a broadcast of one component from another vector.
;; Essential for matrix multiplication: row * column[i]
;; -------------------------------------------------------------------------

;; vmulx: dest = src1 * src2.x (broadcast x component)
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
;; Note: vmulx with $vf0 multiplies by 0.0 (zeroing).
(define_insn "vu0_vmulx"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMULX))]
  "ISA_HAS_VU0"
  "vmulx.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmuly: dest = src1 * src2.y
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
;; Note: vmuly with $vf0 multiplies by 0.0 (zeroing).
(define_insn "vu0_vmuly"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMULY))]
  "ISA_HAS_VU0"
  "vmuly.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmulz: dest = src1 * src2.z
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
;; Note: vmulz with $vf0 multiplies by 0.0 (zeroing).
(define_insn "vu0_vmulz"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMULZ))]
  "ISA_HAS_VU0"
  "vmulz.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmulw: dest = src1 * src2.w
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
;; Note: vmulw with $vf0 multiplies by 1.0 (identity) - useful for move/copy!
(define_insn "vu0_vmulw"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMULW))]
  "ISA_HAS_VU0"
  "vmulw.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Multiply-Accumulate to ACC
;; vmulax/y/z/w: ACC = src1 * src2.x/y/z/w
;; -------------------------------------------------------------------------

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmulax"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMULAX))]
  "ISA_HAS_VU0"
  "vmulax.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmulay"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMULAY))]
  "ISA_HAS_VU0"
  "vmulay.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmulaz"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMULAZ))]
  "ISA_HAS_VU0"
  "vmulaz.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmulaw"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMULAW))]
  "ISA_HAS_VU0"
  "vmulaw.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Multiply-Add with ACC result
;; vmaddax/y/z/w: ACC = ACC + src1 * src2.x/y/z/w
;; -------------------------------------------------------------------------

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmaddax"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMADDAX))]
  "ISA_HAS_VU0"
  "vmaddax.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmadday"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMADDAY))]
  "ISA_HAS_VU0"
  "vmadday.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmaddaz"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMADDAZ))]
  "ISA_HAS_VU0"
  "vmaddaz.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmaddaw"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMADDAW))]
  "ISA_HAS_VU0"
  "vmaddaw.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Multiply-Add with register result
;; vmaddx/y/z/w: dest = ACC + src1 * src2.x/y/z/w
;; -------------------------------------------------------------------------

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmaddx"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMADDX))]
  "ISA_HAS_VU0"
  "vmaddx.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmaddy"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMADDY))]
  "ISA_HAS_VU0"
  "vmaddy.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmaddz"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMADDZ))]
  "ISA_HAS_VU0"
  "vmaddz.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmaddw"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMADDW))]
  "ISA_HAS_VU0"
  "vmaddw.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Multiply-Subtract with ACC result
;; vmsubax/y/z/w: ACC = ACC - src1 * src2.x/y/z/w
;; -------------------------------------------------------------------------

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmsubax"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMSUBAX))]
  "ISA_HAS_VU0"
  "vmsubax.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmsubay"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMSUBAY))]
  "ISA_HAS_VU0"
  "vmsubay.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmsubaz"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMSUBAZ))]
  "ISA_HAS_VU0"
  "vmsubaz.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmsubaw"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMSUBAW))]
  "ISA_HAS_VU0"
  "vmsubaw.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Multiply-Subtract with register result
;; vmsubx/y/z/w: dest = ACC - src1 * src2.x/y/z/w
;; -------------------------------------------------------------------------

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmsubx"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMSUBX))]
  "ISA_HAS_VU0"
  "vmsubx.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmsuby"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMSUBY))]
  "ISA_HAS_VU0"
  "vmsuby.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmsubz"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMSUBZ))]
  "ISA_HAS_VU0"
  "vmsubz.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmsubw"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMSUBW))]
  "ISA_HAS_VU0"
  "vmsubw.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Outer Product (for cross product)
;; vopmula: ACC.xyz = src1.yzx * src2.zxy (cross product multiply)
;; vopmsub: dest.xyz = ACC.xyz - src1.yzx * src2.zxy (cross product complete)
;; -------------------------------------------------------------------------

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vopmula"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VOPMULA))]
  "ISA_HAS_VU0"
  "vopmula.xyz\t$ACC,%u0,%u1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vopmsub"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VOPMSUB))]
  "ISA_HAS_VU0"
  "vopmsub.xyz\t%0,%u1,%u2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Add Operations
;; vaddx/y/z/w: dest = src1 + src2.x/y/z/w (broadcast component add)
;; -------------------------------------------------------------------------

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vaddx"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VADDX))]
  "ISA_HAS_VU0"
  "vaddx.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vaddy"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VADDY))]
  "ISA_HAS_VU0"
  "vaddy.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vaddz"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VADDZ))]
  "ISA_HAS_VU0"
  "vaddz.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vaddw"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VADDW))]
  "ISA_HAS_VU0"
  "vaddw.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vaddw.xyz: Same as vaddw but only affects xyz components, leaves w unchanged.
;; Used for scalar broadcast to avoid vf0.w=1 corruption on w component.
(define_insn "vu0_vaddw_xyz"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")
                      (match_operand:V4SF 2 "register_operand" "0,0")]
                     UNSPEC_VU0_VADDW_XYZ))]
  "ISA_HAS_VU0"
  "vaddw.xyz\t%0,%u1,%2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Subtract Operations
;; vsubx/y/z/w: dest = src1 - src2.x/y/z/w (broadcast component subtract)
;; -------------------------------------------------------------------------

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vsubx"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VSUBX))]
  "ISA_HAS_VU0"
  "vsubx.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vsuby"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VSUBY))]
  "ISA_HAS_VU0"
  "vsuby.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vsubz"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VSUBZ))]
  "ISA_HAS_VU0"
  "vsubz.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vsubw"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VSUBW))]
  "ISA_HAS_VU0"
  "vsubw.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Accumulator Add/Subtract
;; vadda: ACC = src1 + src2 (vector add to accumulator)
;; vsuba: ACC = src1 - src2 (vector subtract to accumulator)
;; -------------------------------------------------------------------------

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vadda"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VADDA))]
  "ISA_HAS_VU0"
  "vadda.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vsuba"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VSUBA))]
  "ISA_HAS_VU0"
  "vsuba.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Add to Accumulator
;; vaddax/y/z/w: ACC = src1 + src2.x/y/z/w
;; -------------------------------------------------------------------------

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vaddax"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VADDAX))]
  "ISA_HAS_VU0"
  "vaddax.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vadday"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VADDAY))]
  "ISA_HAS_VU0"
  "vadday.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vaddaz"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VADDAZ))]
  "ISA_HAS_VU0"
  "vaddaz.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vaddaw"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VADDAW))]
  "ISA_HAS_VU0"
  "vaddaw.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Subtract from Accumulator
;; vsubax/y/z/w: ACC = src1 - src2.x/y/z/w
;; -------------------------------------------------------------------------

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vsubax"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VSUBAX))]
  "ISA_HAS_VU0"
  "vsubax.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vsubay"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VSUBAY))]
  "ISA_HAS_VU0"
  "vsubay.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vsubaz"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VSUBAZ))]
  "ISA_HAS_VU0"
  "vsubaz.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vsubaw"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VSUBAW))]
  "ISA_HAS_VU0"
  "vsubaw.xyzw\t$ACC,%u0,%u1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Float to Fixed-Point Conversions
;; vftoi0:  Convert float to 32-bit signed integer (no fractional bits)
;; vftoi4:  Convert float to 28.4 fixed-point (4 fractional bits)
;; vftoi12: Convert float to 20.12 fixed-point (12 fractional bits)
;; vftoi15: Convert float to 17.15 fixed-point (15 fractional bits)
;; -------------------------------------------------------------------------

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vftoi0"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")]
                     UNSPEC_VU0_VFTOI0))]
  "ISA_HAS_VU0"
  "vftoi0.xyzw\t%0,%u1"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vftoi4"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")]
                     UNSPEC_VU0_VFTOI4))]
  "ISA_HAS_VU0"
  "vftoi4.xyzw\t%0,%u1"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vftoi12"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")]
                     UNSPEC_VU0_VFTOI12))]
  "ISA_HAS_VU0"
  "vftoi12.xyzw\t%0,%u1"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vftoi15"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")]
                     UNSPEC_VU0_VFTOI15))]
  "ISA_HAS_VU0"
  "vftoi15.xyzw\t%0,%u1"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Fixed-Point to Float Conversions
;; vitof0:  Convert 32-bit signed integer to float (no fractional bits)
;; vitof4:  Convert 28.4 fixed-point to float (4 fractional bits)
;; vitof12: Convert 20.12 fixed-point to float (12 fractional bits)
;; vitof15: Convert 17.15 fixed-point to float (15 fractional bits)
;; -------------------------------------------------------------------------

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vitof0"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")]
                     UNSPEC_VU0_VITOF0))]
  "ISA_HAS_VU0"
  "vitof0.xyzw\t%0,%u1"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vitof4"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")]
                     UNSPEC_VU0_VITOF4))]
  "ISA_HAS_VU0"
  "vitof4.xyzw\t%0,%u1"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vitof12"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")]
                     UNSPEC_VU0_VITOF12))]
  "ISA_HAS_VU0"
  "vitof12.xyzw\t%0,%u1"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vitof15"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")]
                     UNSPEC_VU0_VITOF15))]
  "ISA_HAS_VU0"
  "vitof15.xyzw\t%0,%u1"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Standard Type Conversion Patterns
;; These patterns enable autovectorization of float<->int conversions.
;; V4SF lives in COP2, V4SI lives in GPR, transfers via qmtc2/qmfc2.
;; -------------------------------------------------------------------------

;; fix_truncv4sfv4si2: V4SF (COP2) -> V4SI (GPR)
;; Uses vftoi0 to convert floats to integers in COP2, then qmfc2 to GPR.
;; Two instructions that must stay together - use can_delay=no.
(define_insn "fix_truncv4sfv4si2"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
        (fix:V4SI (match_operand:V4SF 1 "register_operand" "C")))
   (clobber (match_scratch:V4SF 2 "=&C"))]
  "ISA_HAS_VU0"
  "vftoi0.xyzw\t%2,%u1\;qmfc2\t%0,%2"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SI")
   (set_attr "length" "8")
   (set_attr "can_delay" "no")])

;; floatv4siv4sf2: V4SI (GPR) -> V4SF (COP2)
;; Uses qmtc2 to transfer integers to COP2, then vitof0 to convert to floats.
;; Two instructions that must stay together - use can_delay=no.
(define_insn "floatv4siv4sf2"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (float:V4SF (match_operand:V4SI 1 "register_operand" "d")))
   (clobber (match_scratch:V4SF 2 "=&C"))]
  "ISA_HAS_VU0"
  "qmtc2\t%1,%2\;vitof0.xyzw\t%0,%2"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")
   (set_attr "length" "8")
   (set_attr "can_delay" "no")])

;; -------------------------------------------------------------------------
;; VU0 Vector Rotate
;; vmr32: Rotate vector elements by 32 bits (1 element)
;;        dest.xyzw = src.yzwx
;; -------------------------------------------------------------------------

;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vmr32"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")]
                     UNSPEC_VU0_VMR32))]
  "ISA_HAS_VU0"
  "vmr32.xyzw\t%0,%u1"
  [(set_attr "type" "fmove")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Explicit Intrinsic Patterns
;; Named patterns for builtins that expose automatic operations explicitly.
;; -------------------------------------------------------------------------

;; vu0_vmax: Explicit max intrinsic
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmax"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMAX))]
  "ISA_HAS_VU0"
  "vmax.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vu0_vmini: Explicit min intrinsic
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vmini"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C,C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                      (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")]
                     UNSPEC_VU0_VMINI))]
  "ISA_HAS_VU0"
  "vmini.xyzw\t%0,%u1,%u2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vu0_vmove: Explicit move intrinsic
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vmove"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")]
                     UNSPEC_VU0_VMOVE))]
  "ISA_HAS_VU0"
  "vmove.xyzw\t%0,%u1"
  [(set_attr "type" "fmove")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Division and Square Root Operations (Q register)
;; These write results to the Q register asynchronously.
;; Component selector: 0=x, 1=y, 2=z, 3=w
;; -------------------------------------------------------------------------

;; vdiv: Q = fs.bc / ft.bc
;; Uses unspec_volatile to prevent elimination (starts async division)
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vdiv"
  [(unspec_volatile [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                     (match_operand:SI 1 "const_int_operand" "n,n,n,n")
                     (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")
                     (match_operand:SI 3 "const_int_operand" "n,n,n,n")]
                    UNSPEC_VU0_VDIV)
   (clobber (reg:SF VU0_Q_REGNUM))]
  "ISA_HAS_VU0"
  {
    static const char *const bc[] = { "x", "y", "z", "w" };
    static char buf[32];
    sprintf (buf, "vdiv\t$Q,%%u0%s,%%u2%s",
             bc[INTVAL (operands[1]) & 3],
             bc[INTVAL (operands[3]) & 3]);
    return buf;
  }
  [(set_attr "type" "fdiv")
   (set_attr "mode" "SF")])

;; vsqrt: Q = sqrt(ft.bc)
;; Uses unspec_volatile to prevent elimination (starts async sqrt)
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vsqrt"
  [(unspec_volatile [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv")
                     (match_operand:SI 1 "const_int_operand" "n,n")]
                    UNSPEC_VU0_VSQRT)
   (clobber (reg:SF VU0_Q_REGNUM))]
  "ISA_HAS_VU0"
  {
    static const char *const bc[] = { "x", "y", "z", "w" };
    static char buf[32];
    sprintf (buf, "vsqrt\t$Q,%%u0%s", bc[INTVAL (operands[1]) & 3]);
    return buf;
  }
  [(set_attr "type" "fsqrt")
   (set_attr "mode" "SF")])

;; vrsqrt: Q = fs.bc / sqrt(ft.bc)
;; Uses unspec_volatile to prevent elimination (starts async division)
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0} for either operand.
(define_insn "vu0_vrsqrt"
  [(unspec_volatile [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv,C,Yv")
                     (match_operand:SI 1 "const_int_operand" "n,n,n,n")
                     (match_operand:V4SF 2 "vu0_reg_or_vf0_operand" "C,C,Yv,Yv")
                     (match_operand:SI 3 "const_int_operand" "n,n,n,n")]
                    UNSPEC_VU0_VRSQRT)
   (clobber (reg:SF VU0_Q_REGNUM))]
  "ISA_HAS_VU0"
  {
    static const char *const bc[] = { "x", "y", "z", "w" };
    static char buf[32];
    sprintf (buf, "vrsqrt\t$Q,%%u0%s,%%u2%s",
             bc[INTVAL (operands[1]) & 3],
             bc[INTVAL (operands[3]) & 3]);
    return buf;
  }
  [(set_attr "type" "fdiv")
   (set_attr "mode" "SF")])

;; vwaitq: Wait for Q register to be ready
(define_insn "vu0_vwaitq"
  [(unspec_volatile [(reg:SF VU0_Q_REGNUM)] UNSPEC_VU0_WAITQ)]
  "ISA_HAS_VU0"
  "vwaitq"
  [(set_attr "type" "nop")
   (set_attr "mode" "none")])

;; -------------------------------------------------------------------------
;; VU0 Q Broadcast Operations
;; These read the Q register and broadcast it across all vector components.
;; -------------------------------------------------------------------------

;; vaddq: dest = src + Q
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vaddq"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")
                      (reg:SF VU0_Q_REGNUM)]
                     UNSPEC_VU0_VADDQ))]
  "ISA_HAS_VU0"
  "vaddq.xyzw\t%0,%u1,$Q"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubq: dest = src - Q
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vsubq"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")
                      (reg:SF VU0_Q_REGNUM)]
                     UNSPEC_VU0_VSUBQ))]
  "ISA_HAS_VU0"
  "vsubq.xyzw\t%0,%u1,$Q"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmulq: dest = src * Q
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vmulq"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")
                      (reg:SF VU0_Q_REGNUM)]
                     UNSPEC_VU0_VMULQ))]
  "ISA_HAS_VU0"
  "vmulq.xyzw\t%0,%u1,$Q"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vaddaQ: ACC = src + Q
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vaddaq"
  [(unspec_volatile [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv")
                     (reg:SF VU0_Q_REGNUM)]
                    UNSPEC_VU0_VADDQA)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vaddaq.xyzw\t$ACC,%u0,$Q"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubaQ: ACC = src - Q
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vsubaq"
  [(unspec_volatile [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv")
                     (reg:SF VU0_Q_REGNUM)]
                    UNSPEC_VU0_VSUBQA)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vsubaq.xyzw\t$ACC,%u0,$Q"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmulaQ: ACC = src * Q
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vmulaq"
  [(unspec_volatile [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv")
                     (reg:SF VU0_Q_REGNUM)]
                    UNSPEC_VU0_VMULQA)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vmulaq.xyzw\t$ACC,%u0,$Q"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaddq: dest = ACC + src * Q
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vmaddq"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")
                      (reg:SF VU0_Q_REGNUM)]
                     UNSPEC_VU0_VMADDQ))]
  "ISA_HAS_VU0"
  "vmaddq.xyzw\t%0,%u1,$Q"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmsubq: dest = ACC - src * Q
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vmsubq"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")
                      (reg:SF VU0_Q_REGNUM)]
                     UNSPEC_VU0_VMSUBQ))]
  "ISA_HAS_VU0"
  "vmsubq.xyzw\t%0,%u1,$Q"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaddaQ: ACC = ACC + src * Q
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vmaddaq"
  [(unspec_volatile [(reg:V4SF VU0_ACC_REGNUM)
                     (match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv")
                     (reg:SF VU0_Q_REGNUM)]
                    UNSPEC_VU0_VMADDQA)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vmaddaq.xyzw\t$ACC,%u0,$Q"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmsubaQ: ACC = ACC - src * Q
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vmsubaq"
  [(unspec_volatile [(reg:V4SF VU0_ACC_REGNUM)
                     (match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv")
                     (reg:SF VU0_Q_REGNUM)]
                    UNSPEC_VU0_VMSUBQA)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vmsubaq.xyzw\t$ACC,%u0,$Q"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaxQ: dest = max(src, Q)
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vmaxq"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")
                      (reg:SF VU0_Q_REGNUM)]
                     UNSPEC_VU0_VMAXQ))]
  "ISA_HAS_VU0"
  "vmaxq.xyzw\t%0,%u1,$Q"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vminiQ: dest = min(src, Q)
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vminiq"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")
                      (reg:SF VU0_Q_REGNUM)]
                     UNSPEC_VU0_VMINIQ))]
  "ISA_HAS_VU0"
  "vminiq.xyzw\t%0,%u1,$Q"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 I Register Load (CTC2 to I)
;; Loads the I register with an immediate float value via CTC2.
;; -------------------------------------------------------------------------

;; ctc2 to I register: set I from GP register bits
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_ctc2_i"
  [(unspec_volatile [(match_operand:SI 0 "register_operand" "d")]
                    UNSPEC_VU0_CTC2_I)
   (clobber (reg:SF VU0_I_REGNUM))]
  "ISA_HAS_VU0"
  "ctc2\t%0,$21"
  [(set_attr "type" "mtc")
   (set_attr "mode" "SI")])

;; -------------------------------------------------------------------------
;; VU0 I Broadcast Operations
;; These read the I register and broadcast it across all vector components.
;; -------------------------------------------------------------------------

;; vaddi: dest = src + I
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vaddi"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")
                      (reg:SF VU0_I_REGNUM)]
                     UNSPEC_VU0_VADDI))]
  "ISA_HAS_VU0"
  "vaddi.xyzw\t%0,%u1,$I"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubi: dest = src - I
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vsubi"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")
                      (reg:SF VU0_I_REGNUM)]
                     UNSPEC_VU0_VSUBI))]
  "ISA_HAS_VU0"
  "vsubi.xyzw\t%0,%u1,$I"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmuli: dest = src * I
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vmuli"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")
                      (reg:SF VU0_I_REGNUM)]
                     UNSPEC_VU0_VMULI))]
  "ISA_HAS_VU0"
  "vmuli.xyzw\t%0,%u1,$I"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vaddaI: ACC = src + I
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vaddai"
  [(unspec_volatile [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv")
                     (reg:SF VU0_I_REGNUM)]
                    UNSPEC_VU0_VADDAI)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vaddai.xyzw\t$ACC,%u0,$I"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubaI: ACC = src - I
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vsubai"
  [(unspec_volatile [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv")
                     (reg:SF VU0_I_REGNUM)]
                    UNSPEC_VU0_VSUBAI)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vsubai.xyzw\t$ACC,%u0,$I"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmulaI: ACC = src * I
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vmulai"
  [(unspec_volatile [(match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv")
                     (reg:SF VU0_I_REGNUM)]
                    UNSPEC_VU0_VMULAI)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vmulai.xyzw\t$ACC,%u0,$I"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaddi: dest = ACC + src * I
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vmaddi"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")
                      (reg:SF VU0_I_REGNUM)]
                     UNSPEC_VU0_VMADDI))]
  "ISA_HAS_VU0"
  "vmaddi.xyzw\t%0,%u1,$I"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmsubi: dest = ACC - src * I
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vmsubi"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")
                      (reg:SF VU0_I_REGNUM)]
                     UNSPEC_VU0_VMSUBI))]
  "ISA_HAS_VU0"
  "vmsubi.xyzw\t%0,%u1,$I"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaddaI: ACC = ACC + src * I
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vmaddai"
  [(unspec_volatile [(reg:V4SF VU0_ACC_REGNUM)
                     (match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv")
                     (reg:SF VU0_I_REGNUM)]
                    UNSPEC_VU0_VMADDAI)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vmaddai.xyzw\t$ACC,%u0,$I"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmsubaI: ACC = ACC - src * I
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vmsubai"
  [(unspec_volatile [(reg:V4SF VU0_ACC_REGNUM)
                     (match_operand:V4SF 0 "vu0_reg_or_vf0_operand" "C,Yv")
                     (reg:SF VU0_I_REGNUM)]
                    UNSPEC_VU0_VMSUBAI)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vmsubai.xyzw\t$ACC,%u0,$I"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaxI: dest = max(src, I)
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vmaxi"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")
                      (reg:SF VU0_I_REGNUM)]
                     UNSPEC_VU0_VMAXI))]
  "ISA_HAS_VU0"
  "vmaxi.xyzw\t%0,%u1,$I"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vminiI: dest = min(src, I)
;; Supports VF0 constant {0.0, 0.0, 0.0, 1.0}.
(define_insn "vu0_vminii"
  [(set (match_operand:V4SF 0 "register_operand" "=C,C")
        (unspec:V4SF [(match_operand:V4SF 1 "vu0_reg_or_vf0_operand" "C,Yv")
                      (reg:SF VU0_I_REGNUM)]
                     UNSPEC_VU0_VMINII))]
  "ISA_HAS_VU0"
  "vminii.xyzw\t%0,%u1,$I"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 CTC2/CFC2 Generic Control Register Transfer
;; For accessing all VU0 control registers by ID.
;; -------------------------------------------------------------------------

;; cfc2: Read VU0 control register to GP register
(define_insn "vu0_cfc2"
  [(set (match_operand:SI 0 "register_operand" "=d")
        (unspec_volatile:SI [(match_operand:SI 1 "const_int_operand" "n")]
                            UNSPEC_VU0_CFC2))]
  "ISA_HAS_VU0"
  "cfc2\t%0,$%1"
  [(set_attr "type" "mfc")
   (set_attr "mode" "SI")])

;; ctc2: Write GP register to VU0 control register
(define_insn "vu0_ctc2"
  [(unspec_volatile [(match_operand:SI 0 "register_operand" "d")
                     (match_operand:SI 1 "const_int_operand" "n")]
                    UNSPEC_VU0_CTC2)]
  "ISA_HAS_VU0"
  "ctc2\t%0,$%1"
  [(set_attr "type" "mtc")
   (set_attr "mode" "SI")])

;; -------------------------------------------------------------------------
;; VU0 Scalar-Vector Autovectorization via VMULx/VADDx/VSUBx
;; These patterns enable automatic generation of broadcast component ops
;; for expressions like: vector * scalar, vector + scalar, vector - scalar
;; The scalar is loaded into a VU0 register's x component via qmtc2,
;; then VMULx/VADDx/VSUBx broadcast the x component across all lanes.
;; This fits GCC's register model better than the I register approach.
;; -------------------------------------------------------------------------

;; Helper: Move float bits from FP register to GP register
(define_insn "mfc1_vu0"
  [(set (match_operand:SI 0 "register_operand" "=d")
        (unspec:SI [(match_operand:SF 1 "register_operand" "f")]
                   UNSPEC_MFC1_VU0))]
  "ISA_HAS_VU0"
  "mfc1\t%0,%1"
  [(set_attr "type" "mfc")
   (set_attr "mode" "SI")])

;; Helper: Transfer scalar (SI bits) to VU0 register x component via qmtc2
;; The scalar value ends up in the x component; y/z/w are undefined but unused.
(define_insn "vu0_qmtc2_scalar"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:SI 1 "register_operand" "d")]
                     UNSPEC_VU0_QMTC2_SCALAR))]
  "ISA_HAS_VU0"
  "qmtc2\t%1,%0"
  [(set_attr "type" "mtc")
   (set_attr "mode" "V4SF")])

;; vec_duplicate fallback: Creates a broadcast vector when not combined
;; with a vector operation. Uses vmulx + vaddw.xyz to broadcast scalar to all lanes.
;; NOTE: We cannot use "vaddx $dest,$vf0,$src" because vf0 = (0,0,0,1) on R5900,
;; which would corrupt the w component with +1. Instead we use:
;;   1. qmtc2 $dest, $gpr     - load scalar to dest.x (garbage in yzw)
;;   2. vmulx.xyzw $dest,$vf0,$dest - dest = vf0 * dest.x = (0,0,0,X)
;;   3. vaddw.xyz $dest,$vf0,$dest  - dest.xyz = 0 + X, w unchanged = (X,X,X,X)
(define_insn_and_split "vec_duplicatev4sf"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (vec_duplicate:V4SF (match_operand:SF 1 "register_operand" "f")))
   (clobber (match_scratch:SI 2 "=&d"))]
  "ISA_HAS_VU0"
  "#"
  "&& reload_completed"
  [(const_int 0)]
{
  rtx vf0 = gen_rtx_REG (V4SFmode, COP2_REG_FIRST);
  /* Load scalar into VU0 register x component */
  emit_insn (gen_mfc1_vu0 (operands[2], operands[1]));
  emit_insn (gen_vu0_qmtc2_scalar (operands[0], operands[2]));
  /* vmulx: dest = vf0 * dest.x = (0,0,0,1) * X = (0,0,0,X) */
  emit_insn (gen_vu0_vmulx (operands[0], vf0, operands[0]));
  /* vaddw.xyz: dest.xyz = vf0.xyz + dest.w = 0 + X, w unchanged */
  emit_insn (gen_vu0_vaddw_xyz (operands[0], vf0, operands[0]));
  DONE;
}
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")
   (set_attr "length" "16")])

;; -------------------------------------------------------------------------
;; VMULx: Vector * Scalar autovectorization
;; -------------------------------------------------------------------------

;; Vector * Scalar (scalar duplicated first)
(define_insn_and_split "*mulv4sf3_scalar"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (mult:V4SF (vec_duplicate:V4SF (match_operand:SF 1 "register_operand" "f"))
                   (match_operand:V4SF 2 "register_operand" "C")))
   (clobber (match_scratch:V4SF 3 "=&C"))
   (clobber (match_scratch:SI 4 "=&d"))]
  "ISA_HAS_VU0"
  "#"
  "&& reload_completed"
  [(const_int 0)]
{
  emit_insn (gen_mfc1_vu0 (operands[4], operands[1]));
  emit_insn (gen_vu0_qmtc2_scalar (operands[3], operands[4]));
  emit_insn (gen_vu0_vmulx (operands[0], operands[2], operands[3]));
  DONE;
}
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")
   (set_attr "length" "12")])

;; Scalar * Vector (commutative variant)
(define_insn_and_split "*mulv4sf3_scalar_alt"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (mult:V4SF (match_operand:V4SF 1 "register_operand" "C")
                   (vec_duplicate:V4SF (match_operand:SF 2 "register_operand" "f"))))
   (clobber (match_scratch:V4SF 3 "=&C"))
   (clobber (match_scratch:SI 4 "=&d"))]
  "ISA_HAS_VU0"
  "#"
  "&& reload_completed"
  [(const_int 0)]
{
  emit_insn (gen_mfc1_vu0 (operands[4], operands[2]));
  emit_insn (gen_vu0_qmtc2_scalar (operands[3], operands[4]));
  emit_insn (gen_vu0_vmulx (operands[0], operands[1], operands[3]));
  DONE;
}
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")
   (set_attr "length" "12")])

;; -------------------------------------------------------------------------
;; VADDx: Vector + Scalar autovectorization
;; -------------------------------------------------------------------------

;; Scalar + Vector
(define_insn_and_split "*addv4sf3_scalar"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (plus:V4SF (vec_duplicate:V4SF (match_operand:SF 1 "register_operand" "f"))
                   (match_operand:V4SF 2 "register_operand" "C")))
   (clobber (match_scratch:V4SF 3 "=&C"))
   (clobber (match_scratch:SI 4 "=&d"))]
  "ISA_HAS_VU0"
  "#"
  "&& reload_completed"
  [(const_int 0)]
{
  emit_insn (gen_mfc1_vu0 (operands[4], operands[1]));
  emit_insn (gen_vu0_qmtc2_scalar (operands[3], operands[4]));
  emit_insn (gen_vu0_vaddx (operands[0], operands[2], operands[3]));
  DONE;
}
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")
   (set_attr "length" "12")])

;; Vector + Scalar (commutative variant)
(define_insn_and_split "*addv4sf3_scalar_alt"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (plus:V4SF (match_operand:V4SF 1 "register_operand" "C")
                   (vec_duplicate:V4SF (match_operand:SF 2 "register_operand" "f"))))
   (clobber (match_scratch:V4SF 3 "=&C"))
   (clobber (match_scratch:SI 4 "=&d"))]
  "ISA_HAS_VU0"
  "#"
  "&& reload_completed"
  [(const_int 0)]
{
  emit_insn (gen_mfc1_vu0 (operands[4], operands[2]));
  emit_insn (gen_vu0_qmtc2_scalar (operands[3], operands[4]));
  emit_insn (gen_vu0_vaddx (operands[0], operands[1], operands[3]));
  DONE;
}
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")
   (set_attr "length" "12")])

;; -------------------------------------------------------------------------
;; VSUBx: Vector - Scalar autovectorization (NOT commutative)
;; -------------------------------------------------------------------------

;; Vector - Scalar
(define_insn_and_split "*subv4sf3_scalar"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (minus:V4SF (match_operand:V4SF 1 "register_operand" "C")
                    (vec_duplicate:V4SF (match_operand:SF 2 "register_operand" "f"))))
   (clobber (match_scratch:V4SF 3 "=&C"))
   (clobber (match_scratch:SI 4 "=&d"))]
  "ISA_HAS_VU0"
  "#"
  "&& reload_completed"
  [(const_int 0)]
{
  emit_insn (gen_mfc1_vu0 (operands[4], operands[2]));
  emit_insn (gen_vu0_qmtc2_scalar (operands[3], operands[4]));
  emit_insn (gen_vu0_vsubx (operands[0], operands[1], operands[3]));
  DONE;
}
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")
   (set_attr "length" "12")])

;; -------------------------------------------------------------------------
;; VU0 Masked Operations (arbitrary dest field)
;; These intrinsics allow specifying which components (xyzw) to operate on.
;; The mask is a 4-bit immediate: x=8, y=4, z=2, w=1
;; -------------------------------------------------------------------------

;; vadd with dest mask
(define_insn "vu0_vadd_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VADD_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vadd.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsub with dest mask
(define_insn "vu0_vsub_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VSUB_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vsub.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmul with dest mask
(define_insn "vu0_vmul_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VMUL_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmul.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vabs with dest mask
(define_insn "vu0_vabs_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VABS_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vabs.%s\t%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fabs")
   (set_attr "mode" "V4SF")])

;; vmax with dest mask
(define_insn "vu0_vmax_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VMAX_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmax.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmini with dest mask
(define_insn "vu0_vmini_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VMINI_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmini.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmove with dest mask
(define_insn "vu0_vmove_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMOVE_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmove.%s\t%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "simd_move")
   (set_attr "mode" "V4SF")])

;; vmadd with dest mask (ACC read)
(define_insn "vu0_vmadd_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")
                      (reg:V4SF VU0_ACC_REGNUM)]
                     UNSPEC_VU0_VMADD_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmadd.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmsub with dest mask (ACC read)
(define_insn "vu0_vmsub_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")
                      (reg:V4SF VU0_ACC_REGNUM)]
                     UNSPEC_VU0_VMSUB_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmsub.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmula with dest mask (ACC write)
(define_insn "vu0_vmula_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMULA_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmula.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmadda with dest mask (ACC read/write)
(define_insn "vu0_vmadda_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")
                      (reg:V4SF VU0_ACC_REGNUM)]
                     UNSPEC_VU0_VMADDA_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmadda.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmsuba with dest mask (ACC read/write)
(define_insn "vu0_vmsuba_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")
                      (reg:V4SF VU0_ACC_REGNUM)]
                     UNSPEC_VU0_VMSUBA_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmsuba.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vadda with dest mask (ACC write)
(define_insn "vu0_vadda_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VADDA_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vadda.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsuba with dest mask (ACC write)
(define_insn "vu0_vsuba_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VSUBA_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vsuba.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Masked Operations
;; vaddx/y/z/w etc with arbitrary dest mask
;; -------------------------------------------------------------------------

;; vaddx with dest mask
(define_insn "vu0_vaddx_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VADDX_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vaddx.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vaddy with dest mask
(define_insn "vu0_vaddy_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VADDY_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vaddy.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vaddz with dest mask
(define_insn "vu0_vaddz_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VADDZ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vaddz.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vaddw with dest mask
(define_insn "vu0_vaddw_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VADDW_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vaddw.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubx with dest mask
(define_insn "vu0_vsubx_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VSUBX_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vsubx.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsuby with dest mask
(define_insn "vu0_vsuby_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VSUBY_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vsuby.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubz with dest mask
(define_insn "vu0_vsubz_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VSUBZ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vsubz.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubw with dest mask
(define_insn "vu0_vsubw_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VSUBW_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vsubw.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmulx with dest mask
(define_insn "vu0_vmulx_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VMULX_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmulx.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmuly with dest mask
(define_insn "vu0_vmuly_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VMULY_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmuly.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmulz with dest mask
(define_insn "vu0_vmulz_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VMULZ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmulz.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmulw with dest mask
(define_insn "vu0_vmulw_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VMULW_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmulw.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaddx with dest mask (reads ACC)
(define_insn "vu0_vmaddx_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VMADDX_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmaddx.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmaddy with dest mask (reads ACC)
(define_insn "vu0_vmaddy_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VMADDY_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmaddy.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmaddz with dest mask (reads ACC)
(define_insn "vu0_vmaddz_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VMADDZ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmaddz.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmaddw with dest mask (reads ACC)
(define_insn "vu0_vmaddw_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VMADDW_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmaddw.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmsubx with dest mask (reads ACC)
(define_insn "vu0_vmsubx_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VMSUBX_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmsubx.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmsuby with dest mask (reads ACC)
(define_insn "vu0_vmsuby_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VMSUBY_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmsuby.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmsubz with dest mask (reads ACC)
(define_insn "vu0_vmsubz_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VMSUBZ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmsubz.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmsubw with dest mask (reads ACC)
(define_insn "vu0_vmsubw_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")
                      (match_operand:SI 3 "const_int_operand" "n")]
                     UNSPEC_VU0_VMSUBW_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[3]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmsubw.%s\t%%0,%%1,%%2", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast ACC Masked Operations
;; vmulax/y/z/w etc with arbitrary dest mask (writes ACC)
;; -------------------------------------------------------------------------

;; vmulax with dest mask (writes ACC)
(define_insn "vu0_vmulax_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMULAX_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmulax.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmulay with dest mask (writes ACC)
(define_insn "vu0_vmulay_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMULAY_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmulay.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmulaz with dest mask (writes ACC)
(define_insn "vu0_vmulaz_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMULAZ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmulaz.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmulaw with dest mask (writes ACC)
(define_insn "vu0_vmulaw_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMULAW_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmulaw.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaddax with dest mask (reads/writes ACC)
(define_insn "vu0_vmaddax_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMADDAX_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmaddax.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmadday with dest mask (reads/writes ACC)
(define_insn "vu0_vmadday_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMADDAY_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmadday.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmaddaz with dest mask (reads/writes ACC)
(define_insn "vu0_vmaddaz_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMADDAZ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmaddaz.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmaddaw with dest mask (reads/writes ACC)
(define_insn "vu0_vmaddaw_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMADDAW_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmaddaw.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmsubax with dest mask (reads/writes ACC)
(define_insn "vu0_vmsubax_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMSUBAX_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmsubax.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmsubay with dest mask (reads/writes ACC)
(define_insn "vu0_vmsubay_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMSUBAY_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmsubay.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmsubaz with dest mask (reads/writes ACC)
(define_insn "vu0_vmsubaz_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMSUBAZ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmsubaz.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmsubaw with dest mask (reads/writes ACC)
(define_insn "vu0_vmsubaw_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMSUBAW_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmsubaw.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vaddax with dest mask (writes ACC)
(define_insn "vu0_vaddax_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VADDAX_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vaddax.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vadday with dest mask (writes ACC)
(define_insn "vu0_vadday_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VADDAY_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vadday.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vaddaz with dest mask (writes ACC)
(define_insn "vu0_vaddaz_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VADDAZ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vaddaz.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vaddaw with dest mask (writes ACC)
(define_insn "vu0_vaddaw_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VADDAW_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vaddaw.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubax with dest mask (writes ACC)
(define_insn "vu0_vsubax_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VSUBAX_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vsubax.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubay with dest mask (writes ACC)
(define_insn "vu0_vsubay_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VSUBAY_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vsubay.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubaz with dest mask (writes ACC)
(define_insn "vu0_vsubaz_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VSUBAZ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vsubaz.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubaw with dest mask (writes ACC)
(define_insn "vu0_vsubaw_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VSUBAW_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vsubaw.%s\t$ACC,%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Q Register Masked Operations
;; vaddq etc with arbitrary dest mask (reads Q register)
;; -------------------------------------------------------------------------

;; vaddq with dest mask
(define_insn "vu0_vaddq_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VADDQ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vaddq.%s\t%%0,%%1,$Q", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubq with dest mask
(define_insn "vu0_vsubq_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VSUBQ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vsubq.%s\t%%0,%%1,$Q", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmulq with dest mask
(define_insn "vu0_vmulq_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMULQ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmulq.%s\t%%0,%%1,$Q", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaxq with dest mask (note: hardware may not support this)
(define_insn "vu0_vmaxq_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMAXQ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmaxq.%s\t%%0,%%1,$Q", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vminiq with dest mask (note: hardware may not support this)
(define_insn "vu0_vminiq_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMINIQ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vminiq.%s\t%%0,%%1,$Q", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmaddq with dest mask (reads ACC)
(define_insn "vu0_vmaddq_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMADDQ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmaddq.%s\t%%0,%%1,$Q", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmsubq with dest mask (reads ACC)
(define_insn "vu0_vmsubq_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMSUBQ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmsubq.%s\t%%0,%%1,$Q", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vaddaq with dest mask (writes ACC)
(define_insn "vu0_vaddaq_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)
                      (match_operand:SI 1 "const_int_operand" "n")]
                     UNSPEC_VU0_VADDAQ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[1]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vaddaq.%s\t$ACC,%%0,$Q", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubaq with dest mask (writes ACC)
(define_insn "vu0_vsubaq_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)
                      (match_operand:SI 1 "const_int_operand" "n")]
                     UNSPEC_VU0_VSUBAQ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[1]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vsubaq.%s\t$ACC,%%0,$Q", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmulaq with dest mask (writes ACC)
(define_insn "vu0_vmulaq_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)
                      (match_operand:SI 1 "const_int_operand" "n")]
                     UNSPEC_VU0_VMULAQ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[1]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmulaq.%s\t$ACC,%%0,$Q", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaddaq with dest mask (reads/writes ACC)
(define_insn "vu0_vmaddaq_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)
                      (match_operand:SI 1 "const_int_operand" "n")]
                     UNSPEC_VU0_VMADDAQ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[1]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmaddaq.%s\t$ACC,%%0,$Q", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmsubaq with dest mask (reads/writes ACC)
(define_insn "vu0_vmsubaq_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)
                      (match_operand:SI 1 "const_int_operand" "n")]
                     UNSPEC_VU0_VMSUBAQ_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[1]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmsubaq.%s\t$ACC,%%0,$Q", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 I Register Masked Operations
;; vaddi etc with arbitrary dest mask (reads I register)
;; -------------------------------------------------------------------------

;; vaddi with dest mask
(define_insn "vu0_vaddi_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VADDI_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vaddi.%s\t%%0,%%1,$I", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubi with dest mask
(define_insn "vu0_vsubi_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VSUBI_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vsubi.%s\t%%0,%%1,$I", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmuli with dest mask
(define_insn "vu0_vmuli_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMULI_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmuli.%s\t%%0,%%1,$I", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaxi with dest mask
(define_insn "vu0_vmaxi_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMAXI_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmaxi.%s\t%%0,%%1,$I", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vminii with dest mask
(define_insn "vu0_vminii_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMINII_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vminii.%s\t%%0,%%1,$I", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmaddi with dest mask (reads ACC)
(define_insn "vu0_vmaddi_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMADDI_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmaddi.%s\t%%0,%%1,$I", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmsubi with dest mask (reads ACC)
(define_insn "vu0_vmsubi_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMSUBI_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmsubi.%s\t%%0,%%1,$I", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vaddai with dest mask (writes ACC)
(define_insn "vu0_vaddai_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)
                      (match_operand:SI 1 "const_int_operand" "n")]
                     UNSPEC_VU0_VADDAI_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[1]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vaddai.%s\t$ACC,%%0,$I", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubai with dest mask (writes ACC)
(define_insn "vu0_vsubai_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)
                      (match_operand:SI 1 "const_int_operand" "n")]
                     UNSPEC_VU0_VSUBAI_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[1]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vsubai.%s\t$ACC,%%0,$I", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmulai with dest mask (writes ACC)
(define_insn "vu0_vmulai_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)
                      (match_operand:SI 1 "const_int_operand" "n")]
                     UNSPEC_VU0_VMULAI_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[1]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmulai.%s\t$ACC,%%0,$I", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaddai with dest mask (reads/writes ACC)
(define_insn "vu0_vmaddai_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)
                      (match_operand:SI 1 "const_int_operand" "n")]
                     UNSPEC_VU0_VMADDAI_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[1]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmaddai.%s\t$ACC,%%0,$I", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; vmsubai with dest mask (reads/writes ACC)
(define_insn "vu0_vmsubai_m"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)
                      (match_operand:SI 1 "const_int_operand" "n")]
                     UNSPEC_VU0_VMSUBAI_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[1]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmsubai.%s\t$ACC,%%0,$I", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Conversion Masked Operations
;; vftoi0/4/12/15 and vitof0/4/12/15 with arbitrary dest mask
;; -------------------------------------------------------------------------

;; vftoi0 with dest mask
(define_insn "vu0_vftoi0_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VFTOI0_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vftoi0.%s\t%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; vftoi4 with dest mask
(define_insn "vu0_vftoi4_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VFTOI4_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vftoi4.%s\t%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; vftoi12 with dest mask
(define_insn "vu0_vftoi12_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VFTOI12_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vftoi12.%s\t%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; vftoi15 with dest mask
(define_insn "vu0_vftoi15_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VFTOI15_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vftoi15.%s\t%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; vitof0 with dest mask
(define_insn "vu0_vitof0_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VITOF0_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vitof0.%s\t%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; vitof4 with dest mask
(define_insn "vu0_vitof4_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VITOF4_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vitof4.%s\t%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; vitof12 with dest mask
(define_insn "vu0_vitof12_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VITOF12_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vitof12.%s\t%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; vitof15 with dest mask
(define_insn "vu0_vitof15_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VITOF15_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vitof15.%s\t%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Data Movement Masked Operations
;; vmr32 with arbitrary dest mask
;; -------------------------------------------------------------------------

;; vmr32 with dest mask
(define_insn "vu0_vmr32_m"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:SI 2 "const_int_operand" "n")]
                     UNSPEC_VU0_VMR32_M))]
  "ISA_HAS_VU0"
  {
    static const char *const dest_suffix[] = {
      "xyzw", "w", "z", "zw", "y", "yw", "yz", "yzw",
      "x", "xw", "xz", "xzw", "xy", "xyw", "xyz", "xyzw"
    };
    static char buf[32];
    int mask = INTVAL (operands[2]) & 0xf;
    if (mask == 0) mask = 0xf;
    sprintf (buf, "vmr32.%s\t%%0,%%1", dest_suffix[mask]);
    return buf;
  }
  [(set_attr "type" "fmove")
   (set_attr "mode" "V4SF")])
