// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
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

/* Helper to initialize a z_stream for deflateBound tests */
static int init_stream_for_bound(z_stream *strm) {
    if (strm == NULL) return Z_STREAM_ERROR;
    memset(strm, 0, sizeof(z_stream));
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    /* Initialize with default deflate settings to get a valid state */
    if (deflateInit(strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return Z_STREAM_ERROR;
    }
    return Z_OK;
}

/* Helper to cleanup a z_stream */
static void cleanup_stream(z_stream *strm) {
    if (strm != NULL) {
        deflateEnd(strm);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Allocate buffers for compression tests */
    /* Use a safe upper bound for destination buffer size based on input size */
    z_size_t max_dest_size = compressBound_z(Size);
    if (max_dest_size == (z_size_t)-1) {
        /* Handle overflow case for compressBound_z */
        return 0;
    }

    /* Also check uLong version for compressBound */
    uLong max_dest_size_ul = compressBound((uLong)Size);
    if (max_dest_size_ul == (uLong)-1) {
        /* Handle overflow case for compressBound */
        return 0;
    }

    /* Allocate destination buffer. Use the larger of the two bounds to be safe */
    size_t alloc_size = (max_dest_size > max_dest_size_ul) ? max_dest_size : max_dest_size_ul;
    Bytef *dest_buf = (Bytef *)malloc(alloc_size);
    if (dest_buf == NULL) {
        return 0;
    }

    /* Test 1: compressBound (uLong version) */
    /* Already called above, but call again with different logic if needed */
    uLong bound1 = compressBound((uLong)Size);
    (void)bound1; /* Suppress unused variable warning */

    /* Test 2: compressBound_z (z_size_t version) */
    z_size_t bound2 = compressBound_z(Size);
    (void)bound2; /* Suppress unused variable warning */

    /* Test 3: compress (uLong version) */
    uLongf dest_len_ul = alloc_size;
    int ret_compress = compress(dest_buf, &dest_len_ul, (const Bytef *)Data, (uLong)Size);
    (void)ret_compress; /* Check return value implicitly, no specific action needed for fuzzing */

    /* Test 4: compress_z (z_size_t version) */
    z_size_t dest_len_z = alloc_size;
    int ret_compress_z = compress_z(dest_buf, &dest_len_z, (const Bytef *)Data, Size);
    (void)ret_compress_z;

    /* Test 5: deflateBound and deflateBound_z */
    /* Need a valid z_stream */
    z_stream strm;
    if (init_stream_for_bound(&strm) == Z_OK) {
        /* Test deflateBound (uLong version) */
        uLong bound_deflate_ul = deflateBound(&strm, (uLong)Size);
        (void)bound_deflate_ul;

        /* Test deflateBound_z (z_size_t version) */
        z_size_t bound_deflate_z = deflateBound_z(&strm, Size);
        (void)bound_deflate_z;

        /* Cleanup stream */
        cleanup_stream(&strm);
    }

    /* Cleanup allocated memory */
    free(dest_buf);

    return 0;
}