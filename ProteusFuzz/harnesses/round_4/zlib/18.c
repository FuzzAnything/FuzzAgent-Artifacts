// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Allocate buffers for source and destination */
    Bytef *source = (Bytef *)malloc(Size);
    if (!source) return 0;
    memcpy(source, Data, Size);

    /* Calculate maximum possible compressed size using compressBound */
    uLong sourceLen = (uLong)Size;
    uLong maxBound = compressBound(sourceLen);
    if (maxBound == (uLong)-1) {
        free(source);
        return 0;
    }

    Bytef *dest = (Bytef *)malloc(maxBound);
    if (!dest) {
        free(source);
        return 0;
    }

    /* Test compress */
    uLongf destLen = maxBound;
    int ret = compress(dest, &destLen, source, sourceLen);
    (void)ret; /* Suppress unused warning */

    /* Test compress2 with varying levels */
    for (int level = 0; level <= 9; level++) {
        destLen = maxBound;
        ret = compress2(dest, &destLen, source, sourceLen, level);
        (void)ret;
    }

    /* Test compressBound_z */
    z_size_t sourceLen_z = (z_size_t)Size;
    z_size_t maxBound_z = compressBound_z(sourceLen_z);
    if (maxBound_z != (z_size_t)-1) {
        Bytef *dest_z = (Bytef *)malloc(maxBound_z);
        if (dest_z) {
            z_size_t destLen_z = maxBound_z;
            /* Test compress_z */
            ret = compress_z(dest_z, &destLen_z, source, sourceLen_z);
            (void)ret;

            /* Test compress2_z with varying levels */
            for (int level = 0; level <= 9; level++) {
                destLen_z = maxBound_z;
                ret = compress2_z(dest_z, &destLen_z, source, sourceLen_z, level);
                (void)ret;
            }
            free(dest_z);
        }
    }

    /* Cleanup */
    free(dest);
    free(source);

    return 0;
}