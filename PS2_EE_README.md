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

### Vector Types (MMI - Implemented)

These use the 128-bit GP registers for integer SIMD:

| Mode | Size | Elements | Description | GCC Status |
|------|------|----------|-------------|------------|
| `V16QI` | 128-bit | 16 × 8-bit int | Parallel byte operations | **Implemented** (builtins + autovec) |
| `V8HI` | 128-bit | 8 × 16-bit int | Parallel halfword operations | **Implemented** (builtins + autovec) |
| `V4SI` | 128-bit | 4 × 32-bit int | Parallel word operations | **Implemented** (builtins + autovec) |
| `V2DI` | 128-bit | 2 × 64-bit int | Parallel doubleword operations | **Implemented** (builtins) |

These modes map to MMI instructions via `__builtin_mmi_*` intrinsics. Autovectorization is supported for add/sub, min/max, logical, and shift operations at `-O3`.

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
| MMI Vector (int) | V16QI, V8HI, V4SI, V2DI | 4/4 (100%, builtins) |

**TImode (128-bit scalar) optimizations:** Logical operations (`&`, `|`, `^`, `~`) on `__int128` use single MMI instructions (pand, por, pxor, pnor).

---

## 1. 128-bit Load/Store Instructions

These instructions operate on the full 128-bit width of GP registers.

| Instruction | Description | Intrinsic | Auto-used |
|-------------|-------------|-----------|-----------|
| `LQ` | Load Quadword (128-bit) | - | ✓ |
| `SQ` | Store Quadword (128-bit) | - | ✓ |

**Callee-saved register preservation**: Function prologues/epilogues use LQ/SQ to save and restore callee-saved registers (s0-s7, gp, fp, ra), preserving the full 128-bit width. This ensures that 128-bit values (`__int128`, vectors) in callee-saved registers are correctly preserved across function calls.

---

## 2. MMI (Multimedia Instructions) - 128-bit Integer SIMD

### 2.1 Arithmetic

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `PADDB` | Parallel Add Byte | `__builtin_mmi_paddb` | `v16qi (+)` | ✓ |
| `PSUBB` | Parallel Subtract Byte | `__builtin_mmi_psubb` | `v16qi (-)` | ✓ |
| `PADDH` | Parallel Add Halfword | `__builtin_mmi_paddh` | `v8hi (+)` | ✓ |
| `PSUBH` | Parallel Subtract Halfword | `__builtin_mmi_psubh` | `v8hi (-)` | ✓ |
| `PADDW` | Parallel Add Word | `__builtin_mmi_paddw` | `v4si (+)` | ✓ |
| `PSUBW` | Parallel Subtract Word | `__builtin_mmi_psubw` | `v4si (-)` | ✓ |
| `PADSBH` | Parallel Add/Subtract Halfword | - | - | - |
| `PADDSB` | Parallel Add with Signed Saturation Byte | `__builtin_mmi_paddsb` | - | ✓ |
| `PSUBSB` | Parallel Subtract with Signed Saturation Byte | `__builtin_mmi_psubsb` | - | ✓ |
| `PADDSH` | Parallel Add with Signed Saturation Halfword | `__builtin_mmi_paddsh` | - | ✓ |
| `PSUBSH` | Parallel Subtract with Signed Saturation Halfword | `__builtin_mmi_psubsh` | - | ✓ |
| `PADDSW` | Parallel Add with Signed Saturation Word | `__builtin_mmi_paddsw` | - | ✓ |
| `PSUBSW` | Parallel Subtract with Signed Saturation Word | `__builtin_mmi_psubsw` | - | ✓ |
| `PADDUB` | Parallel Add with Unsigned Saturation Byte | `__builtin_mmi_paddub` | - | ✓ |
| `PSUBUB` | Parallel Subtract with Unsigned Saturation Byte | `__builtin_mmi_psubub` | - | ✓ |
| `PADDUH` | Parallel Add with Unsigned Saturation Halfword | `__builtin_mmi_padduh` | - | ✓ |
| `PSUBUH` | Parallel Subtract with Unsigned Saturation Halfword | `__builtin_mmi_psubuh` | - | ✓ |
| `PADDUW` | Parallel Add with Unsigned Saturation Word | `__builtin_mmi_padduw` | - | ✓ |
| `PSUBUW` | Parallel Subtract with Unsigned Saturation Word | `__builtin_mmi_psubuw` | - | ✓ |

### 2.2 Multiply and Divide

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `PMULTW` | Parallel Multiply Word | - | - | - |
| `PMULTUW` | Parallel Multiply Unsigned Word | - | - | - |
| `PDIVW` | Parallel Divide Word | - | - | - |
| `PDIVUW` | Parallel Divide Unsigned Word | - | - | - |
| `PMADDW` | Parallel Multiply-Add Word | - | - | - |
| `PMADDUW` | Parallel Multiply-Add Unsigned Word | - | - | - |
| `PMSUBW` | Parallel Multiply-Subtract Word | - | - | - |
| `PMULTH` | Parallel Multiply Halfword | - | - | - |
| `PMADDH` | Parallel Multiply-Add Halfword | - | - | - |
| `PMSUBH` | Parallel Multiply-Subtract Halfword | - | - | - |
| `PHMADH` | Parallel Horizontal Multiply-Add Halfword | - | - | - |
| `PHMSBH` | Parallel Horizontal Multiply-Subtract Halfword | - | - | - |
| `PDIVBW` | Parallel Divide Broadcast Word | - | - | - |
| `PMFHI` | Parallel Move From HI Register | - | - | - |
| `PMFLO` | Parallel Move From LO Register | - | - | - |
| `PMTHI` | Parallel Move To HI Register | - | - | - |
| `PMTLO` | Parallel Move To LO Register | - | - | - |
| `PMFHL.LW` | Parallel Move From HI/LO (Low Word) | - | - | - |
| `PMFHL.UW` | Parallel Move From HI/LO (Upper Word) | - | - | - |
| `PMFHL.SLW` | Parallel Move From HI/LO (Signed Low Word) | - | - | - |
| `PMFHL.LH` | Parallel Move From HI/LO (Low Halfword) | - | - | - |
| `PMFHL.SH` | Parallel Move From HI/LO (Signed Halfword) | - | - | - |
| `PMTHL.LW` | Parallel Move To HI/LO (Low Word) | - | - | - |

### 2.3 Shift Operations

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `MFSA` | Move From SA Register | - | - | - |
| `MTSA` | Move To SA Register | - | - | - |
| `MTSAB` | Move Byte Count to SA Register | - | - | - |
| `MTSAH` | Move Halfword Count to SA Register | - | - | - |
| `PSLLH` | Parallel Shift Left Logical Halfword | `__builtin_mmi_psllh` | `v8hi (<<)` | ✓ |
| `PSRLH` | Parallel Shift Right Logical Halfword | `__builtin_mmi_psrlh` | `v8hi (>>)` | ✓ |
| `PSRAH` | Parallel Shift Right Arithmetic Halfword | `__builtin_mmi_psrah` | `v8hi (>>)` | ✓ |
| `PSLLW` | Parallel Shift Left Logical Word | `__builtin_mmi_psllw` | `v4si (<<)` | ✓ |
| `PSLLVW` | Parallel Shift Left Logical Variable Word | `__builtin_mmi_psllvw` | - | - |
| `PSRLW` | Parallel Shift Right Logical Word | `__builtin_mmi_psrlw` | `v4si (>>)` | ✓ |
| `PSRLVW` | Parallel Shift Right Logical Variable Word | `__builtin_mmi_psrlvw` | - | - |
| `PSRAW` | Parallel Shift Right Arithmetic Word | `__builtin_mmi_psraw` | `v4si (>>)` | ✓ |
| `PSRAVW` | Parallel Shift Right Arithmetic Variable Word | `__builtin_mmi_psravw` | - | - |
| `QFSRV` | Quadword Funnel Shift Right Variable | - | - | - |

### 2.4 Others (Min/Max, Logical, Absolute)

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `PABSH` | Parallel Absolute Halfword | `__builtin_mmi_pabsh` | - | ✓ |
| `PABSW` | Parallel Absolute Word | `__builtin_mmi_pabsw` | - | ✓ |
| `PMAXH` | Parallel Maximum Halfword | `__builtin_mmi_pmaxh` | - | ✓ |
| `PMINH` | Parallel Minimum Halfword | `__builtin_mmi_pminh` | - | ✓ |
| `PMAXW` | Parallel Maximum Word | `__builtin_mmi_pmaxw` | - | ✓ |
| `PMINW` | Parallel Minimum Word | `__builtin_mmi_pminw` | - | ✓ |
| `PAND` | Parallel AND | `__builtin_mmi_pand` | `v16qi/v8hi/v4si/v2di (&)` | ✓ |
| `POR` | Parallel OR | `__builtin_mmi_por` | `v16qi/v8hi/v4si/v2di (\|)` | ✓ |
| `PXOR` | Parallel XOR | `__builtin_mmi_pxor` | `v16qi/v8hi/v4si/v2di (^)` | ✓ |
| `PNOR` | Parallel NOR | `__builtin_mmi_pnor` | `v16qi/v8hi/v4si/v2di (~)` | ✓ |

### 2.5 Compare

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `PCGTB` | Parallel Compare for Greater Than Byte | `__builtin_mmi_pcgtb` | `v16qi (>)` | ✓ |
| `PCEQB` | Parallel Compare for Equal Byte | `__builtin_mmi_pceqb` | `v16qi (==)` | ✓ |
| `PCGTH` | Parallel Compare for Greater Than Halfword | `__builtin_mmi_pcgth` | `v8hi (>)` | ✓ |
| `PCEQH` | Parallel Compare for Equal Halfword | `__builtin_mmi_pceqh` | `v8hi (==)` | ✓ |
| `PCGTW` | Parallel Compare for Greater Than Word | `__builtin_mmi_pcgtw` | `v4si (>)` | ✓ |
| `PCEQW` | Parallel Compare for Equal Word | `__builtin_mmi_pceqw` | `v4si (==)` | ✓ |
| `PLZCW` | Parallel Leading Zero Count Word | - | - | - |

**Vector Comparison Support**: All six comparison operators (==, !=, <, <=, >, >=) work with vector types.
Synthesized operations: NE uses PCEQ+PNOR, LT uses PCGT(swapped), LE uses PCGT+PNOR, GE uses PCGT+PCEQ+POR.

**Loop Autovectorization**: The `vcond` pattern supports MMI, enabling autovectorization of
conditional select operations like `e[i] = (a[i] > b[i]) ? c[i] : d[i]`. The bit select is
implemented using PAND/PNOR/POR sequence. Note: For autovectorization to work, all loads in
the loop must be unconditional (read all values before the conditional select).

### 2.6 Data Rearrangement

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `PPACB` | Parallel Pack to Byte | - | - | - |
| `PPACH` | Parallel Pack to Halfword | - | - | - |
| `PPACW` | Parallel Pack to Word | - | - | - |
| `PINTEH` | Parallel Interleave Even Halfword | - | - | - |
| `PEXTUB` | Parallel Extend Upper from Byte | - | - | - |
| `PEXTLB` | Parallel Extend Lower from Byte | - | - | - |
| `PEXTUH` | Parallel Extend Upper from Halfword | - | - | - |
| `PEXTLH` | Parallel Extend Lower from Halfword | - | - | - |
| `PEXTUW` | Parallel Extend Upper from Word | - | - | - |
| `PEXTLW` | Parallel Extend Lower from Word | - | - | - |
| `PEXT5` | Parallel Extend from 5 bits (RGB555 expand) | - | - | - |
| `PPAC5` | Parallel Pack to 5 bits (RGB555 pack) | - | - | - |
| `PCPYH` | Parallel Copy Halfword | - | - | - |
| `PCPYLD` | Parallel Copy Lower Doubleword | - | - | - |
| `PCPYUD` | Parallel Copy Upper Doubleword | - | - | - |
| `PREVH` | Parallel Reverse Halfword | - | - | - |
| `PINTH` | Parallel Interleave Halfword | - | - | - |
| `PEXEH` | Parallel Exchange Even Halfword | - | - | - |
| `PEXCH` | Parallel Exchange Center Halfword | - | - | - |
| `PEXEW` | Parallel Exchange Even Word | - | - | - |
| `PEXCW` | Parallel Exchange Center Word | - | - | - |
| `PROT3W` | Parallel Rotate 3 Words | - | - | - |

---

## 3. Dual Pipeline Instructions

R5900 has a second multiply/divide unit (Pipeline 1) with dedicated HI1/LO1 registers.

| Instruction | Description | Intrinsic | Auto-used |
|-------------|-------------|-----------|-----------|
| `MULT1` | Multiply Word Pipeline 1 | - | - |
| `MULTU1` | Multiply Unsigned Word Pipeline 1 | - | - |
| `DIV1` | Divide Word Pipeline 1 | - | - |
| `DIVU1` | Divide Unsigned Word Pipeline 1 | - | - |
| `MADD` | Multiply-Add Word | - | ✓ |
| `MADD1` | Multiply-Add Word Pipeline 1 | - | - |
| `MADDU` | Multiply-Add Unsigned Word | - | ✓ |
| `MADDU1` | Multiply-Add Unsigned Word Pipeline 1 | - | - |
| `MFHI1` | Move From HI1 Register | - | - |
| `MFLO1` | Move From LO1 Register | - | - |
| `MTHI1` | Move To HI1 Register | - | - |
| `MTLO1` | Move To LO1 Register | - | - |

---

## 4. COP1 (FPU) Extensions

R5900 FPU is single-precision only with additional operations. Note: Double precision is NOT supported.

### 4.1 Accumulator Operations

The FPU has a dedicated accumulator register for efficient FMA chains. ACC is a fixed register - use intrinsics only (no automatic optimization).

| Instruction | Description | Intrinsic | Auto-used |
|-------------|-------------|-----------|-----------|
| `ADDA.S` | Add to Accumulator | `__builtin_mips_adda_s` | - |
| `SUBA.S` | Subtract to Accumulator | `__builtin_mips_suba_s` | - |
| `MULA.S` | Multiply to Accumulator | `__builtin_mips_mula_s` | - |
| `MADD.S` | Multiply-Add (ACC + fs * ft) | `__builtin_mips_madd_s` | - |
| `MADDA.S` | Multiply-Add to Accumulator | `__builtin_mips_madda_s` | - |
| `MSUB.S` | Multiply-Subtract (ACC - fs * ft) | `__builtin_mips_msub_s` | - |
| `MSUBA.S` | Multiply-Subtract to Accumulator | `__builtin_mips_msuba_s` | - |

### 4.2 Min/Max/Reciprocal

| Instruction | Description | Intrinsic | Auto-used |
|-------------|-------------|-----------|-----------|
| `MIN.S` | Floating-Point Minimum | `__builtin_mips_min_s` | ✓ |
| `MAX.S` | Floating-Point Maximum | `__builtin_mips_max_s` | ✓ |
| `RSQRT.S` | Reciprocal Square Root (fd = fs / sqrt(ft)) | - | ✓ |
| `SQRT.S` | Square Root | - | ✓ |

---

## 5. COP2 (VU0) - Vector Floating-Point Unit

VU0 operates on 128-bit vectors containing 4x32-bit single-precision floats (V4SF mode).
Use `-mvu0` flag to enable. Registers: `$vf0`-`$vf31` (32 x 128-bit).

**Note**: `$vf0` is special - `$vf0.w` is always 1.0.

### 5.1 Data Transfer

| Instruction | Description | Intrinsic | Auto-used |
|-------------|-------------|-----------|-----------|
| `LQC2` | Load Quadword to COP2 | - | ✓ |
| `SQC2` | Store Quadword from COP2 | - | ✓ |
| `QMFC2` | Quadword Move From COP2 to GP | - | ✓ |
| `QMTC2` | Quadword Move To COP2 from GP | - | ✓ |
| `CFC2` | Control Transfer from VU to EE Core | - | - |
| `CTC2` | Control Transfer from EE Core to VU | - | - |

### 5.2 Vector Arithmetic

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `VADD.xyzw` | dest = a + b | `__builtin_vu0_vadd` | `v4sf (+)` | ✓ |
| `VSUB.xyzw` | dest = a - b | `__builtin_vu0_vsub` | `v4sf (-)` | ✓ |
| `VMUL.xyzw` | dest = a * b | `__builtin_vu0_vmul` | `v4sf (*)` | ✓ |
| `VABS.xyzw` | dest = \|a\| | `__builtin_vu0_vabs` | - | ✓ |
| `VMAX.xyzw` | dest = max(a, b) | `__builtin_vu0_vmax` | - | ✓ |
| `VMINI.xyzw` | dest = min(a, b) | `__builtin_vu0_vmini` | - | ✓ |
| `VMOVE.xyzw` | dest = src | `__builtin_vu0_vmove` | - | ✓ |

### 5.3 Broadcast Arithmetic (bc = x/y/z/w)

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `VADDbc.xyzw` | dest = a + b.bc | `__builtin_vu0_vaddbc` | - | - |
| `VSUBbc.xyzw` | dest = a - b.bc | `__builtin_vu0_vsubbc` | - | - |
| `VMULbc.xyzw` | dest = a * b.bc | `__builtin_vu0_vmulbc` | - | - |

### 5.4 Multiply-Accumulate (bc variants: x/y/z/w)

VU0 has an implicit ACC register for efficient FMA chains.

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `VMULA.xyzw` | ACC = a * b | `__builtin_vu0_vmula` | - | - |
| `VMULAbc.xyzw` | ACC = a * b.bc | `__builtin_vu0_mulabc` | - | - |
| `VMADDA.xyzw` | ACC += a * b | `__builtin_vu0_vmadda` | - | - |
| `VMADDAbc.xyzw` | ACC += a * b.bc | `__builtin_vu0_vmaddabc` | - | - |
| `VMADD.xyzw` | dest = ACC + a * b | `__builtin_vu0_vmadd` | - | - |
| `VMADDbc.xyzw` | dest = ACC + a * b.bc | `__builtin_vu0_vmaddbc` | - | - |
| `VMSUBA.xyzw` | ACC -= a * b | `__builtin_vu0_vmsuba` | - | - |
| `VMSUBAbc.xyzw` | ACC -= a * b.bc | `__builtin_vu0_vmsubabc` | - | - |
| `VMSUB.xyzw` | dest = ACC - a * b | `__builtin_vu0_vmsub` | - | - |
| `VMSUBbc.xyzw` | dest = ACC - a * b.bc | `__builtin_vu0_vmsubbc` | - | - |

### 5.5 Add/Subtract to Accumulator (bc variants: x/y/z/w)

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `VADDA.xyzw` | ACC = a + b | `__builtin_vu0_vadda` | - | - |
| `VADDAbc.xyzw` | ACC = a + b.bc | `__builtin_vu0_vaddabc` | - | - |
| `VSUBA.xyzw` | ACC = a - b | `__builtin_vu0_vsuba` | - | - |
| `VSUBAbc.xyzw` | ACC = a - b.bc | `__builtin_vu0_vsubabc` | - | - |

### 5.6 Outer Product (Cross Product)

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `VOPMULA.xyz` | ACC.xyz = a.yzx * b.zxy | `__builtin_vu0_vopmula` | - | - |
| `VOPMSUB.xyz` | dest.xyz = ACC - a.zxy * b.yzx | `__builtin_vu0_vopmsub` | - | - |

### 5.7 Conversion Operations

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `VFTOI0.xyzw` | Float to 32-bit integer | `__builtin_vu0_vftoi0` | - | - |
| `VFTOI4.xyzw` | Float to 28.4 fixed-point | `__builtin_vu0_vftoi4` | - | - |
| `VFTOI12.xyzw` | Float to 20.12 fixed-point | `__builtin_vu0_vftoi12` | - | - |
| `VFTOI15.xyzw` | Float to 17.15 fixed-point | `__builtin_vu0_vftoi15` | - | - |
| `VITOF0.xyzw` | 32-bit integer to float | `__builtin_vu0_vitof0` | - | - |
| `VITOF4.xyzw` | 28.4 fixed-point to float | `__builtin_vu0_vitof4` | - | - |
| `VITOF12.xyzw` | 20.12 fixed-point to float | `__builtin_vu0_vitof12` | - | - |
| `VITOF15.xyzw` | 17.15 fixed-point to float | `__builtin_vu0_vitof15` | - | - |

### 5.8 Data Movement

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `VMR32.xyzw` | dest.xyzw = src.yzwx | `__builtin_vu0_vmr32` | - | - |

### 5.9 Not Yet Implemented

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `VADDi/q`, `VSUBi/q`, `VMULi/q` | Arithmetic with I/Q register | - | - | - |
| `VMADDi/q`, `VMSUBi/q` | Multiply-accumulate with I/Q | - | - | - |
| `VDIV`, `VSQRT`, `VRSQRT` | Division and square root | - | - | - |
| `VCLIP` | Clipping judgment | - | - | - |
| `VIADD/ISUB/IAND/IOR` | Integer operations (VI regs) | - | - | - |
| `VLQI/VSQI`, `VLQD/VSQD` | Load/Store with inc/dec | - | - | - |
| `BC2F/T/FL/TL` | COP2 branch instructions | - | - | - |

---

## 6. Fused Multiply-Add (FMA) Autovectorization

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

## 7. Implementation Summary

| Feature Category | Total Instructions | Implemented | Coverage |
|-----------------|-------------------|-------------|----------|
| 128-bit Load/Store | 2 | 2 | 100% |
| MMI Arithmetic | 27 | 20 | 74% |
| MMI Comparison | 6 | 6 | 100% |
| MMI Min/Max | 4 | 4 | 100% |
| MMI Logical | 4 | 4 | 100% |
| MMI Shift | 9 | 9 | 100% |
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

## 8. Usage Examples

### 128-bit Integer

```c
typedef __int128 int128_t;

int128_t add128(int128_t a, int128_t b) {
    return a + b;  // Uses lq/sq for load/store
}

// Logical operations use MMI instructions
int128_t and128(int128_t a, int128_t b) {
    return a & b;  // Generates: pand
}

int128_t or128(int128_t a, int128_t b) {
    return a | b;  // Generates: por
}

int128_t xor128(int128_t a, int128_t b) {
    return a ^ b;  // Generates: pxor
}

int128_t not128(int128_t a) {
    return ~a;     // Generates: pnor
}
```

### MMI Vector Operations (Integer SIMD)

```c
typedef signed char v16qi __attribute__((vector_size(16)));
typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));
typedef long long v2di __attribute__((vector_size(16)));

// Explicit builtins
v4si add_words(v4si a, v4si b) {
    return __builtin_mmi_paddw(a, b);  // Generates: paddw
}

v2di and_128bit(v2di a, v2di b) {
    return __builtin_mmi_pand(a, b);   // Generates: pand
}

// Shift operations (immediate and variable)
v8hi shift_left_h(v8hi a) {
    return __builtin_mmi_psllh(a, 4);  // Generates: psllh (shift by 4)
}

v4si shift_right_arith(v4si a) {
    return __builtin_mmi_psraw(a, 8);  // Generates: psraw (shift by 8)
}

v4si shift_variable(v4si a, v4si amounts) {
    return __builtin_mmi_psllvw(a, amounts);  // Generates: psllvw (per-element shift)
}

// Vector comparisons (all operators work)
v4si vec_equal(v4si a, v4si b) {
    return a == b;  // Generates: pceqw (result: -1 if equal, 0 otherwise)
}

v4si vec_greater(v4si a, v4si b) {
    return a > b;   // Generates: pcgtw
}

v4si vec_less(v4si a, v4si b) {
    return a < b;   // Generates: pcgtw with swapped operands
}

v4si vec_not_equal(v4si a, v4si b) {
    return a != b;  // Generates: pceqw + pnor
}

v4si vec_less_equal(v4si a, v4si b) {
    return a <= b;  // Generates: pcgtw + pnor
}

v4si vec_greater_equal(v4si a, v4si b) {
    return a >= b;  // Generates: pcgtw + pceqw + por
}
```

### MMI Autovectorization

With `-O3 -ftree-vectorize`, GCC auto-vectorizes integer loops:

```c
// Compile with: -march=r5900 -O3 -ftree-vectorize

// Arithmetic operations
void add_arrays(int *__restrict a, int *__restrict b, int *__restrict c, int n) {
    for (int i = 0; i < n; i++)
        c[i] = a[i] + b[i];  // Auto-vectorized to: paddw
}

// Min/max operations
void max_arrays(int *__restrict a, int *__restrict b, int *__restrict c, int n) {
    for (int i = 0; i < n; i++)
        c[i] = a[i] > b[i] ? a[i] : b[i];  // Auto-vectorized to: pmaxw
}

// Logical operations
void and_arrays(int *__restrict a, int *__restrict b, int *__restrict c, int n) {
    for (int i = 0; i < n; i++)
        c[i] = a[i] & b[i];  // Auto-vectorized to: pand
}

void not_arrays(int *__restrict a, int *__restrict c, int n) {
    for (int i = 0; i < n; i++)
        c[i] = ~a[i];  // Auto-vectorized to: pnor
}

// Shift operations (constant shift amounts)
void shl_arrays(short *__restrict a, short *__restrict c, int n) {
    for (int i = 0; i < n; i++)
        c[i] = a[i] << 4;  // Auto-vectorized to: psllh
}

void shr_arrays(int *__restrict a, int *__restrict c, int n) {
    for (int i = 0; i < n; i++)
        c[i] = a[i] >> 8;  // Auto-vectorized to: psraw (arithmetic)
}

void shr_logical(unsigned int *__restrict a, unsigned int *__restrict c, int n) {
    for (int i = 0; i < n; i++)
        c[i] = a[i] >> 8;  // Auto-vectorized to: psrlw (logical)
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

## 9. Known Limitations

1. **No Double Precision**: R5900 FPU is single-precision only
2. **No Atomics**: LL/SC instructions are not available
3. **No CLZ/CLO**: Count leading zeros/ones not available
4. **VU0 Constraints**: Some VU0 features require careful register management
5. **Accumulator Clobber**: VU0 ACC is implicit; intrinsic sequences must be ordered correctly

---

## 10. References

- EE Core Instruction Set Manual (Sony)
- EE Core Users Manual (Sony)
- VU Users Manual (Sony)
- GCC Source: `gcc/config/mips/mips-vu0.md`
- GCC Source: `gcc/config/mips/mips-mmi.md`
- GCC Source: `gcc/config/mips/5900.md`
