# PlayStation 2 Emotion Engine (R5900) GCC Support

This document describes all the modifications made to GCC to support the PlayStation 2's Emotion Engine (R5900) processor, as well as the extra features and instructions present in the EE Core that are not found in standard MIPS CPUs.

## Overview

The R5900 is a MIPS III-based processor with significant extensions:
- **128-bit GP Registers**: All 32 general-purpose registers are 128-bit wide
- **Dual Pipeline**: Two ALU pipelines (ALU0/ALU1) for parallel integer execution
- **MMI (Multimedia Instructions)**: 128-bit SIMD integer operations
- **COP1 (FPU)**: Single-precision only, with accumulator and extra operations
- **COP2 (VU0)**: 128-bit vector floating-point unit (4x32-bit floats)
- **No LL/SC**: Load-Linked/Store-Conditional atomics are not available
- **No CLZ/CLO**: Count Leading Zeros/Ones instructions are not available
- **Short-Loop Bug**: Hardware erratum requiring workaround

These changes adapt GCC to properly target this processor for the PS2SDK toolchain.

---

## Compiler Flags

| Flag | Description | Status |
|------|-------------|--------|
| `-march=r5900` | Target R5900 processor | **Implemented** |
| `-mfix-r5900` | Enable R5900 short loop erratum workaround (default on) | **Implemented** |
| `-mvu0` | Enable VU0 SIMD operations (requires `-march=r5900`) | Not implemented |
| `-mabi=n32` | Use N32 ABI (recommended for 128-bit register passing) | Not implemented |

---

# Part 1: Currently Implemented Changes

This section documents all the changes that have been made to GCC for R5900 support.

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

The R5900 does not properly support Count Leading Zeros (CLZ) and Count Leading Ones (CLO) instructions. Disabled in `gcc/config/mips/mips.h`:

```c
#define ISA_HAS_CLZ_CLO (mips_isa_rev >= 1 && !TARGET_MIPS16 && !TARGET_MIPS5900)
```

Also disabled in `include/longlong.h`:

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

### MIPS16 Incompatibility

The R5900 does not support MIPS16 compressed instruction set. Added explicit compile-time error in `gcc/config/mips/mips.cc`:

```c
if (TARGET_MIPS5900 && ((mips_base_compression_flags & MASK_MIPS16) != 0))
    error("unsupported combination: %s", "-march=r5900 -mips16");
```

### Double-Precision Float Error

The R5900 FPU does not support double-precision floats:

```c
if (TARGET_MIPS5900 && TARGET_HARD_FLOAT_ABI && TARGET_DOUBLE_FLOAT)
    error("unsupported combination: %s", "-march=r5900 -mhard-float -mdouble-float");
```

### Min/Max Float Instructions (`min.s`, `max.s`)

Enables GCC to use the R5900's hardware min/max float instructions in `gcc/config/mips/mips.h`:

```c
#define ISA_HAS_FMIN_FMAX ((mips_isa_rev >= 6) || TARGET_MIPS5900)
```

### Other ISA Features

Various ISA feature macros are configured to disable unsupported R5900 features:
- `ISA_HAS_DMULT` - disabled for R5900 (no 64-bit multiply)
- `ISA_HAS_DDIV` - disabled for R5900 (no 64-bit divide)
- `ISA_HAS_LDC1_SDC1` - disabled for R5900 (no 64-bit FP load/store)
- `ISA_HAS_LL_SC` - disabled for R5900 (no atomic instructions)

### Test Coverage

- `r5900-no-mips16.c` - Verifies MIPS16 is rejected with error
- `r5900-minmax.c` - Verifies min.s/max.s instructions are generated

---

## 5. Short-Loop Bug Fix and 3-Operand Multiply

### Short-Loop Bug Fix

The R5900 has a hardware bug related to short loops with branch-likely instructions. This fix prevents GCC from using delay slots in certain branch patterns in `gcc/config/mips/mips.md`:

```lisp
(define_delay (and (eq_attr "type" "branch")
                   (not (match_test "TARGET_MIPS16"))
                   (not (match_test "TARGET_FIX_R5900"))  ; <-- Added
                   (eq_attr "branch_likely" "yes"))
  ...)
```

`TARGET_FIX_R5900` is automatically enabled when targeting the R5900 via the `-mfix-r5900` flag (enabled by default).

Short-loop bugfix by frno7.

### 3-Operand Multiply Instruction

Extended the 3-operand multiply pattern to work on R5900 (similar to R3900) in `gcc/config/mips/mips.md`:

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

---

## 7. Threading Support

### Disable Weak Symbol Usage

For proper `std::thread` support on PS2, weak symbol usage is disabled in `libgcc/gthr.h`:

```c
/* PS2 Specific change for std::thread */
#undef GTHREAD_USE_WEAK
#define GTHREAD_USE_WEAK 0
```

This ensures thread-related symbols are always resolved at link time rather than using weak references, which is required for the PS2SDK threading implementation.

---

## 8. Atomic Operations

### POSIX Atomic Implementation

The R5900 does not support LL/SC atomic instructions. Modified `libatomic` to use POSIX atomic implementation for PS2:

```bash
# In configure.tgt
*-*-linux* | *-*-gnu* | *-*-k*bsd*-gnu | *-ps2-elf* \  # Added *-ps2-elf*
```

Also added PS2-specific test skipping in `libatomic/configure` because ps2sdk libraries may not be available during the build phase.

### Test Coverage

- `r5900-no-ll-sc.c` - Verifies LL/SC atomic instructions are NOT generated

---

## Summary of Implemented Changes

| File | Purpose |
|------|---------|
| `gcc/config.gcc` | Platform configuration |
| `gcc/config/mips/ps2sdk.h` | PS2SDK-specific defaults |
| `gcc/config/mips/5900.md` | R5900 instruction patterns & scheduling |
| `gcc/config/mips/mips.cc` | MIPS16 compatibility check |
| `gcc/config/mips/mips.h` | CLZ/CLO and FMIN/FMAX config |
| `gcc/config/mips/mips.md` | Short-loop fix and MADD instruction |
| `include/longlong.h` | CLZ disable and __muldi3 fix |
| `libgcc/config.host` | Build configuration |
| `libgcc/config/mips/sfp-machine.h` | 32-bit float configuration |
| `libgcc/config/t-hardfp-sf` | Single-float hardware support |
| `libgcc/configure` | MIPS16 and single-float detection |
| `libgcc/configure.ac` | Build-time checks |
| `libgcc/gthr.h` | Threading support |
| `libatomic/configure` | Atomic operations config |
| `libatomic/configure.tgt` | Target detection |
| `gcc/testsuite/gcc.target/mips/r5900-*.c` | R5900 test suite |

---

# Part 2: R5900-Specific Features (Future Implementation)

This section documents R5900-specific features that are NOT YET implemented in GCC but are available in the hardware.

---

## R5900-Specific Registers

The R5900 has several register extensions and new registers not found in standard MIPS CPUs.

### General Purpose Registers (128-bit Extended)

| Register | Size | Purpose | GCC Status | GCC Regnum | Constraint |
|----------|------|---------|------------|------------|------------|
| `$0-$31` | 128-bit | Extended GP registers (64-bit lower + 64-bit upper) | Not implemented | 0-31 | `"d"`, `"r"` |

Standard MIPS uses 64-bit GP registers; R5900 extends these to 128-bit. Would require TImode (`__int128`) to access the full width.

### Multiply/Divide Registers (Dual Pipeline)

| Register | Size | Purpose | GCC Status | GCC Regnum | Constraint |
|----------|------|---------|------------|------------|------------|
| `HI` | 64-bit | Upper result of multiply/divide (Pipeline 0) | **Implemented** | 64 | `"x"` |
| `LO` | 64-bit | Lower result of multiply/divide (Pipeline 0) | **Implemented** | 65 | `"l"`, `"x"` |
| `HI1` | 64-bit | Upper result of multiply/divide (Pipeline 1) | Not implemented | 190 | `"Ym"` |
| `LO1` | 64-bit | Lower result of multiply/divide (Pipeline 1) | Not implemented | 191 | `"Yl"`, `"Ym"` |

The R5900 has dual multiply/divide pipelines (MAC0 and MAC1). MULT1/DIV1/MADD1 use HI1/LO1 and can execute in parallel with Pipeline 0 operations.

### Shift Amount Register

| Register | Size | Purpose | GCC Status | GCC Regnum | Constraint |
|----------|------|---------|------------|------------|------------|
| `SA` | 8-bit | Shift amount for QFSRV (funnel shift) | Not implemented | - | - |

Used by MTSAB, MTSAH, and QFSRV instructions for 128-bit funnel shifts.

### FPU (COP1) Registers

| Register | Size | Purpose | GCC Status | GCC Regnum | Constraint |
|----------|------|---------|------------|------------|------------|
| `$f0-$f31` | 32-bit | FP data registers (single-precision only) | **Implemented** | 32-63 | `"f"` |
| `FCR0` | 32-bit | FP Implementation/Revision (read-only) | **Implemented** | - | - |
| `FCR31` | 32-bit | FP Control/Status | **Implemented** | - | - |
| `ACC` | 32-bit | FP Accumulator (for ADDA.S, MULA.S, etc.) | Not implemented | 189 | `"YF"` |

Note: R5900 FPU is single-precision only. Double-precision is NOT supported.

### VU0/COP2 Vector Registers

| Register | Size | Purpose | GCC Status | GCC Regnum | Constraint |
|----------|------|---------|------------|------------|------------|
| `$vf0-$vf31` | 128-bit | Vector FP (4x32-bit floats, xyzw) | Not implemented | 112-143 | `"C"` |
| `$vi0-$vi15` | 16-bit | Integer registers (counters, addresses) | Not implemented | - | - |
| `ACC` | 128-bit | Vector accumulator (4x32-bit floats) | Not implemented | 188 | `"Ya"` |
| `Q` | 32-bit | Division/sqrt result register | Not implemented | 192 | `"Yq"` |
| `I` | 32-bit | Immediate FP value (loaded via CTC2) | Not implemented | 193 | `"Yi"` |

Special notes:
- `$vf0` is constant: x=0.0, y=0.0, z=0.0, w=1.0
- `$vi0` is constant: always 0

---

## Data Types

The R5900 supports various data types through GCC's machine modes.

| Mode | Size | C Type | Register | GCC Status |
|------|------|--------|----------|------------|
| `QImode` | 8-bit | `char` | GP | **Implemented** |
| `HImode` | 16-bit | `short` | GP | **Implemented** |
| `SImode` | 32-bit | `int` | GP | **Implemented** |
| `SFmode` | 32-bit | `float` | FPU (COP1) | **Implemented** |
| `DImode` | 64-bit | `long long` | GP | **Implemented** |
| `TImode` | 128-bit | `__int128` | GP | Not implemented |
| `V4SF` | 128-bit | 4 × 32-bit float | VU0 (COP2) only | Not implemented |
| `V16QI` | 128-bit | 16 × 8-bit int | GP (MMI) | Not implemented |
| `V8HI` | 128-bit | 8 × 16-bit int | GP (MMI) | Not implemented |
| `V4SI` | 128-bit | 4 × 32-bit int | GP (MMI) | Not implemented |
| `V2DI` | 128-bit | 2 × 64-bit int | GP (MMI) | Not implemented |

---

## 128-bit Load/Store Instructions

These instructions operate on the full 128-bit width of GP registers.

| Instruction | Description | GCC Status |
|-------------|-------------|------------|
| `LQ` | Load Quadword (128-bit) | Not implemented |
| `SQ` | Store Quadword (128-bit) | Not implemented |

**Future use cases:**
- Callee-saved register preservation using LQ/SQ
- Unaligned 128-bit access using QFSRV

---

## MMI (Multimedia Instructions) - 128-bit Integer SIMD

### Arithmetic

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `PADDB` | Parallel Add Byte | `__builtin_mmi_paddb(a, b)` | Not implemented |
| `PSUBB` | Parallel Subtract Byte | `__builtin_mmi_psubb(a, b)` | Not implemented |
| `PADDH` | Parallel Add Halfword | `__builtin_mmi_paddh(a, b)` | Not implemented |
| `PSUBH` | Parallel Subtract Halfword | `__builtin_mmi_psubh(a, b)` | Not implemented |
| `PADDW` | Parallel Add Word | `__builtin_mmi_paddw(a, b)` | Not implemented |
| `PSUBW` | Parallel Subtract Word | `__builtin_mmi_psubw(a, b)` | Not implemented |
| `PADSBH` | Parallel Add/Subtract Halfword | `__builtin_mmi_padsbh(a, b)` | Not implemented |
| `PADDSB` | Parallel Add with Signed Saturation Byte | `__builtin_mmi_paddsb(a, b)` | Not implemented |
| `PSUBSB` | Parallel Subtract with Signed Saturation Byte | `__builtin_mmi_psubsb(a, b)` | Not implemented |
| `PADDSH` | Parallel Add with Signed Saturation Halfword | `__builtin_mmi_paddsh(a, b)` | Not implemented |
| `PSUBSH` | Parallel Subtract with Signed Saturation Halfword | `__builtin_mmi_psubsh(a, b)` | Not implemented |
| `PADDSW` | Parallel Add with Signed Saturation Word | `__builtin_mmi_paddsw(a, b)` | Not implemented |
| `PSUBSW` | Parallel Subtract with Signed Saturation Word | `__builtin_mmi_psubsw(a, b)` | Not implemented |
| `PADDUB` | Parallel Add with Unsigned Saturation Byte | `__builtin_mmi_paddub(a, b)` | Not implemented |
| `PSUBUB` | Parallel Subtract with Unsigned Saturation Byte | `__builtin_mmi_psubub(a, b)` | Not implemented |
| `PADDUH` | Parallel Add with Unsigned Saturation Halfword | `__builtin_mmi_padduh(a, b)` | Not implemented |
| `PSUBUH` | Parallel Subtract with Unsigned Saturation Halfword | `__builtin_mmi_psubuh(a, b)` | Not implemented |
| `PADDUW` | Parallel Add with Unsigned Saturation Word | `__builtin_mmi_padduw(a, b)` | Not implemented |
| `PSUBUW` | Parallel Subtract with Unsigned Saturation Word | `__builtin_mmi_psubuw(a, b)` | Not implemented |

### Multiply and Divide

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `PMULTW` | Parallel Multiply Word | `__builtin_mmi_pmultw(a, b)` | Not implemented |
| `PMULTUW` | Parallel Multiply Unsigned Word | `__builtin_mmi_pmultuw(a, b)` | Not implemented |
| `PDIVW` | Parallel Divide Word | `__builtin_mmi_pdivw(a, b)` | Not implemented |
| `PDIVUW` | Parallel Divide Unsigned Word | `__builtin_mmi_pdivuw(a, b)` | Not implemented |
| `PMADDW` | Parallel Multiply-Add Word | `__builtin_mmi_pmaddw(a, b)` | Not implemented |
| `PMADDUW` | Parallel Multiply-Add Unsigned Word | `__builtin_mmi_pmadduw(a, b)` | Not implemented |
| `PMSUBW` | Parallel Multiply-Subtract Word | `__builtin_mmi_pmsubw(a, b)` | Not implemented |
| `PMULTH` | Parallel Multiply Halfword | `__builtin_mmi_pmulth(a, b)` | Not implemented |
| `PMADDH` | Parallel Multiply-Add Halfword | `__builtin_mmi_pmaddh(a, b)` | Not implemented |
| `PMSUBH` | Parallel Multiply-Subtract Halfword | `__builtin_mmi_pmsubh(a, b)` | Not implemented |
| `PHMADH` | Parallel Horizontal Multiply-Add Halfword | `__builtin_mmi_phmadh(a, b)` | Not implemented |
| `PHMSBH` | Parallel Horizontal Multiply-Subtract Halfword | `__builtin_mmi_phmsbh(a, b)` | Not implemented |
| `PDIVBW` | Parallel Divide Broadcast Word | `__builtin_mmi_pdivbw(a, b)` | Not implemented |
| `PMFHI` | Parallel Move From HI Register | `__builtin_mmi_pmfhi()` | Not implemented |
| `PMFLO` | Parallel Move From LO Register | `__builtin_mmi_pmflo()` | Not implemented |
| `PMTHI` | Parallel Move To HI Register | `__builtin_mmi_pmthi(v)` | Not implemented |
| `PMTLO` | Parallel Move To LO Register | `__builtin_mmi_pmtlo(v)` | Not implemented |
| `PMFHL.LW` | Parallel Move From HI/LO (Low Word) | `__builtin_mmi_pmfhl_lw()` | Not implemented |
| `PMFHL.UW` | Parallel Move From HI/LO (Upper Word) | `__builtin_mmi_pmfhl_uw()` | Not implemented |
| `PMFHL.SLW` | Parallel Move From HI/LO (Signed Low Word) | `__builtin_mmi_pmfhl_slw()` | Not implemented |
| `PMFHL.LH` | Parallel Move From HI/LO (Low Halfword) | `__builtin_mmi_pmfhl_lh()` | Not implemented |
| `PMFHL.SH` | Parallel Move From HI/LO (Signed Halfword) | `__builtin_mmi_pmfhl_sh()` | Not implemented |
| `PMTHL.LW` | Parallel Move To HI/LO (Low Word) | `__builtin_mmi_pmthl_lw(v)` | Not implemented |

### Shift Operations

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `PSLLH` | Parallel Shift Left Logical Halfword | `__builtin_mmi_psllh(a, n)` | Not implemented |
| `PSRLH` | Parallel Shift Right Logical Halfword | `__builtin_mmi_psrlh(a, n)` | Not implemented |
| `PSRAH` | Parallel Shift Right Arithmetic Halfword | `__builtin_mmi_psrah(a, n)` | Not implemented |
| `PSLLW` | Parallel Shift Left Logical Word | `__builtin_mmi_psllw(a, n)` | Not implemented |
| `PSLLVW` | Parallel Shift Left Logical Variable Word | `__builtin_mmi_psllvw(a, b)` | Not implemented |
| `PSRLW` | Parallel Shift Right Logical Word | `__builtin_mmi_psrlw(a, n)` | Not implemented |
| `PSRLVW` | Parallel Shift Right Logical Variable Word | `__builtin_mmi_psrlvw(a, b)` | Not implemented |
| `PSRAW` | Parallel Shift Right Arithmetic Word | `__builtin_mmi_psraw(a, n)` | Not implemented |
| `PSRAVW` | Parallel Shift Right Arithmetic Variable Word | `__builtin_mmi_psravw(a, b)` | Not implemented |

### SA Register Operations

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `MFSA` | Move From SA Register | `__builtin_mmi_mfsa()` | Not implemented |
| `MTSA` | Move To SA Register | `__builtin_mmi_mtsa(v)` | Not implemented |
| `MTSAB` | Move Byte Count to SA Register | `__builtin_mmi_mtsab(a, b)` | Not implemented |
| `MTSAH` | Move Halfword Count to SA Register | `__builtin_mmi_mtsah(a, b)` | Not implemented |
| `QFSRV` | Quadword Funnel Shift Right Variable | `__builtin_mmi_qfsrv(a, b)` | Not implemented |

### Min/Max, Logical, Absolute

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `PABSH` | Parallel Absolute Halfword | `__builtin_mmi_pabsh(a)` | Not implemented |
| `PABSW` | Parallel Absolute Word | `__builtin_mmi_pabsw(a)` | Not implemented |
| `PMAXH` | Parallel Maximum Halfword | `__builtin_mmi_pmaxh(a, b)` | Not implemented |
| `PMINH` | Parallel Minimum Halfword | `__builtin_mmi_pminh(a, b)` | Not implemented |
| `PMAXW` | Parallel Maximum Word | `__builtin_mmi_pmaxw(a, b)` | Not implemented |
| `PMINW` | Parallel Minimum Word | `__builtin_mmi_pminw(a, b)` | Not implemented |
| `PAND` | Parallel AND | `__builtin_mmi_pand(a, b)` | Not implemented |
| `POR` | Parallel OR | `__builtin_mmi_por(a, b)` | Not implemented |
| `PXOR` | Parallel XOR | `__builtin_mmi_pxor(a, b)` | Not implemented |
| `PNOR` | Parallel NOR | `__builtin_mmi_pnor(a, b)` | Not implemented |

### Compare

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `PCGTB` | Parallel Compare for Greater Than Byte | `__builtin_mmi_pcgtb(a, b)` | Not implemented |
| `PCEQB` | Parallel Compare for Equal Byte | `__builtin_mmi_pceqb(a, b)` | Not implemented |
| `PCGTH` | Parallel Compare for Greater Than Halfword | `__builtin_mmi_pcgth(a, b)` | Not implemented |
| `PCEQH` | Parallel Compare for Equal Halfword | `__builtin_mmi_pceqh(a, b)` | Not implemented |
| `PCGTW` | Parallel Compare for Greater Than Word | `__builtin_mmi_pcgtw(a, b)` | Not implemented |
| `PCEQW` | Parallel Compare for Equal Word | `__builtin_mmi_pceqw(a, b)` | Not implemented |
| `PLZCW` | Parallel Leading Zero Count Word | - | Not implemented |

### Data Rearrangement

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `PPACB` | Parallel Pack to Byte | `__builtin_mmi_ppacb(a, b)` | Not implemented |
| `PPACH` | Parallel Pack to Halfword | `__builtin_mmi_ppach(a, b)` | Not implemented |
| `PPACW` | Parallel Pack to Word | `__builtin_mmi_ppacw(a, b)` | Not implemented |
| `PPAC5` | Parallel Pack to 5 bits (RGB555 pack) | `__builtin_mmi_ppac5(a)` | Not implemented |
| `PEXTLB` | Parallel Extend Lower from Byte | `__builtin_mmi_pextlb(a, b)` | Not implemented |
| `PEXTLH` | Parallel Extend Lower from Halfword | `__builtin_mmi_pextlh(a, b)` | Not implemented |
| `PEXTLW` | Parallel Extend Lower from Word | `__builtin_mmi_pextlw(a, b)` | Not implemented |
| `PEXTUB` | Parallel Extend Upper from Byte | `__builtin_mmi_pextub(a, b)` | Not implemented |
| `PEXTUH` | Parallel Extend Upper from Halfword | `__builtin_mmi_pextuh(a, b)` | Not implemented |
| `PEXTUW` | Parallel Extend Upper from Word | `__builtin_mmi_pextuw(a, b)` | Not implemented |
| `PEXT5` | Parallel Extend from 5 bits (RGB555 expand) | `__builtin_mmi_pext5(a)` | Not implemented |
| `PCPYH` | Parallel Copy Halfword | `__builtin_mmi_pcpyh(a)` | Not implemented |
| `PCPYLD` | Parallel Copy Lower Doubleword | `__builtin_mmi_pcpyld(a, b)` | Not implemented |
| `PCPYUD` | Parallel Copy Upper Doubleword | `__builtin_mmi_pcpyud(a, b)` | Not implemented |
| `PEXCH` | Parallel Exchange Center Halfword | `__builtin_mmi_pexch(a)` | Not implemented |
| `PEXCW` | Parallel Exchange Center Word | `__builtin_mmi_pexcw(a)` | Not implemented |
| `PEXEH` | Parallel Exchange Even Halfword | `__builtin_mmi_pexeh(a)` | Not implemented |
| `PEXEW` | Parallel Exchange Even Word | `__builtin_mmi_pexew(a)` | Not implemented |
| `PREVH` | Parallel Reverse Halfword | `__builtin_mmi_prevh(a)` | Not implemented |
| `PINTEH` | Parallel Interleave Even Halfword | `__builtin_mmi_pinteh(a, b)` | Not implemented |
| `PINTH` | Parallel Interleave Halfword | `__builtin_mmi_pinth(a, b)` | Not implemented |
| `PROT3W` | Parallel Rotate 3 Words | `__builtin_mmi_prot3w(a)` | Not implemented |

---

## Dual Pipeline Instructions

R5900 has two multiply/divide units (MAC0/Pipeline 0 and MAC1/Pipeline 1) with dedicated HI/LO registers.

### Pipeline 0 (MAC0) - Currently Implemented

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `MULT` | Multiply Word (signed) | `__builtin_mips_mult(a, b)` | **Implemented** |
| `MULTU` | Multiply Word (unsigned) | `__builtin_mips_multu(a, b)` | **Implemented** |
| `DIV` | Divide Word (signed) | `__builtin_mips_div(a, b)` | **Implemented** |
| `DIVU` | Divide Word (unsigned) | `__builtin_mips_divu(a, b)` | **Implemented** |
| `MADD` | Multiply-Add (signed) | `__builtin_mips_madd(a, b)` | **Implemented** |
| `MADDU` | Multiply-Add (unsigned) | `__builtin_mips_maddu(a, b)` | **Implemented** |
| `MFHI` | Move From HI0 | `__builtin_mips_mfhi()` | **Implemented** |
| `MFLO` | Move From LO0 | `__builtin_mips_mflo()` | **Implemented** |
| `MTHI` | Move To HI0 | `__builtin_mips_mthi(v)` | **Implemented** |
| `MTLO` | Move To LO0 | `__builtin_mips_mtlo(v)` | **Implemented** |

### Pipeline 1 (MAC1) - Not Implemented

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `MULT1` | Multiply Word (signed) | `__builtin_mips_mult1(a, b)` | Not implemented |
| `MULTU1` | Multiply Word (unsigned) | `__builtin_mips_multu1(a, b)` | Not implemented |
| `DIV1` | Divide Word (signed) | `__builtin_mips_div1(a, b)` | Not implemented |
| `DIVU1` | Divide Word (unsigned) | `__builtin_mips_divu1(a, b)` | Not implemented |
| `MADD1` | Multiply-Add (signed) | `__builtin_mips_madd1(a, b)` | Not implemented |
| `MADDU1` | Multiply-Add (unsigned) | `__builtin_mips_maddu1(a, b)` | Not implemented |
| `MFHI1` | Move From HI1 | `__builtin_mips_mfhi1()` | Not implemented |
| `MFLO1` | Move From LO1 | `__builtin_mips_mflo1()` | Not implemented |
| `MTHI1` | Move To HI1 | `__builtin_mips_mthi1(v)` | Not implemented |
| `MTLO1` | Move To LO1 | `__builtin_mips_mtlo1(v)` | Not implemented |

---

## COP1 (FPU) - Floating-Point Unit

R5900 FPU is single-precision only with additional operations.

### Standard FPU Operations (Implemented)

| Instruction | Description | GCC Status |
|-------------|-------------|------------|
| `ADD.S` | Floating-Point Add | **Implemented** |
| `SUB.S` | Floating-Point Subtract | **Implemented** |
| `MUL.S` | Floating-Point Multiply | **Implemented** |
| `DIV.S` | Floating-Point Divide | **Implemented** |
| `SQRT.S` | Square Root | **Implemented** |
| `ABS.S` | Absolute Value | **Implemented** |
| `NEG.S` | Negate | **Implemented** |
| `MOV.S` | Move | **Implemented** |
| `CVT.W.S` | Convert to Word | **Implemented** |
| `CVT.S.W` | Convert from Word | **Implemented** |
| `C.cond.S` | FP Compare | **Implemented** |

### R5900-Specific FPU Operations

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `MIN.S` | Floating-Point Minimum | `__builtin_mips_min_s(a, b)` | **Implemented** |
| `MAX.S` | Floating-Point Maximum | `__builtin_mips_max_s(a, b)` | **Implemented** |
| `RSQRT.S` | Reciprocal Square Root (fd = fs / sqrt(ft)) | - | **Implemented** |

### Accumulator Operations (Not Implemented)

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `ADDA.S` | Add to Accumulator | `__builtin_mips_adda_s(a, b)` | Not implemented |
| `SUBA.S` | Subtract to Accumulator | `__builtin_mips_suba_s(a, b)` | Not implemented |
| `MULA.S` | Multiply to Accumulator | `__builtin_mips_mula_s(a, b)` | Not implemented |
| `MADD.S` | Multiply-Add (ACC + fs * ft) | `__builtin_mips_madd_s(a, b)` | Not implemented |
| `MADDA.S` | Multiply-Add to Accumulator | `__builtin_mips_madda_s(a, b)` | Not implemented |
| `MSUB.S` | Multiply-Subtract (ACC - fs * ft) | `__builtin_mips_msub_s(a, b)` | Not implemented |
| `MSUBA.S` | Multiply-Subtract to Accumulator | `__builtin_mips_msuba_s(a, b)` | Not implemented |

---

## COP2 (VU0) - Vector Floating-Point Unit

VU0 operates on 128-bit vectors containing 4x32-bit single-precision floats (V4SF mode).
Would use `-mvu0` flag to enable. Registers: `$vf0`-`$vf31` (32 × 128-bit).

**Note**: `$vf0` is special - `$vf0.w` is always 1.0.

### Data Transfer (Not Implemented)

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `LQC2` | Load Quadword to COP2 | - | Not implemented |
| `SQC2` | Store Quadword from COP2 | - | Not implemented |
| `QMFC2` | Quadword Move From COP2 to GP | - | Not implemented |
| `QMTC2` | Quadword Move To COP2 from GP | - | Not implemented |
| `CFC2` | Control Transfer from VU to EE Core | - | Not implemented |
| `CTC2` | Control Transfer from EE Core to VU | - | Not implemented |

### Vector Arithmetic (Not Implemented)

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `VADD.xyzw` | dest = a + b | `__builtin_vu0_vadd(a, b)` | Not implemented |
| `VSUB.xyzw` | dest = a - b | `__builtin_vu0_vsub(a, b)` | Not implemented |
| `VMUL.xyzw` | dest = a * b | `__builtin_vu0_vmul(a, b)` | Not implemented |
| `VABS.xyzw` | dest = \|a\| | `__builtin_vu0_vabs(a)` | Not implemented |
| `VMAX.xyzw` | dest = max(a, b) | `__builtin_vu0_vmax(a, b)` | Not implemented |
| `VMINI.xyzw` | dest = min(a, b) | `__builtin_vu0_vmini(a, b)` | Not implemented |
| `VMOVE.xyzw` | dest = src | `__builtin_vu0_vmove(a)` | Not implemented |

### Broadcast Arithmetic (Not Implemented)

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `VADDbc.xyzw` | dest = a + b.bc | `__builtin_vu0_vaddbc(a, b)` | Not implemented |
| `VSUBbc.xyzw` | dest = a - b.bc | `__builtin_vu0_vsubbc(a, b)` | Not implemented |
| `VMULbc.xyzw` | dest = a * b.bc | `__builtin_vu0_vmulbc(a, b)` | Not implemented |

### Multiply-Accumulate (Not Implemented)

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `VMULA.xyzw` | ACC = a * b | `__builtin_vu0_vmula(a, b)` | Not implemented |
| `VMULAbc.xyzw` | ACC = a * b.bc | `__builtin_vu0_mulabc(a, b)` | Not implemented |
| `VMADDA.xyzw` | ACC += a * b | `__builtin_vu0_vmadda(a, b)` | Not implemented |
| `VMADDAbc.xyzw` | ACC += a * b.bc | `__builtin_vu0_vmaddabc(a, b)` | Not implemented |
| `VMADD.xyzw` | dest = ACC + a * b | `__builtin_vu0_vmadd(a, b)` | Not implemented |
| `VMADDbc.xyzw` | dest = ACC + a * b.bc | `__builtin_vu0_vmaddbc(a, b)` | Not implemented |
| `VMSUBA.xyzw` | ACC -= a * b | `__builtin_vu0_vmsuba(a, b)` | Not implemented |
| `VMSUBAbc.xyzw` | ACC -= a * b.bc | `__builtin_vu0_vmsubabc(a, b)` | Not implemented |
| `VMSUB.xyzw` | dest = ACC - a * b | `__builtin_vu0_vmsub(a, b)` | Not implemented |
| `VMSUBbc.xyzw` | dest = ACC - a * b.bc | `__builtin_vu0_vmsubbc(a, b)` | Not implemented |

### Outer Product (Cross Product) (Not Implemented)

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `VOPMULA.xyz` | ACC.xyz = a.yzx * b.zxy | `__builtin_vu0_vopmula(a, b)` | Not implemented |
| `VOPMSUB.xyz` | dest.xyz = ACC - a.zxy * b.yzx | `__builtin_vu0_vopmsub(a, b)` | Not implemented |

### Conversion Operations (Not Implemented)

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `VFTOI0.xyzw` | Float to 32-bit integer | `__builtin_vu0_vftoi0(a)` | Not implemented |
| `VFTOI4.xyzw` | Float to 28.4 fixed-point | `__builtin_vu0_vftoi4(a)` | Not implemented |
| `VFTOI12.xyzw` | Float to 20.12 fixed-point | `__builtin_vu0_vftoi12(a)` | Not implemented |
| `VFTOI15.xyzw` | Float to 17.15 fixed-point | `__builtin_vu0_vftoi15(a)` | Not implemented |
| `VITOF0.xyzw` | 32-bit integer to float | `__builtin_vu0_vitof0(a)` | Not implemented |
| `VITOF4.xyzw` | 28.4 fixed-point to float | `__builtin_vu0_vitof4(a)` | Not implemented |
| `VITOF12.xyzw` | 20.12 fixed-point to float | `__builtin_vu0_vitof12(a)` | Not implemented |
| `VITOF15.xyzw` | 17.15 fixed-point to float | `__builtin_vu0_vitof15(a)` | Not implemented |

### Q Register Operations (Not Implemented)

| Instruction | Description | Intrinsic | GCC Status |
|-------------|-------------|-----------|------------|
| `VDIV` | Q = fs.bc / ft.bc | `__builtin_vu0_vdiv(fs, fs_bc, ft, ft_bc)` | Not implemented |
| `VSQRT` | Q = sqrt(ft.bc) | `__builtin_vu0_vsqrt(ft, ft_bc)` | Not implemented |
| `VRSQRT` | Q = fs.bc / sqrt(ft.bc) | `__builtin_vu0_vrsqrt(fs, fs_bc, ft, ft_bc)` | Not implemented |
| `VWAITQ` | Wait for Q register ready | `__builtin_vu0_vwaitq()` | Not implemented |
| `VADDq.xyzw` | dest = a + Q | `__builtin_vu0_vaddq(a)` | Not implemented |
| `VSUBq.xyzw` | dest = a - Q | `__builtin_vu0_vsubq(a)` | Not implemented |
| `VMULq.xyzw` | dest = a * Q | `__builtin_vu0_vmulq(a)` | Not implemented |

---

## Contributors

The following developers contributed to these PS2/R5900 changes:

- **Francisco Javier Trujillo Mata** (fjtrujy)
- **Rick Gaiser** (Maximus32)
- **Ziemas**
- **uyjulian**
- **sp193** (original libgcc patch)
- **davidgf** (CLZ/CLO and __muldi3 fixes)
- **frno7** (short-loop bugfix)

---

## Running the Tests

```bash
# Run all R5900 tests
cd build-mips64r5900el-ps2-elf-stage2
make check-gcc RUNTESTFLAGS="mips.exp=r5900*"

# Run a specific test
make check-gcc RUNTESTFLAGS="mips.exp=r5900-rsqrt.c"
```

---

## Building

These changes are used by the PS2SDK toolchain. The typical target triplet is:

```
mips64r5900el-ps2-elf
```

Standard GCC build procedures apply with appropriate `--target` configuration.

---

## References

- EE Core Instruction Set Manual (Sony)
- EE Core Users Manual (Sony)
- VU Users Manual (Sony)
- GCC Source: `gcc/config/mips/5900.md`
