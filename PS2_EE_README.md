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

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `LQ` | Load Quadword (128-bit) | - | Automatic (`__int128`) |
| `SQ` | Store Quadword (128-bit) | - | Automatic (`__int128`) |

---

## 2. MMI (Multimedia Instructions) - 128-bit Integer SIMD

### 2.1 Parallel Arithmetic

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| **Addition** ||||
| `PADDB` | Parallel Add Byte (16x8-bit) | - | - |
| `PADDH` | Parallel Add Halfword (8x16-bit) | - | - |
| `PADDW` | Parallel Add Word (4x32-bit) | - | - |
| `PADDSB` | Parallel Add Signed Saturation Byte | - | - |
| `PADDSH` | Parallel Add Signed Saturation Halfword | - | - |
| `PADDSW` | Parallel Add Signed Saturation Word | - | - |
| `PADDUB` | Parallel Add Unsigned Saturation Byte | - | - |
| `PADDUH` | Parallel Add Unsigned Saturation Halfword | - | - |
| `PADDUW` | Parallel Add Unsigned Saturation Word | - | - |
| **Subtraction** ||||
| `PSUBB` | Parallel Subtract Byte | - | - |
| `PSUBH` | Parallel Subtract Halfword | - | - |
| `PSUBW` | Parallel Subtract Word | - | - |
| `PSUBSB` | Parallel Subtract Signed Saturation Byte | - | - |
| `PSUBSH` | Parallel Subtract Signed Saturation Halfword | - | - |
| `PSUBSW` | Parallel Subtract Signed Saturation Word | - | - |
| `PSUBUB` | Parallel Subtract Unsigned Saturation Byte | - | - |
| `PSUBUH` | Parallel Subtract Unsigned Saturation Halfword | - | - |
| `PSUBUW` | Parallel Subtract Unsigned Saturation Word | - | - |
| **Absolute Value** ||||
| `PABSH` | Parallel Absolute Halfword | - | - |
| `PABSW` | Parallel Absolute Word | - | - |
| **Add/Subtract Combined** ||||
| `PADSBH` | Parallel Add/Subtract Halfword | - | - |

### 2.2 Parallel Comparison

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `PCEQB` | Parallel Compare Equal Byte | - | - |
| `PCEQH` | Parallel Compare Equal Halfword | - | - |
| `PCEQW` | Parallel Compare Equal Word | - | - |
| `PCGTB` | Parallel Compare Greater Than Byte | - | - |
| `PCGTH` | Parallel Compare Greater Than Halfword | - | - |
| `PCGTW` | Parallel Compare Greater Than Word | - | - |

### 2.3 Parallel Min/Max

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `PMAXH` | Parallel Maximum Halfword | - | - |
| `PMAXW` | Parallel Maximum Word | - | - |
| `PMINH` | Parallel Minimum Halfword | - | - |
| `PMINW` | Parallel Minimum Word | - | - |

### 2.4 Parallel Logical Operations

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `PAND` | Parallel AND (128-bit) | - | - |
| `POR` | Parallel OR (128-bit) | - | Automatic (move) |
| `PXOR` | Parallel XOR (128-bit) | - | - |
| `PNOR` | Parallel NOR (128-bit) | - | - |

### 2.5 Parallel Shift Operations

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `PSLLH` | Parallel Shift Left Logical Halfword | - | - |
| `PSLLW` | Parallel Shift Left Logical Word | - | - |
| `PSLLVW` | Parallel Shift Left Logical Variable Word | - | - |
| `PSRAH` | Parallel Shift Right Arithmetic Halfword | - | - |
| `PSRAW` | Parallel Shift Right Arithmetic Word | - | - |
| `PSRAVW` | Parallel Shift Right Arithmetic Variable Word | - | - |
| `PSRLH` | Parallel Shift Right Logical Halfword | - | - |
| `PSRLW` | Parallel Shift Right Logical Word | - | - |
| `PSRLVW` | Parallel Shift Right Logical Variable Word | - | - |

### 2.6 Parallel Multiply/Divide

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `PMULTH` | Parallel Multiply Halfword | - | - |
| `PMULTW` | Parallel Multiply Word | - | - |
| `PMULTUW` | Parallel Multiply Unsigned Word | - | - |
| `PMADDH` | Parallel Multiply-Add Halfword | - | - |
| `PMADDW` | Parallel Multiply-Add Word | - | - |
| `PMADDUW` | Parallel Multiply-Add Unsigned Word | - | - |
| `PMSUBH` | Parallel Multiply-Subtract Halfword | - | - |
| `PMSUBW` | Parallel Multiply-Subtract Word | - | - |
| `PHMADH` | Parallel Horizontal Multiply-Add Halfword | - | - |
| `PHMSBH` | Parallel Horizontal Multiply-Subtract Halfword | - | - |
| `PDIVBW` | Parallel Divide Broadcast Word | - | - |
| `PDIVW` | Parallel Divide Word | - | - |
| `PDIVUW` | Parallel Divide Unsigned Word | - | - |

### 2.7 Parallel Data Movement/Rearrangement

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `PCPYH` | Parallel Copy Halfword | - | - |
| `PCPYLD` | Parallel Copy Lower Doubleword | - | - |
| `PCPYUD` | Parallel Copy Upper Doubleword | - | - |
| `PEXCH` | Parallel Exchange Center Halfword | - | - |
| `PEXCW` | Parallel Exchange Center Word | - | - |
| `PEXEH` | Parallel Exchange Even Halfword | - | - |
| `PEXEW` | Parallel Exchange Even Word | - | - |
| `PEXTLB` | Parallel Extend Lower from Byte | - | - |
| `PEXTLH` | Parallel Extend Lower from Halfword | - | - |
| `PEXTLW` | Parallel Extend Lower from Word | - | - |
| `PEXTUB` | Parallel Extend Upper from Byte | - | - |
| `PEXTUH` | Parallel Extend Upper from Halfword | - | - |
| `PEXTUW` | Parallel Extend Upper from Word | - | - |
| `PINTH` | Parallel Interleave Halfword | - | - |
| `PINTEH` | Parallel Interleave Even Halfword | - | - |
| `PPACB` | Parallel Pack to Byte | - | - |
| `PPACH` | Parallel Pack to Halfword | - | - |
| `PPACW` | Parallel Pack to Word | - | - |
| `PREVH` | Parallel Reverse Halfword | - | - |
| `PROT3W` | Parallel Rotate 3 Words Left | - | - |

### 2.8 Parallel Format Conversion

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `PEXT5` | Parallel Extend from 5 bits (RGB555 expand) | - | - |
| `PPAC5` | Parallel Pack to 5 bits (RGB555 pack) | - | - |

### 2.9 Other MMI Instructions

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `PLZCW` | Parallel Leading Zero/One Count Word | - | - |
| `QFSRV` | Quadword Funnel Shift Right Variable | - | - |

### 2.10 HI/LO Register Operations

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `PMFHI` | Parallel Move From HI Register | - | - |
| `PMFLO` | Parallel Move From LO Register | - | - |
| `PMTHI` | Parallel Move To HI Register | - | - |
| `PMTLO` | Parallel Move To LO Register | - | - |
| `PMFHL.LW` | Parallel Move From HI/LO (Low Word) | - | - |
| `PMFHL.UW` | Parallel Move From HI/LO (Upper Word) | - | - |
| `PMFHL.SLW` | Parallel Move From HI/LO (Signed Low Word) | - | - |
| `PMFHL.LH` | Parallel Move From HI/LO (Low Halfword) | - | - |
| `PMFHL.SH` | Parallel Move From HI/LO (Signed Halfword) | - | - |
| `PMTHL.LW` | Parallel Move To HI/LO (Low Word) | - | - |

---

## 3. Dual Pipeline Instructions

R5900 has a second multiply/divide unit (Pipeline 1) with dedicated HI1/LO1 registers.

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `MULT1` | Multiply Word Pipeline 1 | - | - |
| `MULTU1` | Multiply Unsigned Word Pipeline 1 | - | - |
| `DIV1` | Divide Word Pipeline 1 | - | - |
| `DIVU1` | Divide Unsigned Word Pipeline 1 | - | - |
| `MADD` | Multiply-Add Word | - | Automatic |
| `MADD1` | Multiply-Add Word Pipeline 1 | - | - |
| `MADDU` | Multiply-Add Unsigned Word | - | Automatic |
| `MADDU1` | Multiply-Add Unsigned Word Pipeline 1 | - | - |
| `MFHI1` | Move From HI1 Register | - | - |
| `MFLO1` | Move From LO1 Register | - | - |
| `MTHI1` | Move To HI1 Register | - | - |
| `MTLO1` | Move To LO1 Register | - | - |

---

## 4. Shift Amount Register (SA)

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `MFSA` | Move from Shift Amount Register | - | - |
| `MTSA` | Move to Shift Amount Register | - | - |
| `MTSAB` | Move Byte Count to SA Register | - | - |
| `MTSAH` | Move Halfword Count to SA Register | - | - |

---

## 5. COP1 (FPU) Extensions

R5900 FPU is single-precision only with additional operations. Note: Double precision is NOT supported.

### 5.1 Accumulator Operations

The FPU has a dedicated accumulator register for efficient FMA chains. ACC is a fixed register - use intrinsics only (no automatic optimization).

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `ADDA.S` | Add to Accumulator | `__builtin_mips_adda_s` | Builtin |
| `SUBA.S` | Subtract to Accumulator | `__builtin_mips_suba_s` | Builtin |
| `MULA.S` | Multiply to Accumulator | `__builtin_mips_mula_s` | Builtin |
| `MADD.S` | Multiply-Add (ACC + fs * ft) | `__builtin_mips_madd_s` | Builtin |
| `MADDA.S` | Multiply-Add to Accumulator | `__builtin_mips_madda_s` | Builtin |
| `MSUB.S` | Multiply-Subtract (ACC - fs * ft) | `__builtin_mips_msub_s` | Builtin |
| `MSUBA.S` | Multiply-Subtract to Accumulator | `__builtin_mips_msuba_s` | Builtin |

### 5.2 Min/Max/Reciprocal

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `MIN.S` | Floating-Point Minimum | `__builtin_mips_min_s` | Builtin, Automatic (fminf) |
| `MAX.S` | Floating-Point Maximum | `__builtin_mips_max_s` | Builtin, Automatic (fmaxf) |
| `RSQRT.S` | Reciprocal Square Root (fd = fs / sqrt(ft)) | - | Automatic |
| `SQRT.S` | Square Root | - | Automatic (std MIPS) |

---

## 6. COP2 (VU0) - Vector Floating-Point Unit

VU0 operates on 128-bit vectors containing 4x32-bit single-precision floats (V4SF mode).
Use `-mvu0` flag to enable. Registers: `$vf0`-`$vf31` (32 x 128-bit).

**Note**: `$vf0` is special - `$vf0.w` is always 1.0.

### 6.1 Data Transfer

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `LQC2` | Load Quadword to COP2 | - | Automatic |
| `SQC2` | Store Quadword from COP2 | - | Automatic |
| `QMFC2` | Quadword Move From COP2 to GP | - | Automatic |
| `QMTC2` | Quadword Move To COP2 from GP | - | Automatic |
| `CFC2` | Control Transfer from VU to EE Core | - | - |
| `CTC2` | Control Transfer from EE Core to VU | - | - |

### 6.2 Vector Arithmetic

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `VADD.xyzw` | dest = a + b | `__builtin_vu0_vadd` | Builtin, Automatic |
| `VSUB.xyzw` | dest = a - b | `__builtin_vu0_vsub` | Builtin, Automatic |
| `VMUL.xyzw` | dest = a * b | `__builtin_vu0_vmul` | Builtin, Automatic |
| `VABS.xyzw` | dest = \|a\| | `__builtin_vu0_vabs` | Builtin, Automatic |
| `VMAX.xyzw` | dest = max(a, b) | `__builtin_vu0_vmax` | Builtin, Automatic |
| `VMINI.xyzw` | dest = min(a, b) | `__builtin_vu0_vmini` | Builtin, Automatic |
| `VMOVE.xyzw` | dest = src | `__builtin_vu0_vmove` | Builtin, Automatic |

### 6.3 Broadcast Arithmetic (bc = x/y/z/w)

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `VADDbc.xyzw` | dest = a + b.bc | `__builtin_vu0_vaddbc` | Builtin |
| `VSUBbc.xyzw` | dest = a - b.bc | `__builtin_vu0_vsubbc` | Builtin |
| `VMULbc.xyzw` | dest = a * b.bc | `__builtin_vu0_vmulbc` | Builtin |

### 6.4 Multiply-Accumulate (bc variants: x/y/z/w)

VU0 has an implicit ACC register for efficient FMA chains.

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `VMULA.xyzw` | ACC = a * b | `__builtin_vu0_vmula` | Builtin |
| `VMULAbc.xyzw` | ACC = a * b.bc | `__builtin_vu0_mulabc` | Builtin |
| `VMADDA.xyzw` | ACC += a * b | `__builtin_vu0_vmadda` | Builtin |
| `VMADDAbc.xyzw` | ACC += a * b.bc | `__builtin_vu0_vmaddabc` | Builtin |
| `VMADD.xyzw` | dest = ACC + a * b | `__builtin_vu0_vmadd` | Builtin |
| `VMADDbc.xyzw` | dest = ACC + a * b.bc | `__builtin_vu0_vmaddbc` | Builtin |
| `VMSUBA.xyzw` | ACC -= a * b | `__builtin_vu0_vmsuba` | Builtin |
| `VMSUBAbc.xyzw` | ACC -= a * b.bc | `__builtin_vu0_vmsubabc` | Builtin |
| `VMSUB.xyzw` | dest = ACC - a * b | `__builtin_vu0_vmsub` | Builtin |
| `VMSUBbc.xyzw` | dest = ACC - a * b.bc | `__builtin_vu0_vmsubbc` | Builtin |

### 6.5 Add/Subtract to Accumulator (bc variants: x/y/z/w)

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `VADDA.xyzw` | ACC = a + b | `__builtin_vu0_vadda` | Builtin |
| `VADDAbc.xyzw` | ACC = a + b.bc | `__builtin_vu0_vaddabc` | Builtin |
| `VSUBA.xyzw` | ACC = a - b | `__builtin_vu0_vsuba` | Builtin |
| `VSUBAbc.xyzw` | ACC = a - b.bc | `__builtin_vu0_vsubabc` | Builtin |

### 6.6 Outer Product (Cross Product)

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `VOPMULA.xyz` | ACC.xyz = a.yzx * b.zxy | `__builtin_vu0_vopmula` | Builtin |
| `VOPMSUB.xyz` | dest.xyz = ACC - a.zxy * b.yzx | `__builtin_vu0_vopmsub` | Builtin |

### 6.7 Conversion Operations

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `VFTOI0.xyzw` | Float to 32-bit integer | `__builtin_vu0_vftoi0` | Builtin |
| `VFTOI4.xyzw` | Float to 28.4 fixed-point | `__builtin_vu0_vftoi4` | Builtin |
| `VFTOI12.xyzw` | Float to 20.12 fixed-point | `__builtin_vu0_vftoi12` | Builtin |
| `VFTOI15.xyzw` | Float to 17.15 fixed-point | `__builtin_vu0_vftoi15` | Builtin |
| `VITOF0.xyzw` | 32-bit integer to float | `__builtin_vu0_vitof0` | Builtin |
| `VITOF4.xyzw` | 28.4 fixed-point to float | `__builtin_vu0_vitof4` | Builtin |
| `VITOF12.xyzw` | 20.12 fixed-point to float | `__builtin_vu0_vitof12` | Builtin |
| `VITOF15.xyzw` | 17.15 fixed-point to float | `__builtin_vu0_vitof15` | Builtin |

### 6.8 Data Movement

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `VMR32.xyzw` | dest.xyzw = src.yzwx | `__builtin_vu0_vmr32` | Builtin |

### 6.9 Not Yet Implemented

| Instruction | Description | Intrinsic | Usage |
|-------------|-------------|-----------|-------|
| `VADDi/q`, `VSUBi/q`, `VMULi/q` | Arithmetic with I/Q register | - | - |
| `VMADDi/q`, `VMSUBi/q` | Multiply-accumulate with I/Q | - | - |
| `VDIV`, `VSQRT`, `VRSQRT` | Division and square root | - | - |
| `VCLIP` | Clipping judgment | - | - |
| `VIADD/ISUB/IAND/IOR` | Integer operations (VI regs) | - | - |
| `VLQI/VSQI`, `VLQD/VSQD` | Load/Store with inc/dec | - | - |
| `BC2F/T/FL/TL` | COP2 branch instructions | - | - |

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
| VU0 Broadcast Arithmetic | 12 | 12 | 100% |
| VU0 Multiply-Accumulate | 30 | 30 | 100% |
| VU0 Add/Sub Accumulator | 10 | 10 | 100% |
| VU0 Outer Product | 2 | 2 | 100% |
| VU0 Conversions | 8 | 8 | 100% |
| VU0 Data Movement | 1 | 1 | 100% |
| VU0 Advanced (I/Q/VI) | ~25 | 0 | 0% |

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
