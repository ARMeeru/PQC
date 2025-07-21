#include <stdint.h>
#include <stdio.h>

// This file demonstrates how fixed-width integer types work in C (super
// important in cryptography), what happens when they overflow, and how to
// safely do arithmetic with overflow detection.

// PQC (Post-Quantum Cryptography) code always uses these specific types so
// behavior stays predictable across all platforms.
void
understand_types ()
{
  // Let's assign the largest possible values to each unsigned type to make the
  // effects of overflow visible.

  uint8_t byte = 255;         // 8-bit unsigned integer: stores values 0 to 255
  uint16_t half = 65535;      // 16-bit unsigned integer: 0 to 65535
  uint32_t word = 0xFFFFFFFF; // 32-bit unsigned: 0 to 4,294,967,295
                              // (0xFFFFFFFF = 32 ones in binary)
  uint64_t dword = 0xFFFFFFFFFFFFFFFF; // 64-bit unsigned: HUGE, 0 to
                                       // 18,446,744,073,709,551,615

  // Unsigned integer overflow is well-defined in C: it wraps around (modular
  // arithmetic). Here, byte is 255 (binary 11111111). Adding 1 turns it into 0
  // (wraps around to the start).
  byte += 1;
  printf ("255 + 1 = %u\n", byte); // Prints: 255 + 1 = 0

  // For signed integers, overflow is UNDEFINED BEHAVIOR. This means anything
  // could happen: the value could wrap, your program could crash, or the
  // compiler could even optimize the code away! This is NEVER safe in
  // security/crypto code.

  int16_t signed_bad = 32767; // The maximum value of a 16-bit signed integer
  // signed_bad += 1;  // Don't do this! The result isn't guaranteed and could
  // bite you in production.
}

// This function safely adds two 16-bit unsigned integers.
// It checks for overflow and only stores the result if it's valid.
// Returns 0 if successful, -1 if adding would overflow.
int
safe_add_u16 (uint16_t a, uint16_t b, uint16_t *result)
{
  // Promote both a and b to 32 bits before adding to preserve the full sum,
  // even if it doesn't fit in 16 bits.
  uint32_t sum
      = (uint32_t)a
        + (uint32_t)
            b; // Cast ensures we don't lose information during addition.

  // UINT16_MAX is 65535 (the largest value a uint16_t can hold).
  // If the sum is bigger than this, storing it in a uint16_t would wrap it
  // around, which we want to avoid.
  if (sum > UINT16_MAX)
    return -1; // Overflow detected! Signal an error.

  // Only reached if no overflow happened.
  *result = (uint16_t)sum; // Save the sum at the location 'result' points to
                           // (think: "output parameter").
  return 0;                // Success!
}

/*
Summary of lessons:
- Use unsigned types and avoid signed overflow in cryptography.
- Always check for overflow before storing results in fixed-width types.
- Use pointers ("*result") to let functions return results via output
parameters.
- Promote types before arithmetic to safely check for overflow.
- Stick to fixed-width types (uint8_t, uint16_t, ...) for predictability and
cross-platform correctness.
*/

int
main ()
{
  understand_types ();
  uint16_t result;
  safe_add_u16 (32767, 1, &result);
  printf ("Result: %hu\n", result);
  return 0;
}
