// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBack at infback.c:191:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

/* Custom input function for inflateBack */
static unsigned read_call(void *desc, unsigned char **buf) {
    struct {
        const uint8_t *data;
        size_t size;
        size_t offset;
    } *state = (struct {
        const uint8_t *data;
        size_t size;
        size_t offset;
    } *)desc;

    if (state == NULL || state->data == NULL) {
        *buf = NULL;
        return 0;
    }

    if (state->offset >= state->size) {
        *buf = NULL;
        return 0;
    }

    /* Return a chunk of data, max 1024 bytes at a time */
    size_t len = state->size - state->offset;
    if (len > 1024) len = 1024;
    
    *buf = (unsigned char *)(state->data + state->offset);
    state->offset += len;
    return (unsigned)len;
}

/* Custom output function for inflateBack */
static int write_call(void *desc, unsigned char *buf, unsigned len) {
    (void)desc;
    (void)buf;
    (void)len;
    /* Just discard the output for fuzzing purposes */
    return 0;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    z_stream strm = {0};
    unsigned char window[32768];
    int ret;
    int windowBits;

    /* Extract windowBits from input, ensure it is in valid range for testing */
    windowBits = (int)Data[0];
    if (windowBits < -15 || windowBits > 15) windowBits = 15;

    /* Allocate output buffer for inflate */
    uint8_t *out_buf = malloc(Size * 2 + 256);
    if (!out_buf) return 0;

    /* Prepare state for inflateBack */
    struct {
        const uint8_t *data;
        size_t size;
        size_t offset;
    } in_state = {Data, Size, 0};

    /* Test inflateInit2_ */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        /* Test inflateReset2 */
        ret = inflateReset2(&strm, -windowBits);
        if (ret != Z_STREAM_ERROR) {
            /* Test inflate with some data */
            if (Size > 0) {
                strm.next_in = Data;
                strm.avail_in = (uInt)Size;
                strm.next_out = out_buf;
                strm.avail_out = (uInt)(Size * 2 + 256);
                ret = inflate(&strm, Z_NO_FLUSH);
                (void)ret;
            }
            inflateEnd(&strm);
        }
    }

    /* Test inflateBackInit_ */
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        /* Test inflateBack */
        in_state.offset = 0;
        ret = inflateBack(&strm, read_call, &in_state, write_call, NULL);
        (void)ret;

        /* Test inflateBackEnd */
        ret = inflateBackEnd(&strm);
        (void)ret;
    }

    /* Test inflateBackEnd with null stream */
    ret = inflateBackEnd(NULL);
    (void)ret;

    /* Test inflateReset2 with invalid state (uninitialized) */
    memset(&strm, 0, sizeof(strm));
    ret = inflateReset2(&strm, 15);
    (void)ret;

    /* Test inflate with uninitialized stream */
    memset(&strm, 0, sizeof(strm));
    if (Size > 0) {
        strm.next_in = Data;
        strm.avail_in = (uInt)Size;
        strm.next_out = out_buf;
        strm.avail_out = (uInt)(Size * 2 + 256);
        ret = inflate(&strm, Z_NO_FLUSH);
        (void)ret;
    }

    free(out_buf);
    return 0;
}