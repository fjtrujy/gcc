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

;; VU0 UNSPEC constants
(define_c_enum "unspec" [
  ;; Basic accumulator operations
  UNSPEC_VU0_VMULA      ;; ACC = a * b
  UNSPEC_VU0_VMADDA     ;; ACC = ACC + a * b
  UNSPEC_VU0_VMADD      ;; dest = ACC + a * b
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
  ;; Outer product operations
  UNSPEC_VU0_VOPMULA
  UNSPEC_VU0_VOPMSUB
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
  [(unspec_volatile:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                          (match_operand:V4SF 1 "register_operand" "C")]
                         UNSPEC_VU0_VMULA)]
  "ISA_HAS_VU0"
  "vmula.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmadda: ACC = ACC + src1 * src2 (accumulates to implicit accumulator)
(define_insn "vu0_vmadda"
  [(unspec_volatile:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                          (match_operand:V4SF 1 "register_operand" "C")]
                         UNSPEC_VU0_VMADDA)]
  "ISA_HAS_VU0"
  "vmadda.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; vmadd: dest = ACC + src1 * src2 (reads from implicit accumulator)
(define_insn "vu0_vmadd"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMADD))]
  "ISA_HAS_VU0"
  "vmadd.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Fused Multiply-Add (for autovectorization)
;; Standard FMA pattern using VU0 accumulator: dest = (a * b) + c
;; Uses vmulaw to load c into ACC (c * $vf0.w = c * 1.0), then vmadd.
;; Note: $vf0.w is always 1.0 on VU0.
;; -------------------------------------------------------------------------

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
  [(unspec_volatile:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                          (match_operand:V4SF 1 "register_operand" "C")]
                         UNSPEC_VU0_VMULAX)]
  "ISA_HAS_VU0"
  "vmulax.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmulay"
  [(unspec_volatile:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                          (match_operand:V4SF 1 "register_operand" "C")]
                         UNSPEC_VU0_VMULAY)]
  "ISA_HAS_VU0"
  "vmulay.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmulaz"
  [(unspec_volatile:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                          (match_operand:V4SF 1 "register_operand" "C")]
                         UNSPEC_VU0_VMULAZ)]
  "ISA_HAS_VU0"
  "vmulaz.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmulaw"
  [(unspec_volatile:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                          (match_operand:V4SF 1 "register_operand" "C")]
                         UNSPEC_VU0_VMULAW)]
  "ISA_HAS_VU0"
  "vmulaw.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Broadcast Multiply-Add with ACC result
;; vmaddax/y/z/w: ACC = ACC + src1 * src2.x/y/z/w
;; -------------------------------------------------------------------------

(define_insn "vu0_vmaddax"
  [(unspec_volatile:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                          (match_operand:V4SF 1 "register_operand" "C")]
                         UNSPEC_VU0_VMADDAX)]
  "ISA_HAS_VU0"
  "vmaddax.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmadday"
  [(unspec_volatile:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                          (match_operand:V4SF 1 "register_operand" "C")]
                         UNSPEC_VU0_VMADDAY)]
  "ISA_HAS_VU0"
  "vmadday.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmaddaz"
  [(unspec_volatile:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                          (match_operand:V4SF 1 "register_operand" "C")]
                         UNSPEC_VU0_VMADDAZ)]
  "ISA_HAS_VU0"
  "vmaddaz.xyzw\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmaddaw"
  [(unspec_volatile:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                          (match_operand:V4SF 1 "register_operand" "C")]
                         UNSPEC_VU0_VMADDAW)]
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
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMADDX))]
  "ISA_HAS_VU0"
  "vmaddx.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmaddy"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMADDY))]
  "ISA_HAS_VU0"
  "vmaddy.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmaddz"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMADDZ))]
  "ISA_HAS_VU0"
  "vmaddz.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vmaddw"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VMADDW))]
  "ISA_HAS_VU0"
  "vmaddw.xyzw\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

;; -------------------------------------------------------------------------
;; VU0 Outer Product (for cross product)
;; vopmula: ACC.xyz = src1.yzx * src2.zxy (cross product multiply)
;; vopmsub: dest.xyz = ACC.xyz - src1.yzx * src2.zxy (cross product complete)
;; -------------------------------------------------------------------------

(define_insn "vu0_vopmula"
  [(unspec_volatile:V4SF [(match_operand:V4SF 0 "register_operand" "C")
                          (match_operand:V4SF 1 "register_operand" "C")]
                         UNSPEC_VU0_VOPMULA)]
  "ISA_HAS_VU0"
  "vopmula.xyz\tACC,%0,%1"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])

(define_insn "vu0_vopmsub"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (unspec:V4SF [(match_operand:V4SF 1 "register_operand" "C")
                      (match_operand:V4SF 2 "register_operand" "C")]
                     UNSPEC_VU0_VOPMSUB))]
  "ISA_HAS_VU0"
  "vopmsub.xyz\t%0,%1,%2"
  [(set_attr "type" "fmul")
   (set_attr "mode" "V4SF")])
