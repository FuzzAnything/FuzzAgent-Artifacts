// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

static int fuzz_inflate_api(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef *out_buf = NULL;
    int ret = Z_OK;

    if (size < 2) return Z_OK;

    // Allocate output buffer
    out_buf = (Bytef *)malloc(size + 1024);
    if (!out_buf) return Z_MEM_ERROR;

    // Initialize inflate
    if (inflateInit(&strm) != Z_OK) {
        free(out_buf);
        return Z_OK;
    }

    strm.next_in = (const Bytef *)data;
    strm.avail_in = size;
    strm.next_out = out_buf;
    strm.avail_out = size + 1024;

    // Test inflatePrime with various bit counts
    // Ensure we have enough data to read data[0] and data[1]
    int bits = (data[0] % 17) - 8; // Range -8 to 8
    int value = data[1] % 256;
    ret = inflatePrime(&strm, bits, value);
    (void)ret;

    // Test inflateResetKeep
    ret = inflateResetKeep(&strm);
    (void)ret;

    // Test inflateReset
    ret = inflateReset(&strm);
    (void)ret;

    // Test inflateCopy
    z_stream dest = {0};
    ret = inflateCopy(&dest, &strm);
    if (ret == Z_OK) {
        inflateEnd(&dest);
    }

    // Cleanup
    inflateEnd(&strm);
    free(out_buf);
    return Z_OK;
}

static int fuzz_deflate_api(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef *in_buf = NULL;
    Bytef *out_buf = NULL;
    int ret = Z_OK;

    if (size < 1) return Z_OK;

    in_buf = (Bytef *)malloc(size);
    out_buf = (Bytef *)malloc(size + 1024);
    if (!in_buf || !out_buf) {
        free(in_buf);
        free(out_buf);
        return Z_MEM_ERROR;
    }

    memcpy(in_buf, data, size);

    // Initialize deflate
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(in_buf);
        free(out_buf);
        return Z_OK;
    }

    strm.next_in = in_buf;
    strm.avail_in = size;
    strm.next_out = out_buf;
    strm.avail_out = size + 1024;

    // Perform a small compression step to ensure state is valid
    ret = deflate(&strm, Z_NO_FLUSH);
    (void)ret;

    // Test deflateResetKeep
    ret = deflateResetKeep(&strm);
    (void)ret;

    // Test deflateReset
    ret = deflateReset(&strm);
    (void)ret;

    // Cleanup
    deflateEnd(&strm);
    free(in_buf);
    free(out_buf);
    return Z_OK;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz inflate-related functions
    fuzz_inflate_api(Data, Size);

    // Fuzz deflate-related functions
    fuzz_deflate_api(Data, Size);

    return 0;
}