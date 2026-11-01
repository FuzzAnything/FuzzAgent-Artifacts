// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

/* Helper to safely initialize an inflate stream with GZIP mode */
static int init_inflate_gzip(z_stream *strm) {
    if (strm == NULL) return Z_STREAM_ERROR;
    memset(strm, 0, sizeof(z_stream));
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    return inflateInit2(strm, 16 + MAX_WBITS);
}

/* Helper to safely initialize a deflate stream with GZIP mode */
static int init_deflate_gzip(z_stream *strm) {
    if (strm == NULL) return Z_STREAM_ERROR;
    memset(strm, 0, sizeof(z_stream));
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    return deflateInit2(strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 16 + MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;

    z_stream source_deflate, dest_deflate;
    z_stream source_inflate, dest_inflate;
    gz_header head;
    int ret;
    int bits, value;

    /* Parse input for bits and value for inflatePrime safely */
    bits = (Data[0] % 17) * (Data[1] % 2 ? 1 : -1); 
    if (bits > 16) bits = 16;
    if (bits < -16) bits = -16;
    value = (int)(Data[2] % 65536);

    /* --- Test inflateGetHeader --- */
    /* Must be called before inflateInit or inflateReset */
    if (init_inflate_gzip(&source_inflate) == Z_OK) {
        memset(&head, 0, sizeof(head));
        /* Try with valid stream */
        ret = inflateGetHeader(&source_inflate, &head);
        (void)ret;

        /* Try with NULL stream (should fail) */
        ret = inflateGetHeader(NULL, &head);
        (void)ret;

        /* Try with NULL head - This caused a crash in zlib internals. 
           We must ensure head is valid or skip this specific invalid call if it's known to be unsafe.
           However, the API expects a valid pointer. Passing NULL is an invalid usage that causes UB in zlib.
           To fuzz safely, we only test valid inputs or inputs that return error codes gracefully.
           Since zlib's inflateGetHeader dereferences head immediately, we cannot pass NULL.
           We will skip the NULL head test to prevent the crash.
        */
        // ret = inflateGetHeader(&source_inflate, NULL); // Removed to prevent SEGV

        inflateEnd(&source_inflate);
    }

    /* --- Test inflateCopy --- */
    if (init_inflate_gzip(&source_inflate) == Z_OK) {
        memset(&dest_inflate, 0, sizeof(dest_inflate));
        ret = inflateCopy(&dest_inflate, &source_inflate);
        if (ret == Z_OK) {
            inflateEnd(&dest_inflate);
        }

        /* Try with NULL dest */
        ret = inflateCopy(NULL, &source_inflate);
        (void)ret;

        /* Try with NULL source */
        memset(&dest_inflate, 0, sizeof(dest_inflate));
        ret = inflateCopy(&dest_inflate, NULL);
        (void)ret;

        inflateEnd(&source_inflate);
    }

    /* --- Test deflateSetHeader --- */
    if (init_deflate_gzip(&source_deflate) == Z_OK) {
        memset(&head, 0, sizeof(head));
        head.text = 1;
        head.time = 1234567890;
        head.xflags = 0;
        head.os = 3;
        head.extra = NULL;
        head.extra_len = 0;
        head.name = NULL;
        head.name_max = 0;
        head.comment = NULL;
        head.comm_max = 0;
        head.hcrc = 1;
        head.done = 0;

        ret = deflateSetHeader(&source_deflate, &head);
        (void)ret;

        /* Try with NULL stream */
        ret = deflateSetHeader(NULL, &head);
        (void)ret;

        /* Try with NULL head - Similar to inflateGetHeader, this may cause issues if dereferenced.
           Checking zlib source, deflateSetHeader checks for NULL head and returns Z_STREAM_ERROR.
           So this is safe to call.
        */
        ret = deflateSetHeader(&source_deflate, NULL);
        (void)ret;

        deflateEnd(&source_deflate);
    }

    /* --- Test inflatePrime --- */
    if (init_inflate_gzip(&source_inflate) == Z_OK) {
        ret = inflatePrime(&source_inflate, bits, value);
        (void)ret;

        /* Try with NULL stream */
        ret = inflatePrime(NULL, bits, value);
        (void)ret;

        inflateEnd(&source_inflate);
    }

    /* --- Test inflateReset --- */
    if (init_inflate_gzip(&source_inflate) == Z_OK) {
        ret = inflateReset(&source_inflate);
        (void)ret;

        /* Try with NULL stream */
        ret = inflateReset(NULL);
        (void)ret;

        inflateEnd(&source_inflate);
    }

    /* --- Test deflateReset --- */
    if (init_deflate_gzip(&source_deflate) == Z_OK) {
        ret = deflateReset(&source_deflate);
        (void)ret;

        /* Try with NULL stream */
        ret = deflateReset(NULL);
        (void)ret;

        deflateEnd(&source_deflate);
    }

    return 0;
}