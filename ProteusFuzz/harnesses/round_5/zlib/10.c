// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateBack at infback.c:191:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

/* Callbacks for inflateBack - renamed to avoid conflict with zlib typedefs */
static unsigned my_in_func(void *desc, z_const unsigned char **buf) {
    struct {
        const unsigned char *data;
        size_t size;
        size_t pos;
    } *ctx = (struct {
        const unsigned char *data;
        size_t size;
        size_t pos;
    } *)desc;

    if (ctx->pos >= ctx->size) {
        *buf = Z_NULL;
        return 0;
    }
    *buf = ctx->data + ctx->pos;
    /* Return remaining bytes, capped at a reasonable chunk size */
    size_t avail = ctx->size - ctx->pos;
    if (avail > 1024) avail = 1024;
    ctx->pos += avail;
    return (unsigned)avail;
}

static int my_out_func(void *desc, unsigned char *buf, unsigned len) {
    /* Discard output for fuzzing purposes */
    (void)desc;
    (void)buf;
    (void)len;
    return 0;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Buffer for inflateBack window */
    unsigned char window[32768];
    memset(window, 0, sizeof(window));

    /* Context for inflateBack input callback */
    struct {
        const unsigned char *data;
        size_t size;
        size_t pos;
    } in_ctx = { Data, Size, 0 };

    /* First sequence: inflateInit2_ -> inflate -> inflateEnd */
    {
        z_stream strm = {0};
        int ret;

        /* Initialize with various window bits to explore states */
        int windowBits = 15;
        if (Size > 0) windowBits = (Data[0] % 16) + 8; /* 8 to 15 */
        if (windowBits < 8 || windowBits > 15) windowBits = 15;

        ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
        if (ret != Z_OK) {
            /* Initialization failed, try to cleanup just in case */
            inflateEnd(&strm);
            return 0;
        }

        /* Prepare input/output buffers */
        strm.next_in = Data;
        strm.avail_in = (uInt)Size;
        strm.next_out = window;
        strm.avail_out = sizeof(window);

        /* Perform inflation */
        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_OK || ret == Z_STREAM_END) {
            /* Try another call to explore more states */
            ret = inflate(&strm, Z_SYNC_FLUSH);
        }
        if (ret == Z_OK || ret == Z_STREAM_END) {
            ret = inflate(&strm, Z_FINISH);
        }

        /* Cleanup */
        inflateEnd(&strm);
    }

    /* Second sequence: inflateBackInit_ -> inflateBack -> inflateBackEnd */
    {
        z_stream strm = {0};
        int ret;
        int windowBits = 15;
        if (Size > 1) windowBits = (Data[1] % 16) + 8; /* 8 to 15 */
        if (windowBits < 8 || windowBits > 15) windowBits = 15;

        ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, (int)sizeof(z_stream));
        if (ret != Z_OK) {
            return 0;
        }

        /* Perform back-end inflation */
        ret = inflateBack(&strm, my_in_func, &in_ctx, my_out_func, NULL);

        /* Cleanup */
        inflateBackEnd(&strm);
    }

    return 0;
}