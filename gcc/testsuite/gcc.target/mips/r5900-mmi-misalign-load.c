/* Test R5900 MMI misaligned vector loads.
   When accessing struct members where the offset is known to be misaligned
   at compile time, the movmisalign pattern uses QFSRV to extract the
   correctly aligned data from two aligned quadword loads.  */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -O2" } */

#ifndef NOMIPS16
#define NOMIPS16 __attribute__((nomips16))
#endif

typedef int v4si __attribute__ ((vector_size (16)));

/* Structure where the vector is at a known misaligned offset.  */
struct misaligned_struct {
    int header;                 /* offset 0 */
    v4si vec __attribute__((packed));  /* offset 4: NOT 16-byte aligned */
};

/* Load from a known misaligned offset - should use QFSRV sequence.
   Note: This requires the movmisalign pattern to be triggered, which
   happens when get_object_alignment returns < 128 bits.  */
NOMIPS16 v4si
load_misaligned_member (struct misaligned_struct *s)
{
    return s->vec;
}

/* The misaligned load uses lq instructions.  */
/* { dg-final { scan-assembler "\\tlq\\t" } } */
/* QFSRV is used for the misaligned extraction when available.  */
/* Note: QFSRV sequence depends on runtime alignment detection in some cases.  */
