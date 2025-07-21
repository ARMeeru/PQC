/*
 * Exercise: Unsigned and Signed Integer Overflow Demo
 *
 * Practice and observe how integer overflow works in C
 * with fixed-width integer types. You will:
 *   1. Demonstrate unsigned overflow for 8, 16, and 32-bit integers.
 *   2. Demonstrate (carefully!) signed overflow for 8 and 16-bit integers.
 *   3. Print each result and comment on the behavior.
 *   4. (Bonus) Try subtracting from zero in unsigned types and describe what happens.
 *
 * Instructions:
 * - Complete the code sections marked 'TODO'.
 * - Try editing input values and observe how output changes.
 * - Comment your observations.
 */

#include <stdio.h>
#include <stdint.h>

int main() {
    // 1. Demonstrate unsigned overflow:
    uint8_t u8 = 255;
    uint16_t u16 = 65535;
    uint32_t u32 = 4294967295U;

    // TODO: Add 1 to each variable and print the result.
    // Example:
    // printf("u8 before: %u, after: %u\n", u8, u8 + 1);

    // 2. Demonstrate signed overflow (undefined!):
    int8_t s8 = 127;
    int16_t s16 = 32767;

    // TODO: Add 1 to each signed variable and print.
    // Warning: This is undefined behavior. What does your compiler/platform do?

    // 3. Bonus: Subtract 1 from 0 for uint8_t and uint16_t
    uint8_t u8_zero = 0;
    uint16_t u16_zero = 0;

    // TODO: Subtract 1 and print the results.
    // What do you observe and why does it happen?

    return 0;
}

/*
 * Extension:
 * 1. Try the same with int32_t and larger types.
 * 2. Try with variables initialized to values other than the maximum.
 * 3. Write down your predictions BEFORE running each section.
 * 4. (Research) Why is unsigned overflow defined and signed overflow undefined in C?
 */