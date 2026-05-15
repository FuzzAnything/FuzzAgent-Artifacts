// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
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
#include "code/zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_to_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for any potential file operations
    write_to_dummy_file(Data, Size);
    
    // Test adler32
    uLong adler = adler32(0L, Z_NULL, 0);
    if (Size > 0) {
        // Test with full buffer
        adler = adler32(adler, Data, (uInt)Size);
        
        // Test with chunks
        size_t chunk_size = Size / 4;
        if (chunk_size > 0) {
            adler = adler32(0L, Z_NULL, 0);
            for (size_t i = 0; i < Size; i += chunk_size) {
                size_t remaining = Size - i;
                size_t current_chunk = remaining < chunk_size ? remaining : chunk_size;
                adler = adler32(adler, Data + i, (uInt)current_chunk);
            }
        }
    }
    
    // Test crc32_z
    uLong crc_z = crc32_z(0L, Z_NULL, 0);
    if (Size > 0) {
        crc_z = crc32_z(crc_z, Data, (z_size_t)Size);
        
        // Test with chunks
        size_t chunk_size = Size / 3;
        if (chunk_size > 0) {
            crc_z = crc32_z(0L, Z_NULL, 0);
            for (size_t i = 0; i < Size; i += chunk_size) {
                size_t remaining = Size - i;
                size_t current_chunk = remaining < chunk_size ? remaining : chunk_size;
                crc_z = crc32_z(crc_z, Data + i, (z_size_t)current_chunk);
            }
        }
    }
    
    // Test adler32_z
    uLong adler_z = adler32_z(1L, Z_NULL, 0);
    if (Size > 0) {
        adler_z = adler32_z(adler_z, Data, (z_size_t)Size);
        
        // Test with different initial values
        adler_z = adler32_z(0L, Data, (z_size_t)Size);
        adler_z = adler32_z(adler, Data, (z_size_t)Size);
    }
    
    // Test crc32_combine64
    if (Size >= 2) {
        // Split data into two parts
        size_t split = Size / 2;
        uLong crc1 = crc32(0L, Data, (uInt)split);
        uLong crc2 = crc32(0L, Data + split, (uInt)(Size - split));
        
        // Test with positive length
        uLong combined64 = crc32_combine64(crc1, crc2, (z_off64_t)(Size - split));
        
        // Test with zero length
        combined64 = crc32_combine64(crc1, crc2, (z_off64_t)0);
    }
    
    // Test crc32
    uLong crc = crc32(0L, Z_NULL, 0);
    if (Size > 0) {
        crc = crc32(crc, Data, (uInt)Size);
        
        // Test incremental updates
        crc = crc32(0L, Z_NULL, 0);
        size_t chunk_size = Size / 5;
        if (chunk_size > 0) {
            for (size_t i = 0; i < Size; i += chunk_size) {
                size_t remaining = Size - i;
                size_t current_chunk = remaining < chunk_size ? remaining : chunk_size;
                crc = crc32(crc, Data + i, (uInt)current_chunk);
            }
        }
        
        // Test with NULL buffer
        crc = crc32(crc, Z_NULL, 0);
    }
    
    // Test crc32_combine
    if (Size >= 2) {
        // Split data into two parts
        size_t split = Size / 2;
        uLong crc1 = crc32(0L, Data, (uInt)split);
        uLong crc2 = crc32(0L, Data + split, (uInt)(Size - split));
        
        // Test with positive length
        uLong combined = crc32_combine(crc1, crc2, (z_off_t)(Size - split));
        
        // Test with zero length
        combined = crc32_combine(crc1, crc2, (z_off_t)0);
    }
    
    // Test edge cases
    // Zero length with NULL buffer
    adler32(0L, Z_NULL, 0);
    crc32_z(0L, Z_NULL, 0);
    adler32_z(1L, Z_NULL, 0);
    crc32(0L, Z_NULL, 0);
    
    // Test with very small chunks
    if (Size > 0) {
        // Single byte chunks
        uLong test_adler = adler32(0L, Z_NULL, 0);
        uLong test_crc = crc32(0L, Z_NULL, 0);
        for (size_t i = 0; i < Size && i < 10; i++) {
            test_adler = adler32(test_adler, Data + i, 1);
            test_crc = crc32(test_crc, Data + i, 1);
        }
    }
    
    return 0;
}