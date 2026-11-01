// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void fuzz_adler32_z(const uint8_t *data, size_t size) {
    uLong adler = 0;
    z_size_t len = (z_size_t)size;
    
    // Test with NULL buffer (should return 1)
    adler = adler32_z(0, NULL, 0);
    
    // Test with valid buffer
    if (size > 0) {
        adler = adler32_z(0, data, len);
        
        // Test with partial data
        if (size > 4) {
            adler = adler32_z(adler, data + 4, len - 4);
        }
        
        // Test with single byte
        adler = adler32_z(0, data, 1);
    }
}

static void fuzz_crc32_z(const uint8_t *data, size_t size) {
    uLong crc = 0;
    z_size_t len = (z_size_t)size;
    
    // Test with NULL buffer
    crc = crc32_z(0, NULL, 0);
    
    // Test with valid buffer
    if (size > 0) {
        crc = crc32_z(0, data, len);
        
        // Test with partial data
        if (size > 4) {
            crc = crc32_z(crc, data + 4, len - 4);
        }
        
        // Test with single byte
        crc = crc32_z(0, data, 1);
    }
}

static void fuzz_crc32(const uint8_t *data, size_t size) {
    uLong crc = 0;
    uInt len = (uInt)(size < UINT_MAX ? size : UINT_MAX);
    
    // Test with NULL buffer
    crc = crc32(0, NULL, 0);
    
    // Test with valid buffer
    if (size > 0) {
        crc = crc32(0, data, len);
        
        // Test with partial data
        if (len > 4) {
            crc = crc32(crc, data + 4, len - 4);
        }
        
        // Test with single byte
        crc = crc32(0, data, 1);
    }
}

static void fuzz_combine_functions(const uint8_t *data, size_t size) {
    uLong adler1, adler2, combined_adler;
    uLong crc1, crc2, combined_crc;
    z_off_t len2_32;
    z_off64_t len2_64;
    
    // Initialize with some values
    adler1 = adler32_z(0, NULL, 0);
    adler2 = adler32_z(0, NULL, 0);
    crc1 = crc32_z(0, NULL, 0);
    crc2 = crc32_z(0, NULL, 0);
    
    if (size > 0) {
        // Calculate checksums from data
        adler1 = adler32_z(0, data, size / 2);
        adler2 = adler32_z(0, data + size / 2, size - size / 2);
        crc1 = crc32_z(0, data, size / 2);
        crc2 = crc32_z(0, data + size / 2, size - size / 2);
        
        len2_32 = (z_off_t)(size - size / 2);
        len2_64 = (z_off64_t)(size - size / 2);
        
        // Test adler32_combine
        combined_adler = adler32_combine(adler1, adler2, len2_32);
        
        // Test crc32_combine
        combined_crc = crc32_combine(crc1, crc2, len2_32);
        
        // Test crc32_combine64
        combined_crc = crc32_combine64(crc1, crc2, len2_64);
        
        // Test with zero length
        combined_adler = adler32_combine(adler1, adler2, 0);
        combined_crc = crc32_combine(crc1, crc2, 0);
        combined_crc = crc32_combine64(crc1, crc2, 0);
    }
    
    // Test with extreme values
    len2_32 = -1;
    len2_64 = -1;
    combined_adler = adler32_combine(adler1, adler2, len2_32);
    combined_crc = crc32_combine(crc1, crc2, len2_32);
    combined_crc = crc32_combine64(crc1, crc2, len2_64);
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Fuzz all target functions
    fuzz_adler32_z(Data, Size);
    fuzz_crc32_z(Data, Size);
    fuzz_crc32(Data, Size);
    fuzz_combine_functions(Data, Size);
    
    return 0;
}