// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// deflateBound at deflate.c:929:15 in zlib.h
// deflateBound_z at deflate.c:856:18 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void initialize_z_stream(z_stream *strm, int level) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    deflateInit(strm, level);
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    int level = Data[0] % 11 - 1; // -1 to 9
    size_t source_len = Size - 1;
    const Bytef *source = Data + 1;

    // Test compressBound
    uLong bound = compressBound((uLong)source_len);
    if (bound != (uLong)-1) {
        Bytef *dest = (Bytef *)malloc(bound);
        if (dest) {
            uLongf dest_len = bound;
            compress(dest, &dest_len, source, (uLong)source_len);
            free(dest);
        }
    }

    // Test compressBound_z
    z_size_t bound_z = compressBound_z((z_size_t)source_len);
    if (bound_z != (z_size_t)-1) {
        Bytef *dest_z = (Bytef *)malloc(bound_z);
        if (dest_z) {
            z_size_t dest_len_z = bound_z;
            compress2_z(dest_z, &dest_len_z, source, (z_size_t)source_len, level);
            free(dest_z);
        }
    }

    // Test deflateBound and deflateBound_z
    z_stream strm;
    initialize_z_stream(&strm, level);
    if (strm.state != NULL) {
        uLong deflate_bound = deflateBound(&strm, (uLong)source_len);
        z_size_t deflate_bound_z = deflateBound_z(&strm, (z_size_t)source_len);
        (void)deflate_bound;
        (void)deflate_bound_z;
        deflateEnd(&strm);
    }

    return 0;
}