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

| Register | Size | Purpose | GCC Status | GCC Regnum | Constraint |
|----------|------|---------|------------|------------|------------|
| `HI` | 64-bit | Upper result of multiply/divide (Pipeline 0) | **Implemented** | 64 | `"x"` |
| `LO` | 64-bit | Lower result of multiply/divide (Pipeline 0) | **Implemented** | 65 | `"l"`, `"x"` |
| `HI1` | 64-bit | Upper result of multiply/divide (Pipeline 1) | **Implemented** | 190 | `"Ym"` |
| `LO1` | 64-bit | Lower result of multiply/divide (Pipeline 1) | **Implemented** | 191 | `"Yl"`, `"Ym"` |

The R5900 has dual multiply/divide pipelines (MAC0 and MAC1). MULT1/DIV1/MADD1 use HI1/LO1 and can execute in parallel with Pipeline 0 operations for improved throughput.

### Shift Amount Register

| Register | Size | Purpose | GCC Status | GCC Regnum |
|----------|------|---------|------------|------------|
| `SA` | 8-bit | Shift amount for QFSRV (funnel shift) | **Implemented** | - |

Used by MTSAB, MTSAH, and QFSRV instructions for 128-bit funnel shifts. Accessible via intrinsics for unaligned 128-bit memory access.

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
| `Q` | 32-bit | Division/sqrt result register | **Implemented** | 192 |
| `I` | 32-bit | Immediate FP value (loaded via CTC2) | **Implemented** | 193 |

Special notes:
- `$vf0` is constant: x=0.0, y=0.0, z=0.0, w=1.0
- `$vi0` is constant: always 0
- VF registers use constraint `"C"` in GCC patterns

### Register Summary

| Category | Total | Implemented | Coverage |
|----------|-------|-------------|----------|
| GP (128-bit) | 32 | 32 | 100% |
| HI/LO (dual pipeline) | 4 | 4 | 100% |
| SA | 1 | 1 | 100% |
| FPU Data | 32 | 32 | 100% |
| FPU ACC | 1 | 1 | 100% |
| VU0 VF | 32 | 32 | 100% |
| VU0 VI | 16 | 0 | 0% |
| VU0 ACC | 1 | 1 | 100% |
| VU0 Q/I | 2 | 2 | 100% |

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

These modes map to MMI instructions via `__builtin_mmi_*` intrinsics. Autovectorization is supported for add/sub, min/max, logical, shift, and multiply (V8HI) operations at `-O3`.

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

**Automatic Unaligned 128-bit Access**: GCC automatically uses QFSRV for unaligned 128-bit loads when the type has `__attribute__((aligned(1)))`. This applies to all 128-bit types: `__int128` (TI), and vector types V16QI, V8HI, V4SI, V4SF. For stores, regular moves are used (which may trap on truly misaligned access on PS2 hardware).

```c
typedef __int128 ti;
typedef ti unaligned_ti __attribute__((aligned(1)));  // Tell GCC this is misaligned

ti load_unaligned(unaligned_ti *ptr) {
    return *ptr;  // Generates: LQ + LQ + MTSAB + QFSRV
}
```

---

## 2. MMI (Multimedia Instructions) - 128-bit Integer SIMD

### 2.1 Arithmetic

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `PADDB` | Parallel Add Byte | `__builtin_mmi_paddb(a, b)` | `v16qi (+)` | ✓ |
| `PSUBB` | Parallel Subtract Byte | `__builtin_mmi_psubb(a, b)` | `v16qi (-)` | ✓ |
| `PADDH` | Parallel Add Halfword | `__builtin_mmi_paddh(a, b)` | `v8hi (+)` | ✓ |
| `PSUBH` | Parallel Subtract Halfword | `__builtin_mmi_psubh(a, b)` | `v8hi (-)` | ✓ |
| `PADDW` | Parallel Add Word | `__builtin_mmi_paddw(a, b)` | `v4si (+)` | ✓ |
| `PSUBW` | Parallel Subtract Word | `__builtin_mmi_psubw(a, b)` | `v4si (-)` | ✓ |
| `PADSBH` | Parallel Add/Subtract Halfword | - | - | - |
| `PADDSB` | Parallel Add with Signed Saturation Byte | `__builtin_mmi_paddsb(a, b)` | - | ✓ |
| `PSUBSB` | Parallel Subtract with Signed Saturation Byte | `__builtin_mmi_psubsb(a, b)` | - | ✓ |
| `PADDSH` | Parallel Add with Signed Saturation Halfword | `__builtin_mmi_paddsh(a, b)` | - | ✓ |
| `PSUBSH` | Parallel Subtract with Signed Saturation Halfword | `__builtin_mmi_psubsh(a, b)` | - | ✓ |
| `PADDSW` | Parallel Add with Signed Saturation Word | `__builtin_mmi_paddsw(a, b)` | - | ✓ |
| `PSUBSW` | Parallel Subtract with Signed Saturation Word | `__builtin_mmi_psubsw(a, b)` | - | ✓ |
| `PADDUB` | Parallel Add with Unsigned Saturation Byte | `__builtin_mmi_paddub(a, b)` | - | ✓ |
| `PSUBUB` | Parallel Subtract with Unsigned Saturation Byte | `__builtin_mmi_psubub(a, b)` | - | ✓ |
| `PADDUH` | Parallel Add with Unsigned Saturation Halfword | `__builtin_mmi_padduh(a, b)` | - | ✓ |
| `PSUBUH` | Parallel Subtract with Unsigned Saturation Halfword | `__builtin_mmi_psubuh(a, b)` | - | ✓ |
| `PADDUW` | Parallel Add with Unsigned Saturation Word | `__builtin_mmi_padduw(a, b)` | - | ✓ |
| `PSUBUW` | Parallel Subtract with Unsigned Saturation Word | `__builtin_mmi_psubuw(a, b)` | - | ✓ |

### 2.2 Multiply and Divide

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `PMULTW` | Parallel Multiply Word | `__builtin_mmi_pmultw(a, b)` | - | - |
| `PMULTUW` | Parallel Multiply Unsigned Word | `__builtin_mmi_pmultuw(a, b)` | - | - |
| `PDIVW` | Parallel Divide Word | `__builtin_mmi_pdivw(a, b)` | - | - |
| `PDIVUW` | Parallel Divide Unsigned Word | `__builtin_mmi_pdivuw(a, b)` | - | - |
| `PMADDW` | Parallel Multiply-Add Word | `__builtin_mmi_pmaddw(a, b)` | - | - |
| `PMADDUW` | Parallel Multiply-Add Unsigned Word | `__builtin_mmi_pmadduw(a, b)` | - | - |
| `PMSUBW` | Parallel Multiply-Subtract Word | `__builtin_mmi_pmsubw(a, b)` | - | - |
| `PMULTH` | Parallel Multiply Halfword | `__builtin_mmi_pmulth(a, b)` | `v8hi (*)` | ✓ |
| `PMADDH` | Parallel Multiply-Add Halfword | `__builtin_mmi_pmaddh(a, b)` | - | - |
| `PMSUBH` | Parallel Multiply-Subtract Halfword | `__builtin_mmi_pmsubh(a, b)` | - | - |
| `PHMADH` | Parallel Horizontal Multiply-Add Halfword | `__builtin_mmi_phmadh(a, b)` | - | - |
| `PHMSBH` | Parallel Horizontal Multiply-Subtract Halfword | `__builtin_mmi_phmsbh(a, b)` | - | - |
| `PDIVBW` | Parallel Divide Broadcast Word | `__builtin_mmi_pdivbw(a, b)` | - | - |
| `PMFHI` | Parallel Move From HI Register | `__builtin_mmi_pmfhi()` | - | - |
| `PMFLO` | Parallel Move From LO Register | `__builtin_mmi_pmflo()` | - | - |
| `PMTHI` | Parallel Move To HI Register | `__builtin_mmi_pmthi(v)` | - | - |
| `PMTLO` | Parallel Move To LO Register | `__builtin_mmi_pmtlo(v)` | - | - |
| `PMFHL.LW` | Parallel Move From HI/LO (Low Word) | `__builtin_mmi_pmfhl_lw()` | - | - |
| `PMFHL.UW` | Parallel Move From HI/LO (Upper Word) | `__builtin_mmi_pmfhl_uw()` | - | - |
| `PMFHL.SLW` | Parallel Move From HI/LO (Signed Low Word) | `__builtin_mmi_pmfhl_slw()` | - | - |
| `PMFHL.LH` | Parallel Move From HI/LO (Low Halfword) | `__builtin_mmi_pmfhl_lh()` | - | - |
| `PMFHL.SH` | Parallel Move From HI/LO (Signed Halfword) | `__builtin_mmi_pmfhl_sh()` | - | - |
| `PMTHL.LW` | Parallel Move To HI/LO (Low Word) | `__builtin_mmi_pmthl_lw(v)` | - | - |

### 2.3 Shift Operations

#### 2.3.1 Parallel Shifts

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `PSLLH` | Parallel Shift Left Logical Halfword | `__builtin_mmi_psllh(a, n)` | `v8hi (<<)` | ✓ |
| `PSRLH` | Parallel Shift Right Logical Halfword | `__builtin_mmi_psrlh(a, n)` | `v8hi (>>)` | ✓ |
| `PSRAH` | Parallel Shift Right Arithmetic Halfword | `__builtin_mmi_psrah(a, n)` | `v8hi (>>)` | ✓ |
| `PSLLW` | Parallel Shift Left Logical Word | `__builtin_mmi_psllw(a, n)` | `v4si (<<)` | ✓ |
| `PSLLVW` | Parallel Shift Left Logical Variable Word | `__builtin_mmi_psllvw(a, b)` | - | - |
| `PSRLW` | Parallel Shift Right Logical Word | `__builtin_mmi_psrlw(a, n)` | `v4si (>>)` | ✓ |
| `PSRLVW` | Parallel Shift Right Logical Variable Word | `__builtin_mmi_psrlvw(a, b)` | - | - |
| `PSRAW` | Parallel Shift Right Arithmetic Word | `__builtin_mmi_psraw(a, n)` | `v4si (>>)` | ✓ |
| `PSRAVW` | Parallel Shift Right Arithmetic Variable Word | `__builtin_mmi_psravw(a, b)` | - | - |

#### 2.3.2 SA Register Operations

| Instruction | Description | Intrinsic | Auto-used |
|-------------|-------------|-----------|-----------|
| `MFSA` | Move From SA Register | `__builtin_mmi_mfsa()` | - |
| `MTSA` | Move To SA Register | `__builtin_mmi_mtsa(v)` | - |
| `MTSAB` | Move Byte Count to SA Register | `__builtin_mmi_mtsab(a, b)` | ✓ |
| `MTSAH` | Move Halfword Count to SA Register | `__builtin_mmi_mtsah(a, b)` | - |
| `QFSRV` | Quadword Funnel Shift Right Variable | `__builtin_mmi_qfsrv(a, b)` | ✓ |

### 2.4 Others (Min/Max, Logical, Absolute)

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `PABSH` | Parallel Absolute Halfword | `__builtin_mmi_pabsh(a)` | - | ✓ |
| `PABSW` | Parallel Absolute Word | `__builtin_mmi_pabsw(a)` | - | ✓ |
| `PMAXH` | Parallel Maximum Halfword | `__builtin_mmi_pmaxh(a, b)` | - | ✓ |
| `PMINH` | Parallel Minimum Halfword | `__builtin_mmi_pminh(a, b)` | - | ✓ |
| `PMAXW` | Parallel Maximum Word | `__builtin_mmi_pmaxw(a, b)` | - | ✓ |
| `PMINW` | Parallel Minimum Word | `__builtin_mmi_pminw(a, b)` | - | ✓ |
| `PAND` | Parallel AND | `__builtin_mmi_pand(a, b)` | `v16qi/v8hi/v4si/v2di (&)` | ✓ |
| `POR` | Parallel OR | `__builtin_mmi_por(a, b)` | `v16qi/v8hi/v4si/v2di (\|)` | ✓ |
| `PXOR` | Parallel XOR | `__builtin_mmi_pxor(a, b)` | `v16qi/v8hi/v4si/v2di (^)` | ✓ |
| `PNOR` | Parallel NOR | `__builtin_mmi_pnor(a, b)` | `v16qi/v8hi/v4si/v2di (~)` | ✓ |

### 2.5 Compare

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `PCGTB` | Parallel Compare for Greater Than Byte | `__builtin_mmi_pcgtb(a, b)` | `v16qi (>)` | ✓ |
| `PCEQB` | Parallel Compare for Equal Byte | `__builtin_mmi_pceqb(a, b)` | `v16qi (==)` | ✓ |
| `PCGTH` | Parallel Compare for Greater Than Halfword | `__builtin_mmi_pcgth(a, b)` | `v8hi (>)` | ✓ |
| `PCEQH` | Parallel Compare for Equal Halfword | `__builtin_mmi_pceqh(a, b)` | `v8hi (==)` | ✓ |
| `PCGTW` | Parallel Compare for Greater Than Word | `__builtin_mmi_pcgtw(a, b)` | `v4si (>)` | ✓ |
| `PCEQW` | Parallel Compare for Equal Word | `__builtin_mmi_pceqw(a, b)` | `v4si (==)` | ✓ |
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
| `PPACB` | Parallel Pack to Byte | `__builtin_mmi_ppacb(a, b)` | - | ✓ (narrow) |
| `PPACH` | Parallel Pack to Halfword | `__builtin_mmi_ppach(a, b)` | - | ✓ (narrow) |
| `PPACW` | Parallel Pack to Word | `__builtin_mmi_ppacw(a, b)` | - | ✓ (narrow) |
| `PPAC5` | Parallel Pack to 5 bits (RGB555 pack) | `__builtin_mmi_ppac5(a)` | - | - |
| `PEXTLB` | Parallel Extend Lower from Byte | `__builtin_mmi_pextlb(a, b)` | - | ✓ (widen) |
| `PEXTLH` | Parallel Extend Lower from Halfword | `__builtin_mmi_pextlh(a, b)` | - | ✓ (widen) |
| `PEXTLW` | Parallel Extend Lower from Word | `__builtin_mmi_pextlw(a, b)` | - | ✓ (widen) |
| `PEXTUB` | Parallel Extend Upper from Byte | `__builtin_mmi_pextub(a, b)` | - | ✓ (widen) |
| `PEXTUH` | Parallel Extend Upper from Halfword | `__builtin_mmi_pextuh(a, b)` | - | ✓ (widen) |
| `PEXTUW` | Parallel Extend Upper from Word | `__builtin_mmi_pextuw(a, b)` | - | ✓ (widen) |
| `PEXT5` | Parallel Extend from 5 bits (RGB555 expand) | `__builtin_mmi_pext5(a)` | - | - |
| `PCPYH` | Parallel Copy Halfword | `__builtin_mmi_pcpyh(a)` | - | - |
| `PCPYLD` | Parallel Copy Lower Doubleword | `__builtin_mmi_pcpyld(a, b)` | - | - |
| `PCPYUD` | Parallel Copy Upper Doubleword | `__builtin_mmi_pcpyud(a, b)` | - | - |
| `PEXCH` | Parallel Exchange Center Halfword | `__builtin_mmi_pexch(a)` | - | - |
| `PEXCW` | Parallel Exchange Center Word | `__builtin_mmi_pexcw(a)` | - | - |
| `PEXEH` | Parallel Exchange Even Halfword | `__builtin_mmi_pexeh(a)` | - | - |
| `PEXEW` | Parallel Exchange Even Word | `__builtin_mmi_pexew(a)` | - | - |
| `PREVH` | Parallel Reverse Halfword | `__builtin_mmi_prevh(a)` | - | - |
| `PINTEH` | Parallel Interleave Even Halfword | `__builtin_mmi_pinteh(a, b)` | - | - |
| `PINTH` | Parallel Interleave Halfword | `__builtin_mmi_pinth(a, b)` | - | - |
| `PROT3W` | Parallel Rotate 3 Words | `__builtin_mmi_prot3w(a)` | - | - |

**Autovectorization Notes**: The data rearrangement instructions are automatically used by GCC for:
- **Widening conversions** (PEXTL*/PEXTU*): Loops like `dst[i] = src[i]` where `dst` is wider than `src` (e.g., `char` to `short`)
- **Narrowing conversions** (PPACB/PPACH/PPACW): Loops like `dst[i] = (T)src[i]` where `dst` is narrower than `src` (e.g., `short` to `char`)
- **Vector interleave**: Shuffle operations with interleave patterns

---

## 3. Dual Pipeline Instructions

R5900 has two multiply/divide units (MAC0/Pipeline 0 and MAC1/Pipeline 1) with dedicated HI/LO registers.
This enables parallel execution of multiply/divide operations on both MAC units for improved throughput.

### Pipeline 0 (MAC0) Intrinsics

| Instruction | Description | Intrinsic | Registers |
|-------------|-------------|-----------|-----------|
| `MULT` | Multiply Word (signed) | `__builtin_mips_mult(a, b)` | HI0:LO0 = a × b |
| `MULTU` | Multiply Word (unsigned) | `__builtin_mips_multu(a, b)` | HI0:LO0 = a × b |
| `DIV` | Divide Word (signed) | `__builtin_mips_div(a, b)` | LO0 = a / b, HI0 = a % b |
| `DIVU` | Divide Word (unsigned) | `__builtin_mips_divu(a, b)` | LO0 = a / b, HI0 = a % b |
| `MADD` | Multiply-Add (signed) | `__builtin_mips_madd(a, b)` | HI0:LO0 += a × b |
| `MADDU` | Multiply-Add (unsigned) | `__builtin_mips_maddu(a, b)` | HI0:LO0 += a × b |
| `MFHI` | Move From HI0 | `__builtin_mips_mfhi()` | Returns HI0 |
| `MFLO` | Move From LO0 | `__builtin_mips_mflo()` | Returns LO0 |
| `MTHI` | Move To HI0 | `__builtin_mips_mthi(v)` | HI0 = v |
| `MTLO` | Move To LO0 | `__builtin_mips_mtlo(v)` | LO0 = v |

### Pipeline 1 (MAC1) Intrinsics

| Instruction | Description | Intrinsic | Registers |
|-------------|-------------|-----------|-----------|
| `MULT1` | Multiply Word (signed) | `__builtin_mips_mult1(a, b)` | HI1:LO1 = a × b |
| `MULTU1` | Multiply Word (unsigned) | `__builtin_mips_multu1(a, b)` | HI1:LO1 = a × b |
| `DIV1` | Divide Word (signed) | `__builtin_mips_div1(a, b)` | LO1 = a / b, HI1 = a % b |
| `DIVU1` | Divide Word (unsigned) | `__builtin_mips_divu1(a, b)` | LO1 = a / b, HI1 = a % b |
| `MADD1` | Multiply-Add (signed) | `__builtin_mips_madd1(a, b)` | HI1:LO1 += a × b |
| `MADDU1` | Multiply-Add (unsigned) | `__builtin_mips_maddu1(a, b)` | HI1:LO1 += a × b |
| `MFHI1` | Move From HI1 | `__builtin_mips_mfhi1()` | Returns HI1 |
| `MFLO1` | Move From LO1 | `__builtin_mips_mflo1()` | Returns LO1 |
| `MTHI1` | Move To HI1 | `__builtin_mips_mthi1(v)` | HI1 = v |
| `MTLO1` | Move To LO1 | `__builtin_mips_mtlo1(v)` | LO1 = v |

### Pipeline 1 Usage Example

```c
// Dual pipeline multiply - both MAC units work in parallel
void dual_multiply(int a, int b, int c, int d, int *result0, int *result1) {
    // Pipeline 1 starts first
    __builtin_mips_mult1(c, d);

    // Pipeline 0 can execute while Pipeline 1 is busy
    *result0 = a * b;

    // Retrieve Pipeline 1 result
    *result1 = __builtin_mips_mflo1();
}

// Full 64-bit multiply using Pipeline 1
long long mult1_64(int a, int b) {
    __builtin_mips_mult1(a, b);
    int hi = __builtin_mips_mfhi1();
    int lo = __builtin_mips_mflo1();
    return ((long long)hi << 32) | (unsigned int)lo;
}

// Division with quotient and remainder via Pipeline 1
void div1_full(int a, int b, int *quot, int *rem) {
    __builtin_mips_div1(a, b);
    *quot = __builtin_mips_mflo1();  // quotient in LO1
    *rem = __builtin_mips_mfhi1();   // remainder in HI1
}

// Multiply-accumulate chain on Pipeline 1
int mac1_chain(int a, int b, int c, int d) {
    __builtin_mips_mult1(a, b);      // HI1:LO1 = a * b
    __builtin_mips_madd1(c, d);      // HI1:LO1 += c * d
    return __builtin_mips_mflo1();   // return low 32 bits
}
```

### HI/LO Register Conflicts with MMI

**Warning**: The R5900 has a single 128-bit HI register and a single 128-bit LO register. These are split into two 64-bit halves:
- **HI0/LO0** (bits 63:0): Used by Pipeline 0 scalar operations (MULT, DIV, MADD)
- **HI1/LO1** (bits 127:64): Used by Pipeline 1 scalar operations (MULT1, DIV1, MADD1)

**MMI parallel multiply/divide instructions (PMULTW, PMADDW, PDIVW, etc.) update BOTH halves simultaneously.** This means:

1. MMI operations will **clobber Pipeline 1 scalar results** in HI1/LO1
2. Users must complete MFHI1/MFLO1 **before** any MMI multiply/divide
3. Mixing Pipeline 1 scalar and MMI operations requires careful scheduling

```c
// CORRECT: Read Pipeline 1 result before MMI operation
__builtin_mips_mult1(a, b);
int result = __builtin_mips_mflo1();  // Save result FIRST
v2di vec_result = __builtin_mmi_pmultw(x, y);  // Now safe to use MMI

// INCORRECT: MMI will clobber the Pipeline 1 result
__builtin_mips_mult1(a, b);
v2di vec_result = __builtin_mmi_pmultw(x, y);  // Clobbers HI1/LO1!
int result = __builtin_mips_mflo1();  // Wrong value!
```

GCC models these register conflicts and will schedule instructions correctly when both operations are visible in the same function. However, be careful with inline assembly or across function boundaries.

---

## 4. COP1 (FPU) Extensions

R5900 FPU is single-precision only with additional operations. Note: Double precision is NOT supported.

### 4.1 Accumulator Operations

The FPU has a dedicated accumulator register for efficient FMA chains. ACC is a fixed register - use intrinsics only (no automatic optimization).

| Instruction | Description | Intrinsic | Auto-used |
|-------------|-------------|-----------|-----------|
| `ADDA.S` | Add to Accumulator | `__builtin_mips_adda_s(a, b)` | - |
| `SUBA.S` | Subtract to Accumulator | `__builtin_mips_suba_s(a, b)` | - |
| `MULA.S` | Multiply to Accumulator | `__builtin_mips_mula_s(a, b)` | - |
| `MADD.S` | Multiply-Add (ACC + fs * ft) | `__builtin_mips_madd_s(a, b)` | - |
| `MADDA.S` | Multiply-Add to Accumulator | `__builtin_mips_madda_s(a, b)` | - |
| `MSUB.S` | Multiply-Subtract (ACC - fs * ft) | `__builtin_mips_msub_s(a, b)` | - |
| `MSUBA.S` | Multiply-Subtract to Accumulator | `__builtin_mips_msuba_s(a, b)` | - |

### 4.2 Min/Max/Reciprocal

| Instruction | Description | Intrinsic | Auto-used |
|-------------|-------------|-----------|-----------|
| `MIN.S` | Floating-Point Minimum | `__builtin_mips_min_s(a, b)` | ✓ |
| `MAX.S` | Floating-Point Maximum | `__builtin_mips_max_s(a, b)` | ✓ |
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
| `VADD.xyzw` | dest = a + b | `__builtin_vu0_vadd(a, b)` | `v4sf (+)` | ✓ |
| `VSUB.xyzw` | dest = a - b | `__builtin_vu0_vsub(a, b)` | `v4sf (-)` | ✓ |
| `VMUL.xyzw` | dest = a * b | `__builtin_vu0_vmul(a, b)` | `v4sf (*)` | ✓ |
| `VABS.xyzw` | dest = \|a\| | `__builtin_vu0_vabs(a)` | - | ✓ |
| `VMAX.xyzw` | dest = max(a, b) | `__builtin_vu0_vmax(a, b)` | - | ✓ |
| `VMINI.xyzw` | dest = min(a, b) | `__builtin_vu0_vmini(a, b)` | - | ✓ |
| `VMOVE.xyzw` | dest = src | `__builtin_vu0_vmove(a)` | - | ✓ |

### 5.3 Broadcast Arithmetic (bc = x/y/z/w)

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `VADDbc.xyzw` | dest = a + b.bc | `__builtin_vu0_vaddbc(a, b)` | - | - |
| `VSUBbc.xyzw` | dest = a - b.bc | `__builtin_vu0_vsubbc(a, b)` | - | - |
| `VMULbc.xyzw` | dest = a * b.bc | `__builtin_vu0_vmulbc(a, b)` | - | - |

### 5.4 Multiply-Accumulate (bc variants: x/y/z/w)

VU0 has an implicit ACC register for efficient FMA chains.

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `VMULA.xyzw` | ACC = a * b | `__builtin_vu0_vmula(a, b)` | - | - |
| `VMULAbc.xyzw` | ACC = a * b.bc | `__builtin_vu0_mulabc(a, b)` | - | - |
| `VMADDA.xyzw` | ACC += a * b | `__builtin_vu0_vmadda(a, b)` | - | - |
| `VMADDAbc.xyzw` | ACC += a * b.bc | `__builtin_vu0_vmaddabc(a, b)` | - | - |
| `VMADD.xyzw` | dest = ACC + a * b | `__builtin_vu0_vmadd(a, b)` | - | - |
| `VMADDbc.xyzw` | dest = ACC + a * b.bc | `__builtin_vu0_vmaddbc(a, b)` | - | - |
| `VMSUBA.xyzw` | ACC -= a * b | `__builtin_vu0_vmsuba(a, b)` | - | - |
| `VMSUBAbc.xyzw` | ACC -= a * b.bc | `__builtin_vu0_vmsubabc(a, b)` | - | - |
| `VMSUB.xyzw` | dest = ACC - a * b | `__builtin_vu0_vmsub(a, b)` | - | - |
| `VMSUBbc.xyzw` | dest = ACC - a * b.bc | `__builtin_vu0_vmsubbc(a, b)` | - | - |

### 5.5 Add/Subtract to Accumulator (bc variants: x/y/z/w)

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `VADDA.xyzw` | ACC = a + b | `__builtin_vu0_vadda(a, b)` | - | - |
| `VADDAbc.xyzw` | ACC = a + b.bc | `__builtin_vu0_vaddabc(a, b)` | - | - |
| `VSUBA.xyzw` | ACC = a - b | `__builtin_vu0_vsuba(a, b)` | - | - |
| `VSUBAbc.xyzw` | ACC = a - b.bc | `__builtin_vu0_vsubabc(a, b)` | - | - |

### 5.6 Outer Product (Cross Product)

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `VOPMULA.xyz` | ACC.xyz = a.yzx * b.zxy | `__builtin_vu0_vopmula(a, b)` | - | - |
| `VOPMSUB.xyz` | dest.xyz = ACC - a.zxy * b.yzx | `__builtin_vu0_vopmsub(a, b)` | - | - |

### 5.7 Conversion Operations

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `VFTOI0.xyzw` | Float to 32-bit integer | `__builtin_vu0_vftoi0(a)` | - | - |
| `VFTOI4.xyzw` | Float to 28.4 fixed-point | `__builtin_vu0_vftoi4(a)` | - | - |
| `VFTOI12.xyzw` | Float to 20.12 fixed-point | `__builtin_vu0_vftoi12(a)` | - | - |
| `VFTOI15.xyzw` | Float to 17.15 fixed-point | `__builtin_vu0_vftoi15(a)` | - | - |
| `VITOF0.xyzw` | 32-bit integer to float | `__builtin_vu0_vitof0(a)` | - | - |
| `VITOF4.xyzw` | 28.4 fixed-point to float | `__builtin_vu0_vitof4(a)` | - | - |
| `VITOF12.xyzw` | 20.12 fixed-point to float | `__builtin_vu0_vitof12(a)` | - | - |
| `VITOF15.xyzw` | 17.15 fixed-point to float | `__builtin_vu0_vitof15(a)` | - | - |

### 5.8 Data Movement

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `VMR32.xyzw` | dest.xyzw = src.yzwx | `__builtin_vu0_vmr32(a)` | - | - |

### 5.9 I Register Operations

The I (Immediate) register holds a 32-bit floating-point value loaded via `CTC2` instruction. Operations suffixed with `i` use this implicit value.

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `CTC2 $reg, $21` | Load I register from GP | `__builtin_vu0_ctc2_i(int bits)` | - | - |
| `CFC2 $reg, $21` | Read I register to GP | `__builtin_vu0_cfc2_i()` | - | - |
| `VADDi.xyzw` | dest = a + I | `__builtin_vu0_vaddi(a)` | - | - |
| `VSUBi.xyzw` | dest = a - I | `__builtin_vu0_vsubi(a)` | - | - |
| `VMULi.xyzw` | dest = a * I | `__builtin_vu0_vmuli(a)` | - | - |
| `VMADDi.xyzw` | dest = ACC + a * I | `__builtin_vu0_vmaddi(a)` | - | - |
| `VMSUBi.xyzw` | dest = ACC - a * I | `__builtin_vu0_vmsubi(a)` | - | - |
| `VMADDAi.xyzw` | ACC += a * I | `__builtin_vu0_vmaddai(a)` | - | - |
| `VMSUBAi.xyzw` | ACC -= a * I | `__builtin_vu0_vmsubai(a)` | - | - |
| `VMULAi.xyzw` | ACC = a * I | `__builtin_vu0_vmulai(a)` | - | - |
| `VADDAi.xyzw` | ACC = a + I | `__builtin_vu0_vaddai(a)` | - | - |
| `VSUBAi.xyzw` | ACC = a - I | `__builtin_vu0_vsubai(a)` | - | - |

**Usage Example:**
```c
typedef float v4sf __attribute__((vector_size(16)));

v4sf scale_by_constant(v4sf v, float scale) {
    // Load scalar into I register
    __builtin_vu0_ctc2_i(*(int*)&scale);
    // Multiply using I register
    return __builtin_vu0_vmuli(v);
}
```

**Autovectorization Note:** Vector-by-scalar operations (`v * scalar`) are autovectorized using **VMULx/VADDx/VSUBx** (broadcast component) instructions instead of the I register. This approach fits GCC's register model better since the scalar is placed in a normal VU0 register:
```c
v4sf v = ...; float s = ...;
v4sf result = v * s;  // Generates: mfc1, qmtc2, vmulx.xyzw
```

### 5.10 Q Register Operations

The Q register holds the result of division and square root operations. These operations have multi-cycle latency; use `VWAITQ` or check status flags before reading Q.

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
| `VDIV` | Q = fs.bc / ft.bc | `__builtin_vu0_vdiv(v4sf fs, v4sf ft)` | - | - |
| `VSQRT` | Q = sqrt(ft.bc) | `__builtin_vu0_vsqrt(v4sf ft)` | - | - |
| `VRSQRT` | Q = fs.bc / sqrt(ft.bc) | `__builtin_vu0_vrsqrt(v4sf fs, v4sf ft)` | - | - |
| `VWAITQ` | Wait for Q register ready | `__builtin_vu0_vwaitq()` | - | - |
| `VADDq.xyzw` | dest = a + Q | `__builtin_vu0_vaddq(a)` | - | - |
| `VSUBq.xyzw` | dest = a - Q | `__builtin_vu0_vsubq(a)` | - | - |
| `VMULq.xyzw` | dest = a * Q | `__builtin_vu0_vmulq(a)` | - | - |
| `VMADDq.xyzw` | dest = ACC + a * Q | `__builtin_vu0_vmaddq(a)` | - | - |
| `VMSUBq.xyzw` | dest = ACC - a * Q | `__builtin_vu0_vmsubq(a)` | - | - |
| `VMADDAq.xyzw` | ACC += a * Q | `__builtin_vu0_vmaddaq(a)` | - | - |
| `VMSUBAq.xyzw` | ACC -= a * Q | `__builtin_vu0_vmsubaq(a)` | - | - |
| `VMULAq.xyzw` | ACC = a * Q | `__builtin_vu0_vmulaq(a)` | - | - |
| `VADDAq.xyzw` | ACC = a + Q | `__builtin_vu0_vaddaq(a)` | - | - |
| `VSUBAq.xyzw` | ACC = a - Q | `__builtin_vu0_vsubaq(a)` | - | - |

**Usage Example:**
```c
typedef float v4sf __attribute__((vector_size(16)));

// Divide and broadcast result to all lanes
v4sf divide_broadcast(v4sf a, v4sf b) {
    __builtin_vu0_vdiv(a, b);   // Q = a.x / b.x
    __builtin_vu0_vwaitq();     // Wait for division to complete
    return __builtin_vu0_vmulq(/* ones vector */);  // Multiply by Q to broadcast
}
```

### 5.11 Not Yet Implemented

| Instruction | Description | Intrinsic | Vector | Autovectorize |
|-------------|-------------|-----------|--------|---------------|
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
| MMI Multiply/Divide | 13 | 13 | 100% |
| MMI Data Rearrangement | 22 | 22 | 100% |
| MMI Format Convert | 2 | 2 | 100% |
| MMI Other | 2 | 0 | 0% |
| MMI HI/LO | 10 | 10 | 100% |
| Dual Pipeline | 12 | 12 | 100% |
| SA Register | 5 | 5 | 100% |
| FPU Extensions | 11 | 11 | 100% |
| VU0 Data Transfer | 6 | 4 | 67% |
| VU0 Basic Arithmetic | 7 | 7 | 100% |
| VU0 Broadcast Arithmetic | 12 | 12 | 100% |
| VU0 Multiply-Accumulate | 30 | 30 | 100% |
| VU0 Add/Sub Accumulator | 10 | 10 | 100% |
| VU0 Outer Product | 2 | 2 | 100% |
| VU0 Conversions | 8 | 8 | 100% |
| VU0 Data Movement | 1 | 1 | 100% |
| VU0 I Register | 12 | 12 | 100% |
| VU0 Q Register | 14 | 14 | 100% |
| VU0 VI/Other | ~10 | 0 | 0% |

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
