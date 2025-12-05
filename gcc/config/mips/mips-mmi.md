;; Machine description for R5900 MMI (Multimedia Instructions).
;; Copyright (C) 2025 Free Software Foundation, Inc.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.
;;
;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; R5900 MMI operates on 128-bit GPRs with parallel integer operations.
;; Unlike Loongson MMI (64-bit in FPU) or MSA (128-bit in FPU), R5900 MMI
;; uses the extended 128-bit general purpose registers.
;;
;; This file provides:
;; - Explicit intrinsics via __builtin_mmi_* functions
;; - Vector move patterns for autovectorization support
;; Standard optab patterns (add/sub) are in mips-msa.md with MMI alternatives.

;; -------------------------------------------------------------------------
;; Mode Iterators for MMI Vector Modes
;; -------------------------------------------------------------------------

(define_mode_iterator VMMI [V16QI V8HI V4SI V2DI])

;; Byte/Half/Word modes (for comparisons - all sizes except doubleword)
(define_mode_iterator VMMIBHW [V16QI V8HI V4SI])

;; Half/Word modes only (for min/max - R5900 has no byte min/max)
(define_mode_iterator VMMIHW [V8HI V4SI])

;; Mode attribute for instruction suffix (byte/half/word)
(define_mode_attr mmi_bhw [(V16QI "b") (V8HI "h") (V4SI "w")])

;; Mode attribute for half/word suffix only
(define_mode_attr mmi_hw [(V8HI "h") (V4SI "w")])

;; -------------------------------------------------------------------------
;; Vector Move Patterns for Autovectorization
;; -------------------------------------------------------------------------

;; MMI vector moves in GP registers using LQ/SQ for memory.
;; R5900 GPRs are 128-bit wide, so $0 is a full 128-bit zero.
;; Alternatives:
;;   0: d,d  -> move (128-bit register copy)
;;   1: d,m  -> lq (128-bit load from memory)
;;   2: d,YG -> move $d,$0 (128-bit vector zero to register)
;;   3: m,d  -> sq (128-bit store to memory)
;;   4: m,YG -> sq $0,mem (128-bit vector zero store to memory)
;; Note: YG is the constraint for vector zero (const_vector), while J is
;; only for integer zero (const_int). VMMI modes are vector modes.
(define_insn "*mov<mode>_mmi"
  [(set (match_operand:VMMI 0 "nonimmediate_operand" "=d,d,d,m,m")
	(match_operand:VMMI 1 "move_operand" "d,m,YG,d,YG"))]
  "ISA_HAS_MMI && !ISA_HAS_MSA
   && (register_operand (operands[0], <MODE>mode)
       || reg_or_0_operand (operands[1], <MODE>mode))"
  { return mips_output_move (operands[0], operands[1]); }
  [(set_attr "type" "move,load,move,store,store")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Parallel Addition - Explicit Builtins
;; -------------------------------------------------------------------------

;; PADDB - Parallel Add Byte (16 x 8-bit)
(define_insn "mmi_paddb"
  [(set (match_operand:V16QI 0 "register_operand" "=d")
	(plus:V16QI (match_operand:V16QI 1 "register_operand" "d")
		    (match_operand:V16QI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "paddb\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PADDH - Parallel Add Halfword (8 x 16-bit)
(define_insn "mmi_paddh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(plus:V8HI (match_operand:V8HI 1 "register_operand" "d")
		   (match_operand:V8HI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "paddh\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PADDW - Parallel Add Word (4 x 32-bit)
(define_insn "mmi_paddw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(plus:V4SI (match_operand:V4SI 1 "register_operand" "d")
		   (match_operand:V4SI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "paddw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Parallel Subtraction - Explicit Builtins
;; -------------------------------------------------------------------------

;; PSUBB - Parallel Subtract Byte (16 x 8-bit)
(define_insn "mmi_psubb"
  [(set (match_operand:V16QI 0 "register_operand" "=d")
	(minus:V16QI (match_operand:V16QI 1 "register_operand" "d")
		     (match_operand:V16QI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "psubb\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PSUBH - Parallel Subtract Halfword (8 x 16-bit)
(define_insn "mmi_psubh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(minus:V8HI (match_operand:V8HI 1 "register_operand" "d")
		    (match_operand:V8HI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "psubh\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PSUBW - Parallel Subtract Word (4 x 32-bit)
(define_insn "mmi_psubw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(minus:V4SI (match_operand:V4SI 1 "register_operand" "d")
		    (match_operand:V4SI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "psubw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Saturating Arithmetic - Explicit Builtins
;; -------------------------------------------------------------------------
;; Named mmi_* for __builtin_mmi_* intrinsics.
;; Note: Standard optab names (ssadd<mode>3, etc.) conflict with MSA patterns
;; in mips-msa.md, so autovectorization of saturating ops is not supported.
;; Use builtins explicitly for saturating arithmetic.

;; PADDSB - Parallel Add with Signed Saturation Byte
(define_insn "mmi_paddsb"
  [(set (match_operand:V16QI 0 "register_operand" "=d")
	(ss_plus:V16QI (match_operand:V16QI 1 "register_operand" "d")
		       (match_operand:V16QI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "paddsb\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PADDSH - Parallel Add with Signed Saturation Halfword
(define_insn "mmi_paddsh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(ss_plus:V8HI (match_operand:V8HI 1 "register_operand" "d")
		      (match_operand:V8HI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "paddsh\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PADDSW - Parallel Add with Signed Saturation Word
(define_insn "mmi_paddsw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(ss_plus:V4SI (match_operand:V4SI 1 "register_operand" "d")
		      (match_operand:V4SI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "paddsw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PSUBSB - Parallel Subtract with Signed Saturation Byte
(define_insn "mmi_psubsb"
  [(set (match_operand:V16QI 0 "register_operand" "=d")
	(ss_minus:V16QI (match_operand:V16QI 1 "register_operand" "d")
			(match_operand:V16QI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "psubsb\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PSUBSH - Parallel Subtract with Signed Saturation Halfword
(define_insn "mmi_psubsh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(ss_minus:V8HI (match_operand:V8HI 1 "register_operand" "d")
		       (match_operand:V8HI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "psubsh\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PSUBSW - Parallel Subtract with Signed Saturation Word
(define_insn "mmi_psubsw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(ss_minus:V4SI (match_operand:V4SI 1 "register_operand" "d")
		       (match_operand:V4SI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "psubsw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PADDUB - Parallel Add with Unsigned Saturation Byte
(define_insn "mmi_paddub"
  [(set (match_operand:V16QI 0 "register_operand" "=d")
	(us_plus:V16QI (match_operand:V16QI 1 "register_operand" "d")
		       (match_operand:V16QI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "paddub\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PADDUH - Parallel Add with Unsigned Saturation Halfword
(define_insn "mmi_padduh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(us_plus:V8HI (match_operand:V8HI 1 "register_operand" "d")
		      (match_operand:V8HI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "padduh\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PADDUW - Parallel Add with Unsigned Saturation Word
(define_insn "mmi_padduw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(us_plus:V4SI (match_operand:V4SI 1 "register_operand" "d")
		      (match_operand:V4SI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "padduw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PSUBUB - Parallel Subtract with Unsigned Saturation Byte
(define_insn "mmi_psubub"
  [(set (match_operand:V16QI 0 "register_operand" "=d")
	(us_minus:V16QI (match_operand:V16QI 1 "register_operand" "d")
			(match_operand:V16QI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "psubub\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PSUBUH - Parallel Subtract with Unsigned Saturation Halfword
(define_insn "mmi_psubuh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(us_minus:V8HI (match_operand:V8HI 1 "register_operand" "d")
		       (match_operand:V8HI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "psubuh\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PSUBUW - Parallel Subtract with Unsigned Saturation Word
(define_insn "mmi_psubuw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(us_minus:V4SI (match_operand:V4SI 1 "register_operand" "d")
		       (match_operand:V4SI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "psubuw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Saturating Subtract - Autovectorization Patterns
;; -------------------------------------------------------------------------
;; Standard optab names for autovectorization. MSA doesn't have ss_minus/us_minus
;; patterns, so these don't conflict.

;; Signed saturating subtract (byte/halfword/word)
(define_insn "sssub<mode>3"
  [(set (match_operand:VMMIBHW 0 "register_operand" "=d")
	(ss_minus:VMMIBHW (match_operand:VMMIBHW 1 "register_operand" "d")
			  (match_operand:VMMIBHW 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "psubs<mmi_bhw>\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; Unsigned saturating subtract (byte/halfword/word)
(define_insn "ussub<mode>3"
  [(set (match_operand:VMMIBHW 0 "register_operand" "=d")
	(us_minus:VMMIBHW (match_operand:VMMIBHW 1 "register_operand" "d")
			  (match_operand:VMMIBHW 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "psubu<mmi_bhw>\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Parallel Logical Operations - Explicit Builtins
;; -------------------------------------------------------------------------

;; PAND - Parallel AND (128-bit)
(define_insn "mmi_pand"
  [(set (match_operand:V2DI 0 "register_operand" "=d")
	(and:V2DI (match_operand:V2DI 1 "register_operand" "d")
		  (match_operand:V2DI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "pand\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; POR - Parallel OR (128-bit)
(define_insn "mmi_por"
  [(set (match_operand:V2DI 0 "register_operand" "=d")
	(ior:V2DI (match_operand:V2DI 1 "register_operand" "d")
		  (match_operand:V2DI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "por\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PXOR - Parallel XOR (128-bit)
(define_insn "mmi_pxor"
  [(set (match_operand:V2DI 0 "register_operand" "=d")
	(xor:V2DI (match_operand:V2DI 1 "register_operand" "d")
		  (match_operand:V2DI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "pxor\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PNOR - Parallel NOR (128-bit)
(define_insn "mmi_pnor"
  [(set (match_operand:V2DI 0 "register_operand" "=d")
	(and:V2DI
	  (not:V2DI (match_operand:V2DI 1 "register_operand" "d"))
	  (not:V2DI (match_operand:V2DI 2 "register_operand" "d"))))]
  "ISA_HAS_MMI"
  "pnor\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; TImode (128-bit scalar) Logical Operations
;; -------------------------------------------------------------------------
;; These enable efficient __int128 logical operations using MMI instructions.

(define_insn "andti3"
  [(set (match_operand:TI 0 "register_operand" "=d")
	(and:TI (match_operand:TI 1 "register_operand" "d")
		(match_operand:TI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "pand\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

(define_insn "iorti3"
  [(set (match_operand:TI 0 "register_operand" "=d")
	(ior:TI (match_operand:TI 1 "register_operand" "d")
		(match_operand:TI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "por\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

(define_insn "xorti3"
  [(set (match_operand:TI 0 "register_operand" "=d")
	(xor:TI (match_operand:TI 1 "register_operand" "d")
		(match_operand:TI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "pxor\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

(define_insn "one_cmplti2"
  [(set (match_operand:TI 0 "register_operand" "=d")
	(not:TI (match_operand:TI 1 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "pnor\t%0,%1,%1"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Parallel Comparison Operations - Explicit Builtins
;; -------------------------------------------------------------------------
;; Comparisons produce all-1s (true) or all-0s (false) per element.

;; PCEQB/H/W - Parallel Compare for Equal
(define_insn "mmi_pceq<mmi_bhw>"
  [(set (match_operand:VMMIBHW 0 "register_operand" "=d")
	(eq:VMMIBHW (match_operand:VMMIBHW 1 "register_operand" "d")
		    (match_operand:VMMIBHW 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "pceq<mmi_bhw>\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PCGTB/H/W - Parallel Compare for Greater Than (signed)
(define_insn "mmi_pcgt<mmi_bhw>"
  [(set (match_operand:VMMIBHW 0 "register_operand" "=d")
	(gt:VMMIBHW (match_operand:VMMIBHW 1 "register_operand" "d")
		    (match_operand:VMMIBHW 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "pcgt<mmi_bhw>\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Parallel Min/Max Operations
;; -------------------------------------------------------------------------
;; Note: R5900 only has halfword and word min/max (no byte variants).

;; PMAXH/W - Parallel Maximum (signed)
(define_insn "mmi_pmax<mmi_hw>"
  [(set (match_operand:VMMIHW 0 "register_operand" "=d")
	(smax:VMMIHW (match_operand:VMMIHW 1 "register_operand" "d")
		     (match_operand:VMMIHW 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "pmax<mmi_hw>\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PMINH/W - Parallel Minimum (signed)
(define_insn "mmi_pmin<mmi_hw>"
  [(set (match_operand:VMMIHW 0 "register_operand" "=d")
	(smin:VMMIHW (match_operand:VMMIHW 1 "register_operand" "d")
		     (match_operand:VMMIHW 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "pmin<mmi_hw>\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Parallel Shift Operations - Explicit Builtins
;; -------------------------------------------------------------------------
;; MMI shifts use a 5-bit immediate shift amount (sa field).
;; Halfword shifts: sa = 0-15, Word shifts: sa = 0-31

;; PSLLH - Parallel Shift Left Logical Halfword (8 x 16-bit)
(define_insn "mmi_psllh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(ashift:V8HI (match_operand:V8HI 1 "register_operand" "d")
		     (match_operand:SI 2 "const_uimm5_operand" "")))]
  "ISA_HAS_MMI"
  "psllh\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PSLLW - Parallel Shift Left Logical Word (4 x 32-bit)
(define_insn "mmi_psllw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(ashift:V4SI (match_operand:V4SI 1 "register_operand" "d")
		     (match_operand:SI 2 "const_uimm5_operand" "")))]
  "ISA_HAS_MMI"
  "psllw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PSRLH - Parallel Shift Right Logical Halfword (8 x 16-bit)
(define_insn "mmi_psrlh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(lshiftrt:V8HI (match_operand:V8HI 1 "register_operand" "d")
		       (match_operand:SI 2 "const_uimm5_operand" "")))]
  "ISA_HAS_MMI"
  "psrlh\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PSRLW - Parallel Shift Right Logical Word (4 x 32-bit)
(define_insn "mmi_psrlw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(lshiftrt:V4SI (match_operand:V4SI 1 "register_operand" "d")
		       (match_operand:SI 2 "const_uimm5_operand" "")))]
  "ISA_HAS_MMI"
  "psrlw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PSRAH - Parallel Shift Right Arithmetic Halfword (8 x 16-bit)
(define_insn "mmi_psrah"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(ashiftrt:V8HI (match_operand:V8HI 1 "register_operand" "d")
		       (match_operand:SI 2 "const_uimm5_operand" "")))]
  "ISA_HAS_MMI"
  "psrah\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PSRAW - Parallel Shift Right Arithmetic Word (4 x 32-bit)
(define_insn "mmi_psraw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(ashiftrt:V4SI (match_operand:V4SI 1 "register_operand" "d")
		       (match_operand:SI 2 "const_uimm5_operand" "")))]
  "ISA_HAS_MMI"
  "psraw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Variable Shift Operations (shift amount in register, word only)
;; -------------------------------------------------------------------------
;; These use per-element shift amounts from a register.

;; PSLLVW - Parallel Shift Left Logical Variable Word
(define_insn "mmi_psllvw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(ashift:V4SI (match_operand:V4SI 1 "register_operand" "d")
		     (match_operand:V4SI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "psllvw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PSRLVW - Parallel Shift Right Logical Variable Word
(define_insn "mmi_psrlvw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(lshiftrt:V4SI (match_operand:V4SI 1 "register_operand" "d")
		       (match_operand:V4SI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "psrlvw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PSRAVW - Parallel Shift Right Arithmetic Variable Word
(define_insn "mmi_psravw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(ashiftrt:V4SI (match_operand:V4SI 1 "register_operand" "d")
		       (match_operand:V4SI 2 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "psravw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

