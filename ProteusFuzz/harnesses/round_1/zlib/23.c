// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compress_z at compress.c:77:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress2 at uncompr.c:83:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Fuzz driver for zlib compression and decompression functions.
 * Targets: uncompress, compress2_z, uncompress2_z, uncompress_z, uncompress2, compress_z
 */

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Allocate buffers for compression and decompression */
    /* Source buffer for compression */
    Bytef *source = (Bytef *)malloc(Size);
    if (!source) return 0;
    memcpy(source, Data, Size);

    /* Destination buffer for compression (worst case: slightly larger than source) */
    z_size_t compress_dest_size = Size + (Size / 10) + 12 + 5;
    Bytef *compress_dest = (Bytef *)malloc(compress_dest_size);
    if (!compress_dest) {
        free(source);
        return 0;
    }

    /* Destination buffer for decompression (worst case: much larger than source) */
    /* We use a safe upper bound, typically 10x or fixed large size */
    z_size_t decompress_dest_size = Size * 10 + 1024;
    if (decompress_dest_size < 1024) decompress_dest_size = 1024;
    Bytef *decompress_dest = (Bytef *)malloc(decompress_dest_size);
    if (!decompress_dest) {
        free(source);
        free(compress_dest);
        return 0;
    }

    /* Helper variables for lengths */
    z_size_t compress_dest_len = compress_dest_size;
    z_size_t source_len = Size;
    z_size_t decompress_dest_len = decompress_dest_size;
    uLong uLong_source_len = (uLong)Size;
    uLongf uLong_dest_len = (uLongf)decompress_dest_size;

    /*
     * 1. Test compress_z
     */
    compress_dest_len = compress_dest_size;
    compress_z(compress_dest, &compress_dest_len, source, source_len);

    /*
     * 2. Test compress2_z with different levels
     */
    int levels[] = {Z_NO_COMPRESSION, Z_DEFAULT_COMPRESSION, Z_BEST_SPEED, Z_BEST_COMPRESSION};
    for (int i = 0; i < 4; i++) {
        compress_dest_len = compress_dest_size;
        compress2_z(compress_dest, &compress_dest_len, source, source_len, levels[i]);
    }

    /*
     * 3. Test uncompress_z (uses full source length)
     */
    decompress_dest_len = decompress_dest_size;
    uncompress_z(decompress_dest, &decompress_dest_len, compress_dest, compress_dest_len);

    /*
     * 4. Test uncompress2_z (allows partial source consumption)
     */
    decompress_dest_len = decompress_dest_size;
    z_size_t source_len_z = compress_dest_len;
    uncompress2_z(decompress_dest, &decompress_dest_len, compress_dest, &source_len_z);

    /*
     * 5. Test uncompress (legacy uLong interface)
     */
    uLong_dest_len = (uLongf)decompress_dest_size;
    uLong_source_len = (uLong)compress_dest_len;
    uncompress(decompress_dest, &uLong_dest_len, compress_dest, uLong_source_len);

    /*
     * 6. Test uncompress2 (legacy uLong interface with source length update)
     */
    uLong_dest_len = (uLongf)decompress_dest_size;
    uLong_source_len = (uLong)compress_dest_len;
    uncompress2(decompress_dest, &uLong_dest_len, compress_dest, &uLong_source_len);

    /* Cleanup */
    free(source);
    free(compress_dest);
    free(decompress_dest);

    return 0;
}