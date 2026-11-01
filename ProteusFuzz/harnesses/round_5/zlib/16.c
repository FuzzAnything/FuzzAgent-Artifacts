// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBack at infback.c:191:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

/* Callback functions for inflateBack - renamed to avoid conflict with typedefs in zlib.h */
static unsigned my_in_func(void *desc, z_const unsigned char **buf) {
    unsigned long *pos = (unsigned long *)desc;
    static unsigned char dummy_buf[256] = {0};
    
    if (*pos < 256) {
        *buf = dummy_buf + *pos;
        *pos += 16;
        return 16;
    }
    *buf = NULL;
    return 0;
}

static int my_out_func(void *desc, unsigned char *buf, unsigned len) {
    /* Discard output for fuzzing */
    (void)desc;
    (void)buf;
    (void)len;
    return 0;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    z_stream strm = {0};
    unsigned char window[32768];
    int ret;
    int windowBits;
    int bits;
    int value;

    /* Initialize stream */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    strm.next_out = Z_NULL;
    strm.avail_out = 0;

    /* Extract parameters from input if available */
    if (Size >= 1) {
        windowBits = Data[0] % 32 - 16; /* Range -16 to 15 */
    } else {
        windowBits = 15;
    }
    
    if (Size >= 2) {
        bits = Data[1] % 17; /* Range 0 to 16 */
    } else {
        bits = 8;
    }
    
    if (Size >= 3) {
        value = Data[2];
    } else {
        value = 0;
    }

    /* Test inflateInit2_ */
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        /* Test inflatePrime */
        inflatePrime(&strm, bits, value);
        
        /* Test inflateReset2 with various windowBits */
        int resetBits = (Size >= 4) ? (Data[3] % 32 - 16) : 15;
        inflateReset2(&strm, resetBits);
        
        /* Cleanup */
        inflateEnd(&strm);
    }

    /* Re-initialize for inflateBack tests */
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    /* Test inflateBackInit_ */
    ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        unsigned long in_pos = 0;
        /* Test inflateBack with renamed callbacks */
        ret = inflateBack(&strm, my_in_func, &in_pos, my_out_func, NULL);
        
        /* Test inflateBackEnd */
        inflateBackEnd(&strm);
    }

    /* Test edge cases with invalid states */
    memset(&strm, 0, sizeof(strm));
    /* Call inflateBackEnd on uninitialized stream */
    inflateBackEnd(&strm);
    
    /* Call inflatePrime on uninitialized stream */
    inflatePrime(&strm, 8, 0);
    
    /* Call inflateReset2 on uninitialized stream */
    inflateReset2(&strm, 15);

    return 0;
}