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

;; VU0/R5900: Combined V4SF move pattern with all alternatives
;; Alternatives:
;;   0: C,YG -> vmove.xyzw from $vf0 (vector zero constant)
;;   1: C,C  -> vmove.xyzw (COP2 to COP2)
;;   2: C,m  -> lqc2 (memory to COP2)
;;   3: m,C  -> sqc2 (COP2 to memory)
;;   4: C,d  -> qmtc2 (GP to COP2)
;;   5: d,C  -> qmfc2 (COP2 to GP)
;;   6: d,d  -> por (GP to GP, 128-bit register copy)
;;   7: d,m  -> lq (memory to GP)
;;   8: m,d  -> sq (GP to memory)
(define_insn "*movv4sf_vu0"
  [(set (match_operand:V4SF 0 "nonimmediate_operand" "=C,C,C,m,C,d,d,d,m")
        (match_operand:V4SF 1 "move_operand"          "YG,C,m,C,d,C,d,m,d"))]
  "ISA_HAS_VU0"
  "@
   vmove.xyzw\t%0,$vf0
   vmove.xyzw\t%0,%1
   lqc2\t%0,%1
   sqc2\t%1,%0
   qmtc2\t%1,%0
   qmfc2\t%0,%1
   por\t%0,$0,%1
   lq\t%0,%1
   sq\t%1,%0"
  [(set_attr "type" "fmove,fmove,fpload,fpstore,mtc,mfc,move,load,store")
   (set_attr "mode" "V4SF")])

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
(define_insn "addv4sf3"
  [(set (match_operand:V4SF 0 "register_operand" "=C,f")
        (plus:V4SF (match_operand:V4SF 1 "register_operand" "C,f")
                   (match_operand:V4SF 2 "register_operand" "C,f")))]
  "ISA_HAS_VU0 || ISA_HAS_MSA"
  "@
   vadd.xyzw\t%0,%1,%2
   fadd.w\t%w0,%w1,%w2"
  [(set_attr "type" "fadd,simd_fadd")
   (set_attr "mode" "V4SF")])

;; Vector subtraction
(define_insn "subv4sf3"
  [(set (match_operand:V4SF 0 "register_operand" "=C,f")
        (minus:V4SF (match_operand:V4SF 1 "register_operand" "C,f")
                    (match_operand:V4SF 2 "register_operand" "C,f")))]
  "ISA_HAS_VU0 || ISA_HAS_MSA"
  "@
   vsub.xyzw\t%0,%1,%2
   fsub.w\t%w0,%w1,%w2"
  [(set_attr "type" "fadd,simd_fadd")
   (set_attr "mode" "V4SF")])

;; Vector multiplication
(define_insn "mulv4sf3"
  [(set (match_operand:V4SF 0 "register_operand" "=C,f")
        (mult:V4SF (match_operand:V4SF 1 "register_operand" "C,f")
                   (match_operand:V4SF 2 "register_operand" "C,f")))]
  "ISA_HAS_VU0 || ISA_HAS_MSA"
  "@
   vmul.xyzw\t%0,%1,%2
   fmul.w\t%w0,%w1,%w2"
  [(set_attr "type" "fmul,simd_fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; Vector Min/Max/Abs (for general vectorization)
;; -------------------------------------------------------------------------

;; Vector absolute value
(define_insn "absv4sf2"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (abs:V4SF (match_operand:V4SF 1 "register_operand" "C")))]
  "ISA_HAS_VU0"
  "vabs.xyzw\t%0,%1"
  [(set_attr "type" "fabs")
   (set_attr "mode" "V4SF")])

;; Vector maximum - VU0 only (MSA provides its own smaxv4sf3)
(define_insn "*smaxv4sf3_vu0"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (smax:V4SF (match_operand:V4SF 1 "register_operand" "C")
                   (match_operand:V4SF 2 "register_operand" "C")))]
  "ISA_HAS_VU0"
  "vmax.xyzw\t%0,%1,%2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; Vector minimum - VU0 only (VU0 uses vmini, MSA provides its own sminv4sf3)
(define_insn "*sminv4sf3_vu0"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (smin:V4SF (match_operand:V4SF 1 "register_operand" "C")
                   (match_operand:V4SF 2 "register_operand" "C")))]
  "ISA_HAS_VU0"
  "vmini.xyzw\t%0,%1,%2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Accumulator Operations (for matrix multiplication)
;; These use the VU0 accumulator register for efficient FMA chains.
;; The accumulator is implicit - the programmer must sequence these correctly.
;; -------------------------------------------------------------------------

;; vmula: ACC = src1 * src2 (stores to implicit accumulator)
(define_insn "vu0_vmula"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMULA))]
  "ISA_HAS_VU0"
  "vmula.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmadda: ACC = ACC + src1 * src2 (accumulates to accumulator)
(define_insn "vu0_vmadda"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMADDA))]
  "ISA_HAS_VU0"
  "vmadda.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmadd: dest = ACC + src1 * src2 (reads from accumulator)
(define_insn "vu0_vmadd"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMADD))]
  "ISA_HAS_VU0"
  "vmadd.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmsuba: ACC = ACC - src1 * src2 (subtracts from accumulator)
(define_insn "vu0_vmsuba"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMSUBA))]
  "ISA_HAS_VU0"
  "vmsuba.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmsub: dest = ACC - src1 * src2 (reads from accumulator)
(define_insn "vu0_vmsub"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMSUB))]
  "ISA_HAS_VU0"
  "vmsub.xyzw\t%0,%1,%2"
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
  "vmulaw.xyzw\tACC,%3,$vf0\;vmadd.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmadd")
   (set_attr "mode" "V4SF")
   (set_attr "length" "8")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Operations (scalar * vector)
;; These multiply a vector by a broadcast of one component from another vector.
;; Essential for matrix multiplication: row * column[i]
;; -------------------------------------------------------------------------

;; vmulx: dest = src1 * src2.x (broadcast x component)
(define_insn "vu0_vmulx"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMULX))]
  "ISA_HAS_VU0"
  "vmulx.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmuly: dest = src1 * src2.y
(define_insn "vu0_vmuly"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMULY))]
  "ISA_HAS_VU0"
  "vmuly.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmulz: dest = src1 * src2.z
(define_insn "vu0_vmulz"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMULZ))]
  "ISA_HAS_VU0"
  "vmulz.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmulw: dest = src1 * src2.w
(define_insn "vu0_vmulw"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMULW))]
  "ISA_HAS_VU0"
  "vmulw.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Multiply-Accumulate to ACC
;; vmulax/y/z/w: ACC = src1 * src2.x/y/z/w
;; -------------------------------------------------------------------------

(define_insn "vu0_vmulax"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMULAX))]
  "ISA_HAS_VU0"
  "vmulax.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmulay"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMULAY))]
  "ISA_HAS_VU0"
  "vmulay.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmulaz"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMULAZ))]
  "ISA_HAS_VU0"
  "vmulaz.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmulaw"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMULAW))]
  "ISA_HAS_VU0"
  "vmulaw.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Multiply-Add with ACC result
;; vmaddax/y/z/w: ACC = ACC + src1 * src2.x/y/z/w
;; -------------------------------------------------------------------------

(define_insn "vu0_vmaddax"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMADDAX))]
  "ISA_HAS_VU0"
  "vmaddax.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmadday"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMADDAY))]
  "ISA_HAS_VU0"
  "vmadday.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmaddaz"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMADDAZ))]
  "ISA_HAS_VU0"
  "vmaddaz.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmaddaw"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMADDAW))]
  "ISA_HAS_VU0"
  "vmaddaw.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Multiply-Add with register result
;; vmaddx/y/z/w: dest = ACC + src1 * src2.x/y/z/w
;; -------------------------------------------------------------------------

(define_insn "vu0_vmaddx"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMADDX))]
  "ISA_HAS_VU0"
  "vmaddx.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmaddy"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMADDY))]
  "ISA_HAS_VU0"
  "vmaddy.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmaddz"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMADDZ))]
  "ISA_HAS_VU0"
  "vmaddz.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmaddw"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMADDW))]
  "ISA_HAS_VU0"
  "vmaddw.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Multiply-Subtract with ACC result
;; vmsubax/y/z/w: ACC = ACC - src1 * src2.x/y/z/w
;; -------------------------------------------------------------------------

(define_insn "vu0_vmsubax"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMSUBAX))]
  "ISA_HAS_VU0"
  "vmsubax.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmsubay"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMSUBAY))]
  "ISA_HAS_VU0"
  "vmsubay.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmsubaz"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMSUBAZ))]
  "ISA_HAS_VU0"
  "vmsubaz.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmsubaw"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMSUBAW))]
  "ISA_HAS_VU0"
  "vmsubaw.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Multiply-Subtract with register result
;; vmsubx/y/z/w: dest = ACC - src1 * src2.x/y/z/w
;; -------------------------------------------------------------------------

(define_insn "vu0_vmsubx"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMSUBX))]
  "ISA_HAS_VU0"
  "vmsubx.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmsuby"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMSUBY))]
  "ISA_HAS_VU0"
  "vmsuby.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmsubz"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMSUBZ))]
  "ISA_HAS_VU0"
  "vmsubz.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmsubw"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMSUBW))]
  "ISA_HAS_VU0"
  "vmsubw.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Outer Product (for cross product)
;; vopmula: ACC.xyz = src1.yzx * src2.zxy (cross product multiply)
;; vopmsub: dest.xyz = ACC.xyz - src1.yzx * src2.zxy (cross product complete)
;; -------------------------------------------------------------------------

(define_insn "vu0_vopmula"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VOPMULA))]
  "ISA_HAS_VU0"
  "vopmula.xyz\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vopmsub"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VOPMSUB))]
  "ISA_HAS_VU0"
  "vopmsub.xyz\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Add Operations
;; vaddx/y/z/w: dest = src1 + src2.x/y/z/w (broadcast component add)
;; -------------------------------------------------------------------------

(define_insn "vu0_vaddx"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VADDX))]
  "ISA_HAS_VU0"
  "vaddx.xyzw\t%0,%1,%2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vaddy"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VADDY))]
  "ISA_HAS_VU0"
  "vaddy.xyzw\t%0,%1,%2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vaddz"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VADDZ))]
  "ISA_HAS_VU0"
  "vaddz.xyzw\t%0,%1,%2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vaddw"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VADDW))]
  "ISA_HAS_VU0"
  "vaddw.xyzw\t%0,%1,%2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Subtract Operations
;; vsubx/y/z/w: dest = src1 - src2.x/y/z/w (broadcast component subtract)
;; -------------------------------------------------------------------------

(define_insn "vu0_vsubx"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VSUBX))]
  "ISA_HAS_VU0"
  "vsubx.xyzw\t%0,%1,%2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vsuby"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VSUBY))]
  "ISA_HAS_VU0"
  "vsuby.xyzw\t%0,%1,%2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vsubz"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VSUBZ))]
  "ISA_HAS_VU0"
  "vsubz.xyzw\t%0,%1,%2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vsubw"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VSUBW))]
  "ISA_HAS_VU0"
  "vsubw.xyzw\t%0,%1,%2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Accumulator Add/Subtract
;; vadda: ACC = src1 + src2 (vector add to accumulator)
;; vsuba: ACC = src1 - src2 (vector subtract to accumulator)
;; -------------------------------------------------------------------------

(define_insn "vu0_vadda"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VADDA))]
  "ISA_HAS_VU0"
  "vadda.xyzw\tACC,%0,%1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vsuba"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VSUBA))]
  "ISA_HAS_VU0"
  "vsuba.xyzw\tACC,%0,%1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Add to Accumulator
;; vaddax/y/z/w: ACC = src1 + src2.x/y/z/w
;; -------------------------------------------------------------------------

(define_insn "vu0_vaddax"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VADDAX))]
  "ISA_HAS_VU0"
  "vaddax.xyzw\tACC,%0,%1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vadday"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VADDAY))]
  "ISA_HAS_VU0"
  "vadday.xyzw\tACC,%0,%1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vaddaz"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VADDAZ))]
  "ISA_HAS_VU0"
  "vaddaz.xyzw\tACC,%0,%1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vaddaw"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VADDAW))]
  "ISA_HAS_VU0"
  "vaddaw.xyzw\tACC,%0,%1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Subtract from Accumulator
;; vsubax/y/z/w: ACC = src1 - src2.x/y/z/w
;; -------------------------------------------------------------------------

(define_insn "vu0_vsubax"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VSUBAX))]
  "ISA_HAS_VU0"
  "vsubax.xyzw\tACC,%0,%1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vsubay"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VSUBAY))]
  "ISA_HAS_VU0"
  "vsubay.xyzw\tACC,%0,%1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vsubaz"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VSUBAZ))]
  "ISA_HAS_VU0"
  "vsubaz.xyzw\tACC,%0,%1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vsubaw"
  [(set (reg:V4SF VU0_ACC_REGNUM)
        (unspec:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                      (match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VSUBAW))]
  "ISA_HAS_VU0"
  "vsubaw.xyzw\tACC,%0,%1"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Float to Fixed-Point Conversions
;; vftoi0:  Convert float to 32-bit signed integer (no fractional bits)
;; vftoi4:  Convert float to 28.4 fixed-point (4 fractional bits)
;; vftoi12: Convert float to 20.12 fixed-point (12 fractional bits)
;; vftoi15: Convert float to 17.15 fixed-point (15 fractional bits)
;; -------------------------------------------------------------------------

(define_insn "vu0_vftoi0"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VFTOI0))]
  "ISA_HAS_VU0"
  "vftoi0.xyzw\t%0,%1"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vftoi4"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VFTOI4))]
  "ISA_HAS_VU0"
  "vftoi4.xyzw\t%0,%1"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vftoi12"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VFTOI12))]
  "ISA_HAS_VU0"
  "vftoi12.xyzw\t%0,%1"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vftoi15"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VFTOI15))]
  "ISA_HAS_VU0"
  "vftoi15.xyzw\t%0,%1"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Fixed-Point to Float Conversions
;; vitof0:  Convert 32-bit signed integer to float (no fractional bits)
;; vitof4:  Convert 28.4 fixed-point to float (4 fractional bits)
;; vitof12: Convert 20.12 fixed-point to float (12 fractional bits)
;; vitof15: Convert 17.15 fixed-point to float (15 fractional bits)
;; -------------------------------------------------------------------------

(define_insn "vu0_vitof0"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VITOF0))]
  "ISA_HAS_VU0"
  "vitof0.xyzw\t%0,%1"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vitof4"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VITOF4))]
  "ISA_HAS_VU0"
  "vitof4.xyzw\t%0,%1"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vitof12"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VITOF12))]
  "ISA_HAS_VU0"
  "vitof12.xyzw\t%0,%1"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vitof15"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VITOF15))]
  "ISA_HAS_VU0"
  "vitof15.xyzw\t%0,%1"
  [(set_attr "type" "fcvt")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Vector Rotate
;; vmr32: Rotate vector elements by 32 bits (1 element)
;;        dest.xyzw = src.yzwx
;; -------------------------------------------------------------------------

(define_insn "vu0_vmr32"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMR32))]
  "ISA_HAS_VU0"
  "vmr32.xyzw\t%0,%1"
  [(set_attr "type" "fmove")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Explicit Intrinsic Patterns
;; Named patterns for builtins that expose automatic operations explicitly.
;; -------------------------------------------------------------------------

;; vu0_vmax: Explicit max intrinsic
(define_insn "vu0_vmax"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMAX))]
  "ISA_HAS_VU0"
  "vmax.xyzw\t%0,%1,%2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vu0_vmini: Explicit min intrinsic
(define_insn "vu0_vmini"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMINI))]
  "ISA_HAS_VU0"
  "vmini.xyzw\t%0,%1,%2"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vu0_vmove: Explicit move intrinsic
(define_insn "vu0_vmove"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")]
                     UNSPEC_VU0_VMOVE))]
  "ISA_HAS_VU0"
  "vmove.xyzw\t%0,%1"
  [(set_attr "type" "fmove")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Division and Square Root Operations (Q register)
;; These write results to the Q register asynchronously.
;; Component selector: 0=x, 1=y, 2=z, 3=w
;; -------------------------------------------------------------------------

;; vdiv: Q = fs.bc / ft.bc
;; Uses unspec_volatile to prevent elimination (starts async division)
(define_insn "vu0_vdiv"
  [(unspec_volatile [(match_operand:V4SF 0 "register_operand" "C")
                     (match_operand:SI 1 "const_int_operand" "n")
                     (match_operand:V4SF 2 "register_operand" "C")
                     (match_operand:SI 3 "const_int_operand" "n")]
                    UNSPEC_VU0_VDIV)
   (clobber (reg:SF VU0_Q_REGNUM))]
  "ISA_HAS_VU0"
  {
    static const char *const bc[] = { "x", "y", "z", "w" };
    static char buf[32];
    sprintf (buf, "vdiv\tQ,%%0.%s,%%2.%s",
             bc[INTVAL (operands[1]) & 3],
             bc[INTVAL (operands[3]) & 3]);
    return buf;
  }
  [(set_attr "type" "fdiv")
   (set_attr "mode" "SF")])

;; vsqrt: Q = sqrt(ft.bc)
;; Uses unspec_volatile to prevent elimination (starts async sqrt)
(define_insn "vu0_vsqrt"
  [(unspec_volatile [(match_operand:V4SF 0 "register_operand" "C")
                     (match_operand:SI 1 "const_int_operand" "n")]
                    UNSPEC_VU0_VSQRT)
   (clobber (reg:SF VU0_Q_REGNUM))]
  "ISA_HAS_VU0"
  {
    static const char *const bc[] = { "x", "y", "z", "w" };
    static char buf[32];
    sprintf (buf, "vsqrt\tQ,%%0.%s", bc[INTVAL (operands[1]) & 3]);
    return buf;
  }
  [(set_attr "type" "fsqrt")
   (set_attr "mode" "SF")])

;; vrsqrt: Q = fs.bc / sqrt(ft.bc)
;; Uses unspec_volatile to prevent elimination (starts async division)
(define_insn "vu0_vrsqrt"
  [(unspec_volatile [(match_operand:V4SF 0 "register_operand" "C")
                     (match_operand:SI 1 "const_int_operand" "n")
                     (match_operand:V4SF 2 "register_operand" "C")
                     (match_operand:SI 3 "const_int_operand" "n")]
                    UNSPEC_VU0_VRSQRT)
   (clobber (reg:SF VU0_Q_REGNUM))]
  "ISA_HAS_VU0"
  {
    static const char *const bc[] = { "x", "y", "z", "w" };
    static char buf[32];
    sprintf (buf, "vrsqrt\tQ,%%0.%s,%%2.%s",
             bc[INTVAL (operands[1]) & 3],
             bc[INTVAL (operands[3]) & 3]);
    return buf;
  }
  [(set_attr "type" "fdiv")
   (set_attr "mode" "SF")])

;; vwaitq: Wait for Q register to be ready
;; Takes a dummy SI argument for builtin compatibility (ignored)
(define_insn "vu0_vwaitq"
  [(unspec_volatile [(match_operand:SI 0 "register_operand" "d")
                     (reg:SF VU0_Q_REGNUM)] UNSPEC_VU0_WAITQ)]
  "ISA_HAS_VU0"
  "vwaitq"
  [(set_attr "type" "nop")
   (set_attr "mode" "none")])

;; -------------------------------------------------------------------------
;; VU0 Q Broadcast Operations
;; These read the Q register and broadcast it across all vector components.
;; -------------------------------------------------------------------------

;; vaddq: dest = src + Q
(define_insn "vu0_vaddq"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)]
                     UNSPEC_VU0_VADDQ))]
  "ISA_HAS_VU0"
  "vaddq.xyzw\t%0,%1,Q"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubq: dest = src - Q
(define_insn "vu0_vsubq"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)]
                     UNSPEC_VU0_VSUBQ))]
  "ISA_HAS_VU0"
  "vsubq.xyzw\t%0,%1,Q"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmulq: dest = src * Q
(define_insn "vu0_vmulq"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)]
                     UNSPEC_VU0_VMULQ))]
  "ISA_HAS_VU0"
  "vmulq.xyzw\t%0,%1,Q"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vaddaQ: ACC = src + Q
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vaddaq"
  [(unspec_volatile [(match_operand:V4SF 0 "register_operand" "C")
                     (reg:SF VU0_Q_REGNUM)]
                    UNSPEC_VU0_VADDQA)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vaddaq.xyzw\tACC,%0,Q"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubaQ: ACC = src - Q
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vsubaq"
  [(unspec_volatile [(match_operand:V4SF 0 "register_operand" "C")
                     (reg:SF VU0_Q_REGNUM)]
                    UNSPEC_VU0_VSUBQA)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vsubaq.xyzw\tACC,%0,Q"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmulaQ: ACC = src * Q
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vmulaq"
  [(unspec_volatile [(match_operand:V4SF 0 "register_operand" "C")
                     (reg:SF VU0_Q_REGNUM)]
                    UNSPEC_VU0_VMULQA)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vmulaq.xyzw\tACC,%0,Q"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaddq: dest = ACC + src * Q
(define_insn "vu0_vmaddq"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)]
                     UNSPEC_VU0_VMADDQ))]
  "ISA_HAS_VU0"
  "vmaddq.xyzw\t%0,%1,Q"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmsubq: dest = ACC - src * Q
(define_insn "vu0_vmsubq"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)]
                     UNSPEC_VU0_VMSUBQ))]
  "ISA_HAS_VU0"
  "vmsubq.xyzw\t%0,%1,Q"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaddaQ: ACC = ACC + src * Q
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vmaddaq"
  [(unspec_volatile [(reg:V4SF VU0_ACC_REGNUM)
                     (match_operand:V4SF 0 "register_operand" "C")
                     (reg:SF VU0_Q_REGNUM)]
                    UNSPEC_VU0_VMADDQA)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vmaddaq.xyzw\tACC,%0,Q"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmsubaQ: ACC = ACC - src * Q
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vmsubaq"
  [(unspec_volatile [(reg:V4SF VU0_ACC_REGNUM)
                     (match_operand:V4SF 0 "register_operand" "C")
                     (reg:SF VU0_Q_REGNUM)]
                    UNSPEC_VU0_VMSUBQA)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vmsubaq.xyzw\tACC,%0,Q"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaxQ: dest = max(src, Q)
(define_insn "vu0_vmaxq"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)]
                     UNSPEC_VU0_VMAXQ))]
  "ISA_HAS_VU0"
  "vmaxq.xyzw\t%0,%1,Q"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vminiQ: dest = min(src, Q)
(define_insn "vu0_vminiq"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_Q_REGNUM)]
                     UNSPEC_VU0_VMINIQ))]
  "ISA_HAS_VU0"
  "vminiq.xyzw\t%0,%1,Q"
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
(define_insn "vu0_vaddi"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)]
                     UNSPEC_VU0_VADDI))]
  "ISA_HAS_VU0"
  "vaddi.xyzw\t%0,%1,I"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubi: dest = src - I
(define_insn "vu0_vsubi"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)]
                     UNSPEC_VU0_VSUBI))]
  "ISA_HAS_VU0"
  "vsubi.xyzw\t%0,%1,I"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmuli: dest = src * I
(define_insn "vu0_vmuli"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)]
                     UNSPEC_VU0_VMULI))]
  "ISA_HAS_VU0"
  "vmuli.xyzw\t%0,%1,I"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vaddaI: ACC = src + I
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vaddai"
  [(unspec_volatile [(match_operand:V4SF 0 "register_operand" "C")
                     (reg:SF VU0_I_REGNUM)]
                    UNSPEC_VU0_VADDAI)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vaddai.xyzw\tACC,%0,I"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vsubaI: ACC = src - I
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vsubai"
  [(unspec_volatile [(match_operand:V4SF 0 "register_operand" "C")
                     (reg:SF VU0_I_REGNUM)]
                    UNSPEC_VU0_VSUBAI)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vsubai.xyzw\tACC,%0,I"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vmulaI: ACC = src * I
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vmulai"
  [(unspec_volatile [(match_operand:V4SF 0 "register_operand" "C")
                     (reg:SF VU0_I_REGNUM)]
                    UNSPEC_VU0_VMULAI)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vmulai.xyzw\tACC,%0,I"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaddi: dest = ACC + src * I
(define_insn "vu0_vmaddi"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)]
                     UNSPEC_VU0_VMADDI))]
  "ISA_HAS_VU0"
  "vmaddi.xyzw\t%0,%1,I"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmsubi: dest = ACC - src * I
(define_insn "vu0_vmsubi"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(reg:V4SF VU0_ACC_REGNUM)
                      (match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)]
                     UNSPEC_VU0_VMSUBI))]
  "ISA_HAS_VU0"
  "vmsubi.xyzw\t%0,%1,I"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaddaI: ACC = ACC + src * I
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vmaddai"
  [(unspec_volatile [(reg:V4SF VU0_ACC_REGNUM)
                     (match_operand:V4SF 0 "register_operand" "C")
                     (reg:SF VU0_I_REGNUM)]
                    UNSPEC_VU0_VMADDAI)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vmaddai.xyzw\tACC,%0,I"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmsubaI: ACC = ACC - src * I
;; Uses unspec_volatile to prevent elimination
(define_insn "vu0_vmsubai"
  [(unspec_volatile [(reg:V4SF VU0_ACC_REGNUM)
                     (match_operand:V4SF 0 "register_operand" "C")
                     (reg:SF VU0_I_REGNUM)]
                    UNSPEC_VU0_VMSUBAI)
   (clobber (reg:V4SF VU0_ACC_REGNUM))]
  "ISA_HAS_VU0"
  "vmsubai.xyzw\tACC,%0,I"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmaxI: dest = max(src, I)
(define_insn "vu0_vmaxi"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)]
                     UNSPEC_VU0_VMAXI))]
  "ISA_HAS_VU0"
  "vmaxi.xyzw\t%0,%1,I"
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")])

;; vminiI: dest = min(src, I)
(define_insn "vu0_vminii"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (reg:SF VU0_I_REGNUM)]
                     UNSPEC_VU0_VMINII))]
  "ISA_HAS_VU0"
  "vminii.xyzw\t%0,%1,I"
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
;; with a vector operation. Uses mfc1 + qmtc2 to load scalar into VU0 reg,
;; then vaddx to broadcast the x component to all lanes.
(define_insn_and_split "vec_duplicatev4sf"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (vec_duplicate:V4SF (match_operand:SF 1 "register_operand" "f")))
   (clobber (match_scratch:V4SF 2 "=&C"))
   (clobber (match_scratch:SI 3 "=&d"))]
  "ISA_HAS_VU0"
  "#"
  "&& reload_completed"
  [(const_int 0)]
{
  /* Load scalar into VU0 register x component */
  emit_insn (gen_mfc1_vu0 (operands[3], operands[1]));
  emit_insn (gen_vu0_qmtc2_scalar (operands[2], operands[3]));
  /* Broadcast x component to all lanes: dest = vf0 + src.x */
  emit_insn (gen_vu0_vaddx (operands[0], gen_rtx_REG (V4SFmode, COP2_REG_FIRST), operands[2]));
  DONE;
}
  [(set_attr "type" "fadd")
   (set_attr "mode" "V4SF")
   (set_attr "length" "12")])

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
