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
