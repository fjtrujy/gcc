/* Test SLP vectorization of struct member initialization with misaligned offsets.
   This is the pattern that originally caused the ICE - multiple consecutive
   scalar stores to struct members that the SLP vectorizer merges into a
   single vector store at a misaligned offset.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -O2 -ftree-slp-vectorize" } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

/* Reproduce the original gsKit GSGLOBAL struct layout pattern.
   The key is having an aligned(64) field that creates misaligned
   offsets for subsequent regular int fields.  */
struct gs_global {
    int field1;
    int field2;
    int field3;
    int field4;
    /* Lots of fields... simulated with padding */
    char padding[180];
    /* Aligned field creates offset jump */
    void *dma_misc __attribute__ ((aligned (64)));
    /* These are at misaligned offsets after the aligned field */
    int PSM;            /* These four will be SLP-merged */
    int PSMZ;
    int PrimContext;
    int PrimFogEnable;
    int PrimAAEnable;
    int PrimAlphaEnable;
    long long PrimAlpha;
};

void *calloc(unsigned int, unsigned int);

/* This function triggers SLP vectorization of the consecutive zero stores.
   The SLP vectorizer merges PSM, PSMZ, PrimContext, PrimFogEnable into
   a V4SI store, and PrimAAEnable, PrimAlphaEnable + PrimAlpha into another.
   These stores are at misaligned offsets due to the aligned(64) field.  */
NOMIPS16 struct gs_global *
init_gs_global (void)
{
    struct gs_global *p = calloc(1, sizeof(struct gs_global));
    
    /* Aligned stores (offset 0) - will use sq */
    p->field1 = 2;
    p->field2 = 1;
    p->field3 = 0;
    p->field4 = 32768;
    
    p->dma_misc = (void *)0x70000000;
    
    /* Misaligned consecutive stores - SLP merges these.
       After the aligned(64) dma_misc, these are at:
       PSM: offset 256 + 0 = 256 (16-byte aligned - OK)
       PSMZ: offset 256 + 4
       PrimContext: offset 256 + 8
       PrimFogEnable: offset 256 + 12
       These four MIGHT get merged at offset 256 which is aligned.
       
       But PrimAAEnable starts at 256+16=272, PrimAlphaEnable at 276...
       These would be misaligned if merged with different fields.  */
    p->PSM = 0;
    p->PSMZ = 0;
    p->PrimContext = 0;
    p->PrimFogEnable = 0;
    p->PrimAAEnable = 0;
    p->PrimAlphaEnable = 0;
    p->PrimAlpha = 0x44;
    
    return p;
}

/* Should compile without ICE.  */
/* Aligned vector stores use sq.  */
/* { dg-final { scan-assembler "\\tsq\\t" } } */
/* May also have sd for misaligned portions or scalar stores.  */

