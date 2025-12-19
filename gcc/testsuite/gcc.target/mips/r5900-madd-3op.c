/* Test R5900 Pipeline 0 3-operand multiply-add builtins */
/* { dg-do compile } */
/* { dg-options "-march=r5900 -mhard-float" } */

/*
 * R5900 supports 3-operand multiply-add on Pipeline 0.
 * These forms compute (HI:LO + rs*rt) and write the low 32 bits
 * to both a GPR and LO (HI gets updated high 32 bits).
 *
 * Available builtins:
 * - __builtin_mips_madd_3op(acc, a, b)  -> madd rd,rs,rt (signed)
 * - __builtin_mips_maddu_3op(acc, a, b) -> maddu rd,rs,rt (unsigned)
 *
 * The first argument 'acc' represents the current HI:LO value.
 */

/* Helper to get initial HI:LO value using inline assembly */
static inline long long get_hilo_signed (int a, int b)
{
  long long result;
  int hi, lo;
  __asm__ volatile ("mult\t%1,%2\n\tmfhi\t%0\n\tmflo\t%3"
                    : "=r" (hi), "=r" (a), "=r" (b), "=r" (lo)
                    : "1" (a), "2" (b)
                    : "hi", "lo");
  result = ((long long)hi << 32) | (unsigned int)lo;
  return result;
}

static inline unsigned long long get_hilo_unsigned (unsigned int a, unsigned int b)
{
  unsigned long long result;
  unsigned int hi, lo;
  __asm__ volatile ("multu\t%1,%2\n\tmfhi\t%0\n\tmflo\t%3"
                    : "=r" (hi), "=r" (a), "=r" (b), "=r" (lo)
                    : "1" (a), "2" (b)
                    : "hi", "lo");
  result = ((unsigned long long)hi << 32) | lo;
  return result;
}

/* Test signed 3-operand multiply-add */
int test_madd_3op (int a, int b, int c, int d)
{
  /* First multiply to set HI:LO */
  long long acc = get_hilo_signed (a, b);
  /* Then madd with result to GPR */
  return __builtin_mips_madd_3op (acc, c, d);
}

/* Test unsigned 3-operand multiply-add */
unsigned int test_maddu_3op (unsigned int a, unsigned int b, 
                             unsigned int c, unsigned int d)
{
  /* First multiply to set HI:LO */
  unsigned long long acc = get_hilo_unsigned (a, b);
  /* Then maddu with result to GPR */
  return __builtin_mips_maddu_3op (acc, c, d);
}

/* { dg-final { scan-assembler "\tmult\t" } } */
/* { dg-final { scan-assembler "\tmultu\t" } } */
/* { dg-final { scan-assembler "\tmadd\t\\\$\[0-9\]+,\\\$\[0-9\]+,\\\$\[0-9\]+" } } */
/* { dg-final { scan-assembler "\tmaddu\t\\\$\[0-9\]+,\\\$\[0-9\]+,\\\$\[0-9\]+" } } */

