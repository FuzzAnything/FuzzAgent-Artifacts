// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateBound at deflate.c:929:15 in zlib.h
// deflateBound_z at deflate.c:856:18 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
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
    if (strm == NULL) return;
    memset(strm, 0, sizeof(z_stream));
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Use input data as source for compression */
    const Bytef *source = (const Bytef *)Data;
    uLong sourceLen = (uLong)Size;
    z_size_t sourceLen_z = (z_size_t)Size;

    /* 1. Test compressBound */
    uLong bound = compressBound(sourceLen);
    if (bound == (uLong)-1) {
        /* Error case, try with smaller length if possible */
        if (sourceLen > 0) {
            bound = compressBound(sourceLen / 2);
        }
    }

    /* 2. Test compressBound_z */
    z_size_t bound_z = compressBound_z(sourceLen_z);
    if (bound_z == (z_size_t)-1) {
        if (sourceLen_z > 0) {
            bound_z = compressBound_z(sourceLen_z / 2);
        }
    }

    /* Allocate buffers based on bounds if valid */
    Bytef *dest = NULL;
    uLongf destLen = 0;
    Bytef *dest_z = NULL;
    z_size_t destLen_z = 0;

    if (bound != (uLong)-1) {
        dest = (Bytef *)malloc(bound);
        if (dest) {
            destLen = bound;
        }
    }

    if (bound_z != (z_size_t)-1) {
        dest_z = (Bytef *)malloc(bound_z);
        if (dest_z) {
            destLen_z = bound_z;
        }
    }

    /* 3. Test compress */
    if (dest != NULL && sourceLen > 0) {
        uLongf destLen_copy = destLen;
        int ret = compress(dest, &destLen_copy, source, sourceLen);
        (void)ret; /* Suppress unused result warning */
    }

    /* 4. Test compress_z */
    if (dest_z != NULL && sourceLen_z > 0) {
        z_size_t destLen_z_copy = destLen_z;
        int ret = compress_z(dest_z, &destLen_z_copy, source, sourceLen_z);
        (void)ret;
    }

    /* 5. Test deflateBound and deflateBound_z */
    z_stream strm;
    init_stream(&strm);
    
    /* Initialize stream for deflateBound calls */
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        uLong db = deflateBound(&strm, sourceLen);
        (void)db;

        z_size_t db_z = deflateBound_z(&strm, sourceLen_z);
        (void)db_z;

        deflateEnd(&strm);
    } else {
        /* Even if init fails, we can still test the functions with invalid stream */
        uLong db = deflateBound(&strm, sourceLen);
        (void)db;
        z_size_t db_z = deflateBound_z(&strm, sourceLen_z);
        (void)db_z;
    }

    /* Cleanup */
    if (dest) free(dest);
    if (dest_z) free(dest_z);

    return 0;
}