// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32_z at adler32.c:61:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize variables
    uLong crc1 = 0, crc2 = 0, adler = 0, op = 0;
    z_off64_t len2 = 0;
    uInt u_len = 0;
    z_size_t z_len = 0;
    
    // Use first 8 bytes for crc1 (if available)
    if (Size >= sizeof(uLong)) {
        memcpy(&crc1, Data, sizeof(uLong));
    }
    
    // Use next 8 bytes for crc2 (if available)
    if (Size >= 2 * sizeof(uLong)) {
        memcpy(&crc2, Data + sizeof(uLong), sizeof(uLong));
    }
    
    // Use next 8 bytes for len2 (if available), ensure non-negative
    if (Size >= 3 * sizeof(uLong)) {
        memcpy(&len2, Data + 2 * sizeof(uLong), sizeof(z_off64_t));
        if (len2 < 0) {
            len2 = -len2;
        }
    }
    
    // Use next 8 bytes for op (if available)
    if (Size >= 4 * sizeof(uLong)) {
        memcpy(&op, Data + 3 * sizeof(uLong), sizeof(uLong));
    }
    
    // Calculate buffer pointer and length for buffer-based functions
    const Bytef *buf = NULL;
    size_t buf_offset = 0;
    
    // Use remaining data as buffer
    if (Size > 4 * sizeof(uLong)) {
        buf_offset = 4 * sizeof(uLong);
        buf = (const Bytef *)(Data + buf_offset);
        size_t remaining = Size - buf_offset;
        
        // For uInt length functions, cap at UINT_MAX if needed
        u_len = (remaining > UINT_MAX) ? UINT_MAX : (uInt)remaining;
        z_len = remaining;
    }
    
    // Test crc32_combine64
    if (Size >= 3 * sizeof(uLong)) {
        crc32_combine64(crc1, crc2, len2);
    }
    
    // Test crc32_z
    if (buf != NULL) {
        crc32_z(crc1, buf, z_len);
    }
    
    // Test adler32
    if (buf != NULL) {
        adler32(adler, buf, u_len);
    }
    
    // Test crc32
    if (buf != NULL) {
        crc32(crc1, buf, u_len);
    }
    
    // Test adler32_z
    if (buf != NULL) {
        adler32_z(adler, buf, z_len);
    }
    
    // Test crc32_combine_op
    if (Size >= 3 * sizeof(uLong)) {
        crc32_combine_op(crc1, crc2, op);
    }
    
    // Additional test: use different combinations of parameters
    if (Size >= sizeof(uLong)) {
        // Test with zero buffer
        crc32_z(crc1, NULL, 0);
        adler32_z(adler, NULL, 0);
        crc32(crc1, NULL, 0);
        adler32(adler, NULL, 0);
        
        // Test with initial values
        crc32_z(0L, buf, z_len);
        adler32_z(1L, buf, z_len);
        crc32(0L, buf, u_len);
        adler32(1L, buf, u_len);
    }
    
    return 0;
}