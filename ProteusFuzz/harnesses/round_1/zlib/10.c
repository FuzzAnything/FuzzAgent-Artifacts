// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBack at infback.c:191:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

/* Callback functions for inflateBack with unique names to avoid conflicts */
static unsigned my_in_func(void FAR *desc, unsigned char FAR **buf) {
    struct {
        const unsigned char *data;
        size_t size;
        size_t offset;
    } *d = (struct {
        const unsigned char *data;
        size_t size;
        size_t offset;
    } *)desc;

    if (d->offset >= d->size) {
        *buf = Z_NULL;
        return 0;
    }
    *buf = (unsigned char FAR *)(d->data + d->offset);
    d->offset += 1024; /* Read in chunks */
    if (d->offset > d->size) d->offset = d->size;
    return 1;
}

static int my_out_func(void FAR *desc, unsigned char FAR *buf, unsigned len) {
    /* Just discard output for fuzzing purposes */
    (void)desc;
    (void)buf;
    (void)len;
    return 0;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    unsigned char window[32768];
    unsigned char output[4096];
    int ret;
    int windowBits;

    if (Size < 2) {
        return 0;
    }

    /* Derive windowBits from input to explore different states */
    windowBits = (Data[0] % 16) + 8;
    if (windowBits < 8) windowBits = 8;
    if (windowBits > 15) windowBits = 15;

    /* Part 1: inflateInit2_ -> inflate -> inflateEnd */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    strm.avail_out = 0;
    strm.next_out = Z_NULL;

    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }

    /* Feed input data */
    if (Size > 0) {
        strm.next_in = (const unsigned char FAR *)Data;
        strm.avail_in = (uInt)Size;
    }
    strm.next_out = output;
    strm.avail_out = sizeof(output);

    /* Call inflate multiple times to explore states */
    for (int i = 0; i < 3; i++) {
        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            break;
        }
        if (ret == Z_OK && strm.avail_out == 0) {
            /* Reset output buffer to continue */
            strm.next_out = output;
            strm.avail_out = sizeof(output);
        }
        if (ret < 0) {
            /* Error occurred, try to continue or break */
            break;
        }
    }

    ret = inflateEnd(&strm);
    (void)ret; /* Ignore return for fuzzing, just ensure cleanup */

    /* Part 2: inflateBackInit_ -> inflateBack -> inflateBackEnd */
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    strm.avail_out = 0;
    strm.next_out = Z_NULL;

    /* Reuse windowBits, but ensure valid range for inflateBack */
    if (windowBits < 8) windowBits = 8;
    if (windowBits > 15) windowBits = 15;

    ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }

    /* Setup opaque for callbacks */
    struct {
        const unsigned char *data;
        size_t size;
        size_t offset;
    } in_desc = {(const unsigned char *)Data, Size, 0};
    strm.opaque = &in_desc;

    /* Call inflateBack */
    ret = inflateBack(&strm, my_in_func, &in_desc, my_out_func, Z_NULL);
    (void)ret;

    ret = inflateBackEnd(&strm);
    (void)ret;

    return 0;
}