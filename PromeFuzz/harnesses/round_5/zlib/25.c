// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for basic operations
    if (Size < 8) {
        return 0;
    }

    // Split input data for various parameters
    size_t split = Size / 2;
    const Bytef *buf = (const Bytef *)Data;
    uInt len1 = (uInt)(split < 0xFFFF ? split : 0xFFFF);
    uInt len2 = (uInt)((Size - split) < 0xFFFF ? (Size - split) : 0xFFFF);
    
    // Extract values for combine functions from the data
    uLong crc1 = *(const uLong*)Data;
    uLong crc2 = *(const uLong*)(Data + sizeof(uLong));
    uLong adler1 = *(const uLong*)(Data + 2*sizeof(uLong));
    uLong adler2 = *(const uLong*)(Data + 3*sizeof(uLong));
    
    // Get len2 parameter for combine functions (ensure non-negative)
    z_off64_t len2_64 = (z_off64_t)(Size & 0x7FFFFFFF); // Ensure non-negative
    z_off_t len2_off = (z_off_t)(Size & 0x7FFF); // Ensure non-negative for 32-bit
    
    // Test crc32_combine64
    (void)crc32_combine64(crc1, crc2, len2_64);
    
    // Test adler32_z with various adler values
    uLong adler = 1L; // Initial value
    (void)adler32_z(adler, buf, Size);
    (void)adler32_z(0L, buf, Size);
    (void)adler32_z(adler1, buf, Size);
    
    // Test with NULL buffer (should be valid)
    (void)adler32_z(adler, NULL, 0);
    
    // Test adler32_combine64
    (void)adler32_combine64(adler1, adler2, len2_64);
    
    // Test crc32 function
    uLong crc = 0L;
    crc = crc32(crc, buf, len1);
    crc = crc32(crc, buf + len1, len2);
    
    // Test with Z_NULL and zero length (initialization)
    (void)crc32(0L, Z_NULL, 0);
    
    // Test adler32 function
    adler = adler32(0L, Z_NULL, 0); // Initialize
    adler = adler32(adler, buf, len1);
    adler = adler32(adler, buf + len1, len2);
    
    // Test adler32_combine
    (void)adler32_combine(adler1, adler2, len2_off);
    
    // Test edge cases for combine functions
    (void)crc32_combine64(0, 0, 0);
    (void)adler32_combine64(1L, 1L, 0);
    (void)adler32_combine(1L, 1L, 0);
    
    // Test with maximum length values (within reasonable bounds)
    if (Size > 100) {
        (void)crc32(crc, buf, 100);
        (void)adler32(adler, buf, 100);
        (void)adler32_z(adler, buf, 100);
    }
    
    return 0;
}