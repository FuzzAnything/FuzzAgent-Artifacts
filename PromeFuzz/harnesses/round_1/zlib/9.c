// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateBack at infback.c:191:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

/* Callback descriptor structure */
struct callback_desc {
    const uint8_t *data;
    size_t size;
    size_t pos;
};

/* Callback functions for inflateBack */
static unsigned in_callback(void FAR *desc, unsigned char FAR **buf) {
    struct callback_desc *d = (struct callback_desc *)desc;
    
    if (d->pos >= d->size) {
        return 0; /* No more data */
    }
    
    *buf = (unsigned char FAR *)&d->data[d->pos];
    unsigned available = (unsigned)(d->size - d->pos);
    d->pos = d->size; /* Consume all remaining data */
    return available;
}

static int out_callback(void FAR *desc, unsigned char FAR *buf, unsigned len) {
    /* Discard output */
    (void)desc;
    (void)buf;
    (void)len;
    return 0;
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    z_stream strm1, strm2;
    int ret;
    unsigned char window[32768]; /* 32KB window for inflateBack */
    
    /* Initialize first stream for inflate */
    memset(&strm1, 0, sizeof(strm1));
    strm1.zalloc = Z_NULL;
    strm1.zfree = Z_NULL;
    strm1.opaque = Z_NULL;
    strm1.next_in = (Bytef *)Data;
    strm1.avail_in = (uInt)(Size > UINT_MAX ? UINT_MAX : Size);
    
    /* Initialize with random window bits (allow raw inflate with negative values) */
    int windowBits1 = (Size > 0) ? (Data[0] % 48) - 24 : 15; /* Range: -24 to 23 */
    
    ret = inflateInit2_(&strm1, windowBits1, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        /* Initialization failed, clean up and return */
        inflateEnd(&strm1);
        return 0;
    }
    
    /* Prepare output buffer for inflate */
    unsigned char out_buf[4096];
    strm1.next_out = out_buf;
    strm1.avail_out = sizeof(out_buf);
    
    /* Call inflate with various flush modes */
    int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FINISH, Z_BLOCK};
    for (int i = 0; i < 4 && ret >= 0; i++) {
        ret = inflate(&strm1, flush_modes[i % 4]);
        if (ret == Z_STREAM_END || ret == Z_NEED_DICT || ret < 0) {
            break;
        }
        /* Reset output buffer if full */
        if (strm1.avail_out == 0) {
            strm1.next_out = out_buf;
            strm1.avail_out = sizeof(out_buf);
        }
    }
    
    /* Clean up first stream */
    inflateEnd(&strm1);
    
    /* Initialize second stream for inflateBack */
    memset(&strm2, 0, sizeof(strm2));
    strm2.zalloc = Z_NULL;
    strm2.zfree = Z_NULL;
    strm2.opaque = Z_NULL;
    
    /* Use different window bits for inflateBack (must be 8-15) */
    int windowBits2 = (Size > 1) ? (Data[1] % 8) + 8 : 15;
    
    ret = inflateBackInit_(&strm2, windowBits2, window, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    /* Prepare callback descriptor */
    struct callback_desc in_desc;
    in_desc.data = Data;
    in_desc.size = Size;
    in_desc.pos = 0;
    
    /* Call inflateBack */
    ret = inflateBack(&strm2, in_callback, &in_desc, out_callback, NULL);
    
    /* Clean up second stream */
    inflateBackEnd(&strm2);
    
    return 0;
}