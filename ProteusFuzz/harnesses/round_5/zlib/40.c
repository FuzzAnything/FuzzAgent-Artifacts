// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

static void fuzz_adler32_z(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Test with NULL buffer (should return 1)
    uLong res1 = adler32_z(0, NULL, 0);
    
    // Test with data
    uLong res2 = adler32_z(0, Data, Size);
    
    // Test with different initial values
    uLong res3 = adler32_z(1, Data, Size);
    uLong res4 = adler32_z(0xFFFFFFFF, Data, Size);
    
    // Test with zero length
    uLong res5 = adler32_z(res2, Data, 0);
    
    (void)res1; (void)res2; (void)res3; (void)res4; (void)res5;
}

static void fuzz_crc32(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Test with NULL buffer
    uLong res1 = crc32(0, NULL, 0);
    
    // Test with data
    uLong res2 = crc32(0, Data, Size);
    
    // Test with different initial values
    uLong res3 = crc32(1, Data, Size);
    uLong res4 = crc32(0xFFFFFFFF, Data, Size);
    
    // Test with zero length
    uLong res5 = crc32(res2, Data, 0);
    
    // Incremental processing simulation
    uLong res6 = 0;
    for (size_t i = 0; i < Size; i++) {
        res6 = crc32(res6, Data + i, 1);
    }
    
    (void)res1; (void)res2; (void)res3; (void)res4; (void)res5; (void)res6;
}

static void fuzz_adler32(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Test with NULL buffer
    uLong res1 = adler32(0, NULL, 0);
    
    // Test with data
    uLong res2 = adler32(0, Data, Size);
    
    // Test with different initial values
    uLong res3 = adler32(1, Data, Size);
    uLong res4 = adler32(0xFFFFFFFF, Data, Size);
    
    // Test with zero length
    uLong res5 = adler32(res2, Data, 0);
    
    // Incremental processing simulation
    uLong res6 = 0;
    for (size_t i = 0; i < Size; i++) {
        res6 = adler32(res6, Data + i, 1);
    }
    
    (void)res1; (void)res2; (void)res3; (void)res4; (void)res5; (void)res6;
}

static void fuzz_crc32_combine64(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    // Extract two CRC values from input safely using memcpy to avoid alignment issues
    uLong crc1 = 0;
    uLong crc2 = 0;
    
    if (Size >= sizeof(uLong)) {
        memcpy(&crc1, Data, sizeof(uLong));
    }
    if (Size >= 2 * sizeof(uLong)) {
        memcpy(&crc2, Data + sizeof(uLong), sizeof(uLong));
    }
    
    // Test with various lengths
    z_off64_t len2 = 0;
    if (Size >= 3 * sizeof(uLong)) {
        memcpy(&len2, Data + 2 * sizeof(uLong), sizeof(z_off64_t));
    }
    
    // Test combine
    uLong res1 = crc32_combine64(crc1, crc2, len2);
    
    // Test with zero length
    uLong res2 = crc32_combine64(crc1, crc2, 0);
    
    // Test with max length
    uLong res3 = crc32_combine64(crc1, crc2, (z_off64_t)-1);
    
    (void)res1; (void)res2; (void)res3;
}

static void fuzz_adler32_combine(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    // Extract two Adler values from input safely using memcpy
    uLong adler1 = 0;
    uLong adler2 = 0;
    
    if (Size >= sizeof(uLong)) {
        memcpy(&adler1, Data, sizeof(uLong));
    }
    if (Size >= 2 * sizeof(uLong)) {
        memcpy(&adler2, Data + sizeof(uLong), sizeof(uLong));
    }
    
    // Test with various lengths
    z_off_t len2 = 0;
    if (Size >= 3 * sizeof(uLong)) {
        memcpy(&len2, Data + 2 * sizeof(uLong), sizeof(z_off_t));
    }
    
    // Test combine
    uLong res1 = adler32_combine(adler1, adler2, len2);
    
    // Test with zero length
    uLong res2 = adler32_combine(adler1, adler2, 0);
    
    // Test with max length
    uLong res3 = adler32_combine(adler1, adler2, (z_off_t)-1);
    
    (void)res1; (void)res2; (void)res3;
}

static void fuzz_adler32_combine64(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    // Extract two Adler values from input safely using memcpy
    uLong adler1 = 0;
    uLong adler2 = 0;
    
    if (Size >= sizeof(uLong)) {
        memcpy(&adler1, Data, sizeof(uLong));
    }
    if (Size >= 2 * sizeof(uLong)) {
        memcpy(&adler2, Data + sizeof(uLong), sizeof(uLong));
    }
    
    // Test with various lengths
    z_off64_t len2 = 0;
    if (Size >= 3 * sizeof(uLong)) {
        memcpy(&len2, Data + 2 * sizeof(uLong), sizeof(z_off64_t));
    }
    
    // Test combine
    uLong res1 = adler32_combine64(adler1, adler2, len2);
    
    // Test with zero length
    uLong res2 = adler32_combine64(adler1, adler2, 0);
    
    // Test with max length
    uLong res3 = adler32_combine64(adler1, adler2, (z_off64_t)-1);
    
    (void)res1; (void)res2; (void)res3;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_adler32_z(Data, Size);
    fuzz_crc32(Data, Size);
    fuzz_adler32(Data, Size);
    fuzz_crc32_combine64(Data, Size);
    fuzz_adler32_combine(Data, Size);
    fuzz_adler32_combine64(Data, Size);
    
    return 0;
}