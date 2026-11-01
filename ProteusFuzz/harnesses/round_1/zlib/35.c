// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
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

static int fuzz_deflate_prime(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef out_buf[4096];
    int ret = 0;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    if (size >= 2) {
        int bits = data[0] % 17;
        int value = (data[1] << 8) | (size > 2 ? data[2] : 0);
        ret = deflatePrime(&strm, bits, value);
        (void)ret;
    }

    deflateEnd(&strm);
    return 0;
}

static int fuzz_deflate_reset_keep(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef out_buf[4096];
    Bytef in_buf[256];
    int ret = 0;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    if (size > 0) {
        size_t in_size = size > sizeof(in_buf) ? sizeof(in_buf) : size;
        memcpy(in_buf, data, in_size);
        strm.next_in = in_buf;
        strm.avail_in = in_size;

        if (deflate(&strm, Z_NO_FLUSH) != Z_OK) {
            deflateEnd(&strm);
            return 0;
        }
    }

    ret = deflateResetKeep(&strm);
    (void)ret;

    deflateEnd(&strm);
    return 0;
}

static int fuzz_deflate_pending(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef out_buf[4096];
    unsigned pending;
    int bits;
    int ret = 0;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    if (size > 0) {
        Bytef in_buf[256];
        size_t in_size = size > sizeof(in_buf) ? sizeof(in_buf) : size;
        memcpy(in_buf, data, in_size);
        strm.next_in = in_buf;
        strm.avail_in = in_size;

        if (deflate(&strm, Z_NO_FLUSH) != Z_OK) {
            deflateEnd(&strm);
            return 0;
        }
    }

    ret = deflatePending(&strm, &pending, &bits);
    (void)ret;

    deflateEnd(&strm);
    return 0;
}

static int fuzz_deflate_used(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef out_buf[4096];
    int bits;
    int ret = 0;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    if (size > 0) {
        Bytef in_buf[256];
        size_t in_size = size > sizeof(in_buf) ? sizeof(in_buf) : size;
        memcpy(in_buf, data, in_size);
        strm.next_in = in_buf;
        strm.avail_in = in_size;

        if (deflate(&strm, Z_NO_FLUSH) != Z_OK) {
            deflateEnd(&strm);
            return 0;
        }
    }

    ret = deflateUsed(&strm, &bits);
    (void)ret;

    deflateEnd(&strm);
    return 0;
}

static int fuzz_deflate_tune(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef out_buf[4096];
    int ret = 0;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    if (size >= 4) {
        int good_length = data[0] % 20;
        int max_lazy = data[1] % 20;
        int nice_length = data[2] % 20;
        int max_chain = data[3] % 20;
        ret = deflateTune(&strm, good_length, max_lazy, nice_length, max_chain);
        (void)ret;
    }

    deflateEnd(&strm);
    return 0;
}

static int fuzz_deflate_copy(const uint8_t *data, size_t size) {
    z_stream source = {0};
    z_stream dest = {0};
    Bytef out_buf[4096];
    int ret = 0;

    if (deflateInit(&source, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    source.next_out = out_buf;
    source.avail_out = sizeof(out_buf);

    if (size > 0) {
        Bytef in_buf[256];
        size_t in_size = size > sizeof(in_buf) ? sizeof(in_buf) : size;
        memcpy(in_buf, data, in_size);
        source.next_in = in_buf;
        source.avail_in = in_size;

        if (deflate(&source, Z_NO_FLUSH) != Z_OK) {
            deflateEnd(&source);
            return 0;
        }
    }

    dest.zalloc = source.zalloc;
    dest.zfree = source.zfree;
    dest.opaque = source.opaque;

    ret = deflateCopy(&dest, &source);
    (void)ret;

    if (ret == Z_OK) {
        deflateEnd(&dest);
    }

    deflateEnd(&source);
    return 0;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    fuzz_deflate_prime(Data, Size);
    fuzz_deflate_reset_keep(Data, Size);
    fuzz_deflate_pending(Data, Size);
    fuzz_deflate_used(Data, Size);
    fuzz_deflate_tune(Data, Size);
    fuzz_deflate_copy(Data, Size);

    return 0;
}