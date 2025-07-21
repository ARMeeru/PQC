/*
 * Exercise: Print Ranges of Standard Integer Types
 *
 * Your task is to write a C program that:
 *   1. Prints the minimum and maximum values for
 *      - int8_t, uint8_t
 *      - int16_t, uint16_t
 *      - int32_t, uint32_t
 *      - int64_t, uint64_t
 *   2. Uses both the limits defined in <stdint.h>/<limits.h>
 *   3. Shows that these limits match what you expect for the bit-width
 *   4. (Bonus) Print the size (in bytes) of each type using sizeof
 *
 * Instructions:
 *   - Fill in the TODO sections.
 *   - Use printf to display each limit.
 *   - Compare output to theory (e.g., 2^8-1 for uint8_t max)
 */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>

int main() {
    // 8-bit signed and unsigned
    printf("int8_t:   min = %" PRId8 ", max = %" PRId8 ", sizeof = %zu\n",
           INT8_MIN, INT8_MAX, sizeof(int8_t));
    printf("uint8_t:  min = %" PRIu8 ", max = %" PRIu8 ", sizeof = %zu\n",
           0, UINT8_MAX, sizeof(uint8_t));

    // 16-bit signed and unsigned
    printf("int16_t:  min = %" PRId16 ", max = %" PRId16 ", sizeof = %zu\n",
           INT16_MIN, INT16_MAX, sizeof(int16_t));
    printf("uint16_t: min = %" PRIu16 ", max = %" PRIu16 ", sizeof = %zu\n",
           0, UINT16_MAX, sizeof(uint16_t));

    // 32-bit signed and unsigned
    printf("int32_t:  min = %" PRId32 ", max = %" PRId32 ", sizeof = %zu\n",
           INT32_MIN, INT32_MAX, sizeof(int32_t));
    printf("uint32_t: min = %" PRIu32 ", max = %" PRIu32 ", sizeof = %zu\n",
           0U, UINT32_MAX, sizeof(uint32_t));

    // 64-bit signed and unsigned
    printf("int64_t:  min = %" PRId64 ", max = %" PRId64 ", sizeof = %zu\n",
           INT64_MIN, INT64_MAX, sizeof(int64_t));
    printf("uint64_t: min = %" PRIu64 ", max = %" PRIu64 ", sizeof = %zu\n",
           0ULL, UINT64_MAX, sizeof(uint64_t));

    /*
     * Extension:
     * 1. Try to write your own macros to compute 2^N, then compare with STDINT constants.
     * 2. Compare with <limits.h> types: CHAR_MIN, SHRT_MAX, etc.
     */

    return 0;
}