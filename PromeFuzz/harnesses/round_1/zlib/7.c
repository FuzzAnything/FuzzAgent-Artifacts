// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Initialize z_stream structure
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    
    // Set up input buffer from fuzzer data
    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)(Size > UINT_MAX ? UINT_MAX : Size);
    
    // Allocate output buffer
    size_t out_size = Size * 2 + 1024;
    Bytef *out_buf = (Bytef *)malloc(out_size);
    if (!out_buf) return 0;
    
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(out_size > UINT_MAX ? UINT_MAX : out_size);
    
    // Initialize decompression with raw mode (negative windowBits)
    int windowBits = -15;
    int ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }
    
    // Perform inflation with Z_NO_FLUSH
    ret = inflate(&strm, Z_NO_FLUSH);
    // We don't care about the return value for fuzzing purposes
    
    // Prepare CRC values for crc32_combine
    uLong crc1 = 0;
    uLong crc2 = 0;
    z_off_t len2 = (z_off_t)Size;
    
    // Call crc32_combine with non-negative len2
    if (len2 >= 0) {
        uLong combined = crc32_combine(crc1, crc2, len2);
        (void)combined; // Prevent unused variable warning
    }
    
    // Reset the inflate stream
    ret = inflateReset(&strm);
    // We don't care about the return value for fuzzing purposes
    
    // End the inflate stream
    ret = inflateEnd(&strm);
    // We don't care about the return value for fuzzing purposes
    
    // Clean up
    free(out_buf);
    
    return 0;
}