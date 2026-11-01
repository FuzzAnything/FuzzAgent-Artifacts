// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static int fuzz_inflate(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    Bytef *out_buf = NULL;
    int ret;

    if (Size < 1) return 0;

    out_buf = (Bytef *)malloc(1024);
    if (!out_buf) return 0;

    // Initialize with negative window size for raw deflate or gzip
    if (inflateInit2(&strm, -15) != Z_OK) {
        free(out_buf);
        return 0;
    }

    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = 1024;

    ret = inflate(&strm, Z_NO_FLUSH);
    
    // Try to recover from data error
    if (ret == Z_DATA_ERROR) {
        ret = inflateSync(&strm);
        if (ret == Z_OK) {
            // Continue decompression after sync
            ret = inflate(&strm, Z_NO_FLUSH);
        }
    }

    inflateEnd(&strm);
    free(out_buf);
    return 0;
}

static int fuzz_deflate_reset(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    Bytef *out_buf = NULL;
    int ret;

    if (Size < 1) return 0;

    out_buf = (Bytef *)malloc(1024);
    if (!out_buf) return 0;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Compress some data
    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = 1024;
    
    ret = deflate(&strm, Z_NO_FLUSH);
    
    // Reset the stream
    ret = deflateReset(&strm);
    
    // Compress again to ensure state is fresh
    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = 1024;
    ret = deflate(&strm, Z_FINISH);

    deflateEnd(&strm);
    free(out_buf);
    return 0;
}

static int fuzz_inflate_sync(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    Bytef *out_buf = NULL;
    int ret;

    if (Size < 1) return 0;

    out_buf = (Bytef *)malloc(1024);
    if (!out_buf) return 0;

    if (inflateInit(&strm) != Z_OK) {
        free(out_buf);
        return 0;
    }

    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = 1024;

    // Attempt decompression
    ret = inflate(&strm, Z_NO_FLUSH);

    // If data error, try to sync
    if (ret == Z_DATA_ERROR) {
        ret = inflateSync(&strm);
        // Try to continue after sync
        if (ret == Z_OK) {
            ret = inflate(&strm, Z_NO_FLUSH);
        }
    }

    inflateEnd(&strm);
    free(out_buf);
    return 0;
}

static int fuzz_deflate_set_header(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    gz_header head = {0};
    Bytef *out_buf = NULL;
    int ret;

    if (Size < 1) return 0;

    out_buf = (Bytef *)malloc(1024);
    if (!out_buf) return 0;

    // Initialize with GZIP wrapper (wrap=2)
    if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 16 + 8, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Setup header
    head.text = 1;
    head.time = 0;
    head.xflags = 0;
    head.os = 0;
    head.extra = NULL;
    head.extra_len = 0;
    head.name = NULL;
    head.comment = NULL;
    head.hcrc = 0;
    head.done = 0;

    ret = deflateSetHeader(&strm, &head);
    if (ret != Z_OK) {
        deflateEnd(&strm);
        free(out_buf);
        return 0;
    }

    // Compress data
    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = 1024;
    ret = deflate(&strm, Z_FINISH);

    deflateEnd(&strm);
    free(out_buf);
    return 0;
}

static int fuzz_inflate_get_header(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    gz_header head = {0};
    Bytef *out_buf = NULL;
    int ret;

    if (Size < 1) return 0;

    out_buf = (Bytef *)malloc(1024);
    if (!out_buf) return 0;

    // Initialize with GZIP wrapper
    if (inflateInit2(&strm, 16 + 8) != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Setup header retrieval
    head.text = 0;
    head.time = 0;
    head.xflags = 0;
    head.os = 0;
    head.extra = NULL;
    head.extra_len = 0;
    head.name = NULL;
    head.comment = NULL;
    head.hcrc = 0;
    head.done = 0;

    ret = inflateGetHeader(&strm, &head);
    if (ret != Z_OK) {
        inflateEnd(&strm);
        free(out_buf);
        return 0;
    }

    // Decompress
    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = 1024;
    ret = inflate(&strm, Z_NO_FLUSH);

    inflateEnd(&strm);
    free(out_buf);
    return 0;
}

static int fuzz_inflate_copy(const uint8_t *Data, size_t Size) {
    z_stream source = {0};
    z_stream dest = {0};
    Bytef *out_buf = NULL;
    int ret;

    if (Size < 1) return 0;

    out_buf = (Bytef *)malloc(1024);
    if (!out_buf) return 0;

    // Initialize source
    if (inflateInit(&source) != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Feed some data to source
    source.next_in = (Bytef *)Data;
    source.avail_in = (uInt)Size;
    source.next_out = out_buf;
    source.avail_out = 1024;
    ret = inflate(&source, Z_NO_FLUSH);

    // Copy source to dest
    ret = inflateCopy(&dest, &source);
    if (ret != Z_OK) {
        inflateEnd(&source);
        free(out_buf);
        return 0;
    }

    // Continue decompression on dest
    dest.next_in = source.next_in;
    dest.avail_in = source.avail_in;
    dest.next_out = out_buf;
    dest.avail_out = 1024;
    ret = inflate(&dest, Z_NO_FLUSH);

    inflateEnd(&dest);
    inflateEnd(&source);
    free(out_buf);
    return 0;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz inflate
    fuzz_inflate(Data, Size);

    // Fuzz deflateReset
    fuzz_deflate_reset(Data, Size);

    // Fuzz inflateSync
    fuzz_inflate_sync(Data, Size);

    // Fuzz deflateSetHeader
    fuzz_deflate_set_header(Data, Size);

    // Fuzz inflateGetHeader
    fuzz_inflate_get_header(Data, Size);

    // Fuzz inflateCopy
    fuzz_inflate_copy(Data, Size);

    return 0;
}