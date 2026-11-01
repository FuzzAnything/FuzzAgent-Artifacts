// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
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

/* Helper function to initialize z_stream for deflateBound and deflateBound_z */
static int init_z_stream(z_stream *strm) {
    if (strm == NULL) return Z_STREAM_ERROR;
    memset(strm, 0, sizeof(z_stream));
    /* Use default allocators */
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    
    /* Initialize with default compression level */
    int ret = deflateInit(strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return ret;
    }
    return Z_OK;
}

/* Helper function to cleanup z_stream */
static void cleanup_z_stream(z_stream *strm) {
    if (strm != NULL) {
        deflateEnd(strm);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Allocate buffers for compression tests */
    /* We allocate a bit more than Size to handle potential expansion in worst case scenarios */
    size_t max_buf_size = Size * 2 + 1024;
    if (max_buf_size < Size) {
        /* Overflow check */
        return 0;
    }

    Bytef *source = (Bytef *)malloc(Size);
    Bytef *dest = (Bytef *)malloc(max_buf_size);
    
    if (source == NULL || dest == NULL) {
        free(source);
        free(dest);
        return 0;
    }

    memcpy(source, Data, Size);

    /* --- Test 1: compressBound --- */
    uLong sourceLen_uLong = (uLong)Size;
    uLong bound_uLong = compressBound(sourceLen_uLong);
    /* No cleanup needed for compressBound, just a calculation */

    /* --- Test 2: compressBound_z --- */
    z_size_t sourceLen_z = (z_size_t)Size;
    z_size_t bound_z = compressBound_z(sourceLen_z);
    /* No cleanup needed */

    /* --- Test 3: compress --- */
    uLongf destLen_uLongf = max_buf_size;
    int ret_compress = compress(dest, &destLen_uLongf, source, sourceLen_uLong);
    /* No specific cleanup for compress, just check return */

    /* --- Test 4: compress_z --- */
    z_size_t destLen_z = max_buf_size;
    int ret_compress_z = compress_z(dest, &destLen_z, source, sourceLen_z);
    /* No specific cleanup */

    /* --- Test 5 & 6: deflateBound and deflateBound_z --- */
    /* We need a valid z_stream for these */
    z_stream strm;
    if (init_z_stream(&strm) == Z_OK) {
        /* Set input data for the stream context if needed, though these functions 
           primarily rely on the stream state (strategy) and length */
        strm.next_in = source;
        strm.avail_in = (uInt)Size;
        strm.total_in = 0;

        /* Test deflateBound */
        uLong bound_deflate = deflateBound(&strm, sourceLen_uLong);

        /* Test deflateBound_z */
        z_size_t bound_deflate_z = deflateBound_z(&strm, sourceLen_z);

        /* Cleanup the stream */
        cleanup_z_stream(&strm);
    }

    /* Free allocated memory */
    free(source);
    free(dest);

    return 0;
}