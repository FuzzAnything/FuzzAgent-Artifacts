// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Buffers for compression and decompression
    uint8_t *compress_buf = NULL;
    uint8_t *decompress_buf = NULL;
    size_t compress_size = 0;
    size_t decompress_size = 0;

    // Allocate buffers based on input size with safety limits
    if (Size > 0 && Size < 1024 * 1024) {
        compress_size = Size * 2 + 1024;
        decompress_size = Size * 2 + 1024;
        compress_buf = (uint8_t *)malloc(compress_size);
        decompress_buf = (uint8_t *)malloc(decompress_size);
    }

    if (!compress_buf || !decompress_buf) {
        free(compress_buf);
        free(decompress_buf);
        return 0;
    }

    z_stream deflate_strm = {0};
    z_stream inflate_strm = {0};
    z_stream inflate_copy_strm = {0};
    gz_header head = {0};

    // Initialize deflate stream for GZIP mode (wrap=2)
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    deflate_strm.next_in = Z_NULL;
    deflate_strm.avail_in = 0;

    int ret = deflateInit2(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15 + 16, 8, Z_DEFAULT_STRATEGY);
    // -15 + 16 = 1 (GZIP)
    if (ret != Z_OK) {
        free(compress_buf);
        free(decompress_buf);
        return 0;
    }

    // Initialize inflate stream
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    inflate_strm.next_in = Z_NULL;
    inflate_strm.avail_in = 0;

    ret = inflateInit2(&inflate_strm, 16 + MAX_WBITS); // GZIP mode
    if (ret != Z_OK) {
        deflateEnd(&deflate_strm);
        free(compress_buf);
        free(decompress_buf);
        return 0;
    }

    // Test inflateGetHeader
    head.text = 0;
    head.time = 0;
    head.xflags = 0;
    head.os = 0;
    head.extra = Z_NULL;
    head.extra_len = 0;
    head.extra_max = 0;
    head.name = Z_NULL;
    head.name_max = 0;
    head.comment = Z_NULL;
    head.comm_max = 0;
    head.hcrc = 0;
    head.done = 0;

    ret = inflateGetHeader(&inflate_strm, &head);
    // Ignore return value for fuzzing, just ensure it doesn't crash

    // Test deflateSetHeader
    head.text = Data[0] & 1;
    head.time = (uLong)(Data[1] * 256 + Data[2]);
    head.xflags = Data[3];
    head.os = Data[4];
    
    // Safe pointer assignment for extra, name fields
    size_t offset = 5;
    if (Size > offset) {
        head.extra = (Bytef *)Data + offset;
        head.extra_len = Size - offset;
        head.extra_max = head.extra_len;
    } else {
        head.extra = Z_NULL;
        head.extra_len = 0;
        head.extra_max = 0;
    }
    
    head.name = Z_NULL;
    head.name_max = 0;
    head.comment = Z_NULL;
    head.comm_max = 0;
    head.hcrc = (Size > 0 ? Data[Size - 1] & 1 : 0);

    ret = deflateSetHeader(&deflate_strm, &head);

    // Perform compression
    deflate_strm.next_in = (Bytef *)Data;
    deflate_strm.avail_in = (uInt)Size;
    deflate_strm.next_out = compress_buf;
    deflate_strm.avail_out = (uInt)compress_size;

    ret = deflate(&deflate_strm, Z_FINISH);
    size_t compressed_len = compress_size - deflate_strm.avail_out;

    // Test deflateReset
    ret = deflateReset(&deflate_strm);

    // Prepare for decompression
    inflate_strm.next_in = compress_buf;
    inflate_strm.avail_in = (uInt)compressed_len;
    inflate_strm.next_out = decompress_buf;
    inflate_strm.avail_out = (uInt)decompress_size;

    // Test inflate
    ret = inflate(&inflate_strm, Z_NO_FLUSH);
    if (ret == Z_OK || ret == Z_STREAM_END) {
        // Test inflateReset
        ret = inflateReset(&inflate_strm);
        
        // Test inflateCopy
        inflate_copy_strm.zalloc = Z_NULL;
        inflate_copy_strm.zfree = Z_NULL;
        inflate_copy_strm.opaque = Z_NULL;
        inflate_copy_strm.next_in = Z_NULL;
        inflate_copy_strm.avail_in = 0;
        inflate_copy_strm.next_out = Z_NULL;
        inflate_copy_strm.avail_out = 0;
        
        ret = inflateCopy(&inflate_copy_strm, &inflate_strm);
        if (ret == Z_OK) {
            inflateEnd(&inflate_copy_strm);
        }
    }

    // Cleanup
    inflateEnd(&inflate_strm);
    deflateEnd(&deflate_strm);
    free(compress_buf);
    free(decompress_buf);

    return 0;
}