// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static int fuzz_deflate_reset(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    uint8_t *out_buf = NULL;
    int ret = Z_OK;

    if (size < 1) return 0;

    out_buf = (uint8_t *)malloc(size + 1024);
    if (!out_buf) return 0;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(out_buf);
        return 0;
    }

    strm.next_in = data;
    strm.avail_in = (uInt)size;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(size + 1024);

    if (deflate(&strm, Z_NO_FLUSH) != Z_OK && strm.avail_out > 0) {
        // Continue even if not fully compressed, just to test reset
    }

    // Test deflateReset
    ret = deflateReset(&strm);
    if (ret == Z_OK) {
        // Try to compress again with reset state
        strm.next_in = data;
        strm.avail_in = (uInt)size;
        strm.next_out = out_buf;
        strm.avail_out = (uInt)(size + 1024);
        deflate(&strm, Z_FINISH);
    }

    deflateEnd(&strm);
    free(out_buf);
    return 0;
}

static int fuzz_inflate(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    uint8_t *out_buf = NULL;
    int ret = Z_OK;

    if (size < 1) return 0;

    out_buf = (uint8_t *)malloc(size * 2);
    if (!out_buf) return 0;

    if (inflateInit_(&strm, ZLIB_VERSION, (int)sizeof(z_stream)) != Z_OK) {
        free(out_buf);
        return 0;
    }

    strm.next_in = data;
    strm.avail_in = (uInt)size;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(size * 2);

    ret = inflate(&strm, Z_NO_FLUSH);
    if (ret == Z_OK || ret == Z_STREAM_END) {
        // Try inflateReset
        ret = inflateReset(&strm);
        if (ret == Z_OK) {
            strm.next_in = data;
            strm.avail_in = (uInt)size;
            strm.next_out = out_buf;
            strm.avail_out = (uInt)(size * 2);
            inflate(&strm, Z_FINISH);
        }
    }

    inflateEnd(&strm);
    free(out_buf);
    return 0;
}

static int fuzz_deflate_end(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    uint8_t *out_buf = NULL;
    int ret = Z_OK;

    if (size < 1) return 0;

    out_buf = (uint8_t *)malloc(size + 1024);
    if (!out_buf) return 0;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(out_buf);
        return 0;
    }

    strm.next_in = data;
    strm.avail_in = (uInt)size;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(size + 1024);

    deflate(&strm, Z_FINISH);
    ret = deflateEnd(&strm);

    free(out_buf);
    return 0;
}

static int fuzz_inflate_end(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    uint8_t *out_buf = NULL;
    int ret = Z_OK;

    if (size < 1) return 0;

    out_buf = (uint8_t *)malloc(size * 2);
    if (!out_buf) return 0;

    if (inflateInit_(&strm, ZLIB_VERSION, (int)sizeof(z_stream)) != Z_OK) {
        free(out_buf);
        return 0;
    }

    strm.next_in = data;
    strm.avail_in = (uInt)size;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(size * 2);

    inflate(&strm, Z_NO_FLUSH);
    ret = inflateEnd(&strm);

    free(out_buf);
    return 0;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz deflateReset
    fuzz_deflate_reset(Data, Size);

    // Fuzz inflate and inflateReset
    fuzz_inflate(Data, Size);

    // Fuzz deflateEnd
    fuzz_deflate_end(Data, Size);

    // Fuzz inflateEnd
    fuzz_inflate_end(Data, Size);

    return 0;
}