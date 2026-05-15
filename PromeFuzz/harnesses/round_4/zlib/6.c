// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBack at infback.c:191:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Callback functions for inflateBack */
static unsigned in_callback(void FAR *in_desc, unsigned char FAR * FAR *buf) {
    /* Provide fuzzer data as input */
    static unsigned char dummy_input[1] = {0};
    *buf = dummy_input;
    return 0; /* No more input */
}

static int out_callback(void FAR *out_desc, unsigned char FAR *buf, unsigned len) {
    /* Discard output */
    (void)out_desc;
    (void)buf;
    (void)len;
    return 0; /* Success */
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    z_stream strm;
    int ret;
    unsigned char window[32768]; /* 32KB window for inflateBack */

    /* Initialize strm */
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    /* Set up input data */
    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)(Size > UINT_MAX ? UINT_MAX : Size);
    
    /* 1. inflateInit2_ */
    ret = inflateInit2_(&strm, 15, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        /* If initialization fails, skip further steps but clean up if needed */
        if (ret == Z_MEM_ERROR || ret == Z_STREAM_ERROR) {
            return 0;
        }
    }
    
    /* 2. inflate */
    /* Provide a small output buffer */
    unsigned char out_buf[1024];
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);
    
    ret = inflate(&strm, Z_NO_FLUSH);
    /* Ignore return value - continue anyway */
    
    /* 3. inflateEnd */
    ret = inflateEnd(&strm);
    /* Ignore return value */
    
    /* Reset strm for inflateBack */
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    /* 4. inflateBackInit_ */
    ret = inflateBackInit_(&strm, 15, window, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        if (ret == Z_MEM_ERROR || ret == Z_STREAM_ERROR) {
            return 0;
        }
    }
    
    /* 5. inflateBack */
    ret = inflateBack(&strm, in_callback, NULL, out_callback, NULL);
    /* Ignore return value */
    
    /* 6. inflateBackEnd */
    ret = inflateBackEnd(&strm);
    /* Ignore return value */
    
    return 0;
}