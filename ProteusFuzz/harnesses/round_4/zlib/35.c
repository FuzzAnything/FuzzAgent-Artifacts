// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateBack at infback.c:191:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

/* Callback for inflateBack input */
static unsigned my_in_func(void *desc, z_const unsigned char **buf) {
    unsigned char *data = (unsigned char *)desc;
    *buf = data;
    return 1; /* Return 1 to indicate data is available, 0 for end */
}

/* Callback for inflateBack output */
static int my_out_func(void *desc, unsigned char *buf, unsigned len) {
    /* Just discard output for fuzzing purposes */
    (void)desc;
    (void)buf;
    (void)len;
    return 0;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    z_stream strm = {0};
    unsigned char window[32768];
    int ret;

    /* Initialize stream with custom allocators if needed, but defaults are fine */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    /* Test inflateBackInit_ */
    int windowBits = (Data[0] % 8) + 8; /* Range 8-15 */
    ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        /* Initialization failed, try standard inflate path */
        ret = inflateInit(&strm);
        if (ret != Z_OK) return 0;

        /* Test inflateReset2 */
        int resetBits = (Data[1] % 8) + 8;
        if (Data[1] & 0x80) resetBits = -resetBits; /* Negative for raw deflate */
        inflateReset2(&strm, resetBits);

        /* Test inflatePrime */
        int bits = Data[2] % 17; /* 0-16 */
        int value = Data[3];
        if (Data[2] & 0x80) bits = -bits; /* Negative to clear buffer */
        inflatePrime(&strm, bits, value);

        /* Test inflate with fuzz data */
        if (Size > 4) {
            strm.next_in = (const Bytef *)Data + 4;
            strm.avail_in = Size - 4;
            unsigned char out_buf[4096];
            strm.next_out = out_buf;
            strm.avail_out = sizeof(out_buf);

            int flush = Data[0] % 4; /* Z_NO_FLUSH, Z_PARTIAL_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH */
            if (Data[0] & 0x80) flush = Z_FINISH;
            if (Data[0] & 0x40) flush = Z_BLOCK;

            inflate(&strm, flush);
        }

        inflateEnd(&strm);
        return 0;
    }

    /* If inflateBackInit_ succeeded, test inflateBack */
    if (Size > 4) {
        /* Prepare input for inflateBack */
        unsigned char input_data[Size - 4];
        memcpy(input_data, Data + 4, Size - 4);
        
        /* Use the input data as the descriptor for the in_func */
        ret = inflateBack(&strm, my_in_func, input_data, my_out_func, NULL);
        (void)ret; /* Ignore return for fuzzing, just ensure no crash */
    }

    /* Test inflateBackEnd */
    inflateBackEnd(&strm);

    return 0;
}