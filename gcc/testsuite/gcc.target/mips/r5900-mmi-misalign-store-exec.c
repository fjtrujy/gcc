/* Runtime test for R5900 MMI misaligned vector stores.
   Verifies that misaligned stores write the correct data.  */
/* { dg-do run { target { mips*-*-* } } } */
/* { dg-options "-march=r5900 -O2" } */

typedef int v4si __attribute__ ((vector_size (16)));

/* Buffer with space before and after for detecting overwrites.  */
struct test_buffer {
    int before[4];     /* offset 0-15: guard area */
    int header;        /* offset 16: creates misalignment */
    int data[4];       /* offset 20-35: misaligned target */
    int after[4];      /* offset 36-51: guard area */
} __attribute__((packed));

void abort (void);

__attribute__((noinline, nomips16)) void
store_misaligned (struct test_buffer *buf, v4si val)
{
    /* Store to misaligned offset (20 % 16 = 4).  */
    *((v4si *)buf->data) = val;
}

int main (void)
{
    struct test_buffer buf;
    int i;
    
    /* Initialize with pattern to detect overwrites.  */
    for (i = 0; i < 4; i++)
    {
        buf.before[i] = 0xDEADBEEF;
        buf.data[i] = 0;
        buf.after[i] = 0xCAFEBABE;
    }
    buf.header = 0x12345678;
    
    /* Store vector to misaligned address.  */
    v4si val = {0x11111111, 0x22222222, 0x33333333, 0x44444444};
    store_misaligned (&buf, val);
    
    /* Verify the store wrote correct values.  */
    if (buf.data[0] != 0x11111111) abort ();
    if (buf.data[1] != 0x22222222) abort ();
    if (buf.data[2] != 0x33333333) abort ();
    if (buf.data[3] != 0x44444444) abort ();
    
    /* Verify guard areas weren't corrupted.  */
    for (i = 0; i < 4; i++)
    {
        if (buf.before[i] != (int)0xDEADBEEF) abort ();
        if (buf.after[i] != (int)0xCAFEBABE) abort ();
    }
    
    /* Verify header wasn't corrupted.  */
    if (buf.header != 0x12345678) abort ();
    
    return 0;
}

