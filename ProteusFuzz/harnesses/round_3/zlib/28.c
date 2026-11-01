// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>
#include <limits.h>

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Allocate buffers for source and destination
    const Bytef *source = (const Bytef *)Data;
    uLong sourceLen = (Size < ULONG_MAX) ? (uLong)Size : ULONG_MAX;
    z_size_t sourceLen_z = (Size < SIZE_MAX) ? (z_size_t)Size : SIZE_MAX;

    // Calculate required destination size using compressBound
    uLong destLen_uLong = compressBound(sourceLen);
    if (destLen_uLong == (uLong)-1) {
        // Error in calculation, skip this input
        return 0;
    }

    // Calculate required destination size using compressBound_z
    z_size_t destLen_z = compressBound_z(sourceLen_z);
    if (destLen_z == (z_size_t)-1) {
        // Error in calculation, skip this input
        return 0;
    }

    // Allocate destination buffer based on the larger of the two bounds
    size_t allocSize = (destLen_uLong > destLen_z) ? destLen_uLong : destLen_z;
    Bytef *dest = (Bytef *)malloc(allocSize);
    if (!dest) {
        return 0;
    }

    // Test compress
    uLongf destLen_uLong_ptr = allocSize;
    int ret_compress = compress(dest, &destLen_uLong_ptr, source, sourceLen);
    (void)ret_compress; // Suppress unused variable warning

    // Test compress2 with varying levels
    int level = (Data[0] % 10); // Use first byte to determine compression level
    destLen_uLong_ptr = allocSize;
    int ret_compress2 = compress2(dest, &destLen_uLong_ptr, source, sourceLen, level);
    (void)ret_compress2;

    // Test compress_z
    z_size_t destLen_z_ptr = allocSize;
    int ret_compress_z = compress_z(dest, &destLen_z_ptr, source, sourceLen_z);
    (void)ret_compress_z;

    // Test compress2_z with varying levels
    destLen_z_ptr = allocSize;
    int ret_compress2_z = compress2_z(dest, &destLen_z_ptr, source, sourceLen_z, level);
    (void)ret_compress2_z;

    // Cleanup
    free(dest);

    return 0;
}