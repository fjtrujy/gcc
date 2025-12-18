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
