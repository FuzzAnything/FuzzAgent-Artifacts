// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
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

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Create and initialize z_stream structure
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Set up input data for inflation
    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)(Size > UINT_MAX ? UINT_MAX : Size);
    
    // Allocate output buffer
    #define OUT_BUFFER_SIZE 4096
    Bytef out_buffer[OUT_BUFFER_SIZE];
    strm.next_out = out_buffer;
    strm.avail_out = OUT_BUFFER_SIZE;
    
    // Initialize inflation with windowBits = 15 (32K window size)
    int windowBits = 15;
    const char *version = ZLIB_VERSION;
    int stream_size = sizeof(z_stream);
    
    int ret = inflateInit2_(&strm, windowBits, version, stream_size);
    if (ret != Z_OK) {
        // If initialization fails, clean up and return
        inflateEnd(&strm);
        return 0;
    }
    
    // Perform inflation with Z_NO_FLUSH (process as much as possible)
    ret = inflate(&strm, Z_NO_FLUSH);
    // We ignore the return value for fuzzing purposes
    
    // Prepare data for crc32_combine
    uLong crc1 = 0;
    uLong crc2 = 0;
    z_off_t len2 = 0;
    
    // Use fuzzer data to generate CRC values and length
    if (Size >= 8) {
        // Use first 4 bytes for crc1, next 4 for crc2
        memcpy(&crc1, Data, sizeof(crc1) > 4 ? 4 : sizeof(crc1));
        memcpy(&crc2, Data + 4, sizeof(crc2) > 4 ? 4 : sizeof(crc2));
        
        // Use next 8 bytes for len2 if available, otherwise use Size
        if (Size >= 16) {
            memcpy(&len2, Data + 8, sizeof(len2) > 8 ? 8 : sizeof(len2));
        } else {
            len2 = (z_off_t)Size;
        }
        
        // Ensure len2 is non-negative
        if (len2 < 0) {
            len2 = -len2;
        }
    } else {
        // Use Size as len2 if we don't have enough data
        crc1 = 0;
        crc2 = 0;
        len2 = (z_off_t)Size;
    }
    
    // Call crc32_combine
    uLong combined_crc = crc32_combine(crc1, crc2, len2);
    (void)combined_crc; // Suppress unused variable warning
    
    // Reset the inflation stream
    ret = inflateReset(&strm);
    // We ignore the return value for fuzzing purposes
    
    // End inflation and clean up
    ret = inflateEnd(&strm);
    // We ignore the return value for fuzzing purposes
    
    return 0;
}