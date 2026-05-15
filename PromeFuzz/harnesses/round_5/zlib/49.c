// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for basic operations
    if (Size < 1) {
        return 0;
    }

    // Extract compression level from first byte (0-9 valid for zlib)
    int level = Data[0] % 10;
    const uint8_t *source = Data + 1;
    size_t sourceLen = Size - 1;
    
    if (sourceLen == 0) {
        return 0;
    }

    // Test compressBound with uLong
    uLong sourceLen_uLong = (uLong)sourceLen;
    uLong bound = compressBound(sourceLen_uLong);
    
    // If compressBound indicates overflow, skip compression tests
    if (bound == (uLong)-1) {
        // Still test uncompress with random data
        uLongf destLen = sourceLen * 2;
        Bytef *dest = (Bytef *)malloc(destLen);
        if (dest) {
            uncompress(dest, &destLen, (const Bytef *)source, (uLong)sourceLen);
            free(dest);
        }
        return 0;
    }

    // Allocate destination buffers for compression
    Bytef *dest_compress = (Bytef *)malloc(bound);
    Bytef *dest_compress2 = (Bytef *)malloc(bound);
    Bytef *dest_compress_z = (Bytef *)malloc(bound);
    Bytef *dest_compress2_z = (Bytef *)malloc(bound);
    
    if (!dest_compress || !dest_compress2 || !dest_compress_z || !dest_compress2_z) {
        free(dest_compress);
        free(dest_compress2);
        free(dest_compress_z);
        free(dest_compress2_z);
        return 0;
    }

    // Test compress
    uLongf destLen_compress = bound;
    int ret_compress = compress(dest_compress, &destLen_compress, 
                                (const Bytef *)source, sourceLen_uLong);

    // Test compress2
    uLongf destLen_compress2 = bound;
    int ret_compress2 = compress2(dest_compress2, &destLen_compress2,
                                  (const Bytef *)source, sourceLen_uLong, level);

    // Test compress_z (using z_size_t)
    z_size_t sourceLen_z = (z_size_t)sourceLen;
    z_size_t destLen_compress_z = (z_size_t)bound;
    int ret_compress_z = compress_z(dest_compress_z, &destLen_compress_z,
                                    (const Bytef *)source, sourceLen_z);

    // Test compress2_z
    z_size_t destLen_compress2_z = (z_size_t)bound;
    int ret_compress2_z = compress2_z(dest_compress2_z, &destLen_compress2_z,
                                      (const Bytef *)source, sourceLen_z, level);

    // Test uncompress with compressed data (if compression succeeded)
    if (ret_compress == Z_OK && destLen_compress > 0) {
        uLongf uncompressedLen = sourceLen_uLong * 2;
        Bytef *uncompressed = (Bytef *)malloc(uncompressedLen);
        if (uncompressed) {
            uncompress(uncompressed, &uncompressedLen, 
                      dest_compress, destLen_compress);
            free(uncompressed);
        }
    }

    // Also test uncompress with original data (should fail or produce garbage)
    uLongf destLen_random = sourceLen_uLong * 2;
    Bytef *dest_random = (Bytef *)malloc(destLen_random);
    if (dest_random) {
        uncompress(dest_random, &destLen_random, 
                  (const Bytef *)source, sourceLen_uLong);
        free(dest_random);
    }

    // Clean up
    free(dest_compress);
    free(dest_compress2);
    free(dest_compress_z);
    free(dest_compress2_z);

    return 0;
}