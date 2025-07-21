/*
 * Exercise: Integer Type Experiments
 *
 * In this exercise, you'll explore the behavior of C's integer types through
 * experimentation. This will deepen your practical understanding before
 * entering modular arithmetic.
 *
 * Objectives:
 *   1. Assign out-of-range values to various integer types. Observe what
 * happens at compile and run time.
 *   2. Mix signed and unsigned types in expressions and print the results.
 *   3. Use sizeof to verify the memory size of each standard type.
 *   4. (Optional) Experiment with type conversion (casting) between
 * signed/unsigned and different bit-widths.
 *
 * Instructions:
 *   - Complete each TODO section below.
 *   - Record your predictions before running, then compare with actual output.
 *   - Comment your findings directly in the code!
 */

#include <stdint.h>
#include <stdio.h>

int
main ()
{
  // 1. Assign out-of-range values to each type and print the result
  uint8_t u8 = 300;     // 300 is greater than 255; what happens?
  int8_t s8 = 130;      // 130 > 127; how is it stored?
  int16_t s16 = -40000; // Less than INT16_MIN

  printf ("uint8_t u8 = 300; Value: %u (expected: 300 mod 256 = 44)\n", u8);
  printf ("int8_t s8 = 130;  Value: %d (expected: wraps to negative?)\n", s8);
  printf ("int16_t s16 = -40000; Value: %d\n", s16);

  // 2. Mix signed and unsigned types in an expression
  uint16_t a = 60000;
  int16_t b = -5;
  int result = a + b; // What happens here? Is the result as expected?

  printf ("uint16_t a = 60000, int16_t b = -5; a + b = %d\n", result);

  // 3. Use sizeof to display how much memory each type uses
  printf ("sizeof(uint8_t): %zu\n", sizeof (uint8_t));
  printf ("sizeof(int8_t): %zu\n", sizeof (int8_t));
  printf ("sizeof(uint16_t): %zu\n", sizeof (uint16_t));
  printf ("sizeof(int16_t): %zu\n", sizeof (int16_t));
  printf ("sizeof(uint32_t): %zu\n", sizeof (uint32_t));
  printf ("sizeof(int32_t): %zu\n", sizeof (int32_t));
  printf ("sizeof(uint64_t): %zu\n", sizeof (uint64_t));
  printf ("sizeof(int64_t): %zu\n", sizeof (int64_t));

  // 4. (Optional) Experiment with type conversion/casting
  int8_t small = -50;
  uint8_t casted = (uint8_t)small; // What value does this produce?

  printf ("int8_t small = -50; casted to uint8_t: %u\n", casted);

  /*
   * Extension:
   * 1. Try arithmetic with mixed types, e.g., uint8_t + int16_t, and see what
   * result type you get.
   * 2. Try bitwise operations between signed and unsigned. Does the result
   * always make sense?
   * 3. Summarize any compiler warnings you receive, and adjust code to avoid
   * or silence them.
   */

  return 0;
}
