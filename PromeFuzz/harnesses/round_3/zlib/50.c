// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "code/zlib.h"

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
        return 0;
    }

    // Extract parameters from fuzzer input
    uLong crc1 = *((uLong*)Data);
    uLong crc2 = *((uLong*)(Data + 4));
    z_off_t len2_32 = (z_off_t)(*((int32_t*)(Data + 8)));
    z_off64_t len2_64 = (z_off64_t)(*((int64_t*)(Data + 8)));
    
    // Ensure non-negative lengths for functions that require it
    if (len2_32 < 0) len2_32 = -len2_32;
    if (len2_64 < 0) len2_64 = -len2_64;
    
    // Test crc32_combine64
    uLong result1 = crc32_combine64(crc1, crc2, len2_64);
    
    // Test crc32_combine
    uLong result2 = crc32_combine(crc1, crc2, len2_32);
    
    // Test crc32_combine_gen64
    uLong op64 = crc32_combine_gen64(len2_64);
    
    // Test crc32_combine_gen
    uLong op32 = crc32_combine_gen(len2_32);
    
    // Test crc32_combine_op with generated operators
    uLong result3 = crc32_combine_op(crc1, crc2, op64);
    uLong result4 = crc32_combine_op(crc1, crc2, op32);
    
    // Test crc32 function
    // Use remaining data as buffer for crc32
    size_t remaining_size = Size - 16;
    if (remaining_size > 0) {
        const Bytef* buf = (const Bytef*)(Data + 16);
        uInt len = (uInt)(remaining_size > UINT_MAX ? UINT_MAX : remaining_size);
        
        // Test incremental CRC calculation
        uLong crc = crc32(0L, Z_NULL, 0);
        crc = crc32(crc, buf, len);
        
        // Test with non-zero initial CRC
        crc = crc32(crc1, buf, len);
    }
    
    // Test edge cases
    crc32_combine64(0, 0, 0);
    crc32_combine(0, 0, 0);
    crc32_combine_op(0, 0, 0);
    crc32_combine_gen64(0);
    crc32_combine_gen(0);
    crc32(0, Z_NULL, 0);
    
    return 0;
}