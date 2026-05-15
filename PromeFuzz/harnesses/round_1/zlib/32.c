// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

static void dummy_crc32_z_test(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLong crc = 0;
    const Bytef *buf = (const Bytef *)data;
    z_size_t len = (z_size_t)(size > 1000 ? 1000 : size);
    
    crc = crc32_z(crc, buf, len);
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Extract parameters from fuzzer input safely
    uLong crc1 = 0;
    uLong crc2 = 0;
    
    // Read crc1 and crc2 safely, using available bytes
    if (Size >= sizeof(uLong)) {
        memcpy(&crc1, Data, sizeof(uLong));
    } else {
        memcpy(&crc1, Data, Size);
    }
    
    if (Size >= 2 * sizeof(uLong)) {
        memcpy(&crc2, Data + sizeof(uLong), sizeof(uLong));
    } else if (Size > sizeof(uLong)) {
        memcpy(&crc2, Data + sizeof(uLong), Size - sizeof(uLong));
    }
    
    // Extract length parameters safely
    size_t offset = 2 * sizeof(uLong);
    uint8_t len_byte = 0;
    
    if (offset < Size) {
        len_byte = Data[offset];
    } else if (Size > 0) {
        // If we don't have enough data for offset, use the last byte
        len_byte = Data[Size - 1];
    }
    
    // Ensure non-negative lengths by masking
    z_off_t len_t = (z_off_t)(len_byte & 0x7F);
    z_off64_t len64_t = (z_off64_t)(len_byte & 0x7F);
    
    // Test crc32_combine64
    crc32_combine64(crc1, crc2, len64_t);
    
    // Test crc32_combine
    crc32_combine(crc1, crc2, len_t);
    
    // Test crc32_combine_gen
    uLong op = crc32_combine_gen(len_t);
    
    // Test crc32_combine_op
    crc32_combine_op(crc1, crc2, op);
    
    // Test crc32_combine_gen64
    uLong op64 = crc32_combine_gen64(len64_t);
    crc32_combine_op(crc1, crc2, op64);
    
    // Test crc32_z with the fuzzer input
    dummy_crc32_z_test(Data, Size);
    
    return 0;
}