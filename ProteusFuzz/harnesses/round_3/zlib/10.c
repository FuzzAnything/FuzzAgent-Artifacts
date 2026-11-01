// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBack at infback.c:191:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

/* Context structure for inflateBack callbacks */
typedef struct {
    const unsigned char *data;
    size_t size;
    size_t offset;
} InCtx;

/* Custom input function for inflateBack */
static unsigned int dummy_in_func(void FAR *desc, unsigned char FAR **buf) {
    InCtx *ctx = (InCtx *)desc;

    if (ctx->offset >= ctx->size) {
        *buf = Z_NULL;
        return 0;
    }
    *buf = (unsigned char FAR *)(ctx->data + ctx->offset);
    ctx->offset += 1;
    return 1;
}

/* Custom output function for inflateBack */
static int dummy_out_func(void FAR *desc, unsigned char FAR *buf, unsigned int len) {
    /* Do nothing, just discard output */
    (void)desc;
    (void)buf;
    (void)len;
    return 0;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    unsigned char window[32768];
    int ret;
    int windowBits = 15; /* Default window bits */
    
    /* Ensure minimum size for meaningful testing */
    if (Size < 1) {
        return 0;
    }

    /* Try different window bits based on input if possible */
    if (Size > 0) {
        windowBits = (Data[0] % 8) + 8; /* Range 8-15 */
        if (windowBits < 8) windowBits = 8;
        if (windowBits > 15) windowBits = 15;
    }

    /* 1. inflateInit2_ */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }

    /* Prepare input for inflate */
    if (Size > 0) {
        strm.next_in = (const unsigned char *)Data;
        strm.avail_in = Size;
    }

    /* Prepare output buffer for inflate */
    unsigned char out_buf[4096];
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    /* 2. inflate */
    ret = inflate(&strm, Z_NO_FLUSH);
    /* Try another call with Z_SYNC_FLUSH if first was Z_OK */
    if (ret == Z_OK && strm.avail_in > 0) {
        ret = inflate(&strm, Z_SYNC_FLUSH);
    }
    /* Try Z_FINISH */
    if (ret == Z_OK || ret == Z_STREAM_END) {
        ret = inflate(&strm, Z_FINISH);
    }

    /* 3. inflateEnd */
    ret = inflateEnd(&strm);
    (void)ret; /* Ignore return for fuzzing, just ensure no crash */

    /* Re-initialize for inflateBack */
    memset(&strm, 0, sizeof(z_stream));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    /* 4. inflateBackInit_ */
    ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }

    /* Prepare context for inflateBack callbacks */
    InCtx in_ctx = {0};
    if (Size > 0) {
        in_ctx.data = Data;
        in_ctx.size = Size;
        in_ctx.offset = 0;
    }

    /* 5. inflateBack */
    ret = inflateBack(&strm, dummy_in_func, (void *)&in_ctx, dummy_out_func, Z_NULL);
    (void)ret; /* Ignore return, just ensure no crash */

    /* 6. inflateBackEnd */
    ret = inflateBackEnd(&strm);
    (void)ret;

    return 0;
}