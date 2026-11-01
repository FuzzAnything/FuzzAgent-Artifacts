// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflate at deflate.c:981:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int fuzz_inflate_reset_keep(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef out_buf[1024];
    int ret;

    if (inflateInit(&strm) != Z_OK) {
        return 0;
    }

    strm.next_in = data;
    strm.avail_in = size;
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    /* Perform some initial inflation to establish state */
    inflate(&strm, Z_NO_FLUSH);

    /* Call target function */
    ret = inflateResetKeep(&strm);

    inflateEnd(&strm);
    return ret;
}

static int fuzz_inflate_copy(const uint8_t *data, size_t size) {
    z_stream source = {0};
    z_stream dest = {0};
    Bytef out_buf[1024];
    int ret;

    if (inflateInit(&source) != Z_OK) {
        return 0;
    }

    source.next_in = data;
    source.avail_in = size;
    source.next_out = out_buf;
    source.avail_out = sizeof(out_buf);

    /* Perform some initial inflation to establish state */
    inflate(&source, Z_NO_FLUSH);

    /* Call target function */
    ret = inflateCopy(&dest, &source);

    if (ret == Z_OK) {
        inflateEnd(&dest);
    }

    inflateEnd(&source);
    return ret;
}

static int fuzz_inflate_validate(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef out_buf[1024];
    int ret;
    int check;

    if (inflateInit(&strm) != Z_OK) {
        return 0;
    }

    strm.next_in = data;
    strm.avail_in = size;
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    /* Perform some initial inflation to establish state */
    inflate(&strm, Z_NO_FLUSH);

    /* Toggle check based on first byte of input */
    check = size > 0 ? (data[0] % 2) : 0;

    /* Call target function */
    ret = inflateValidate(&strm, check);

    inflateEnd(&strm);
    return ret;
}

static int fuzz_deflate_reset(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef in_buf[1024];
    Bytef out_buf[1024];
    int ret;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    /* Prepare some input data */
    size_t input_size = size < sizeof(in_buf) ? size : sizeof(in_buf);
    memcpy(in_buf, data, input_size);

    strm.next_in = in_buf;
    strm.avail_in = input_size;
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    /* Perform some initial compression to establish state */
    deflate(&strm, Z_NO_FLUSH);

    /* Call target function */
    ret = deflateReset(&strm);

    deflateEnd(&strm);
    return ret;
}

static int fuzz_inflate_get_header(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    gz_header header = {0};
    Bytef out_buf[1024];
    int ret;

    /* Initialize header structure */
    header.text = 0;
    header.time = 0;
    header.xflags = 0;
    header.os = 0;
    header.extra = NULL;
    header.extra_len = 0;
    header.extra_max = 0;
    header.name = NULL;
    header.name_max = 0;
    header.comment = NULL;
    header.comm_max = 0;
    header.hcrc = 0;
    header.done = 0;

    /* Call target function before inflateInit as per spec */
    ret = inflateGetHeader(&strm, &header);

    if (ret == Z_OK) {
        if (inflateInit(&strm) == Z_OK) {
            strm.next_in = data;
            strm.avail_in = size;
            strm.next_out = out_buf;
            strm.avail_out = sizeof(out_buf);
            inflate(&strm, Z_NO_FLUSH);
            inflateEnd(&strm);
        }
    }

    return ret;
}

static int fuzz_inflate_reset(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef out_buf[1024];
    int ret;

    if (inflateInit(&strm) != Z_OK) {
        return 0;
    }

    strm.next_in = data;
    strm.avail_in = size;
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    /* Perform some initial inflation to establish state */
    inflate(&strm, Z_NO_FLUSH);

    /* Call target function */
    ret = inflateReset(&strm);

    inflateEnd(&strm);
    return ret;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Fuzz all target functions */
    fuzz_inflate_reset_keep(Data, Size);
    fuzz_inflate_copy(Data, Size);
    fuzz_inflate_validate(Data, Size);
    fuzz_deflate_reset(Data, Size);
    fuzz_inflate_get_header(Data, Size);
    fuzz_inflate_reset(Data, Size);

    return 0;
}