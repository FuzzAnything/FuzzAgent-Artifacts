// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void initialize_stream(z_streamp strm, const uint8_t *data, size_t size) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = (Bytef *)data;
    strm->avail_in = (uInt)size;
    strm->next_out = Z_NULL;
    strm->avail_out = 0;
    strm->total_in = 0;
    strm->total_out = 0;
    strm->msg = Z_NULL;
    strm->data_type = 0;
    strm->adler = 0;
    strm->reserved = 0;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    z_stream strm;
    int windowBits, flush, bits, value, check;
    int ret;
    Bytef out_buffer[4096];

    /* Initialize stream with input data */
    initialize_stream(&strm, Data, Size);
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);

    /* Extract parameters from input data */
    windowBits = (Data[0] % 32) - 16;  /* Range: -16 to 15 */
    flush = Data[0] % 4;               /* 0:Z_NO_FLUSH, 1:Z_SYNC_FLUSH, 2:Z_FULL_FLUSH, 3:Z_FINISH */
    bits = Data[0] % 17;               /* 0-16 bits */
    value = Data[0];
    check = Data[0] % 2;               /* 0 or 1 */

    /* Test inflateInit2_ */
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }

    /* Test inflatePrime */
    if (Size > 1) {
        inflatePrime(&strm, bits, value);
    }

    /* Test inflateValidate */
    inflateValidate(&strm, check);

    /* Test inflate */
    inflate(&strm, flush);

    /* Test inflateSync if we got a data error */
    if (ret == Z_DATA_ERROR) {
        inflateSync(&strm);
    }

    /* Test inflateResetKeep */
    if (Size > 2) {
        /* Reset input/output for another round */
        strm.next_in = (Bytef *)Data + 1;
        strm.avail_in = (uInt)(Size - 1);
        strm.next_out = out_buffer;
        strm.avail_out = sizeof(out_buffer);
        
        inflateResetKeep(&strm);
        
        /* Try inflate again after reset */
        inflate(&strm, flush % 2);
    }

    /* Cleanup */
    inflateEnd(&strm);

    return 0;
}