// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
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

static int fuzz_deflateTune(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    unsigned char out_buf[1024];
    int ret;

    if (Size < 4) return 0;

    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    ret = deflateInit2_(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) return 0;

    int good_length = (int)Data[0];
    int max_lazy = (int)Data[1];
    int nice_length = (int)Data[2];
    int max_chain = (int)Data[3];

    ret = deflateTune(&strm, good_length, max_lazy, nice_length, max_chain);

    deflateEnd(&strm);
    return ret;
}

static int fuzz_deflateUsed(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    unsigned char out_buf[1024];
    int bits;
    int ret;

    if (Size < 1) return 0;

    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    ret = deflateInit2_(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) return 0;

    if (Data[0] & 1) {
        ret = deflateUsed(&strm, &bits);
    } else {
        ret = deflateUsed(&strm, NULL);
    }

    deflateEnd(&strm);
    return ret;
}

static int fuzz_deflatePrime(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    unsigned char out_buf[1024];
    int ret;

    if (Size < 2) return 0;

    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    ret = deflateInit2_(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) return 0;

    int bits = Data[0] % 17;
    int value = (int)Data[1];

    ret = deflatePrime(&strm, bits, value);

    deflateEnd(&strm);
    return ret;
}

static int fuzz_deflateResetKeep(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    unsigned char out_buf[1024];
    int ret;

    if (Size < 1) return 0;

    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    ret = deflateInit2_(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) return 0;

    if (Size > 0) {
        strm.next_in = Data;
        strm.avail_in = Size;
        deflate(&strm, Z_NO_FLUSH);
    }

    ret = deflateResetKeep(&strm);

    deflateEnd(&strm);
    return ret;
}

static int fuzz_deflateCopy(const uint8_t *Data, size_t Size) {
    z_stream source = {0};
    z_stream dest = {0};
    unsigned char out_buf[1024];
    int ret;

    if (Size < 1) return 0;

    source.next_out = out_buf;
    source.avail_out = sizeof(out_buf);
    source.zalloc = Z_NULL;
    source.zfree = Z_NULL;
    source.opaque = Z_NULL;

    ret = deflateInit2_(&source, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) return 0;

    if (Size > 0) {
        source.next_in = Data;
        source.avail_in = Size;
        deflate(&source, Z_NO_FLUSH);
    }

    dest.zalloc = Z_NULL;
    dest.zfree = Z_NULL;
    dest.opaque = Z_NULL;

    ret = deflateCopy(&dest, &source);

    deflateEnd(&source);
    if (ret == Z_OK) {
        deflateEnd(&dest);
    }
    return ret;
}

static int fuzz_deflateInit2_(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    unsigned char out_buf[1024];
    int ret;

    if (Size < 6) return 0;

    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    int level = (int)Data[0] % 10;
    int method = (int)Data[1] % 2; // 0 or 1
    int windowBits = (int)Data[2] % 16;
    int memLevel = (int)Data[3] % 10;
    int strategy = (int)Data[4] % 4;
    const char *version = (Size > 5) ? (const char *)Data + 5 : ZLIB_VERSION;
    int stream_size = (int)sizeof(z_stream);

    // Ensure version string is null-terminated if derived from fuzz data
    char ver_buf[256];
    if (Size > 5 && Size < 256) {
        memcpy(ver_buf, Data + 5, Size - 5);
        ver_buf[Size - 5] = '\0';
        version = ver_buf;
    } else {
        version = ZLIB_VERSION;
    }

    ret = deflateInit2_(&strm, level, method, windowBits, memLevel, strategy, version, stream_size);

    if (ret == Z_OK) {
        deflateEnd(&strm);
    }
    return ret;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    int op = Data[0] % 6;

    switch (op) {
        case 0:
            fuzz_deflateTune(Data, Size);
            break;
        case 1:
            fuzz_deflateUsed(Data, Size);
            break;
        case 2:
            fuzz_deflatePrime(Data, Size);
            break;
        case 3:
            fuzz_deflateResetKeep(Data, Size);
            break;
        case 4:
            fuzz_deflateCopy(Data, Size);
            break;
        case 5:
            fuzz_deflateInit2_(Data, Size);
            break;
        default:
            break;
    }

    return 0;
}