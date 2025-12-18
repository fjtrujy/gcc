# PS2 R5900 GCC Changes

This document describes all the modifications made on top of GCC release to support the PlayStation 2's Emotion Engine (R5900) processor.

## Overview

The R5900 is a 64-bit MIPS processor with several unique characteristics:
- 32-bit only floating-point unit (no 64-bit float support)
- Does not support MIPS16 instruction set extension
- Does not support CLZ/CLO instructions (count leading zeros/ones)
- Has custom instructions: `rsqrt.s`, `min.s`, `max.s`
- Has a 3-operand multiply instruction
- Known short-loop hardware bug

These changes adapt GCC to properly target this processor for the PS2SDK toolchain.


---

## 1. Single-Precision Float Support

### 32-Bit Float Only

The R5900's FPU only supports 32-bit single-precision floats. This is configured in `libgcc/config/mips/sfp-machine.h`:

```c
#if defined(__mips64) && !defined(_MIPS_ARCH_R5900)
#define _FP_W_TYPE_SIZE 64
...
```

For R5900, the default (32-bit) floating-point word size is used instead of 64-bit.

### libgcc Single-Float Support

Added `t-hardfp-sf` configuration for targets with hardware single-float only. This enables hardware 32-bit float operations while emulating 64-bit double operations in software.

### Test Coverage

- `r5900-float32.c` - Verifies single-precision FP instructions are generated
- `r5900-no-double.c` - Verifies double-precision FP instructions are NOT generated

---

## 2. Hardware Bug Workarounds

### CLZ/CLO Instruction Disable

The R5900 does not properly support Count Leading Zeros (CLZ) and Count Leading Ones (CLO) instructions. Disabled in `include/longlong.h`:

```c
#if (__mips == 32 || __mips == 64) && !defined(__mips16) && !defined(_MIPS_ARCH_R5900)
#define count_leading_zeros(COUNT,X) ((COUNT) = __builtin_clz(X))
```

### Recursive `__muldi3` Prevention

The R5900 requires a custom implementation to prevent recursive calls in 64-bit multiplication:

```c
#ifdef _MIPS_ARCH_R5900
#define __umulsidi3(u, v) \
  ({UDItype __w;                        \
    __asm__ ("multu %1,%2\n\tpmfhl.lw %0" \
             : "=d" (__w)               \
             : "d" ((USItype) (u)),     \
               "d" ((USItype) (v))      \
             : "hi", "lo");             \
    __w; })
#endif
```

This uses the `pmfhl.lw` (Pack Move From HI/LO - Lower Word) instruction specific to the R5900.

### Test Coverage

- `r5900-no-clz.c` - Verifies CLZ/CLO instructions are NOT generated
- `r5900-muldi3.c` - Verifies 64-bit multiplication with `multu`

Fixes by davidgf.

---

## 3. R5900 Instruction Pipeline Scheduling

### File: `gcc/config/mips/5900.md`

Added comprehensive instruction scheduling information for the R5900 processor.

**Defined execution units:**
- `r5900_alu0`, `r5900_alu1` - Two ALU units
- `r5900_c1` - FPU (Coprocessor 1)
- `r5900_br` - Branch unit
- `r5900_ls` - Load/Store unit

**Instruction reservations with cycle counts:**

| Instruction Type | Cycles | Execution Unit |
|------------------|--------|----------------|
| ALU operations | 1 | ALU0 or ALU1 |
| Load/Store | 1 | LS |
| FP Load/Store | 2 | C1 |
| FP Convert | 4 | C1 |
| FP Move/Abs/Neg | 4 | C1 |
| FP Compare | 4 | C1 |
| FP Add | 4 | C1 |
| FP Multiply | 4 | C1 |
| FP Divide | 8 | C1 |
| FP Square Root | 8 | C1 |
| FP Reciprocal Square Root | 14 | C1 |
| Integer Multiply | 4 | ALU0 |
| Integer Divide | 37 | ALU0 |
| Branch/Jump/Call | 1 | BR |
| HI/LO Register | 1 | ALU0 |
| FP Min/Max | 4 | C1 |

### Reciprocal Square Root (`rsqrt.s`)

Added support for the R5900's hardware reciprocal square root instruction:

```asm
rsqrt.s %0, %1, %2   ; %0 = %1 / sqrt(%2)
```

This is used for efficient calculation of `a / sqrt(b)` operations common in 3D graphics.

### Test Coverage

- `r5900-rsqrt.c` - Verifies rsqrt.s instruction is generated
- `r5900-fsqrt.c` - Verifies sqrt.s instruction is generated

---

## 4. Compiler-Level Feature Disabling

### CLZ/CLO Instruction Disable

The R5900 CLZ/CLO instructions are also disabled at the compiler level:

```c
#define ISA_HAS_CLZ_CLO (mips_isa_rev >= 1 && !TARGET_MIPS16 && !TARGET_MIPS5900)
```

### MIPS16 Incompatibility

The R5900 does not support MIPS16 compressed instruction set. Added explicit compile-time error:

```c
if (TARGET_MIPS5900 && ((mips_base_compression_flags & MASK_MIPS16) != 0))
    error("unsupported combination: %s", "-march=r5900 -mips16");
```

### Min/Max Float Instructions (`min.s`, `max.s`)

Enables GCC to use the R5900's hardware min/max float instructions:

```c
#define ISA_HAS_FMIN_FMAX ((mips_isa_rev >= 6) || TARGET_MIPS5900)
```

### Test Coverage

- `r5900-no-mips16.c` - Verifies MIPS16 is rejected with error
- `r5900-minmax.c` - Verifies min.s/max.s instructions are generated

---

## 5. Short-Loop Bug Fix and 3-Operand Multiply

### Short-Loop Bug Fix

The R5900 has a hardware bug related to short loops with branch-likely instructions. This fix prevents GCC from using delay slots in certain branch patterns:

```lisp
(define_delay (and (eq_attr "type" "branch")
                   (not (match_test "TARGET_MIPS16"))
                   (not (match_test "TARGET_FIX_R5900"))  ; <-- Added
                   (eq_attr "branch_likely" "yes"))
  ...)
```

`TARGET_FIX_R5900` is automatically enabled when targeting the R5900.

Short-loop bugfix by frno7.

### 3-Operand Multiply Instruction

Extended the 3-operand multiply pattern to work on R5900 (similar to R3900):

```c
(TARGET_MIPS3900 || TARGET_MIPS5900) && !TARGET_MIPS16
```

This enables efficient 3-operand multiply operations.

### Test Coverage

- `r5900-fix-shortloop.c` - Short-loop bug fix verification
- `r5900-mult.c` - Verifies mult instruction generation
- `r5900-mult3.c` - Verifies 3-operand mult instruction
- `r5900-madd.c` - Verifies multiply instruction patterns

---

## 6. PS2SDK Platform Configuration

### File: `gcc/config/mips/ps2sdk.h`

Created a PS2SDK-specific header that defines default linking behavior:

```c
#define LIB_SPEC "\
    -lm \
    --start-group \
        %{g:-lg} %{!g:-lc} \
        %{pg:-lprofglue} \
        -lcdvd \
        -lpthread \
        -lpthreadglue \
        -lcglue \
        -lkernel \
    --end-group"
```

**Libraries included by default:**
- `libm` - Math library
- `libc` or `libg` (debug) - C library
- `libprofglue` - Profiling support (when `-pg` is used)
- `libcdvd` - CD/DVD access
- `libpthread` / `libpthreadglue` - Threading support
- `libcglue` - C runtime glue layer
- `libkernel` - PS2 kernel interface

### Default Startup Files

```c
#define STARTFILE_SPEC "crt0.o%s crti.o%s crtbegin.o%s"
#define ENDFILE_SPEC "crtend.o%s crtn.o%s"
```

### Builtin Macro

Defines `__ps2sdk__` preprocessor macro for PS2-specific code detection.

### PIC/ABI Configuration

Enables Position Independent Code (PIC) and ABI calls by default for shared object support.

### Test Coverage

- `r5900-basic.c` - Basic R5900 target compilation
