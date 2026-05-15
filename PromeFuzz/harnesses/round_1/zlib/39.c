// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for various operations
    if (Size < 24) {  // Need at least 24 bytes for the three 8-byte reads
        return 0;
    }

    // Extract parameters from fuzzer input safely
    uLong crc1, crc2, adler1, adler2;
    z_off64_t len2;
    
    // Use memcpy to avoid alignment issues and buffer overflows
    memcpy(&crc1, Data, sizeof(uLong));
    memcpy(&crc2, Data + sizeof(uLong), sizeof(uLong));
    memcpy(&len2, Data + 2 * sizeof(uLong), sizeof(z_off64_t));
    
    // Ensure len2 is non-negative for combine functions
    if (len2 < 0) {
        len2 = -len2;
    }

    // Test crc32_combine64
    uLong combined_crc = crc32_combine64(crc1, crc2, len2);
    (void)combined_crc;

    // Test crc32_z - ensure we don't read past the buffer
    size_t offset = 3 * sizeof(uLong);
    if (Size > offset) {
        size_t buffer_size = Size - offset;
        const Bytef* buffer = (const Bytef*)(Data + offset);
        uLong crc = crc32_z(0L, buffer, buffer_size);
        (void)crc;
    }

    // Test adler32
    memcpy(&adler1, Data + sizeof(uLong), sizeof(uLong));
    memcpy(&adler2, Data + 2 * sizeof(uLong), sizeof(uLong));
    
    if (Size > offset) {
        size_t buffer_size = Size - offset;
        const Bytef* buffer = (const Bytef*)(Data + offset);
        uInt adler_buffer_size = (uInt)(buffer_size > 65535 ? 65535 : buffer_size);
        uLong adler = adler32(0L, buffer, adler_buffer_size);
        (void)adler;
    }

    // Test adler32_combine64
    uLong combined_adler = adler32_combine64(adler1, adler2, len2);
    (void)combined_adler;

    // Test adler32_z
    if (Size > offset) {
        size_t buffer_size = Size - offset;
        const Bytef* buffer = (const Bytef*)(Data + offset);
        uLong adler_z = adler32_z(1L, buffer, buffer_size);
        (void)adler_z;
    }

    // Test adler32_combine
    z_off_t len2_offt = (z_off_t)len2;
    if (len2_offt < 0) {
        len2_offt = -len2_offt;
    }
    uLong combined_adler_offt = adler32_combine(adler1, adler2, len2_offt);
    (void)combined_adler_offt;

    return 0;
}