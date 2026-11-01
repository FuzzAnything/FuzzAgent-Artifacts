// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateBack at infback.c:191:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

/* Callbacks for inflateBack - renamed to avoid conflict with zlib typedefs */
static unsigned my_in_func(void *desc, z_const unsigned char **buf) {
    struct {
        const uint8_t *data;
        size_t size;
        size_t offset;
    } *state = (struct {
        const uint8_t *data;
        size_t size;
        size_t offset;
    } *)desc;

    if (state->offset >= state->size) {
        *buf = Z_NULL;
        return 0;
    }
    *buf = state->data + state->offset;
    size_t available = state->size - state->offset;
    if (available > 1024) available = 1024;
    state->offset += available;
    return (unsigned)available;
}

static int my_out_func(void *desc, unsigned char *buf, unsigned len) {
    /* Just discard output for fuzzing */
    (void)desc;
    (void)buf;
    (void)len;
    return 0;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Part 1: inflateInit2_ -> inflate -> inflateEnd */
    z_stream strm = {0};
    unsigned char out_buf[4096];
    int ret;

    /* Initialize with random window bits if possible, else default */
    int windowBits = (Size > 0) ? (Data[0] % 16) : 15;
    if (windowBits < 8) windowBits = 8;

    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        /* Initialization failed, try to proceed or skip */
        if (ret == Z_VERSION_ERROR || ret == Z_STREAM_ERROR || ret == Z_MEM_ERROR) {
            /* Expected error cases, continue to cleanup if needed */
            inflateEnd(&strm);
            return 0;
        }
    }

    /* Setup input */
    strm.next_in = Data;
    strm.avail_in = Size;
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    /* Call inflate */
    ret = inflate(&strm, Z_NO_FLUSH);
    if (ret != Z_STREAM_END && ret != Z_OK && ret != Z_DATA_ERROR && ret != Z_NEED_DICT) {
        /* Handle other errors */
    }

    /* Cleanup inflate */
    inflateEnd(&strm);

    /* Part 2: inflateBackInit_ -> inflateBack -> inflateBackEnd */
    z_stream strm_back = {0};
    unsigned char window[32768];
    struct {
        const uint8_t *data;
        size_t size;
        size_t offset;
    } in_state = {Data, Size, 0};

    int windowBits_back = (Size > 1) ? (Data[1] % 8) + 8 : 15;
    if (windowBits_back < 8) windowBits_back = 8;
    if (windowBits_back > 15) windowBits_back = 15;

    ret = inflateBackInit_(&strm_back, windowBits_back, window, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        /* Initialization failed */
        if (ret == Z_VERSION_ERROR || ret == Z_STREAM_ERROR || ret == Z_MEM_ERROR) {
            inflateBackEnd(&strm_back);
            return 0;
        }
    }

    /* Call inflateBack */
    ret = inflateBack(&strm_back, my_in_func, &in_state, my_out_func, NULL);
    if (ret != Z_STREAM_END && ret != Z_DATA_ERROR && ret != Z_BUF_ERROR) {
        /* Handle other errors */
    }

    /* Cleanup inflateBack */
    inflateBackEnd(&strm_back);

    return 0;
}