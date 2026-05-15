// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
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
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Prepare buffer for crc32 and adler32 functions
    const Bytef *buf = (const Bytef *)Data;
    
    // Test crc32_z with various lengths
    uLong crc = crc32_z(0L, buf, (z_size_t)Size);
    if (Size > 1) {
        // Test with partial buffer
        crc = crc32_z(crc, buf + 1, (z_size_t)(Size - 1));
    }
    
    // Test crc32 (traditional version)
    crc = crc32(0L, buf, (uInt)Size);
    if (Size > 2) {
        // Test incremental update
        uInt half = (uInt)(Size / 2);
        crc = crc32(crc, buf, half);
        crc = crc32(crc, buf + half, (uInt)(Size - half));
    }
    
    // Test adler32_z
    uLong adler = adler32_z(1L, buf, (z_size_t)Size);
    if (Size > 1) {
        adler = adler32_z(adler, buf + 1, (z_size_t)(Size - 1));
    }
    
    // Generate CRC values for combine functions
    uLong crc1 = crc32(0L, buf, (uInt)(Size > 0 ? 1 : 0));
    uLong crc2 = 0;
    if (Size > 1) {
        crc2 = crc32(0L, buf + 1, (uInt)(Size - 1));
    }
    
    // Test crc32_combine64
    z_off64_t len2_64 = (z_off64_t)(Size > 1 ? Size - 1 : 0);
    uLong combined = crc32_combine64(crc1, crc2, len2_64);
    
    // Test crc32_combine
    z_off_t len2 = (z_off_t)(Size > 1 ? Size - 1 : 0);
    combined = crc32_combine(crc1, crc2, len2);
    
    // Test crc32_combine_op
    // Generate an operator by combining two CRCs with length 1
    uLong op = 0;
    if (Size > 2) {
        // Use a simple method to generate an operator-like value
        uLong temp1 = crc32(0L, buf, 1);
        uLong temp2 = crc32(0L, buf + 1, 1);
        op = crc32_combine(temp1, temp2, 1);
    }
    combined = crc32_combine_op(crc1, crc2, op);
    
    // Test with zero operator
    combined = crc32_combine_op(crc1, crc2, 0);
    
    // Test edge cases
    // Zero length buffers
    crc32_z(0L, NULL, 0);
    crc32(0L, NULL, 0);
    adler32_z(1L, NULL, 0);
    
    // Test with initial CRC values
    crc32_z(crc, buf, (z_size_t)(Size > 0 ? 1 : 0));
    crc32(adler, buf, (uInt)(Size > 0 ? 1 : 0));
    adler32_z(crc, buf, (z_size_t)(Size > 0 ? 1 : 0));
    
    // Test combine functions with zero length
    crc32_combine64(crc1, crc2, 0);
    crc32_combine(crc1, crc2, 0);
    
    // Test with same CRC values
    crc32_combine64(crc1, crc1, len2_64);
    crc32_combine(crc1, crc1, len2);
    crc32_combine_op(crc1, crc1, op);
    
    // Clean up dummy file if created
    remove("./dummy_file");
    
    return 0;
}