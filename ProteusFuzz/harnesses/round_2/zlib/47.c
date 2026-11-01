// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void test_adler32_z(const uint8_t *data, size_t size) {
    uLong adler = 0;
    // Test with NULL buffer
    adler = adler32_z(adler, Z_NULL, 0);
    
    // Test with valid data
    if (size > 0) {
        adler = adler32_z(adler, (const Bytef *)data, (z_size_t)size);
    }
    
    // Test with partial data
    if (size > 10) {
        uLong adler2 = adler32_z(0, (const Bytef *)data, (z_size_t)10);
        uLong adler3 = adler32_z(adler2, (const Bytef *)(data + 10), (z_size_t)(size - 10));
        (void)adler3;
    }
}

static void test_crc32_z(const uint8_t *data, size_t size) {
    uLong crc = 0;
    // Test with NULL buffer
    crc = crc32_z(crc, Z_NULL, 0);
    
    // Test with valid data
    if (size > 0) {
        crc = crc32_z(crc, (const Bytef *)data, (z_size_t)size);
    }
    
    // Test with partial data
    if (size > 10) {
        uLong crc2 = crc32_z(0, (const Bytef *)data, (z_size_t)10);
        uLong crc3 = crc32_z(crc2, (const Bytef *)(data + 10), (z_size_t)(size - 10));
        (void)crc3;
    }
}

static void test_adler32_combine(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t split = size / 2;
    uLong adler1 = adler32_z(0, (const Bytef *)data, (z_size_t)split);
    uLong adler2 = adler32_z(0, (const Bytef *)(data + split), (z_size_t)(size - split));
    
    // Test adler32_combine
    uLong combined = adler32_combine(adler1, adler2, (z_off_t)(size - split));
    (void)combined;
    
    // Test with zero length second part
    if (split > 0) {
        uLong adler1_part = adler32_z(0, (const Bytef *)data, (z_size_t)split);
        uLong combined_zero = adler32_combine(adler1_part, 0, 0);
        (void)combined_zero;
    }
}

static void test_adler32_combine64(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t split = size / 2;
    uLong adler1 = adler32_z(0, (const Bytef *)data, (z_size_t)split);
    uLong adler2 = adler32_z(0, (const Bytef *)(data + split), (z_size_t)(size - split));
    
    // Test adler32_combine64
    uLong combined = adler32_combine64(adler1, adler2, (z_off64_t)(size - split));
    (void)combined;
    
    // Test with zero length second part
    if (split > 0) {
        uLong adler1_part = adler32_z(0, (const Bytef *)data, (z_size_t)split);
        uLong combined_zero = adler32_combine64(adler1_part, 0, 0);
        (void)combined_zero;
    }
}

static void test_crc32_combine(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t split = size / 2;
    uLong crc1 = crc32_z(0, (const Bytef *)data, (z_size_t)split);
    uLong crc2 = crc32_z(0, (const Bytef *)(data + split), (z_size_t)(size - split));
    
    // Test crc32_combine
    uLong combined = crc32_combine(crc1, crc2, (z_off_t)(size - split));
    (void)combined;
    
    // Test with zero length second part
    if (split > 0) {
        uLong crc1_part = crc32_z(0, (const Bytef *)data, (z_size_t)split);
        uLong combined_zero = crc32_combine(crc1_part, 0, 0);
        (void)combined_zero;
    }
}

static void test_crc32_combine64(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t split = size / 2;
    uLong crc1 = crc32_z(0, (const Bytef *)data, (z_size_t)split);
    uLong crc2 = crc32_z(0, (const Bytef *)(data + split), (z_size_t)(size - split));
    
    // Test crc32_combine64
    uLong combined = crc32_combine64(crc1, crc2, (z_off64_t)(size - split));
    (void)combined;
    
    // Test with zero length second part
    if (split > 0) {
        uLong crc1_part = crc32_z(0, (const Bytef *)data, (z_size_t)split);
        uLong combined_zero = crc32_combine64(crc1_part, 0, 0);
        (void)combined_zero;
    }
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        // Test edge cases with empty input
        test_adler32_z(NULL, 0);
        test_crc32_z(NULL, 0);
        return 0;
    }
    
    // Run all test functions
    test_adler32_z(Data, Size);
    test_crc32_z(Data, Size);
    test_adler32_combine(Data, Size);
    test_adler32_combine64(Data, Size);
    test_crc32_combine(Data, Size);
    test_crc32_combine64(Data, Size);
    
    return 0;
}