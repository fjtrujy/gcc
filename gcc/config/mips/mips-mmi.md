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
;; Mode Iterators and Attributes for Pack/Unpack Autovectorization
;; -------------------------------------------------------------------------

;; Source modes for pack operations (wider types that pack to narrower)
(define_mode_iterator MMI_DWH [V2DI V4SI V8HI])

;; Source modes for unpack operations (narrower types that unpack to wider)
(define_mode_iterator MMI_WHB [V4SI V8HI V16QI])

;; Half-width mode (result of packing)
(define_mode_attr MMI_VHMODE
  [(V8HI "V16QI")
   (V4SI "V8HI")
   (V2DI "V4SI")])

;; Double-width mode (result of unpacking)
(define_mode_attr MMI_VDMODE
  [(V4SI "V2DI")
   (V8HI "V4SI")
   (V16QI "V8HI")])

;; Truncated mode (same element count, half element width)
(define_mode_attr MMI_VTRUNCMODE
  [(V8HI "V8QI")
   (V4SI "V4HI")
   (V2DI "V2SI")])

;; Doubled-width concat mode for vec_select patterns
(define_mode_attr MMI_VCONCATMODE
  [(V16QI "V32QI")
   (V8HI "V16HI")
   (V4SI "V8SI")])

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
;; Parallel Absolute Value - Explicit Builtins
;; -------------------------------------------------------------------------

;; PABSH - Parallel Absolute Halfword
(define_insn "mmi_pabsh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(abs:V8HI (match_operand:V8HI 1 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "pabsh\t%0,%1"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PABSW - Parallel Absolute Word
(define_insn "mmi_pabsw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(abs:V4SI (match_operand:V4SI 1 "register_operand" "d")))]
  "ISA_HAS_MMI"
  "pabsw\t%0,%1"
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

;; -------------------------------------------------------------------------
;; SA Register Operations (for QFSRV)
;; -------------------------------------------------------------------------
;; The SA (Shift Amount) register is used by QFSRV for variable 128-bit
;; funnel shifts. It must be set via MTSAB (byte count) or MTSAH (halfword
;; count) before using QFSRV.

;; MTSAB - Move Byte Count to Shift Amount Register
;; SA = (rs[3:0] XOR imm[3:0]) * 8
;; Common usage: mtsab $0, N  (sets shift to N bytes)
;;               mtsab $r, 0  (sets shift from register)
(define_insn "mmi_mtsab"
  [(unspec_volatile [(match_operand:SI 0 "register_operand" "d")
		     (match_operand:SI 1 "const_int_operand" "")]
		    UNSPEC_MMI_MTSAB)]
  "ISA_HAS_MMI"
  "mtsab\t%0,%1"
  [(set_attr "type" "arith")
   (set_attr "mode" "SI")])

;; MTSAH - Move Halfword Count to Shift Amount Register
;; SA = (rs[2:0] XOR imm[2:0]) * 16
;; Common usage: mtsah $0, N  (sets shift to N halfwords)
;;               mtsah $r, 0  (sets shift from register)
(define_insn "mmi_mtsah"
  [(unspec_volatile [(match_operand:SI 0 "register_operand" "d")
		     (match_operand:SI 1 "const_int_operand" "")]
		    UNSPEC_MMI_MTSAH)]
  "ISA_HAS_MMI"
  "mtsah\t%0,%1"
  [(set_attr "type" "arith")
   (set_attr "mode" "SI")])

;; MFSA - Move From Shift Amount Register
;; Used for context save/restore
(define_insn "mmi_mfsa"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec_volatile:SI [(const_int 0)] UNSPEC_MMI_MFSA))]
  "ISA_HAS_MMI"
  "mfsa\t%0"
  [(set_attr "type" "arith")
   (set_attr "mode" "SI")])

;; MTSA - Move To Shift Amount Register
;; Used for context restore only (not for setting shift amount directly)
(define_insn "mmi_mtsa"
  [(unspec_volatile [(match_operand:SI 0 "register_operand" "d")]
		    UNSPEC_MMI_MTSA)]
  "ISA_HAS_MMI"
  "mtsa\t%0"
  [(set_attr "type" "arith")
   (set_attr "mode" "SI")])

;; -------------------------------------------------------------------------
;; QFSRV - Quadword Funnel Shift Right Variable
;; -------------------------------------------------------------------------
;; rd = (rs || rt) >> SA
;; Concatenates rs (high) and rt (low) into 256 bits, shifts right by SA
;; register value, and stores the low 128 bits in rd.
;;
;; Primary use case: Unaligned 128-bit loads
;;   1. Load two aligned quadwords containing the unaligned data
;;   2. Set SA register with misalignment using MTSAB
;;   3. Use QFSRV to extract correctly aligned data
;;
;; Also useful for 128-bit rotation when rs == rt

(define_insn "mmi_qfsrv"
  [(set (match_operand:TI 0 "register_operand" "=d")
	(unspec:TI [(match_operand:TI 1 "register_operand" "d")
		    (match_operand:TI 2 "register_operand" "d")]
		   UNSPEC_MMI_QFSRV))]
  "ISA_HAS_MMI"
  "qfsrv\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Unaligned 128-bit Memory Access using QFSRV
;; -------------------------------------------------------------------------
;; R5900's LQ/SQ require 16-byte alignment. For unaligned access, we use:
;;   Load:  LQ + LQ + MTSAB + QFSRV (optimized)
;;   Store: Regular move (may trap on truly misaligned access)
;; CRITICAL: Never FAIL - use emit_move_insn as fallback to avoid ICE.

(define_expand "movmisalignti"
  [(set (match_operand:TI 0 "nonimmediate_operand")
	(match_operand:TI 1 "nonimmediate_operand"))]
  "ISA_HAS_MMI"
{
  /* Handle mem-to-mem: force source to register first */
  if (MEM_P (operands[0]) && MEM_P (operands[1]))
    operands[1] = force_reg (TImode, operands[1]);

  /* Load: Use optimized QFSRV sequence */
  if (REG_P (operands[0]) && MEM_P (operands[1]))
    {
      if (mips_expand_movmisalign_ti (operands[0], operands[1]))
	DONE;
    }
  /* Store or fallback: Use regular move */
  emit_move_insn (operands[0], operands[1]);
  DONE;
})

;; Unaligned 128-bit access for V4SF (VU0 float vector).
;; Uses QFSRV for loads, regular moves for stores.
;; V4SF is not in MSA_NO_V4SF so it needs its own pattern.

(define_expand "movmisalignv4sf"
  [(set (match_operand:V4SF 0 "nonimmediate_operand")
	(match_operand:V4SF 1 "nonimmediate_operand"))]
  "ISA_HAS_MMI"
{
  /* Handle mem-to-mem: force source to register first */
  if (MEM_P (operands[0]) && MEM_P (operands[1]))
    operands[1] = force_reg (V4SFmode, operands[1]);

  /* Load: Use optimized QFSRV sequence */
  if (REG_P (operands[0]) && MEM_P (operands[1]))
    {
      if (mips_expand_movmisalign_128 (operands[0], operands[1], V4SFmode))
	DONE;
    }
  /* Store or fallback: Use regular move */
  emit_move_insn (operands[0], operands[1]);
  DONE;
})

;; -------------------------------------------------------------------------
;; Data Rearrangement Instructions
;; -------------------------------------------------------------------------
;; These instructions reorganize data within 128-bit registers without
;; performing arithmetic operations. They're used for packing, unpacking,
;; interleaving, shuffling, and copying vector elements.

;; -------------------------------------------------------------------------
;; Pack Instructions
;; -------------------------------------------------------------------------
;; Pack operations take elements from two source registers and combine
;; them into a single destination register with narrower elements.

;; PPACB - Parallel Pack to Byte
;; Takes low-order bytes from each halfword in rs and rt, packs into rd
(define_insn "mmi_ppacb"
  [(set (match_operand:V16QI 0 "register_operand" "=d")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "d")
		       (match_operand:V16QI 2 "register_operand" "d")]
		      UNSPEC_MMI_PPACB))]
  "ISA_HAS_MMI"
  "ppacb\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PPACH - Parallel Pack to Halfword
;; Takes low-order halfwords from each word in rs and rt, packs into rd
(define_insn "mmi_ppach"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "d")
		      (match_operand:V8HI 2 "register_operand" "d")]
		     UNSPEC_MMI_PPACH))]
  "ISA_HAS_MMI"
  "ppach\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PPACW - Parallel Pack to Word
;; Takes low-order words from each doubleword in rs and rt, packs into rd
(define_insn "mmi_ppacw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "d")
		      (match_operand:V4SI 2 "register_operand" "d")]
		     UNSPEC_MMI_PPACW))]
  "ISA_HAS_MMI"
  "ppacw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PPAC5 - Parallel Pack to 5 bits
;; Packs 4 words in 8-8-8-8 format to 4 halfwords in 1-5-5-5 format (RGB555)
(define_insn "mmi_ppac5"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(unspec:V8HI [(match_operand:V4SI 1 "register_operand" "d")]
		     UNSPEC_MMI_PPAC5))]
  "ISA_HAS_MMI"
  "ppac5\t%0,%1"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Extend Lower Instructions
;; -------------------------------------------------------------------------
;; Extend operations interleave elements from the lower 64 bits of two
;; source registers into the full 128-bit destination.

;; PEXTLB - Parallel Extend Lower from Byte
;; Interleaves bytes from lower 64 bits of rs and rt
(define_insn "mmi_pextlb"
  [(set (match_operand:V16QI 0 "register_operand" "=d")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "d")
		       (match_operand:V16QI 2 "register_operand" "d")]
		      UNSPEC_MMI_PEXTLB))]
  "ISA_HAS_MMI"
  "pextlb\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PEXTLH - Parallel Extend Lower from Halfword
;; Interleaves halfwords from lower 64 bits of rs and rt
(define_insn "mmi_pextlh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "d")
		      (match_operand:V8HI 2 "register_operand" "d")]
		     UNSPEC_MMI_PEXTLH))]
  "ISA_HAS_MMI"
  "pextlh\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PEXTLW - Parallel Extend Lower from Word
;; Interleaves words from lower 64 bits of rs and rt
(define_insn "mmi_pextlw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "d")
		      (match_operand:V4SI 2 "register_operand" "d")]
		     UNSPEC_MMI_PEXTLW))]
  "ISA_HAS_MMI"
  "pextlw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Extend Upper Instructions
;; -------------------------------------------------------------------------
;; Extend operations interleave elements from the upper 64 bits of two
;; source registers into the full 128-bit destination.

;; PEXTUB - Parallel Extend Upper from Byte
;; Interleaves bytes from upper 64 bits of rs and rt
(define_insn "mmi_pextub"
  [(set (match_operand:V16QI 0 "register_operand" "=d")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "d")
		       (match_operand:V16QI 2 "register_operand" "d")]
		      UNSPEC_MMI_PEXTUB))]
  "ISA_HAS_MMI"
  "pextub\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PEXTUH - Parallel Extend Upper from Halfword
;; Interleaves halfwords from upper 64 bits of rs and rt
(define_insn "mmi_pextuh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "d")
		      (match_operand:V8HI 2 "register_operand" "d")]
		     UNSPEC_MMI_PEXTUH))]
  "ISA_HAS_MMI"
  "pextuh\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PEXTUW - Parallel Extend Upper from Word
;; Interleaves words from upper 64 bits of rs and rt
(define_insn "mmi_pextuw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "d")
		      (match_operand:V4SI 2 "register_operand" "d")]
		     UNSPEC_MMI_PEXTUW))]
  "ISA_HAS_MMI"
  "pextuw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PEXT5 - Parallel Extend from 5 bits
;; Expands 4 halfwords in 1-5-5-5 format to 4 words in 8-8-8-8 format (RGB555)
(define_insn "mmi_pext5"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(unspec:V4SI [(match_operand:V8HI 1 "register_operand" "d")]
		     UNSPEC_MMI_PEXT5))]
  "ISA_HAS_MMI"
  "pext5\t%0,%1"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Copy Instructions
;; -------------------------------------------------------------------------
;; Copy operations duplicate or combine doublewords/halfwords.

;; PCPYH - Parallel Copy Halfword
;; Broadcasts low halfword of each doubleword to all halfwords in that doubleword
(define_insn "mmi_pcpyh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "d")]
		     UNSPEC_MMI_PCPYH))]
  "ISA_HAS_MMI"
  "pcpyh\t%0,%1"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PCPYLD - Parallel Copy Lower Doubleword
;; Concatenates low doubleword of rs (to high) with low doubleword of rt (to low)
(define_insn "mmi_pcpyld"
  [(set (match_operand:V2DI 0 "register_operand" "=d")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "d")
		      (match_operand:V2DI 2 "register_operand" "d")]
		     UNSPEC_MMI_PCPYLD))]
  "ISA_HAS_MMI"
  "pcpyld\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PCPYUD - Parallel Copy Upper Doubleword
;; Concatenates high doubleword of rs (to high) with high doubleword of rt (to low)
(define_insn "mmi_pcpyud"
  [(set (match_operand:V2DI 0 "register_operand" "=d")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "d")
		      (match_operand:V2DI 2 "register_operand" "d")]
		     UNSPEC_MMI_PCPYUD))]
  "ISA_HAS_MMI"
  "pcpyud\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Exchange Instructions
;; -------------------------------------------------------------------------
;; Exchange operations swap elements within a single register.

;; PEXCH - Parallel Exchange Center Halfword
;; Exchanges center halfwords within each doubleword
(define_insn "mmi_pexch"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "d")]
		     UNSPEC_MMI_PEXCH))]
  "ISA_HAS_MMI"
  "pexch\t%0,%1"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PEXCW - Parallel Exchange Center Word
;; Exchanges center words (words 1 and 2 of 4)
(define_insn "mmi_pexcw"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "d")]
		     UNSPEC_MMI_PEXCW))]
  "ISA_HAS_MMI"
  "pexcw\t%0,%1"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PEXEH - Parallel Exchange Even Halfword
;; Exchanges even-positioned halfwords within each doubleword
(define_insn "mmi_pexeh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "d")]
		     UNSPEC_MMI_PEXEH))]
  "ISA_HAS_MMI"
  "pexeh\t%0,%1"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PEXEW - Parallel Exchange Even Word
;; Exchanges even-positioned words (words 0 and 2)
(define_insn "mmi_pexew"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "d")]
		     UNSPEC_MMI_PEXEW))]
  "ISA_HAS_MMI"
  "pexew\t%0,%1"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PREVH - Parallel Reverse Halfword
;; Reverses order of all 8 halfwords
(define_insn "mmi_prevh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "d")]
		     UNSPEC_MMI_PREVH))]
  "ISA_HAS_MMI"
  "prevh\t%0,%1"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Interleave Instructions
;; -------------------------------------------------------------------------
;; Interleave operations combine elements from two source registers
;; by alternating elements.

;; PINTEH - Parallel Interleave Even Halfword
;; Interleaves even halfwords from rs and rt at word level
(define_insn "mmi_pinteh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "d")
		      (match_operand:V8HI 2 "register_operand" "d")]
		     UNSPEC_MMI_PINTEH))]
  "ISA_HAS_MMI"
  "pinteh\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; PINTH - Parallel Interleave Halfword
;; Interleaves halfwords from upper 64 bits of rs with lower 64 bits of rt
(define_insn "mmi_pinth"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "d")
		      (match_operand:V8HI 2 "register_operand" "d")]
		     UNSPEC_MMI_PINTH))]
  "ISA_HAS_MMI"
  "pinth\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Rotation Instructions
;; -------------------------------------------------------------------------

;; PROT3W - Parallel Rotate 3 Words Left
;; Rotates the three lower words left by one position; word 3 unchanged
;; [w3 w2 w1 w0] -> [w3 w1 w0 w2]
(define_insn "mmi_prot3w"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "d")]
		     UNSPEC_MMI_PROT3W))]
  "ISA_HAS_MMI"
  "prot3w\t%0,%1"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; Autovectorization Patterns for Data Rearrangement
;; -------------------------------------------------------------------------
;; These patterns use standard optab names that GCC's autovectorizer recognizes.
;; They enable automatic selection of PEXTL*/PEXTU*/PPAC* instructions.

;; -------------------------------------------------------------------------
;; vec_interleave_low - Interleave lower halves (uses PEXTL*)
;; -------------------------------------------------------------------------
;; Interleaves elements from the lower halves of two vectors.
;; For V16QI: takes bytes 0-7 from each input, interleaves to produce bytes 0-15
;; For V8HI: takes halfwords 0-3 from each input, interleaves to produce 0-7
;; For V4SI: takes words 0-1 from each input, interleaves to produce 0-3

(define_insn "vec_interleave_low_v16qi"
  [(set (match_operand:V16QI 0 "register_operand" "=d")
	(vec_select:V16QI
	  (vec_concat:V32QI
	    (match_operand:V16QI 1 "register_operand" "d")
	    (match_operand:V16QI 2 "register_operand" "d"))
	  (parallel [(const_int 0) (const_int 16)
		     (const_int 1) (const_int 17)
		     (const_int 2) (const_int 18)
		     (const_int 3) (const_int 19)
		     (const_int 4) (const_int 20)
		     (const_int 5) (const_int 21)
		     (const_int 6) (const_int 22)
		     (const_int 7) (const_int 23)])))]
  "ISA_HAS_MMI"
  "pextlb\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

(define_insn "vec_interleave_low_v8hi"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(vec_select:V8HI
	  (vec_concat:V16HI
	    (match_operand:V8HI 1 "register_operand" "d")
	    (match_operand:V8HI 2 "register_operand" "d"))
	  (parallel [(const_int 0) (const_int 8)
		     (const_int 1) (const_int 9)
		     (const_int 2) (const_int 10)
		     (const_int 3) (const_int 11)])))]
  "ISA_HAS_MMI"
  "pextlh\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

(define_insn "vec_interleave_low_v4si"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(vec_select:V4SI
	  (vec_concat:V8SI
	    (match_operand:V4SI 1 "register_operand" "d")
	    (match_operand:V4SI 2 "register_operand" "d"))
	  (parallel [(const_int 0) (const_int 4)
		     (const_int 1) (const_int 5)])))]
  "ISA_HAS_MMI"
  "pextlw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; vec_interleave_high - Interleave upper halves (uses PEXTU*)
;; -------------------------------------------------------------------------
;; Interleaves elements from the upper halves of two vectors.
;; For V16QI: takes bytes 8-15 from each input, interleaves to produce bytes 0-15
;; For V8HI: takes halfwords 4-7 from each input, interleaves to produce 0-7
;; For V4SI: takes words 2-3 from each input, interleaves to produce 0-3

(define_insn "vec_interleave_high_v16qi"
  [(set (match_operand:V16QI 0 "register_operand" "=d")
	(vec_select:V16QI
	  (vec_concat:V32QI
	    (match_operand:V16QI 1 "register_operand" "d")
	    (match_operand:V16QI 2 "register_operand" "d"))
	  (parallel [(const_int 8) (const_int 24)
		     (const_int 9) (const_int 25)
		     (const_int 10) (const_int 26)
		     (const_int 11) (const_int 27)
		     (const_int 12) (const_int 28)
		     (const_int 13) (const_int 29)
		     (const_int 14) (const_int 30)
		     (const_int 15) (const_int 31)])))]
  "ISA_HAS_MMI"
  "pextub\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

(define_insn "vec_interleave_high_v8hi"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(vec_select:V8HI
	  (vec_concat:V16HI
	    (match_operand:V8HI 1 "register_operand" "d")
	    (match_operand:V8HI 2 "register_operand" "d"))
	  (parallel [(const_int 4) (const_int 12)
		     (const_int 5) (const_int 13)
		     (const_int 6) (const_int 14)
		     (const_int 7) (const_int 15)])))]
  "ISA_HAS_MMI"
  "pextuh\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

(define_insn "vec_interleave_high_v4si"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(vec_select:V4SI
	  (vec_concat:V8SI
	    (match_operand:V4SI 1 "register_operand" "d")
	    (match_operand:V4SI 2 "register_operand" "d"))
	  (parallel [(const_int 2) (const_int 6)
		     (const_int 3) (const_int 7)])))]
  "ISA_HAS_MMI"
  "pextuw\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; -------------------------------------------------------------------------
;; vec_pack_trunc - Pack with truncation (narrowing)
;; -------------------------------------------------------------------------
;; vec_pack_trunc patterns are defined in mips-msa.md with the condition
;; "ISA_HAS_MSA || ISA_HAS_MMI". The mips_expand_vec_pack_trunc() function
;; in mips.cc handles both MSA and MMI cases, emitting PPACB/PPACH/PPACW.

;; -------------------------------------------------------------------------
;; vec_unpack - Unpack/widen operations
;; -------------------------------------------------------------------------
;; vec_unpack patterns are defined in mips-msa.md with the condition
;; "ISA_HAS_MSA || ISA_HAS_MMI". The mips_expand_vec_unpack() function
;; in mips.cc handles both MSA and MMI cases.

;; =========================================================================
;; Parallel Move From/To HI/LO Registers
;; =========================================================================
;; The R5900 has 128-bit HI and LO registers (parallel to the 128-bit GPRs).
;; These instructions move data between GPRs and the parallel HI/LO registers.
;; We use unspec_volatile because HI/LO are implicit state not tracked by GCC.

;; PMFHI - Parallel Move From HI Register (128-bit)
(define_insn "mmi_pmfhi"
  [(set (match_operand:TI 0 "register_operand" "=d")
	(unspec_volatile:TI [(const_int 0)] UNSPEC_MMI_PMFHI))]
  "ISA_HAS_MMI"
  "pmfhi\t%0"
  [(set_attr "type" "mfhi")
   (set_attr "mode" "TI")])

;; PMFLO - Parallel Move From LO Register (128-bit)
(define_insn "mmi_pmflo"
  [(set (match_operand:TI 0 "register_operand" "=d")
	(unspec_volatile:TI [(const_int 0)] UNSPEC_MMI_PMFLO))]
  "ISA_HAS_MMI"
  "pmflo\t%0"
  [(set_attr "type" "mflo")
   (set_attr "mode" "TI")])

;; PMTHI - Parallel Move To HI Register (128-bit)
(define_insn "mmi_pmthi"
  [(unspec_volatile [(match_operand:TI 0 "register_operand" "d")]
		    UNSPEC_MMI_PMTHI)]
  "ISA_HAS_MMI"
  "pmthi\t%0"
  [(set_attr "type" "mtlo")
   (set_attr "mode" "TI")])

;; PMTLO - Parallel Move To LO Register (128-bit)
(define_insn "mmi_pmtlo"
  [(unspec_volatile [(match_operand:TI 0 "register_operand" "d")]
		    UNSPEC_MMI_PMTLO)]
  "ISA_HAS_MMI"
  "pmtlo\t%0"
  [(set_attr "type" "mtlo")
   (set_attr "mode" "TI")])

;; PMFHL.LW - Parallel Move From HI/LO (Low Word)
;; Extracts low 32-bit words from each 64-bit HI/LO pair, producing 2 x 64-bit
(define_insn "mmi_pmfhl_lw"
  [(set (match_operand:V2DI 0 "register_operand" "=d")
	(unspec_volatile:V2DI [(const_int 0)] UNSPEC_MMI_PMFHL_LW))]
  "ISA_HAS_MMI"
  "pmfhl.lw\t%0"
  [(set_attr "type" "mflo")
   (set_attr "mode" "TI")])

;; PMFHL.UW - Parallel Move From HI/LO (Upper Word)
;; Extracts upper 32-bit words from each 64-bit HI/LO pair
(define_insn "mmi_pmfhl_uw"
  [(set (match_operand:V2DI 0 "register_operand" "=d")
	(unspec_volatile:V2DI [(const_int 0)] UNSPEC_MMI_PMFHL_UW))]
  "ISA_HAS_MMI"
  "pmfhl.uw\t%0"
  [(set_attr "type" "mflo")
   (set_attr "mode" "TI")])

;; PMFHL.SLW - Parallel Move From HI/LO (Signed Low Word with saturation)
(define_insn "mmi_pmfhl_slw"
  [(set (match_operand:V2DI 0 "register_operand" "=d")
	(unspec_volatile:V2DI [(const_int 0)] UNSPEC_MMI_PMFHL_SLW))]
  "ISA_HAS_MMI"
  "pmfhl.slw\t%0"
  [(set_attr "type" "mflo")
   (set_attr "mode" "TI")])

;; PMFHL.LH - Parallel Move From HI/LO (Low Halfword)
;; Packs low 16-bit halfwords from each 32-bit word in HI/LO
;; Explicit use of HI/LO prevents DCE from removing preceding writes.
(define_insn "mmi_pmfhl_lh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(unspec_volatile:V8HI [(reg:TI 64) (reg:TI 65)] UNSPEC_MMI_PMFHL_LH))]
  "ISA_HAS_MMI"
  "pmfhl.lh\t%0"
  [(set_attr "type" "mflo")
   (set_attr "mode" "TI")])

;; PMFHL.SH - Parallel Move From HI/LO (Signed Halfword with saturation)
(define_insn "mmi_pmfhl_sh"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(unspec_volatile:V8HI [(const_int 0)] UNSPEC_MMI_PMFHL_SH))]
  "ISA_HAS_MMI"
  "pmfhl.sh\t%0"
  [(set_attr "type" "mflo")
   (set_attr "mode" "TI")])

;; PMTHL.LW - Parallel Move To HI/LO (Low Word)
;; Writes low 32-bit words to each 64-bit HI/LO pair
(define_insn "mmi_pmthl_lw"
  [(unspec_volatile [(match_operand:V4SI 0 "register_operand" "d")]
		    UNSPEC_MMI_PMTHL_LW)]
  "ISA_HAS_MMI"
  "pmthl.lw\t%0"
  [(set_attr "type" "mtlo")
   (set_attr "mode" "TI")])

;; =========================================================================
;; Parallel Multiply Instructions
;; =========================================================================
;; R5900 parallel multiply instructions produce widened results.
;; PMULTW: 2 x 32-bit -> 2 x 64-bit (result in rd, HI, LO)
;; PMULTH: 8 x 16-bit -> 8 x 32-bit (4 results in rd, 4 in HI/LO)
;; We model rd as the primary output, with HI/LO as clobbers.
;; Users who need HI/LO contents can use PMFHI/PMFLO builtins after.

;; PMULTW - Parallel Multiply Word (signed)
;; Multiplies rs[31:0] * rt[31:0] and rs[95:64] * rt[95:64]
;; Produces two 64-bit products in rd
(define_insn "mmi_pmultw"
  [(set (match_operand:V2DI 0 "register_operand" "=d")
	(unspec:V2DI [(match_operand:V4SI 1 "register_operand" "d")
		      (match_operand:V4SI 2 "register_operand" "d")]
		     UNSPEC_MMI_PMULTW))
   (clobber (reg:TI 64))
   (clobber (reg:TI 65))]
  "ISA_HAS_MMI"
  "pmultw\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "TI")])

;; PMULTUW - Parallel Multiply Unsigned Word
(define_insn "mmi_pmultuw"
  [(set (match_operand:V2DI 0 "register_operand" "=d")
	(unspec:V2DI [(match_operand:V4SI 1 "register_operand" "d")
		      (match_operand:V4SI 2 "register_operand" "d")]
		     UNSPEC_MMI_PMULTUW))
   (clobber (reg:TI 64))
   (clobber (reg:TI 65))]
  "ISA_HAS_MMI"
  "pmultuw\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "TI")])

;; PMULTH - Parallel Multiply Halfword (signed)
;; Multiplies 8 pairs of 16-bit halfwords, producing 8 x 32-bit results
;; 4 results (even indices) go to rd, 4 (odd indices) to HI/LO
;; Uses set instead of clobber for HI/LO to establish data dependency
;; with PMFHL instructions.
(define_insn "mmi_pmulth"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(unspec:V4SI [(match_operand:V8HI 1 "register_operand" "d")
		      (match_operand:V8HI 2 "register_operand" "d")]
		     UNSPEC_MMI_PMULTH))
   (set (reg:TI 64) (unspec:TI [(match_dup 1) (match_dup 2)] UNSPEC_MMI_PMULTH))
   (set (reg:TI 65) (unspec:TI [(match_dup 1) (match_dup 2)] UNSPEC_MMI_PMULTH))]
  "ISA_HAS_MMI"
  "pmulth\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "TI")])

;; V8HI multiply implementation for MMI (used by unified mulv8hi3 expand)
;; Uses PMULTH (widening to 32-bit) + PMFHL.LH (pack low 16-bits)
;; Uses match_scratch for temp register which is allocated before reload.
;; HI/LO are set (not clobbered) to establish data dependency with PMFHL.
(define_insn_and_split "mmi_mulv8hi3_internal"
  [(set (match_operand:V8HI 0 "register_operand" "=d")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "d")
		      (match_operand:V8HI 2 "register_operand" "d")]
		     UNSPEC_MMI_PMULTH))
   (clobber (match_scratch:V4SI 3 "=&d"))
   (set (reg:TI 64) (unspec:TI [(match_dup 1) (match_dup 2)] UNSPEC_MMI_PMULTH))
   (set (reg:TI 65) (unspec:TI [(match_dup 1) (match_dup 2)] UNSPEC_MMI_PMULTH))]
  "ISA_HAS_MMI"
  "#"
  "&& reload_completed"
  [(const_int 0)]
{
  /* Use the pre-allocated scratch register (operands[3]) */
  emit_insn (gen_mmi_pmulth (operands[3], operands[1], operands[2]));
  emit_insn (gen_mmi_pmfhl_lh (operands[0]));
  DONE;
}
  [(set_attr "type" "imul")
   (set_attr "mode" "TI")])

;; =========================================================================
;; Parallel Multiply-Add/Subtract Instructions
;; =========================================================================
;; These instructions accumulate into the HI/LO registers.
;; (GPR[rd], HI, LO) <- (HI, LO) +/- GPR[rs] * GPR[rt]

;; PMADDW - Parallel Multiply-Add Word (signed)
(define_insn "mmi_pmaddw"
  [(set (match_operand:V2DI 0 "register_operand" "=d")
	(unspec:V2DI [(match_operand:V4SI 1 "register_operand" "d")
		      (match_operand:V4SI 2 "register_operand" "d")]
		     UNSPEC_MMI_PMADDW))
   (clobber (reg:TI 64))
   (clobber (reg:TI 65))]
  "ISA_HAS_MMI"
  "pmaddw\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "TI")])

;; PMADDUW - Parallel Multiply-Add Unsigned Word
(define_insn "mmi_pmadduw"
  [(set (match_operand:V2DI 0 "register_operand" "=d")
	(unspec:V2DI [(match_operand:V4SI 1 "register_operand" "d")
		      (match_operand:V4SI 2 "register_operand" "d")]
		     UNSPEC_MMI_PMADDUW))
   (clobber (reg:TI 64))
   (clobber (reg:TI 65))]
  "ISA_HAS_MMI"
  "pmadduw\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "TI")])

;; PMADDH - Parallel Multiply-Add Halfword (signed)
(define_insn "mmi_pmaddh"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(unspec:V4SI [(match_operand:V8HI 1 "register_operand" "d")
		      (match_operand:V8HI 2 "register_operand" "d")]
		     UNSPEC_MMI_PMADDH))
   (clobber (reg:TI 64))
   (clobber (reg:TI 65))]
  "ISA_HAS_MMI"
  "pmaddh\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "TI")])

;; PMSUBW - Parallel Multiply-Subtract Word (signed)
(define_insn "mmi_pmsubw"
  [(set (match_operand:V2DI 0 "register_operand" "=d")
	(unspec:V2DI [(match_operand:V4SI 1 "register_operand" "d")
		      (match_operand:V4SI 2 "register_operand" "d")]
		     UNSPEC_MMI_PMSUBW))
   (clobber (reg:TI 64))
   (clobber (reg:TI 65))]
  "ISA_HAS_MMI"
  "pmsubw\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "TI")])

;; PMSUBH - Parallel Multiply-Subtract Halfword (signed)
(define_insn "mmi_pmsubh"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(unspec:V4SI [(match_operand:V8HI 1 "register_operand" "d")
		      (match_operand:V8HI 2 "register_operand" "d")]
		     UNSPEC_MMI_PMSUBH))
   (clobber (reg:TI 64))
   (clobber (reg:TI 65))]
  "ISA_HAS_MMI"
  "pmsubh\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "TI")])

;; =========================================================================
;; Parallel Horizontal Multiply Instructions
;; =========================================================================
;; These multiply adjacent halfword pairs and add/subtract the results.

;; PHMADH - Parallel Horizontal Multiply-Add Halfword
;; Multiplies adjacent pairs, adds results: (a*b + c*d) for each quad
(define_insn "mmi_phmadh"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(unspec:V4SI [(match_operand:V8HI 1 "register_operand" "d")
		      (match_operand:V8HI 2 "register_operand" "d")]
		     UNSPEC_MMI_PHMADH))
   (clobber (reg:TI 64))
   (clobber (reg:TI 65))]
  "ISA_HAS_MMI"
  "phmadh\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "TI")])

;; PHMSBH - Parallel Horizontal Multiply-Subtract Halfword
;; Multiplies adjacent pairs, subtracts results: (a*b - c*d) for each quad
(define_insn "mmi_phmsbh"
  [(set (match_operand:V4SI 0 "register_operand" "=d")
	(unspec:V4SI [(match_operand:V8HI 1 "register_operand" "d")
		      (match_operand:V8HI 2 "register_operand" "d")]
		     UNSPEC_MMI_PHMSBH))
   (clobber (reg:TI 64))
   (clobber (reg:TI 65))]
  "ISA_HAS_MMI"
  "phmsbh\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "TI")])

;; =========================================================================
;; Parallel Divide Instructions
;; =========================================================================
;; Division results go to HI (remainder) and LO (quotient).
;; No destination register, so we use unspec_volatile with no output.
;; Users must use PMFHI/PMFLO builtins to retrieve results.

;; PDIVW - Parallel Divide Word (signed)
;; Divides rs[31:0]/rt[31:0] and rs[95:64]/rt[95:64]
;; Quotients in LO, remainders in HI
(define_insn "mmi_pdivw"
  [(unspec_volatile [(match_operand:V4SI 0 "register_operand" "d")
		     (match_operand:V4SI 1 "register_operand" "d")]
		    UNSPEC_MMI_PDIVW)]
  "ISA_HAS_MMI"
  "pdivw\t%0,%1"
  [(set_attr "type" "idiv")
   (set_attr "mode" "TI")])

;; PDIVUW - Parallel Divide Unsigned Word
(define_insn "mmi_pdivuw"
  [(unspec_volatile [(match_operand:V4SI 0 "register_operand" "d")
		     (match_operand:V4SI 1 "register_operand" "d")]
		    UNSPEC_MMI_PDIVUW)]
  "ISA_HAS_MMI"
  "pdivuw\t%0,%1"
  [(set_attr "type" "idiv")
   (set_attr "mode" "TI")])

;; PDIVBW - Parallel Divide Broadcast Word
;; Divides four 32-bit words in rs by the low 16-bit halfword of rt
;; Four quotients in LO, four 16-bit remainders in HI
(define_insn "mmi_pdivbw"
  [(unspec_volatile [(match_operand:V4SI 0 "register_operand" "d")
		     (match_operand:V8HI 1 "register_operand" "d")]
		    UNSPEC_MMI_PDIVBW)]
  "ISA_HAS_MMI"
  "pdivbw\t%0,%1"
  [(set_attr "type" "idiv")
   (set_attr "mode" "TI")])

