// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_z at crc32.c:626:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // Handle empty input
    if (Size == 0) {
        return 0;
    }

    // Prepare buffer for functions that need it
    Bytef *buffer = (Bytef *)Data;
    
    // Split input for different parameters safely
    size_t half_size = Size / 2;
    size_t quarter_size = Size / 4;
    
    // Get initial values from input data safely
    uLong crc_init = 0;
    uLong adler_init = 1L;
    
    // Safely extract initial values without buffer overflow
    if (Size >= sizeof(uLong)) {
        // Copy only available bytes for crc_init
        size_t copy_size = (Size >= sizeof(uLong)) ? sizeof(uLong) : Size;
        memcpy(&crc_init, Data, copy_size);
    }
    
    if (Size >= 2 * sizeof(uLong)) {
        // Copy only available bytes for adler_init
        size_t copy_size = (Size - sizeof(uLong) >= sizeof(uLong)) ? sizeof(uLong) : Size - sizeof(uLong);
        memcpy(&adler_init, Data + sizeof(uLong), copy_size);
    }

    // 1. Test crc32_z
    uLong crc_result = crc32_z(crc_init, buffer, Size);
    
    // Test with different lengths
    if (half_size > 0) {
        crc_result = crc32_z(crc_result, buffer + half_size, half_size);
    }
    
    // 2. Test crc32_combine64
    uLong crc1 = crc32_z(0, buffer, quarter_size);
    uLong crc2 = crc32_z(0, buffer + quarter_size, quarter_size);
    z_off64_t len2 = (z_off64_t)quarter_size;
    
    if (len2 >= 0) {
        uLong combined_crc = crc32_combine64(crc1, crc2, len2);
        (void)combined_crc;
    }
    
    // 3. Test adler32
    uLong adler_result = adler32(adler_init, buffer, (uInt)Size);
    
    // Test with different lengths
    if (half_size > 0) {
        adler_result = adler32(adler_result, buffer + half_size, (uInt)half_size);
    }
    
    // 4. Test adler32_combine
    uLong adler1 = adler32(1L, buffer, (uInt)quarter_size);
    uLong adler2 = adler32(1L, buffer + quarter_size, (uInt)quarter_size);
    z_off_t len2_off = (z_off_t)quarter_size;
    
    if (len2_off >= 0) {
        uLong combined_adler = adler32_combine(adler1, adler2, len2_off);
        (void)combined_adler;
    }
    
    // 5. Test adler32_z
    uLong adler_z_result = adler32_z(adler_init, buffer, Size);
    
    // Test with different lengths
    if (half_size > 0) {
        adler_z_result = adler32_z(adler_z_result, buffer + half_size, half_size);
    }
    
    // 6. Test adler32_combine64
    uLong adler_z1 = adler32_z(1L, buffer, quarter_size);
    uLong adler_z2 = adler32_z(1L, buffer + quarter_size, quarter_size);
    z_off64_t len2_64 = (z_off64_t)quarter_size;
    
    if (len2_64 >= 0) {
        uLong combined_adler64 = adler32_combine64(adler_z1, adler_z2, len2_64);
        (void)combined_adler64;
    }
    
    // Test edge cases
    // NULL buffer with zero length
    crc32_z(0, NULL, 0);
    adler32(0L, NULL, 0);
    adler32_z(0L, NULL, 0);
    
    // Test with zero length buffer (non-NULL)
    if (Size > 0) {
        crc32_z(crc_init, buffer, 0);
        adler32(adler_init, buffer, 0);
        adler32_z(adler_init, buffer, 0);
    }
    
    // Test combine functions with zero length
    crc32_combine64(crc1, crc2, 0);
    adler32_combine(adler1, adler2, 0);
    adler32_combine64(adler_z1, adler_z2, 0);
    
    return 0;
}