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
  [(VU0_ACC_REGNUM		188)])

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
;;   0: C,C  -> vmove.xyzw (COP2 to COP2)
;;   1: C,m  -> lqc2 (memory to COP2)
;;   2: m,C  -> sqc2 (COP2 to memory)
;;   3: C,d  -> qmtc2 (GP to COP2)
;;   4: d,C  -> qmfc2 (COP2 to GP)
;;   5: d,d  -> por (GP to GP, 128-bit register copy)
;;   6: d,m  -> lq (memory to GP)
;;   7: m,d  -> sq (GP to memory)
(define_insn "*movv4sf_vu0"
  [(set (match_operand:V4SF 0 "nonimmediate_operand" "=C,C,m,C,d,d,d,m")
        (match_operand:V4SF 1 "move_operand"          "C,m,C,d,C,d,m,d"))]
  "ISA_HAS_VU0"
  "@
   vmove.xyzw\t%0,%1
   lqc2\t%0,%1
   sqc2\t%1,%0
   qmtc2\t%1,%0
   qmfc2\t%0,%1
   por\t%0,$0,%1
   lq\t%0,%1
   sq\t%1,%0"
  [(set_attr "type" "fmove,fpload,fpstore,mtc,mfc,move,load,store")
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
