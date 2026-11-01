// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateBound at deflate.c:929:15 in zlib.h
// deflateBound_z at deflate.c:856:18 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// deflateBound at deflate.c:929:15 in zlib.h
// deflateBound_z at deflate.c:856:18 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void init_stream(z_stream *strm) {
    if (strm) {
        memset(strm, 0, sizeof(z_stream));
        strm->zalloc = Z_NULL;
        strm->zfree = Z_NULL;
        strm->opaque = Z_NULL;
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test 1: compressBound and compress
    uLong sourceLen = (Size > 1024) ? 1024 : Size;
    uLong bound = compressBound((uLong)sourceLen);
    if (bound != (uLong)-1) {
        Bytef *dest = (Bytef *)malloc(bound);
        if (dest) {
            uLongf destLen = bound;
            int ret = compress(dest, &destLen, (const Bytef *)Data, (uLong)sourceLen);
            (void)ret; // Ignore return for fuzzing, just check for crashes
            free(dest);
        }
    }

    // Test 2: compressBound_z and compress_z
    z_size_t sourceLen_z = (Size > 1024) ? 1024 : Size;
    z_size_t bound_z = compressBound_z(sourceLen_z);
    if (bound_z != (z_size_t)-1) {
        Bytef *dest_z = (Bytef *)malloc(bound_z);
        if (dest_z) {
            z_size_t destLen_z = bound_z;
            int ret_z = compress_z(dest_z, &destLen_z, (const Bytef *)Data, sourceLen_z);
            (void)ret_z;
            free(dest_z);
        }
    }

    // Test 3: deflateBound and deflateBound_z
    z_stream strm;
    init_stream(&strm);
    
    // Initialize stream with deflate to get a valid state for bounds
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        uLong bound_deflate = deflateBound(&strm, (uLong)sourceLen);
        (void)bound_deflate; // Just call to test logic
        
        z_size_t bound_deflate_z = deflateBound_z(&strm, sourceLen_z);
        (void)bound_deflate_z; // Just call to test logic
        
        deflateEnd(&strm);
    }

    // Test 4: Edge cases with small/large inputs
    if (Size >= 4) {
        uLong smallLen = Data[0] % 100;
        uLong bound_small = compressBound(smallLen);
        if (bound_small != (uLong)-1) {
            Bytef *dest_small = (Bytef *)malloc(bound_small);
            if (dest_small) {
                uLongf destLen_small = bound_small;
                compress(dest_small, &destLen_small, (const Bytef *)Data, smallLen);
                free(dest_small);
            }
        }
    }

    // Test 5: Invalid stream for deflateBound (should return conservative estimate)
    z_stream invalid_strm;
    memset(&invalid_strm, 0, sizeof(z_stream));
    // Do not initialize, just pass to see if it handles gracefully
    uLong bound_invalid = deflateBound(&invalid_strm, (uLong)sourceLen);
    (void)bound_invalid;
    
    z_size_t bound_invalid_z = deflateBound_z(&invalid_strm, sourceLen_z);
    (void)bound_invalid_z;

    return 0;
}