// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// get_crc_table at crc32.c:482:29 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    // Extract parameters from fuzzer input
    uLong crc1 = *(const uLong*)Data;
    uLong crc2 = *(const uLong*)(Data + 4);
    
    // Ensure non-negative lengths for combine functions
    z_off64_t len2_64 = (z_off64_t)(Size);
    z_off_t len2 = (z_off_t)(Size);
    if (len2_64 < 0) len2_64 = -len2_64;
    if (len2 < 0) len2 = -len2;
    
    // Buffer for crc32 and crc32_z functions
    const Bytef* buf = (const Bytef*)(Data);
    uInt len_uInt = (uInt)(Size > UINT_MAX ? UINT_MAX : Size);
    z_size_t len_z = (z_size_t)Size;
    
    // Test crc32_combine64
    uLong combined64 = crc32_combine64(crc1, crc2, len2_64);
    (void)combined64;
    
    // Test crc32
    uLong crc_result = crc32(0, Z_NULL, 0);
    if (Size > 0) {
        crc_result = crc32(crc_result, buf, len_uInt);
    }
    
    // Test crc32_z
    uLong crc_z_result = crc32_z(0, Z_NULL, 0);
    if (Size > 0) {
        crc_z_result = crc32_z(crc_z_result, buf, len_z);
    }
    
    // Test crc32_combine_gen
    uLong combine_gen_result = crc32_combine_gen(len2);
    (void)combine_gen_result;
    
    // Test crc32_combine
    uLong combined = crc32_combine(crc1, crc2, len2);
    (void)combined;
    
    // Test get_crc_table
    const z_crc_t FAR* crc_table = get_crc_table();
    (void)crc_table;
    
    // Additional test: combine using generated operator
    if (len2 >= 0) {
        uLong op = crc32_combine_gen(len2);
        uLong combined_op = crc32_combine_op(crc1, crc2, op);
        (void)combined_op;
    }
    
    // Test incremental CRC calculation
    if (Size > 16) {
        size_t chunk1 = Size / 3;
        size_t chunk2 = Size / 3;
        size_t chunk3 = Size - chunk1 - chunk2;
        
        uLong running_crc = crc32(0L, Z_NULL, 0);
        running_crc = crc32(running_crc, buf, (uInt)chunk1);
        running_crc = crc32(running_crc, buf + chunk1, (uInt)chunk2);
        running_crc = crc32(running_crc, buf + chunk1 + chunk2, (uInt)chunk3);
        
        // Compare with single call
        uLong single_crc = crc32(0L, buf, (uInt)Size);
        (void)single_crc;
    }
    
    return 0;
}