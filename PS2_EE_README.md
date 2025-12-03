# PlayStation 2 Emotion Engine (R5900) GCC Support

This document describes the extra features and instructions present in the PS2 Emotion Engine (EE) Core that are not found in standard MIPS CPUs, along with their current GCC implementation status.

## Overview

The EE Core is based on MIPS III architecture with significant extensions:
- **128-bit GP Registers**: All 32 general-purpose registers are 128-bit wide
- **Dual Pipeline**: Two ALU pipelines (ALU0/ALU1) for parallel integer execution
- **MMI (Multimedia Instructions)**: 128-bit SIMD integer operations
- **COP1 (FPU) Extensions**: Single-precision only, with accumulator and extra operations
- **COP2 (VU0)**: 128-bit vector floating-point unit (4x32-bit floats)
- **No LL/SC**: Load-Linked/Store-Conditional atomics are not available
- **No CLZ/CLO**: Count Leading Zeros/Ones instructions are not available

## Compiler Flags

| Flag | Description |
|------|-------------|
| `-march=r5900` | Target R5900 processor |
| `-mvu0` | Enable VU0 SIMD operations (requires `-march=r5900`) |
| `-mfix-r5900` | Enable R5900 short loop erratum workaround (default on) |
| `-mabi=n32` | Use N32 ABI (recommended for 128-bit register passing) |

---

## R5900-Specific Registers

The R5900 has several register extensions and new registers not found in standard MIPS CPUs.

### General Purpose Registers (128-bit Extended)

| Register | Size | Purpose | GCC Status | GCC Regnum |
|----------|------|---------|------------|------------|
| `$0-$31` | 128-bit | Extended GP registers (64-bit lower + 64-bit upper) | **Implemented** | 0-31 |

Standard MIPS uses 64-bit GP registers; R5900 extends these to 128-bit. GCC uses TImode (`__int128`) to access the full width.

### Multiply/Divide Registers (Dual Pipeline)

| Register | Size | Purpose | GCC Status | GCC Regnum |
|----------|------|---------|------------|------------|
| `HI` | 64-bit | Upper result of multiply/divide (Pipeline 0) | **Implemented** | 64 |
| `LO` | 64-bit | Lower result of multiply/divide (Pipeline 0) | **Implemented** | 65 |
| `HI1` | 64-bit | Upper result of multiply/divide (Pipeline 1) | Not implemented | - |
| `LO1` | 64-bit | Lower result of multiply/divide (Pipeline 1) | Not implemented | - |

The R5900 has dual multiply/divide pipelines. MULT1/DIV1/MADD1 use HI1/LO1.

### Shift Amount Register

| Register | Size | Purpose | GCC Status | GCC Regnum |
|----------|------|---------|------------|------------|
| `SA` | 8-bit | Shift amount for QFSRV (funnel shift) | Not implemented | - |

Used by MTSAB, MTSAH, and QFSRV instructions for 128-bit funnel shifts.

### FPU (COP1) Registers

| Register | Size | Purpose | GCC Status | GCC Regnum |
|----------|------|---------|------------|------------|
| `$f0-$f31` | 32-bit | FP data registers (single-precision only) | **Implemented** | 32-63 |
| `FCR0` | 32-bit | FP Implementation/Revision (read-only) | **Implemented** | - |
| `FCR31` | 32-bit | FP Control/Status | **Implemented** | - |
| `ACC` | 32-bit | FP Accumulator (for ADDA.S, MULA.S, etc.) | **Implemented** | 189 |

Note: R5900 FPU is single-precision only. Double-precision is NOT supported.

### VU0/COP2 Vector Registers

| Register | Size | Purpose | GCC Status | GCC Regnum |
|----------|------|---------|------------|------------|
| `$vf0-$vf31` | 128-bit | Vector FP (4x32-bit floats, xyzw) | **Implemented** | 112-143 |
| `$vi0-$vi15` | 16-bit | Integer registers (counters, addresses) | Not implemented | - |
| `ACC` | 128-bit | Vector accumulator (4x32-bit floats) | **Implemented** | 188 |
| `I` | 32-bit | Immediate FP value (loaded via instruction) | Not implemented | - |
| `Q` | 32-bit | Division/sqrt result register | Not implemented | - |

Special notes:
- `$vf0` is constant: x=0.0, y=0.0, z=0.0, w=1.0
- `$vi0` is constant: always 0
- VF registers use constraint `"C"` in GCC patterns

### VU1-Only Registers (Not Available in Macro Mode)

| Register | Size | Purpose | GCC Status |
|----------|------|---------|------------|
| `R` | 23-bit | Random number generator | N/A (VU1 only) |
| `P` | 32-bit | EFU (Elementary Function Unit) result | N/A (VU1 only) |

VU1 runs independently and is not accessible as a coprocessor from the EE Core.

### COP0 System Control Registers

Standard MIPS COP0 registers are available, plus R5900-specific extensions:

| Register | COP0 Num | Purpose | GCC Status |
|----------|----------|---------|------------|
| `Status` | 12 | Processor status, interrupt control | **Implemented** |
| `Cause` | 13 | Exception cause | **Implemented** |
| `EPC` | 14 | Exception program counter | **Implemented** |
| `Config` | 16 | Processor configuration | **Implemented** |
| `BadPAddr` | 23 | Physical address causing bus error | **Implemented** |
| `PCCR` | 25 | Performance counter control | **Implemented** |
| `PCR0/PCR1` | 25 | Performance counters 0 and 1 | **Implemented** |

### Register Summary

| Category | Total | Implemented | Coverage |
|----------|-------|-------------|----------|
| GP (128-bit) | 32 | 32 | 100% |
| HI/LO (dual pipeline) | 4 | 2 | 50% |
| SA | 1 | 0 | 0% |
| FPU Data | 32 | 32 | 100% |
| FPU ACC | 1 | 1 | 100% |
| VU0 VF | 32 | 32 | 100% |
| VU0 VI | 16 | 0 | 0% |
| VU0 ACC | 1 | 1 | 100% |
| VU0 I/Q | 2 | 0 | 0% |
| COP0 | ~32 | ~32 | ~100% |

---

## Data Types and Modes

The R5900 supports various data types through GCC's machine modes.

### Scalar Integer Types

| Mode | Size | C Type | Register | GCC Status |
|------|------|--------|----------|------------|
| `QImode` | 8-bit | `char` | GP | **Implemented** |
| `HImode` | 16-bit | `short` | GP | **Implemented** |
| `SImode` | 32-bit | `int` | GP | **Implemented** |
| `DImode` | 64-bit | `long long` | GP | **Implemented** |
| `TImode` | 128-bit | `__int128` | GP | **Implemented** (R5900 native, no splitting) |

### Scalar Floating-Point Types

| Mode | Size | C Type | Register | GCC Status | Notes |
|------|------|--------|----------|------------|-------|
| `SFmode` | 32-bit | `float` | FPU | **Implemented** | Native support |
| `DFmode` | 64-bit | `double` | FPU | **NOT supported** | R5900 is single-precision only |

**Important**: The R5900 FPU only supports single-precision. Double-precision operations will be emulated in software.

### Vector Types (VU0)

These require `-march=r5900 -mvu0`:

| Mode | Size | Elements | C Type | Register | GCC Status |
|------|------|----------|--------|----------|------------|
| `V4SF` | 128-bit | 4 × 32-bit float | `float __attribute__((vector_size(16)))` | VU0 (COP2) | **Implemented** |

**V4SF Operations Implemented:**
- Move: `movv4sf` (lqc2/sqc2/vmove/por)
- Arithmetic: `addv4sf3`, `subv4sf3`, `mulv4sf3`
- FMA: `fmav4sf4` (vmulaw + vmadd)
- Unary: `absv4sf2`
- Compare: `smaxv4sf3`, `sminv4sf3`

### Vector Types (MMI - Not Yet Implemented)

These would use the 128-bit GP registers for integer SIMD:

| Mode | Size | Elements | Description | GCC Status |
|------|------|----------|-------------|------------|
| `V16QI` | 128-bit | 16 × 8-bit int | Parallel byte operations | Not implemented |
| `V8HI` | 128-bit | 8 × 16-bit int | Parallel halfword operations | Not implemented |
| `V4SI` | 128-bit | 4 × 32-bit int | Parallel word operations | Not implemented |
| `V2DI` | 128-bit | 2 × 64-bit int | Parallel doubleword operations | Not implemented |

These modes would map to MMI instructions (PADDB, PADDH, PADDW, etc.) but are not currently implemented in GCC.

### Type Usage Examples

```c
// 128-bit integer (uses LQ/SQ instructions)
__int128 quad_value;

// Vector of 4 floats (uses VU0)
typedef float v4sf __attribute__((vector_size(16)));
v4sf vec_a, vec_b, vec_c;
vec_c = vec_a + vec_b;  // Generates: vadd.xyzw

// FMA operation
vec_c = vec_a * vec_b + vec_c;  // Generates: vmulaw.xyzw + vmadd.xyzw
```

### Data Type Summary

| Category | Types | Implemented |
|----------|-------|-------------|
| Integer Scalar | QI, HI, SI, DI, TI | 5/5 (100%) |
| Float Scalar | SF | 1/1 (100%) |
| Double Scalar | DF | 0/1 (0% - not supported by HW) |
| VU0 Vector (float) | V4SF | 1/1 (100%) |
| MMI Vector (int) | V16QI, V8HI, V4SI, V2DI | 0/4 (0%) |

---

## 1. 128-bit Load/Store Instructions

These instructions operate on the full 128-bit width of GP registers.

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `LQ` | Load Quadword (128-bit) | Implemented | Automatic via `__int128` |
| `SQ` | Store Quadword (128-bit) | Implemented | Automatic via `__int128` |

**Usage**: Use `__int128` type or `typedef __int128 int128_t;`

---

## 2. MMI (Multimedia Instructions) - 128-bit Integer SIMD

### 2.1 Parallel Arithmetic

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| **Addition** ||||
| `PADDB` | Parallel Add Byte (16x8-bit) | Not implemented | - |
| `PADDH` | Parallel Add Halfword (8x16-bit) | Not implemented | - |
| `PADDW` | Parallel Add Word (4x32-bit) | Not implemented | - |
| `PADDSB` | Parallel Add Signed Saturation Byte | Not implemented | - |
| `PADDSH` | Parallel Add Signed Saturation Halfword | Not implemented | - |
| `PADDSW` | Parallel Add Signed Saturation Word | Not implemented | - |
| `PADDUB` | Parallel Add Unsigned Saturation Byte | Not implemented | - |
| `PADDUH` | Parallel Add Unsigned Saturation Halfword | Not implemented | - |
| `PADDUW` | Parallel Add Unsigned Saturation Word | Not implemented | - |
| **Subtraction** ||||
| `PSUBB` | Parallel Subtract Byte | Not implemented | - |
| `PSUBH` | Parallel Subtract Halfword | Not implemented | - |
| `PSUBW` | Parallel Subtract Word | Not implemented | - |
| `PSUBSB` | Parallel Subtract Signed Saturation Byte | Not implemented | - |
| `PSUBSH` | Parallel Subtract Signed Saturation Halfword | Not implemented | - |
| `PSUBSW` | Parallel Subtract Signed Saturation Word | Not implemented | - |
| `PSUBUB` | Parallel Subtract Unsigned Saturation Byte | Not implemented | - |
| `PSUBUH` | Parallel Subtract Unsigned Saturation Halfword | Not implemented | - |
| `PSUBUW` | Parallel Subtract Unsigned Saturation Word | Not implemented | - |
| **Absolute Value** ||||
| `PABSH` | Parallel Absolute Halfword | Not implemented | - |
| `PABSW` | Parallel Absolute Word | Not implemented | - |
| **Add/Subtract Combined** ||||
| `PADSBH` | Parallel Add/Subtract Halfword | Not implemented | - |

### 2.2 Parallel Comparison

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `PCEQB` | Parallel Compare Equal Byte | Not implemented | - |
| `PCEQH` | Parallel Compare Equal Halfword | Not implemented | - |
| `PCEQW` | Parallel Compare Equal Word | Not implemented | - |
| `PCGTB` | Parallel Compare Greater Than Byte | Not implemented | - |
| `PCGTH` | Parallel Compare Greater Than Halfword | Not implemented | - |
| `PCGTW` | Parallel Compare Greater Than Word | Not implemented | - |

### 2.3 Parallel Min/Max

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `PMAXH` | Parallel Maximum Halfword | Not implemented | - |
| `PMAXW` | Parallel Maximum Word | Not implemented | - |
| `PMINH` | Parallel Minimum Halfword | Not implemented | - |
| `PMINW` | Parallel Minimum Word | Not implemented | - |

### 2.4 Parallel Logical Operations

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `PAND` | Parallel AND (128-bit) | Not implemented | - |
| `POR` | Parallel OR (128-bit) | Implemented (move) | Automatic |
| `PXOR` | Parallel XOR (128-bit) | Not implemented | - |
| `PNOR` | Parallel NOR (128-bit) | Not implemented | - |

### 2.5 Parallel Shift Operations

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `PSLLH` | Parallel Shift Left Logical Halfword | Not implemented | - |
| `PSLLW` | Parallel Shift Left Logical Word | Not implemented | - |
| `PSLLVW` | Parallel Shift Left Logical Variable Word | Not implemented | - |
| `PSRAH` | Parallel Shift Right Arithmetic Halfword | Not implemented | - |
| `PSRAW` | Parallel Shift Right Arithmetic Word | Not implemented | - |
| `PSRAVW` | Parallel Shift Right Arithmetic Variable Word | Not implemented | - |
| `PSRLH` | Parallel Shift Right Logical Halfword | Not implemented | - |
| `PSRLW` | Parallel Shift Right Logical Word | Not implemented | - |
| `PSRLVW` | Parallel Shift Right Logical Variable Word | Not implemented | - |

### 2.6 Parallel Multiply/Divide

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `PMULTH` | Parallel Multiply Halfword | Not implemented | - |
| `PMULTW` | Parallel Multiply Word | Not implemented | - |
| `PMULTUW` | Parallel Multiply Unsigned Word | Not implemented | - |
| `PMADDH` | Parallel Multiply-Add Halfword | Not implemented | - |
| `PMADDW` | Parallel Multiply-Add Word | Not implemented | - |
| `PMADDUW` | Parallel Multiply-Add Unsigned Word | Not implemented | - |
| `PMSUBH` | Parallel Multiply-Subtract Halfword | Not implemented | - |
| `PMSUBW` | Parallel Multiply-Subtract Word | Not implemented | - |
| `PHMADH` | Parallel Horizontal Multiply-Add Halfword | Not implemented | - |
| `PHMSBH` | Parallel Horizontal Multiply-Subtract Halfword | Not implemented | - |
| `PDIVBW` | Parallel Divide Broadcast Word | Not implemented | - |
| `PDIVW` | Parallel Divide Word | Not implemented | - |
| `PDIVUW` | Parallel Divide Unsigned Word | Not implemented | - |

### 2.7 Parallel Data Movement/Rearrangement

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `PCPYH` | Parallel Copy Halfword | Not implemented | - |
| `PCPYLD` | Parallel Copy Lower Doubleword | Not implemented | - |
| `PCPYUD` | Parallel Copy Upper Doubleword | Not implemented | - |
| `PEXCH` | Parallel Exchange Center Halfword | Not implemented | - |
| `PEXCW` | Parallel Exchange Center Word | Not implemented | - |
| `PEXEH` | Parallel Exchange Even Halfword | Not implemented | - |
| `PEXEW` | Parallel Exchange Even Word | Not implemented | - |
| `PEXTLB` | Parallel Extend Lower from Byte | Not implemented | - |
| `PEXTLH` | Parallel Extend Lower from Halfword | Not implemented | - |
| `PEXTLW` | Parallel Extend Lower from Word | Not implemented | - |
| `PEXTUB` | Parallel Extend Upper from Byte | Not implemented | - |
| `PEXTUH` | Parallel Extend Upper from Halfword | Not implemented | - |
| `PEXTUW` | Parallel Extend Upper from Word | Not implemented | - |
| `PINTH` | Parallel Interleave Halfword | Not implemented | - |
| `PINTEH` | Parallel Interleave Even Halfword | Not implemented | - |
| `PPACB` | Parallel Pack to Byte | Not implemented | - |
| `PPACH` | Parallel Pack to Halfword | Not implemented | - |
| `PPACW` | Parallel Pack to Word | Not implemented | - |
| `PREVH` | Parallel Reverse Halfword | Not implemented | - |
| `PROT3W` | Parallel Rotate 3 Words Left | Not implemented | - |

### 2.8 Parallel Format Conversion

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `PEXT5` | Parallel Extend from 5 bits (RGB555 expand) | Not implemented | - |
| `PPAC5` | Parallel Pack to 5 bits (RGB555 pack) | Not implemented | - |

### 2.9 Other MMI Instructions

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `PLZCW` | Parallel Leading Zero/One Count Word | Not implemented | - |
| `QFSRV` | Quadword Funnel Shift Right Variable | Not implemented | - |

### 2.10 HI/LO Register Operations

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `PMFHI` | Parallel Move From HI Register | Not implemented | - |
| `PMFLO` | Parallel Move From LO Register | Not implemented | - |
| `PMTHI` | Parallel Move To HI Register | Not implemented | - |
| `PMTLO` | Parallel Move To LO Register | Not implemented | - |
| `PMFHL.LW` | Parallel Move From HI/LO (Low Word) | Not implemented | - |
| `PMFHL.UW` | Parallel Move From HI/LO (Upper Word) | Not implemented | - |
| `PMFHL.SLW` | Parallel Move From HI/LO (Signed Low Word) | Not implemented | - |
| `PMFHL.LH` | Parallel Move From HI/LO (Low Halfword) | Not implemented | - |
| `PMFHL.SH` | Parallel Move From HI/LO (Signed Halfword) | Not implemented | - |
| `PMTHL.LW` | Parallel Move To HI/LO (Low Word) | Not implemented | - |

---

## 3. Dual Pipeline Instructions

R5900 has a second multiply/divide unit (Pipeline 1) with dedicated HI1/LO1 registers.

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `MULT1` | Multiply Word Pipeline 1 | Not implemented | - |
| `MULTU1` | Multiply Unsigned Word Pipeline 1 | Not implemented | - |
| `DIV1` | Divide Word Pipeline 1 | Not implemented | - |
| `DIVU1` | Divide Unsigned Word Pipeline 1 | Not implemented | - |
| `MADD` | Multiply-Add Word | Implemented | Automatic |
| `MADD1` | Multiply-Add Word Pipeline 1 | Not implemented | - |
| `MADDU` | Multiply-Add Unsigned Word | Implemented | Automatic |
| `MADDU1` | Multiply-Add Unsigned Word Pipeline 1 | Not implemented | - |
| `MFHI1` | Move From HI1 Register | Not implemented | - |
| `MFLO1` | Move From LO1 Register | Not implemented | - |
| `MTHI1` | Move To HI1 Register | Not implemented | - |
| `MTLO1` | Move To LO1 Register | Not implemented | - |

---

## 4. Shift Amount Register (SA)

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `MFSA` | Move from Shift Amount Register | Not implemented | - |
| `MTSA` | Move to Shift Amount Register | Not implemented | - |
| `MTSAB` | Move Byte Count to SA Register | Not implemented | - |
| `MTSAH` | Move Halfword Count to SA Register | Not implemented | - |

---

## 5. COP1 (FPU) Extensions

R5900 FPU is single-precision only with additional operations. Note: Double precision is NOT supported.

### 5.1 Accumulator Operations

The FPU has a dedicated accumulator register for efficient FMA chains. ACC is a fixed register - use intrinsics only (no automatic optimization).

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `ADDA.S` | Add to Accumulator | **Implemented** | `__builtin_mips_adda_s` |
| `SUBA.S` | Subtract to Accumulator | **Implemented** | `__builtin_mips_suba_s` |
| `MULA.S` | Multiply to Accumulator | **Implemented** | `__builtin_mips_mula_s` |
| `MADD.S` | Multiply-Add (ACC + fs * ft) | **Implemented** | `__builtin_mips_madd_s` |
| `MADDA.S` | Multiply-Add to Accumulator | **Implemented** | `__builtin_mips_madda_s` |
| `MSUB.S` | Multiply-Subtract (ACC - fs * ft) | **Implemented** | `__builtin_mips_msub_s` |
| `MSUBA.S` | Multiply-Subtract to Accumulator | **Implemented** | `__builtin_mips_msuba_s` |

### 5.2 Min/Max/Reciprocal

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `MIN.S` | Floating-Point Minimum | Implemented | Automatic (fminf) |
| `MAX.S` | Floating-Point Maximum | Implemented | Automatic (fmaxf) |
| `RSQRT.S` | Reciprocal Square Root (fd = fs / sqrt(ft)) | Implemented | Automatic |
| `SQRT.S` | Square Root | Standard MIPS | Automatic |

---

## 6. COP2 (VU0) - Vector Floating-Point Unit

VU0 operates on 128-bit vectors containing 4x32-bit single-precision floats (V4SF mode).
Use `-mvu0` flag to enable. Registers: `$vf0`-`$vf31` (32 x 128-bit).

**Note**: `$vf0` is special - `$vf0.w` is always 1.0.

### 6.1 Data Transfer

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `LQC2` | Load Quadword to COP2 | Implemented | Automatic |
| `SQC2` | Store Quadword from COP2 | Implemented | Automatic |
| `QMFC2` | Quadword Move From COP2 to GP | Implemented | Automatic |
| `QMTC2` | Quadword Move To COP2 from GP | Implemented | Automatic |
| `CFC2` | Control Transfer from VU to EE Core | Not implemented | - |
| `CTC2` | Control Transfer from EE Core to VU | Not implemented | - |

### 6.2 Vector Arithmetic (Autovectorized)

These are automatically generated by GCC when autovectorization is enabled.

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `VADD.xyzw` | Vector Add | Implemented | Automatic |
| `VSUB.xyzw` | Vector Subtract | Implemented | Automatic |
| `VMUL.xyzw` | Vector Multiply | Implemented | Automatic |
| `VABS.xyzw` | Vector Absolute Value | Implemented | Automatic |
| `VMAX.xyzw` | Vector Maximum | Implemented | Automatic |
| `VMINI.xyzw` | Vector Minimum | Implemented | Automatic |
| `VMOVE.xyzw` | Vector Move | Implemented | Automatic |

### 6.3 Accumulator Operations

VU0 has an implicit ACC register for efficient FMA chains.

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `VMULA.xyzw` | ACC = a * b | Implemented | `__builtin_vu0_vmula` |
| `VMADDA.xyzw` | ACC += a * b | Implemented | `__builtin_vu0_vmadda` |
| `VMADD.xyzw` | dest = ACC + a * b | Implemented | `__builtin_vu0_vmadd` |

### 6.4 Broadcast Multiply Operations

Multiply vector by broadcast of single component.

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `VMULX.xyzw` | dest = a * b.x | Implemented | `__builtin_vu0_vmulx` |
| `VMULY.xyzw` | dest = a * b.y | Implemented | `__builtin_vu0_vmuly` |
| `VMULZ.xyzw` | dest = a * b.z | Implemented | `__builtin_vu0_vmulz` |
| `VMULW.xyzw` | dest = a * b.w | Implemented | `__builtin_vu0_vmulw` |

### 6.5 Broadcast Multiply to Accumulator

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `VMULAX.xyzw` | ACC = a * b.x | Implemented | `__builtin_vu0_vmulax` |
| `VMULAY.xyzw` | ACC = a * b.y | Implemented | `__builtin_vu0_vmulay` |
| `VMULAZ.xyzw` | ACC = a * b.z | Implemented | `__builtin_vu0_vmulaz` |
| `VMULAW.xyzw` | ACC = a * b.w | Implemented | `__builtin_vu0_vmulaw` |

### 6.6 Broadcast Multiply-Add to Accumulator

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `VMADDAX.xyzw` | ACC += a * b.x | Implemented | `__builtin_vu0_vmaddax` |
| `VMADDAY.xyzw` | ACC += a * b.y | Implemented | `__builtin_vu0_vmadday` |
| `VMADDAZ.xyzw` | ACC += a * b.z | Implemented | `__builtin_vu0_vmaddaz` |
| `VMADDAW.xyzw` | ACC += a * b.w | Implemented | `__builtin_vu0_vmaddaw` |

### 6.7 Broadcast Multiply-Add with Result

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `VMADDX.xyzw` | dest = ACC + a * b.x | Implemented | `__builtin_vu0_vmaddx` |
| `VMADDY.xyzw` | dest = ACC + a * b.y | Implemented | `__builtin_vu0_vmaddy` |
| `VMADDZ.xyzw` | dest = ACC + a * b.z | Implemented | `__builtin_vu0_vmaddz` |
| `VMADDW.xyzw` | dest = ACC + a * b.w | Implemented | `__builtin_vu0_vmaddw` |

### 6.8 Outer Product (Cross Product)

| Instruction | Description | GCC Status | Intrinsic |
|-------------|-------------|------------|-----------|
| `VOPMULA.xyz` | ACC.xyz = a.yzx * b.zxy | Implemented | `__builtin_vu0_vopmula` |
| `VOPMSUB.xyz` | dest.xyz = ACC - a.zxy * b.yzx | Implemented | `__builtin_vu0_vopmsub` |

### 6.9 VU0 Instructions Not Yet Implemented

| Instruction | Description | GCC Status |
|-------------|-------------|------------|
| `VADDi/q/bc` | Add with I/Q register or broadcast | Not implemented |
| `VSUBi/q/bc` | Subtract with I/Q register or broadcast | Not implemented |
| `VMULi/q` | Multiply with I/Q register | Not implemented |
| `VADDA/i/q/bc` | Add to Accumulator variants | Not implemented |
| `VSUBA/i/q/bc` | Subtract to Accumulator variants | Not implemented |
| `VMADDi/q` | Multiply-Add with I/Q register | Not implemented |
| `VMSUB/i/q/bc` | Multiply-Subtract variants | Not implemented |
| `VMSUBA/i/q/bc` | Multiply-Subtract to Accumulator | Not implemented |
| `VDIV` | Vector Divide | Not implemented |
| `VSQRT` | Vector Square Root | Not implemented |
| `VRSQRT` | Vector Reciprocal Square Root | Not implemented |
| `VCLIP` | Clipping Judgment | Not implemented |
| `VFTOI0/4/12/15` | Float to Fixed-Point | Not implemented |
| `VITOF0/4/12/15` | Fixed-Point to Float | Not implemented |
| `VMR32` | Vector Rotate | Not implemented |
| `VIADD/ISUB/IAND/IOR` | Integer operations | Not implemented |
| `VLQI/VSQI` | Load/Store with increment | Not implemented |
| `VLQD/VSQD` | Load/Store with decrement | Not implemented |
| `VILWR/VISWR` | Integer Load/Store | Not implemented |
| `VRINIT/VRGET/VRNEXT/VRXOR` | Random number | Not implemented |
| `VWAITQ` | Wait for Q register | Not implemented |
| `VCALLMS/VCALLMSR` | Call microsubroutine | Not implemented |

### 6.10 VU0 Branch Instructions

| Instruction | Description | GCC Status |
|-------------|-------------|------------|
| `BC2F` | Branch on COP2 False | Not implemented |
| `BC2FL` | Branch on COP2 False Likely | Not implemented |
| `BC2T` | Branch on COP2 True | Not implemented |
| `BC2TL` | Branch on COP2 True Likely | Not implemented |

---

## 7. Fused Multiply-Add (FMA) Autovectorization

GCC can automatically vectorize FMA patterns to VU0. Enable with:
```
-march=r5900 -mvu0 -O2 -ftree-vectorize -ffast-math
```

The compiler generates:
```asm
vmulaw.xyzw  ACC, c, $vf0    ; ACC = c (via multiply by vf0.w=1.0)
vmadd.xyzw   result, a, b    ; result = ACC + a*b = c + a*b
```

---

## 8. Implementation Summary

| Feature Category | Total Instructions | Implemented | Coverage |
|-----------------|-------------------|-------------|----------|
| 128-bit Load/Store | 2 | 2 | 100% |
| MMI Arithmetic | 27 | 0 | 0% |
| MMI Comparison | 6 | 0 | 0% |
| MMI Min/Max | 4 | 0 | 0% |
| MMI Logical | 4 | 1 | 25% |
| MMI Shift | 9 | 0 | 0% |
| MMI Multiply/Divide | 13 | 0 | 0% |
| MMI Data Movement | 23 | 0 | 0% |
| MMI Format Convert | 2 | 0 | 0% |
| MMI Other | 2 | 0 | 0% |
| MMI HI/LO | 10 | 0 | 0% |
| Dual Pipeline | 12 | 2 | 17% |
| SA Register | 4 | 0 | 0% |
| FPU Extensions | 11 | 11 | 100% |
| VU0 Data Transfer | 6 | 4 | 67% |
| VU0 Basic Arithmetic | 7 | 7 | 100% |
| VU0 Accumulator | 3 | 3 | 100% |
| VU0 Broadcast Multiply | 4 | 4 | 100% |
| VU0 Broadcast MulAcc | 4 | 4 | 100% |
| VU0 Broadcast MulAdd | 4 | 4 | 100% |
| VU0 Outer Product | 2 | 2 | 100% |
| VU0 Advanced | ~40 | 0 | 0% |

---

## 9. Usage Examples

### 128-bit Integer

```c
typedef __int128 int128_t;

int128_t add128(int128_t a, int128_t b) {
    return a + b;  // Uses lq/sq for load/store
}
```

### VU0 Vector Operations

```c
typedef float v4sf __attribute__((vector_size(16)));

v4sf add_vectors(v4sf a, v4sf b) {
    return a + b;  // Generates: vadd.xyzw
}

v4sf mul_vectors(v4sf a, v4sf b) {
    return a * b;  // Generates: vmul.xyzw
}

v4sf fma_vectors(v4sf a, v4sf b, v4sf c) {
    return a * b + c;  // Generates: vmulaw.xyzw + vmadd.xyzw
}
```

### VU0 Builtins (Matrix Multiply)

```c
typedef float v4sf __attribute__((vector_size(16)));

v4sf matrix_vector_multiply(v4sf row0, v4sf row1, v4sf row2, v4sf row3, v4sf vec) {
    __builtin_vu0_vmulax(row0, vec);      // ACC = row0 * vec.x
    __builtin_vu0_vmadday(row1, vec);     // ACC += row1 * vec.y
    __builtin_vu0_vmaddaz(row2, vec);     // ACC += row2 * vec.z
    return __builtin_vu0_vmaddw(row3, vec); // result = ACC + row3 * vec.w
}
```

### Cross Product

```c
typedef float v4sf __attribute__((vector_size(16)));

v4sf cross_product(v4sf a, v4sf b) {
    __builtin_vu0_vopmula(a, b);       // ACC.xyz = a.yzx * b.zxy
    return __builtin_vu0_vopmsub(a, b); // result.xyz = ACC - a.zxy * b.yzx
}
```

---

## 10. Known Limitations

1. **No Double Precision**: R5900 FPU is single-precision only
2. **No Atomics**: LL/SC instructions are not available
3. **No CLZ/CLO**: Count leading zeros/ones not available
4. **VU0 Constraints**: Some VU0 features require careful register management
5. **Accumulator Clobber**: VU0 ACC is implicit; intrinsic sequences must be ordered correctly

---

## 11. References

- EE Core Instruction Set Manual (Sony)
- EE Core Users Manual (Sony)
- VU Users Manual (Sony)
- GCC Source: `gcc/config/mips/mips-vu0.md`
- GCC Source: `gcc/config/mips/5900.md`
