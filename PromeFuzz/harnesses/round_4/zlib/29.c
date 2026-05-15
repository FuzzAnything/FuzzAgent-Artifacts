// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare buffer for checksum functions
    const Bytef *buf = (const Bytef *)Data;
    
    // Split input into two parts for combine operations
    size_t split = Size / 2;
    const Bytef *buf1 = buf;
    const Bytef *buf2 = buf + split;
    size_t len1 = split;
    size_t len2 = Size - split;
    
    // Test adler32_z with various lengths
    uLong adler_z = 1L;
    adler_z = adler32_z(adler_z, buf, Size);
    adler_z = adler32_z(adler_z, NULL, 0);  // Test NULL buffer
    if (len1 > 0) {
        adler_z = adler32_z(adler_z, buf1, len1);
    }
    if (len2 > 0) {
        adler_z = adler32_z(adler_z, buf2, len2);
    }
    
    // Test adler32 with uInt lengths
    uLong adler = 0L;
    adler = adler32(adler, Z_NULL, 0);
    uInt len1_u = (uInt)(len1 % 0xFFFF);  // Keep within uInt range
    uInt len2_u = (uInt)(len2 % 0xFFFF);
    adler = adler32(adler, buf1, len1_u);
    adler = adler32(adler, buf2, len2_u);
    
    // Test crc32_z
    uLong crc_z = 0L;
    crc_z = crc32_z(crc_z, buf, Size);
    if (len1 > 0) {
        crc_z = crc32_z(crc_z, buf1, len1);
    }
    
    // Test crc32
    uLong crc = 0L;
    crc = crc32(crc, Z_NULL, 0);
    crc = crc32(crc, buf1, len1_u);
    crc = crc32(crc, buf2, len2_u);
    
    // Test crc32_combine operations
    uLong crc1 = crc32(0L, buf1, len1_u);
    uLong crc2 = crc32(0L, buf2, len2_u);
    
    // Generate operator for combine_op
    uLong op = 0;
    if (len2 > 0) {
        // crc32_combine_gen is needed but not in target list
        // We'll use a dummy value for fuzzing
        op = crc2 ^ 0xFFFFFFFF;
    }
    
    // Test crc32_combine_op
    uLong combined_op = crc32_combine_op(crc1, crc2, op);
    
    // Test crc32_combine64
    z_off64_t len2_off = (z_off64_t)len2;
    uLong combined64 = crc32_combine64(crc1, crc2, len2_off);
    
    // Test with file operations to explore different states
    write_dummy_file(Data, Size);
    
    // Read back and compute checksums
    FILE *f = fopen("./dummy_file", "rb");
    if (f) {
        uint8_t *file_buf = malloc(Size);
        if (file_buf) {
            size_t read = fread(file_buf, 1, Size, f);
            if (read > 0) {
                // Compute checksums on file data
                uLong file_adler = adler32(0L, Z_NULL, 0);
                file_adler = adler32(file_adler, (const Bytef*)file_buf, (uInt)read);
                
                uLong file_crc = crc32(0L, Z_NULL, 0);
                file_crc = crc32(file_crc, (const Bytef*)file_buf, (uInt)read);
            }
            free(file_buf);
        }
        fclose(f);
    }
    
    remove_dummy_file();
    
    // Use computed values to avoid optimization
    volatile uLong dummy = adler_z + adler + crc_z + crc + combined_op + combined64;
    (void)dummy;
    
    return 0;
}