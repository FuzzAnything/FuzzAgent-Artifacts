// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

static int fuzz_deflate_pending(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    unsigned pending;
    int bits;
    int ret;
    Bytef *out_buf;

    if (Size < 1) return 0;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) return 0;

    out_buf = (Bytef *)malloc(65536);
    if (!out_buf) {
        deflateEnd(&strm);
        return 0;
    }

    strm.next_in = (Bytef *)Data;
    strm.avail_in = Size;
    strm.next_out = out_buf;
    strm.avail_out = 65536;

    ret = deflate(&strm, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR) {
        free(out_buf);
        deflateEnd(&strm);
        return 0;
    }

    ret = deflatePending(&strm, &pending, &bits);
    (void)pending;
    (void)bits;

    free(out_buf);
    deflateEnd(&strm);
    return 0;
}

static int fuzz_deflate_reset_keep(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    Bytef *out_buf;
    int ret;

    if (Size < 1) return 0;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) return 0;

    out_buf = (Bytef *)malloc(65536);
    if (!out_buf) {
        deflateEnd(&strm);
        return 0;
    }

    strm.next_in = (Bytef *)Data;
    strm.avail_in = Size;
    strm.next_out = out_buf;
    strm.avail_out = 65536;

    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        free(out_buf);
        deflateEnd(&strm);
        return 0;
    }

    ret = deflateResetKeep(&strm);
    if (ret == Z_OK) {
        strm.next_in = (Bytef *)Data;
        strm.avail_in = Size;
        strm.next_out = out_buf;
        strm.avail_out = 65536;
        deflate(&strm, Z_FINISH);
    }

    free(out_buf);
    deflateEnd(&strm);
    return 0;
}

static int fuzz_deflate_copy(const uint8_t *Data, size_t Size) {
    z_stream source = {0};
    z_stream dest = {0};
    Bytef *out_buf;
    int ret;

    if (Size < 1) return 0;

    if (deflateInit(&source, Z_DEFAULT_COMPRESSION) != Z_OK) return 0;

    out_buf = (Bytef *)malloc(65536);
    if (!out_buf) {
        deflateEnd(&source);
        return 0;
    }

    source.next_in = (Bytef *)Data;
    source.avail_in = Size;
    source.next_out = out_buf;
    source.avail_out = 65536;

    ret = deflate(&source, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        free(out_buf);
        deflateEnd(&source);
        return 0;
    }

    ret = deflateCopy(&dest, &source);
    if (ret == Z_OK) {
        deflateEnd(&dest);
    }

    free(out_buf);
    deflateEnd(&source);
    return 0;
}

static int fuzz_deflate_prime(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    Bytef *out_buf;
    int ret;
    int bits;
    int value;

    if (Size < 3) return 0;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) return 0;

    out_buf = (Bytef *)malloc(65536);
    if (!out_buf) {
        deflateEnd(&strm);
        return 0;
    }

    strm.next_out = out_buf;
    strm.avail_out = 65536;

    bits = Data[0] % 17;
    value = ((int)Data[1] << 8) | (int)Data[2];

    ret = deflatePrime(&strm, bits, value);
    (void)ret;

    deflateEnd(&strm);
    free(out_buf);
    return 0;
}

static int fuzz_deflate_used(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    Bytef *out_buf;
    int bits;
    int ret;

    if (Size < 1) return 0;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) return 0;

    out_buf = (Bytef *)malloc(65536);
    if (!out_buf) {
        deflateEnd(&strm);
        return 0;
    }

    strm.next_in = (Bytef *)Data;
    strm.avail_in = Size;
    strm.next_out = out_buf;
    strm.avail_out = 65536;

    ret = deflate(&strm, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR) {
        free(out_buf);
        deflateEnd(&strm);
        return 0;
    }

    ret = deflateUsed(&strm, &bits);
    (void)ret;
    (void)bits;

    free(out_buf);
    deflateEnd(&strm);
    return 0;
}

static int fuzz_deflate_tune(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    Bytef *out_buf;
    int ret;
    int good_length, max_lazy, nice_length, max_chain;

    if (Size < 4) return 0;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) return 0;

    out_buf = (Bytef *)malloc(65536);
    if (!out_buf) {
        deflateEnd(&strm);
        return 0;
    }

    good_length = Data[0] % 200;
    max_lazy = Data[1] % 200;
    nice_length = Data[2] % 200;
    max_chain = Data[3] % 200;

    ret = deflateTune(&strm, good_length, max_lazy, nice_length, max_chain);
    (void)ret;

    strm.next_in = (Bytef *)Data;
    strm.avail_in = Size;
    strm.next_out = out_buf;
    strm.avail_out = 65536;
    deflate(&strm, Z_FINISH);

    free(out_buf);
    deflateEnd(&strm);
    return 0;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    switch (Data[0] % 6) {
        case 0:
            fuzz_deflate_pending(Data, Size);
            break;
        case 1:
            fuzz_deflate_reset_keep(Data, Size);
            break;
        case 2:
            fuzz_deflate_copy(Data, Size);
            break;
        case 3:
            fuzz_deflate_prime(Data, Size);
            break;
        case 4:
            fuzz_deflate_used(Data, Size);
            break;
        case 5:
            fuzz_deflate_tune(Data, Size);
            break;
    }
    return 0;
}