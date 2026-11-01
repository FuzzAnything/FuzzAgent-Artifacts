// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "zlib.h"

/* Helper to safely extract a value from input data */
static uint32_t extract_u32(const uint8_t *data, size_t offset, size_t size) {
    if (offset + 4 > size) {
        return 0;
    }
    uint32_t val = 0;
    memcpy(&val, data + offset, 4);
    return val;
}

/* Helper to safely extract a 64-bit value from input data */
static int64_t extract_i64(const uint8_t *data, size_t offset, size_t size) {
    if (offset + 8 > size) {
        return 0;
    }
    int64_t val = 0;
    memcpy(&val, data + offset, 8);
    return val;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    /* Extract parameters from input */
    uint32_t crc1 = extract_u32(Data, 0, Size);
    uint32_t crc2 = extract_u32(Data, 4, Size);
    uint32_t adler1 = extract_u32(Data, 8, Size);
    uint32_t adler2 = extract_u32(Data, 12, Size);
    
    /* Extract lengths, ensuring they are positive and within reasonable bounds */
    int64_t len2_32 = extract_i64(Data, 16, Size);
    int64_t len2_64 = extract_i64(Data, 24, Size);
    
    /* Clamp lengths to avoid overflow issues */
    if (len2_32 < 0) len2_32 = 0;
    if (len2_64 < 0) len2_64 = 0;
    
    /* Limit maximum length to prevent excessive computation */
    const int64_t MAX_LEN = 1000000000LL;
    if (len2_32 > MAX_LEN) len2_32 = MAX_LEN;
    if (len2_64 > MAX_LEN) len2_64 = MAX_LEN;

    /* 1. Test crc32_z */
    /* Initialize CRC */
    uLong crc = crc32_z(0, Z_NULL, 0);
    
    /* Update with input data if available */
    if (Size > 32) {
        size_t buf_len = (Size > 32) ? (Size - 32) : 0;
        crc = crc32_z(crc, Data + 32, (z_size_t)buf_len);
    }

    /* 2. Test adler32_combine */
    uLong combined_adler_32 = adler32_combine(adler1, adler2, (z_off_t)len2_32);
    (void)combined_adler_32; /* Suppress unused variable warning */

    /* 3. Test adler32_combine64 */
    uLong combined_adler_64 = adler32_combine64(adler1, adler2, (z_off64_t)len2_64);
    (void)combined_adler_64;

    /* 4. Test crc32_combine */
    uLong combined_crc_32 = crc32_combine(crc1, crc2, (z_off_t)len2_32);
    (void)combined_crc_32;

    /* 5. Test crc32_combine_gen */
    uLong gen_table = crc32_combine_gen((z_off_t)len2_32);
    (void)gen_table;

    /* 6. Test crc32_combine64 */
    uLong combined_crc_64 = crc32_combine64(crc1, crc2, (z_off64_t)len2_64);
    (void)combined_crc_64;

    /* Test crc32_combine using the generated table if needed (internal logic uses it) */
    /* Note: crc32_combine internally handles the generation or usage, so explicit usage of gen_table 
       with crc32_combine is not directly exposed in the API signature provided, but we tested gen separately. */

    return 0;
}