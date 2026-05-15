// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// get_crc_table at crc32.c:482:29 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"

static uLong safe_read_uLong(const uint8_t *data, size_t size, size_t offset) {
    uLong value = 0;
    if (offset + sizeof(uLong) <= size) {
        memcpy(&value, data + offset, sizeof(uLong));
    }
    return value;
}

static z_off_t safe_read_z_off_t(const uint8_t *data, size_t size, size_t offset) {
    z_off_t value = 0;
    if (offset + sizeof(z_off_t) <= size) {
        memcpy(&value, data + offset, sizeof(z_off_t));
    }
    return value;
}

static z_off64_t safe_read_z_off64_t(const uint8_t *data, size_t size, size_t offset) {
    z_off64_t value = 0;
    if (offset + sizeof(z_off64_t) <= size) {
        memcpy(&value, data + offset, sizeof(z_off64_t));
    }
    return value;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Safely read parameters from fuzzer input
    uLong crc1 = safe_read_uLong(Data, Size, 0);
    uLong crc2 = safe_read_uLong(Data, Size, sizeof(uLong));
    
    // For length parameters, ensure non-negative
    z_off_t len2 = safe_read_z_off_t(Data, Size, 2 * sizeof(uLong));
    if (len2 < 0) {
        len2 = -len2;
    }
    
    z_off64_t len2_64 = safe_read_z_off64_t(Data, Size, 2 * sizeof(uLong));
    if (len2_64 < 0) {
        len2_64 = -len2_64;
    }
    
    // Extract operator for crc32_combine_op
    uLong op = safe_read_uLong(Data, Size, 3 * sizeof(uLong));
    
    // Buffer for crc32 function
    const Bytef* buf = NULL;
    uInt buf_len = 0;
    
    size_t buffer_offset = 4 * sizeof(uLong);
    if (buffer_offset < Size) {
        buf = (const Bytef*)(Data + buffer_offset);
        buf_len = (uInt)(Size - buffer_offset);
        // Ensure we don't exceed uInt range
        if (buf_len != (Size - buffer_offset)) {
            buf_len = 0xFFFFFFFF;  // Max uInt value
        }
    }

    // Test crc32_combine64
    (void)crc32_combine64(crc1, crc2, len2_64);
    
    // Test crc32_combine
    (void)crc32_combine(crc1, crc2, len2);
    
    // Test crc32_combine_gen
    uLong gen_op = crc32_combine_gen(len2);
    
    // Test crc32_combine_op
    (void)crc32_combine_op(crc1, crc2, op);
    (void)crc32_combine_op(crc1, crc2, gen_op);
    
    // Test crc32 with various configurations
    uLong crc = 0;
    
    // Initial call with zero CRC
    crc = crc32(0, buf, buf_len);
    
    // Update CRC with same buffer
    if (buf_len > 0) {
        crc = crc32(crc, buf, buf_len);
    }
    
    // Test with NULL buffer and zero length
    (void)crc32(crc, Z_NULL, 0);
    
    // Test incremental CRC calculation
    if (buf_len > 10) {
        uLong partial_crc = crc32(0, buf, 5);
        partial_crc = crc32(partial_crc, buf + 5, buf_len - 5);
    }
    
    // Test get_crc_table
    const z_crc_t* crc_table = get_crc_table();
    (void)crc_table;
    
    return 0;
}