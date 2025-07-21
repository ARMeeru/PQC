/*
 * Exercise: Safe uint16_t Addition with Overflow Detection
 *
 * Practice writing a function that adds two unsigned 16-bit integers
 * and checks for overflow before storing the result.
 *
 * Objectives:
 *   1. Implement a function `int safe_add_u16(uint16_t a, uint16_t b, uint16_t
 * *result)` that:
 *         - Adds a and b
 *         - If the sum fits in 16 bits: stores it in *result and returns 0
 *         - If it would overflow: returns -1 and does NOT store a value
 *   2. Test your function with various input pairs to show:
 *         - No overflow
 *         - Boundary case (maximum valid sum)
 *         - Actual overflow (e.g., 40000 + 40000)
 *   3. Print the results and observe what happens.
 *
 * Skeleton below provides an incomplete implementation. Fill in the TODOs!
 */

#include <stdint.h>
#include <stdio.h>

/**
 * Attempt to add two uint16_t numbers safely.
 * Returns 0 on success, -1 on overflow.
 */
int
safe_add_u16 (uint16_t a, uint16_t b, uint16_t *result)
{
  // TODO: Promote a and b to larger type (e.g., uint32_t), perform addition,
  //       check for overflow, set *result if safe, return status code.
  return -99; // Placeholder: replace with your code
}

int
main ()
{
  uint16_t res;

  // Test 1: No overflow
  if (safe_add_u16 (12345, 22222, &res) == 0)
    printf ("12345 + 22222 = %u (safe)\n", res);
  else
    printf ("12345 + 22222 = overflow!\n");

  // Test 2: Boundary case (max sum that fits)
  if (safe_add_u16 (65535, 0, &res) == 0)
    printf ("65535 + 0 = %u (safe)\n", res);
  else
    printf ("65535 + 0 = overflow!\n");

  // Test 3: Actual overflow
  if (safe_add_u16 (40000, 40000, &res) == 0)
    printf ("40000 + 40000 = %u (safe)\n", res);
  else
    printf ("40000 + 40000 = overflow!\n");

  // TODO: Add more test cases as you see fit, including edge values.

  return 0;
}

/*
 * Extension:
 * 1. Try making a similar function for subtraction. What about safe multiply?
 * 2. Think: Is there a way to implement this with bitwise operations instead
 * of promotion?
 * 3. Why is the result parameter a pointer?
 */
