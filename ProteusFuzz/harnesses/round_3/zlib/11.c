// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use the input data as the buffer for checksum calculations
    const Bytef *buf = (const Bytef *)Data;
    
    // Initialize checksums
    uLong adler1 = 1; // Initial value for adler32
    uLong adler2 = 1;
    uLong crc1 = 0;
    uLong crc2 = 0;

    // Calculate initial checksums for the first part of the data
    size_t len1 = Size / 2;
    if (len1 == 0) len1 = 1;
    
    adler1 = adler32_z(adler1, buf, len1);
    crc1 = crc32_z(crc1, buf, len1);
    
    // Calculate checksums for the second part of the data
    size_t len2 = Size - len1;
    const Bytef *buf2 = buf + len1;
    
    adler2 = adler32_z(adler2, buf2, len2);
    crc2 = crc32_z(crc2, buf2, len2);

    // Combine checksums using 32-bit length functions
    z_off_t len2_off = (z_off_t)len2;
    uLong combined_adler32 = adler32_combine(adler1, adler2, len2_off);
    uLong combined_crc32 = crc32_combine(crc1, crc2, len2_off);

    // Combine checksums using 64-bit length functions
    z_off64_t len2_off64 = (z_off64_t)len2;
    uLong combined_adler32_64 = adler32_combine64(adler1, adler2, len2_off64);
    uLong combined_crc32_64 = crc32_combine(crc1, crc2, len2_off); // crc32_combine uses 32-bit internally but delegates to 64-bit

    // Test with NULL buffer for adler32_z
    uLong adler_null = adler32_z(0, Z_NULL, 0);

    // Test with varying lengths for crc32
    uInt len_uInt = (uInt)(len2 < UINT_MAX ? len2 : UINT_MAX);
    uLong crc32_result = crc32(0, buf2, len_uInt);

    // Test with zero length
    uLong adler_zero = adler32_z(1, buf, 0);
    uLong crc_zero = crc32_z(0, buf, 0);

    // Ensure all return values are used to prevent compiler warnings
    (void)combined_adler32;
    (void)combined_crc32;
    (void)combined_adler32_64;
    (void)combined_crc32_64;
    (void)adler_null;
    (void)crc32_result;
    (void)adler_zero;
    (void)crc_zero;

    return 0;
}