// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

static void cleanup_stream(z_stream *strm) {
    if (strm != NULL) {
        inflateEnd(strm);
        deflateEnd(strm);
        free(strm->next_out);
        free(strm->next_in);
        free(strm);
    }
}

static int fuzz_inflateGetHeader(const uint8_t *Data, size_t Size) {
    z_stream *strm = (z_stream *)calloc(1, sizeof(z_stream));
    if (!strm) return 0;

    gz_header head;
    memset(&head, 0, sizeof(head));

    int ret = inflateGetHeader(strm, &head);
    cleanup_stream(strm);
    return ret;
}

static int fuzz_inflateCopy(const uint8_t *Data, size_t Size) {
    z_stream *source = (z_stream *)calloc(1, sizeof(z_stream));
    z_stream *dest = (z_stream *)calloc(1, sizeof(z_stream));
    if (!source || !dest) {
        cleanup_stream(source);
        cleanup_stream(dest);
        return 0;
    }

    uint8_t *out_buf = (uint8_t *)malloc(1024);
    if (!out_buf) {
        cleanup_stream(source);
        cleanup_stream(dest);
        return 0;
    }

    source->next_out = out_buf;
    source->avail_out = 1024;
    source->zalloc = Z_NULL;
    source->zfree = Z_NULL;
    source->opaque = Z_NULL;

    if (inflateInit2(source, -15) != Z_OK) {
        cleanup_stream(source);
        cleanup_stream(dest);
        free(out_buf);
        return 0;
    }

    if (Size > 0) {
        source->next_in = Data;
        source->avail_in = Size;
        inflate(source, Z_NO_FLUSH);
    }

    int ret = inflateCopy(dest, source);
    cleanup_stream(source);
    cleanup_stream(dest);
    free(out_buf);
    return ret;
}

static int fuzz_deflateSetHeader(const uint8_t *Data, size_t Size) {
    z_stream *strm = (z_stream *)calloc(1, sizeof(z_stream));
    if (!strm) return 0;

    gz_header head;
    memset(&head, 0, sizeof(head));
    head.text = 1;
    head.time = 1234567890;
    head.xflags = 0;
    head.os = 3;
    head.extra = NULL;
    head.extra_len = 0;
    head.extra_max = 0;
    head.name = (Bytef *)"test_file";
    head.name_max = 10;
    head.comment = (Bytef *)"test_comment";
    head.comm_max = 20;
    head.hcrc = 1;
    head.done = 0;

    uint8_t *out_buf = (uint8_t *)malloc(1024);
    if (!out_buf) {
        cleanup_stream(strm);
        return 0;
    }

    strm->next_out = out_buf;
    strm->avail_out = 1024;
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;

    if (deflateInit2(strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 16 + 15, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        cleanup_stream(strm);
        free(out_buf);
        return 0;
    }

    int ret = deflateSetHeader(strm, &head);
    cleanup_stream(strm);
    free(out_buf);
    return ret;
}

static int fuzz_inflatePrime(const uint8_t *Data, size_t Size) {
    z_stream *strm = (z_stream *)calloc(1, sizeof(z_stream));
    if (!strm) return 0;

    uint8_t *out_buf = (uint8_t *)malloc(1024);
    if (!out_buf) {
        cleanup_stream(strm);
        return 0;
    }

    strm->next_out = out_buf;
    strm->avail_out = 1024;
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;

    if (inflateInit2(strm, -15) != Z_OK) {
        cleanup_stream(strm);
        free(out_buf);
        return 0;
    }

    int bits = 0;
    int value = 0;
    if (Size >= 2) {
        bits = Data[0] % 17;
        value = Data[1];
    } else if (Size == 1) {
        bits = Data[0] % 17;
    }

    int ret = inflatePrime(strm, bits, value);
    cleanup_stream(strm);
    free(out_buf);
    return ret;
}

static int fuzz_inflateReset(const uint8_t *Data, size_t Size) {
    z_stream *strm = (z_stream *)calloc(1, sizeof(z_stream));
    if (!strm) return 0;

    uint8_t *out_buf = (uint8_t *)malloc(1024);
    if (!out_buf) {
        cleanup_stream(strm);
        return 0;
    }

    strm->next_out = out_buf;
    strm->avail_out = 1024;
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;

    if (inflateInit2(strm, -15) != Z_OK) {
        cleanup_stream(strm);
        free(out_buf);
        return 0;
    }

    if (Size > 0) {
        strm->next_in = Data;
        strm->avail_in = Size;
        inflate(strm, Z_NO_FLUSH);
    }

    int ret = inflateReset(strm);
    cleanup_stream(strm);
    free(out_buf);
    return ret;
}

static int fuzz_deflateReset(const uint8_t *Data, size_t Size) {
    z_stream *strm = (z_stream *)calloc(1, sizeof(z_stream));
    if (!strm) return 0;

    uint8_t *out_buf = (uint8_t *)malloc(1024);
    if (!out_buf) {
        cleanup_stream(strm);
        return 0;
    }

    strm->next_out = out_buf;
    strm->avail_out = 1024;
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;

    if (deflateInit2(strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 16 + 15, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        cleanup_stream(strm);
        free(out_buf);
        return 0;
    }

    if (Size > 0) {
        strm->next_in = Data;
        strm->avail_in = Size;
        deflate(strm, Z_NO_FLUSH);
    }

    int ret = deflateReset(strm);
    cleanup_stream(strm);
    free(out_buf);
    return ret;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    fuzz_inflateGetHeader(Data, Size);
    fuzz_inflateCopy(Data, Size);
    fuzz_deflateSetHeader(Data, Size);
    fuzz_inflatePrime(Data, Size);
    fuzz_inflateReset(Data, Size);
    fuzz_deflateReset(Data, Size);
    return 0;
}