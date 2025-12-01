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

;; VU0: 128-bit load to COP2 register: lqc2 $vfN, offset(base)
(define_insn "*movv4sf_load_vu0"
  [(set (match_operand:V4SF 0 "register_operand" "=C")
        (match_operand:V4SF 1 "memory_operand" "m"))]
  "ISA_HAS_VU0"
  "lqc2\t%0,%1"
  [(set_attr "type" "fpload")
   (set_attr "mode" "V4SF")])

;; VU0: 128-bit store from COP2 register: sqc2 $vfN, offset(base)
(define_insn "*movv4sf_store_vu0"
  [(set (match_operand:V4SF 0 "memory_operand" "=m")
        (match_operand:V4SF 1 "register_operand" "C"))]
  "ISA_HAS_VU0"
  "sqc2\t%1,%0"
  [(set_attr "type" "fpstore")
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
