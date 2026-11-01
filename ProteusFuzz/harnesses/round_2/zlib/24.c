// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
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
#include <zlib.h>

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Allocate buffers for compression operations
    // Use a safe upper bound for destination size to prevent immediate overflow
    uLong max_compressed_size = compressBound(Size);
    if (max_compressed_size == (uLong)-1) {
        return 0;
    }

    Bytef *dest_buf = (Bytef *)malloc(max_compressed_size);
    if (!dest_buf) {
        return 0;
    }

    // Test compress and compress_z
    uLong destLen = max_compressed_size;
    uLong destLen_z = max_compressed_size;
    int ret = compress(dest_buf, &destLen, (const Bytef *)Data, Size);
    (void)ret; // Ignore return for fuzzing, just trigger execution

    ret = compress_z(dest_buf, &destLen_z, (const Bytef *)Data, Size);
    (void)ret;

    // Test compressBound and compressBound_z
    uLong bound = compressBound(Size);
    (void)bound;
    z_size_t bound_z = compressBound_z(Size);
    (void)bound_z;

    // Setup z_stream for deflateBound and deflateBound_z
    z_stream strm = {0};
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    // Initialize stream for deflateBound tests
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Test deflateBound and deflateBound_z
        uLong db = deflateBound(&strm, Size);
        (void)db;
        z_size_t db_z = deflateBound_z(&strm, Size);
        (void)db_z;

        deflateEnd(&strm);
    }

    // Test with invalid stream for deflateBound (should return conservative estimate)
    z_stream invalid_strm = {0};
    uLong db_invalid = deflateBound(&invalid_strm, Size);
    (void)db_invalid;
    z_size_t db_z_invalid = deflateBound_z(&invalid_strm, Size);
    (void)db_z_invalid;

    // Cleanup
    free(dest_buf);
    return 0;
}