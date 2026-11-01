// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static int fuzz_deflate_used(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    int bits = 0;
    int ret;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    if (size > 0) {
        strm.next_in = data;
        strm.avail_in = (uInt)size;
    }

    unsigned char out_buf[1024];
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END) {
        deflateEnd(&strm);
        return 0;
    }

    ret = deflateUsed(&strm, &bits);
    if (ret == Z_STREAM_ERROR) {
        // Expected if stream is invalid, but we initialized it, so this is a bug or edge case
    }

    deflateEnd(&strm);
    return 0;
}

static int fuzz_deflate_copy(const uint8_t *data, size_t size) {
    z_stream source = {0};
    z_stream dest = {0};
    int ret;

    if (deflateInit(&source, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    if (size > 0) {
        source.next_in = data;
        source.avail_in = (uInt)size;
    }

    unsigned char out_buf[1024];
    source.next_out = out_buf;
    source.avail_out = sizeof(out_buf);

    ret = deflate(&source, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END) {
        deflateEnd(&source);
        return 0;
    }

    ret = deflateCopy(&dest, &source);
    if (ret == Z_OK) {
        deflateEnd(&dest);
    }

    deflateEnd(&source);
    return 0;
}

static int fuzz_deflate_reset_keep(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    int ret;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    if (size > 0) {
        strm.next_in = data;
        strm.avail_in = (uInt)size;
    }

    unsigned char out_buf[1024];
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END) {
        deflateEnd(&strm);
        return 0;
    }

    ret = deflateResetKeep(&strm);
    if (ret == Z_STREAM_ERROR) {
        // Handle error if stream is invalid
    }

    deflateEnd(&strm);
    return 0;
}

static int fuzz_deflate_pending(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    unsigned pending = 0;
    int bits = 0;
    int ret;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    if (size > 0) {
        strm.next_in = data;
        strm.avail_in = (uInt)size;
    }

    unsigned char out_buf[1024];
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END) {
        deflateEnd(&strm);
        return 0;
    }

    ret = deflatePending(&strm, &pending, &bits);
    if (ret == Z_STREAM_ERROR || ret == Z_BUF_ERROR) {
        // Handle error cases
    }

    deflateEnd(&strm);
    return 0;
}

static int fuzz_deflate_tune(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    int ret;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    int good_length = (data[0] % 100) + 1;
    int max_lazy = (data[1] % 100) + 1;
    int nice_length = (data[2] % 100) + 1;
    int max_chain = (data[3] % 100) + 1;

    ret = deflateTune(&strm, good_length, max_lazy, nice_length, max_chain);
    if (ret == Z_STREAM_ERROR) {
        // Handle error if stream is invalid
    }

    deflateEnd(&strm);
    return 0;
}

static int fuzz_deflate_reset(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    int ret;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    if (size > 0) {
        strm.next_in = data;
        strm.avail_in = (uInt)size;
    }

    unsigned char out_buf[1024];
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END) {
        deflateEnd(&strm);
        return 0;
    }

    ret = deflateReset(&strm);
    if (ret == Z_STREAM_ERROR) {
        // Handle error if stream is invalid
    }

    deflateEnd(&strm);
    return 0;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    switch (Data[0] % 6) {
        case 0:
            fuzz_deflate_used(Data, Size);
            break;
        case 1:
            fuzz_deflate_copy(Data, Size);
            break;
        case 2:
            fuzz_deflate_reset_keep(Data, Size);
            break;
        case 3:
            fuzz_deflate_pending(Data, Size);
            break;
        case 4:
            fuzz_deflate_tune(Data, Size);
            break;
        case 5:
            fuzz_deflate_reset(Data, Size);
            break;
    }

    return 0;
}