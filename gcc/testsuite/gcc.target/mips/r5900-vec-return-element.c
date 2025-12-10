/* Test R5900 128-bit vector return value element access.
   This test verifies that elements accessed from function return values
   are correct for all 128-bit vector types.

   Compile: mips64r5900el-ps2-elf-gcc -march=r5900 -mvu0 -mabi=n32 -O2 \
            r5900-vec-return-element.c -o vec-test.elf
   Run:     flatpak run --filesystem=host net.pcsx2.PCSX2 /path/to/vec-test.elf
*/

#include <stdio.h>

/* Vector type definitions */
typedef float v4sf __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));
typedef short v8hi __attribute__((vector_size(16)));
typedef signed char v16qi __attribute__((vector_size(16)));
typedef long long v2di __attribute__((vector_size(16)));

/* =========================================================================
   V4SF - 4x 32-bit float (VU0 registers, returns in $vf1)
   ========================================================================= */

__attribute__((noinline)) v4sf
return_v4sf (void)
{
  return (v4sf){1.0f, 2.0f, 3.0f, 4.0f};
}

int
test_v4sf (void)
{
  v4sf result = return_v4sf ();
  int errors = 0;

  printf ("  v4sf: {%f, %f, %f, %f}\n",
          result[0], result[1], result[2], result[3]);

  if (result[0] != 1.0f) { printf ("  FAIL: v4sf[0] expected 1.0, got %f\n", result[0]); errors++; }
  if (result[1] != 2.0f) { printf ("  FAIL: v4sf[1] expected 2.0, got %f\n", result[1]); errors++; }
  if (result[2] != 3.0f) { printf ("  FAIL: v4sf[2] expected 3.0, got %f\n", result[2]); errors++; }
  if (result[3] != 4.0f) { printf ("  FAIL: v4sf[3] expected 4.0, got %f\n", result[3]); errors++; }

  if (errors == 0)
    printf ("  PASS: v4sf all elements correct\n");

  return errors;
}

/* =========================================================================
   V4SI - 4x 32-bit int (GP registers, returns in $2)
   ========================================================================= */

__attribute__((noinline)) v4si
return_v4si (void)
{
  return (v4si){100, 200, 300, 400};
}

int
test_v4si (void)
{
  v4si result = return_v4si ();
  int errors = 0;

  printf ("  v4si: {%d, %d, %d, %d}\n",
          result[0], result[1], result[2], result[3]);

  if (result[0] != 100) { printf ("  FAIL: v4si[0] expected 100, got %d\n", result[0]); errors++; }
  if (result[1] != 200) { printf ("  FAIL: v4si[1] expected 200, got %d\n", result[1]); errors++; }
  if (result[2] != 300) { printf ("  FAIL: v4si[2] expected 300, got %d\n", result[2]); errors++; }
  if (result[3] != 400) { printf ("  FAIL: v4si[3] expected 400, got %d\n", result[3]); errors++; }

  if (errors == 0)
    printf ("  PASS: v4si all elements correct\n");

  return errors;
}

/* =========================================================================
   V8HI - 8x 16-bit short (GP registers, returns in $2)
   ========================================================================= */

__attribute__((noinline)) v8hi
return_v8hi (void)
{
  return (v8hi){10, 20, 30, 40, 50, 60, 70, 80};
}

int
test_v8hi (void)
{
  v8hi result = return_v8hi ();
  int errors = 0;

  printf ("  v8hi: {%d, %d, %d, %d, %d, %d, %d, %d}\n",
          result[0], result[1], result[2], result[3],
          result[4], result[5], result[6], result[7]);

  if (result[0] != 10) { printf ("  FAIL: v8hi[0] expected 10, got %d\n", result[0]); errors++; }
  if (result[1] != 20) { printf ("  FAIL: v8hi[1] expected 20, got %d\n", result[1]); errors++; }
  if (result[2] != 30) { printf ("  FAIL: v8hi[2] expected 30, got %d\n", result[2]); errors++; }
  if (result[3] != 40) { printf ("  FAIL: v8hi[3] expected 40, got %d\n", result[3]); errors++; }
  if (result[4] != 50) { printf ("  FAIL: v8hi[4] expected 50, got %d\n", result[4]); errors++; }
  if (result[5] != 60) { printf ("  FAIL: v8hi[5] expected 60, got %d\n", result[5]); errors++; }
  if (result[6] != 70) { printf ("  FAIL: v8hi[6] expected 70, got %d\n", result[6]); errors++; }
  if (result[7] != 80) { printf ("  FAIL: v8hi[7] expected 80, got %d\n", result[7]); errors++; }

  if (errors == 0)
    printf ("  PASS: v8hi all elements correct\n");

  return errors;
}

/* =========================================================================
   V16QI - 16x 8-bit signed char (GP registers, returns in $2)
   ========================================================================= */

__attribute__((noinline)) v16qi
return_v16qi (void)
{
  return (v16qi){1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
}

int
test_v16qi (void)
{
  v16qi result = return_v16qi ();
  int errors = 0;

  printf ("  v16qi: {%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d}\n",
          result[0], result[1], result[2], result[3],
          result[4], result[5], result[6], result[7],
          result[8], result[9], result[10], result[11],
          result[12], result[13], result[14], result[15]);

  for (int i = 0; i < 16; i++)
    {
      if (result[i] != (i + 1))
        {
          printf ("  FAIL: v16qi[%d] expected %d, got %d\n", i, i + 1, result[i]);
          errors++;
        }
    }

  if (errors == 0)
    printf ("  PASS: v16qi all elements correct\n");

  return errors;
}

/* =========================================================================
   V2DI - 2x 64-bit long long (GP registers, returns in $2)
   ========================================================================= */

__attribute__((noinline)) v2di
return_v2di (void)
{
  return (v2di){0x123456789ABCDEF0LL, 0xFEDCBA9876543210LL};
}

int
test_v2di (void)
{
  v2di result = return_v2di ();
  int errors = 0;

  printf ("  v2di: {0x%llx, 0x%llx}\n", result[0], result[1]);

  if (result[0] != 0x123456789ABCDEF0LL)
    {
      printf ("  FAIL: v2di[0] expected 0x123456789ABCDEF0, got 0x%llx\n", result[0]);
      errors++;
    }
  if (result[1] != 0xFEDCBA9876543210LL)
    {
      printf ("  FAIL: v2di[1] expected 0xFEDCBA9876543210, got 0x%llx\n", result[1]);
      errors++;
    }

  if (errors == 0)
    printf ("  PASS: v2di all elements correct\n");

  return errors;
}

/* =========================================================================
   Additional test: v4sf with parameter passing
   ========================================================================= */

__attribute__((noinline)) v4sf
return_v4sf_modified (v4sf input)
{
  return (v4sf){input[0] + 1.0f, input[1] + 1.0f, input[2] + 1.0f, input[3] + 1.0f};
}

int
test_v4sf_param (void)
{
  v4sf input = (v4sf){10.0f, 20.0f, 30.0f, 40.0f};
  v4sf result = return_v4sf_modified (input);
  int errors = 0;

  printf ("  v4sf param: {%f, %f, %f, %f}\n",
          result[0], result[1], result[2], result[3]);

  if (result[0] != 11.0f) { printf ("  FAIL: v4sf_param[0] expected 11.0, got %f\n", result[0]); errors++; }
  if (result[1] != 21.0f) { printf ("  FAIL: v4sf_param[1] expected 21.0, got %f\n", result[1]); errors++; }
  if (result[2] != 31.0f) { printf ("  FAIL: v4sf_param[2] expected 31.0, got %f\n", result[2]); errors++; }
  if (result[3] != 41.0f) { printf ("  FAIL: v4sf_param[3] expected 41.0, got %f\n", result[3]); errors++; }

  if (errors == 0)
    printf ("  PASS: v4sf with parameter all elements correct\n");

  return errors;
}

/* =========================================================================
   Main test driver
   ========================================================================= */

int
main (void)
{
  int total_errors = 0;

  printf ("=== R5900 Vector Return Element Access Test ===\n\n");

  printf ("Testing v4sf (4x float, VU0 $vf1):\n");
  total_errors += test_v4sf ();
  printf ("\n");

  printf ("Testing v4si (4x int, GP $2):\n");
  total_errors += test_v4si ();
  printf ("\n");

  printf ("Testing v8hi (8x short, GP $2):\n");
  total_errors += test_v8hi ();
  printf ("\n");

  printf ("Testing v16qi (16x char, GP $2):\n");
  total_errors += test_v16qi ();
  printf ("\n");

  printf ("Testing v2di (2x long long, GP $2):\n");
  total_errors += test_v2di ();
  printf ("\n");

  printf ("Testing v4sf with parameter:\n");
  total_errors += test_v4sf_param ();
  printf ("\n");

  printf ("=== Summary ===\n");
  printf ("Total errors: %d\n", total_errors);

  if (total_errors == 0)
    printf ("ALL TESTS PASSED\n");
  else
    printf ("SOME TESTS FAILED\n");

  /* Loop forever so PCSX2 doesn't exit immediately */
  while (1)
    ;

  return total_errors;
}
