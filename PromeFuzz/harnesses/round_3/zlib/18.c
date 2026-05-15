// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_crc32(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLong crc = 0L;
    const Bytef *buf = (const Bytef *)data;
    
    // Test crc32 with full buffer
    crc = crc32(crc, buf, (uInt)size);
    
    // Test incremental updates
    if (size > 1) {
        uLong crc2 = 0L;
        size_t half = size / 2;
        crc2 = crc32(crc2, buf, (uInt)half);
        crc2 = crc32(crc2, buf + half, (uInt)(size - half));
    }
    
    // Test with NULL buffer
    crc32(0L, Z_NULL, 0);
}

static void fuzz_crc32_z(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLong crc = 0L;
    const Bytef *buf = (const Bytef *)data;
    
    // Test crc32_z with full buffer
    crc = crc32_z(crc, buf, (z_size_t)size);
    
    // Test incremental updates
    if (size > 1) {
        uLong crc2 = 0L;
        size_t half = size / 2;
        crc2 = crc32_z(crc2, buf, (z_size_t)half);
        crc2 = crc32_z(crc2, buf + half, (z_size_t)(size - half));
    }
    
    // Test with NULL buffer
    crc32_z(0L, Z_NULL, 0);
}

static void fuzz_adler32(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLong adler = 1L;
    const Bytef *buf = (const Bytef *)data;
    
    // Test adler32 with full buffer
    adler = adler32(adler, buf, (uInt)size);
    
    // Test incremental updates
    if (size > 1) {
        uLong adler2 = 1L;
        size_t half = size / 2;
        adler2 = adler32(adler2, buf, (uInt)half);
        adler2 = adler32(adler2, buf + half, (uInt)(size - half));
    }
    
    // Test with NULL buffer
    adler32(0L, Z_NULL, 0);
}

static void fuzz_adler32_z(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLong adler = 1L;
    const Bytef *buf = (const Bytef *)data;
    
    // Test adler32_z with full buffer
    adler = adler32_z(adler, buf, (z_size_t)size);
    
    // Test incremental updates
    if (size > 1) {
        uLong adler2 = 1L;
        size_t half = size / 2;
        adler2 = adler32_z(adler2, buf, (z_size_t)half);
        adler2 = adler32_z(adler2, buf + half, (z_size_t)(size - half));
    }
    
    // Test with NULL buffer
    adler32_z(0L, Z_NULL, 0);
}

static void fuzz_crc32_combine(const uint8_t *data, size_t size) {
    if (size < 8) return;
    
    // Extract crc1, crc2, and len2 from data
    uLong crc1 = 0, crc2 = 0;
    z_off_t len2 = 0;
    
    // Use first 8 bytes for parameters
    memcpy(&crc1, data, sizeof(uLong) < 4 ? sizeof(uLong) : 4);
    memcpy(&crc2, data + 4, sizeof(uLong) < 4 ? sizeof(uLong) : 4);
    
    // Use remaining data to determine len2
    if (size > 8) {
        len2 = (z_off_t)(data[8] % 256);
        if (size > 9) {
            len2 |= (z_off_t)(data[9] % 256) << 8;
        }
        // Ensure non-negative
        if (len2 < 0) len2 = -len2;
    }
    
    crc32_combine(crc1, crc2, len2);
}

static void fuzz_crc32_combine64(const uint8_t *data, size_t size) {
    if (size < 8) return;
    
    // Extract crc1, crc2, and len2 from data
    uLong crc1 = 0, crc2 = 0;
    z_off64_t len2 = 0;
    
    // Use first 8 bytes for parameters
    memcpy(&crc1, data, sizeof(uLong) < 4 ? sizeof(uLong) : 4);
    memcpy(&crc2, data + 4, sizeof(uLong) < 4 ? sizeof(uLong) : 4);
    
    // Use remaining data to determine len2
    if (size > 8) {
        len2 = (z_off64_t)(data[8] % 256);
        if (size > 9) {
            len2 |= (z_off64_t)(data[9] % 256) << 8;
        }
        if (size > 10) {
            len2 |= (z_off64_t)(data[10] % 256) << 16;
        }
        if (size > 11) {
            len2 |= (z_off64_t)(data[11] % 256) << 24;
        }
        // Ensure non-negative
        if (len2 < 0) len2 = -len2;
    }
    
    crc32_combine64(crc1, crc2, len2);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz all target functions
    fuzz_crc32(Data, Size);
    fuzz_crc32_z(Data, Size);
    fuzz_adler32(Data, Size);
    fuzz_adler32_z(Data, Size);
    fuzz_crc32_combine(Data, Size);
    fuzz_crc32_combine64(Data, Size);
    
    return 0;
}