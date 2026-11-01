// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static int test_deflate_reset_keep(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    unsigned char out_buf[1024];
    int ret;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    if (size > 0) {
        strm.next_in = data;
        strm.avail_in = size;
        deflate(&strm, Z_NO_FLUSH);
    }

    ret = deflateResetKeep(&strm);
    if (ret == Z_OK) {
        deflateEnd(&strm);
        return 1;
    }
    deflateEnd(&strm);
    return 0;
}

static int test_inflate_reset(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    unsigned char out_buf[1024];
    int ret;

    if (inflateInit(&strm) != Z_OK) {
        return 0;
    }

    if (size > 0) {
        strm.next_in = data;
        strm.avail_in = size;
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        inflate(&strm, Z_NO_FLUSH);
    }

    ret = inflateReset(&strm);
    inflateEnd(&strm);
    return (ret == Z_OK) ? 1 : 0;
}

static int test_deflate_reset(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    unsigned char out_buf[1024];
    int ret;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    if (size > 0) {
        strm.next_in = data;
        strm.avail_in = size;
        deflate(&strm, Z_NO_FLUSH);
    }

    ret = deflateReset(&strm);
    deflateEnd(&strm);
    return (ret == Z_OK) ? 1 : 0;
}

static int test_inflate_reset_keep(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    unsigned char out_buf[1024];
    int ret;

    if (inflateInit(&strm) != Z_OK) {
        return 0;
    }

    if (size > 0) {
        strm.next_in = data;
        strm.avail_in = size;
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        inflate(&strm, Z_NO_FLUSH);
    }

    ret = inflateResetKeep(&strm);
    inflateEnd(&strm);
    return (ret == Z_OK) ? 1 : 0;
}

static int test_inflate_get_header(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    gz_header header;
    unsigned char out_buf[1024];
    int ret;

    memset(&header, 0, sizeof(header));

    if (inflateInit(&strm) != Z_OK) {
        return 0;
    }

    ret = inflateGetHeader(&strm, &header);
    if (ret != Z_OK) {
        inflateEnd(&strm);
        return 0;
    }

    if (size > 0) {
        strm.next_in = data;
        strm.avail_in = size;
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        inflate(&strm, Z_NO_FLUSH);
    }

    inflateEnd(&strm);
    return 1;
}

static int test_inflate_copy(const uint8_t *data, size_t size) {
    z_stream source = {0};
    z_stream dest = {0};
    unsigned char out_buf[1024];
    int ret;

    if (inflateInit(&source) != Z_OK) {
        return 0;
    }

    if (size > 0) {
        source.next_in = data;
        source.avail_in = size;
        source.next_out = out_buf;
        source.avail_out = sizeof(out_buf);
        inflate(&source, Z_NO_FLUSH);
    }

    ret = inflateCopy(&dest, &source);
    if (ret == Z_OK) {
        inflateEnd(&dest);
    }
    inflateEnd(&source);
    return (ret == Z_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    test_deflate_reset_keep(Data, Size);
    test_inflate_reset(Data, Size);
    test_deflate_reset(Data, Size);
    test_inflate_reset_keep(Data, Size);
    test_inflate_get_header(Data, Size);
    test_inflate_copy(Data, Size);

    return 0;
}