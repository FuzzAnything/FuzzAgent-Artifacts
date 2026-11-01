// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// get_crc_table at crc32.c:482:29 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void fuzzer_seed_data(uint8_t *data, size_t size) {
    if (size == 0) return;
    // Simple seeding to ensure non-zero data for interesting paths
    for (size_t i = 0; i < size; i++) {
        data[i] = (uint8_t)((i * 17 + 13) % 256);
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare environment and data
    // We need a mutable buffer for crc32
    uint8_t *buffer = (uint8_t *)malloc(Size);
    if (!buffer) {
        return 0;
    }
    memcpy(buffer, Data, Size);
    
    // Seed the buffer to ensure variety if input is all zeros
    fuzzer_seed_data(buffer, Size);

    // 2. Test get_crc_table
    // This function is safe to call repeatedly and returns a static table.
    const z_crc_t *crc_table = get_crc_table();
    if (crc_table != NULL) {
        // Just accessing the table to ensure it's initialized
        (void)crc_table[0];
    }

    // 3. Test crc32
    // Compute CRC over the input data.
    uLong crc = crc32(0L, buffer, (uInt)Size);
    // Compute incremental CRC
    if (Size > 1) {
        uLong partial_crc = crc32(0L, buffer, (uInt)(Size / 2));
        crc = crc32(partial_crc, buffer + (Size / 2), (uInt)(Size - Size / 2));
    }

    // 4. Test crc32_combine_gen and crc32_combine
    // We need a length for the second stream. Use modulo to keep it within bounds.
    z_off_t len2 = (z_off_t)(Size % 1000000);
    if (len2 < 0) len2 = 0;
    
    uLong op = crc32_combine_gen(len2);
    // Combine two CRCs: one from the first half, one from the second half (simulated)
    // crc1 is from first part, crc2 is from second part
    uLong crc1 = crc32(0L, buffer, (uInt)(Size / 2));
    uLong crc2 = crc32(0L, buffer + (Size / 2), (uInt)(Size - Size / 2));
    
    uLong combined_crc = crc32_combine(crc1, crc2, len2);
    (void)combined_crc;

    // 5. Test crc32_combine_gen64
    z_off64_t len2_64 = (z_off64_t)(Size % 1000000);
    if (len2_64 < 0) len2_64 = 0;
    
    uLong op64 = crc32_combine_gen64(len2_64);
    (void)op64;

    // 6. Test crc32_combine_op
    // Use the generated 'op' or a value derived from input
    uLong op_val = (Size > 0) ? (uLong)Data[0] : 0;
    // Ensure op is not 0 to test the calculation path, or 0 to test the return 0 path
    uLong combined_op = crc32_combine_op(crc1, crc2, op_val);
    
    // Also test with op=0 explicitly
    uLong combined_op_zero = crc32_combine_op(crc1, crc2, 0);
    (void)combined_op;
    (void)combined_op_zero;

    // 7. Cleanup
    free(buffer);

    return 0;
}