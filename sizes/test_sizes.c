#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>


int main() {
    // Signed integers
    int intVariable;
    int64_t int64Variable;

    // Unsigned integers
    uint32_t uint32Variable;
    uint64_t uint64Variable;

    // Print sizes and ranges
    printf("Size of int: %lu bytes\n", sizeof(int));
    printf("Size of int64_t: %lu bytes\n", sizeof(int64_t));
    printf("Size of uint32_t: %lu bytes\n", sizeof(uint32_t));
    printf("Size of uint64_t: %lu bytes\n", sizeof(uint64_t));

    // Maximum and minimum values
    printf("\nSigned Integer Range:\n");
    printf("   int: %d to %d\n", INT_MIN, INT_MAX);
    printf("   int64_t: %" PRId64 " to %" PRId64 "\n", INT64_MIN, INT64_MAX);

    printf("\nUnsigned Integer Range:\n");
    printf("   uint32_t: 0 to %" PRIu32 "\n", UINT32_MAX);
    printf("   uint64_t: 0 to %" PRIu64 "\n", UINT64_MAX);

    return 0;
}
