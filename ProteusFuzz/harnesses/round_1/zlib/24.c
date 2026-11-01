// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void fill_buffer_with_pattern(uint8_t *buffer, size_t size, uint8_t pattern) {
    if (buffer && size > 0) {
        memset(buffer, pattern, size);
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Allocate buffers for compression
    // Use a safe upper bound for destination buffer size
    // compressBound can return (uLong)-1 on error, check for that
    uLong sourceLen = (uLong)Size;
    uLong bound = compressBound(sourceLen);
    
    if (bound == (uLong)-1) {
        // Error in calculating bound, skip this input or handle gracefully
        return 0;
    }

    uint8_t *dest = (uint8_t *)malloc(bound);
    if (!dest) {
        return 0;
    }

    // Test compress2
    {
        uLongf destLen = bound;
        int level = (Data[0] % 10); // Use first byte to determine compression level (0-9)
        int ret = compress2(dest, &destLen, (const Bytef *)Data, sourceLen, level);
        // Check return value, but don't crash on error
        (void)ret;
    }

    // Test compress
    {
        uLongf destLen = bound;
        int ret = compress(dest, &destLen, (const Bytef *)Data, sourceLen);
        (void)ret;
    }

    // Test compress2_z
    {
        z_size_t destLen_z = bound;
        int level = (Data[0] % 10);
        int ret = compress2_z(dest, &destLen_z, (const Bytef *)Data, (z_size_t)sourceLen, level);
        (void)ret;
    }

    // Test compress_z
    {
        z_size_t destLen_z = bound;
        int ret = compress_z(dest, &destLen_z, (const Bytef *)Data, (z_size_t)sourceLen);
        (void)ret;
    }

    // Test compressBound_z
    {
        z_size_t bound_z = compressBound_z((z_size_t)sourceLen);
        if (bound_z != (z_size_t)-1) {
            // Just use the value, no action needed
            (void)bound_z;
        }
    }

    // Cleanup
    free(dest);
    return 0;
}