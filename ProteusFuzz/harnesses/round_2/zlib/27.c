// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    z_stream copy_strm = {0};
    gz_header header;
    uint8_t *in_buf = NULL;
    uint8_t *out_buf = NULL;
    int ret;

    if (Size < 1) {
        return 0;
    }

    /* Allocate buffers for input and output */
    size_t in_size = Size;
    size_t out_size = Size * 2 + 256;
    in_buf = (uint8_t *)malloc(in_size);
    out_buf = (uint8_t *)malloc(out_size);
    if (!in_buf || !out_buf) {
        free(in_buf);
        free(out_buf);
        return 0;
    }
    memcpy(in_buf, Data, in_size);

    /* Initialize the stream */
    if (inflateInit(&strm) != Z_OK) {
        free(in_buf);
        free(out_buf);
        return 0;
    }

    /* Prepare input and output pointers */
    strm.next_in = in_buf;
    strm.avail_in = in_size;
    strm.next_out = out_buf;
    strm.avail_out = out_size;

    /* Fuzz inflateGetHeader */
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&strm, &header);

    /* Fuzz inflateResetKeep */
    inflateResetKeep(&strm);

    /* Fuzz inflateReset2 with various windowBits derived from input */
    int windowBits = (Data[0] % 32) - 16; /* Range: -16 to 15 */
    if (windowBits < -15 || windowBits > 15) {
        windowBits = 15;
    }
    inflateReset2(&strm, windowBits);

    /* Fuzz inflateUndermine */
    int subvert = (Data[0] & 1);
    inflateUndermine(&strm, subvert);

    /* Fuzz inflateValidate */
    int check = (Data[0] & 1);
    inflateValidate(&strm, check);

    /* Fuzz inflateCopy */
    if (inflateCopy(&copy_strm, &strm) == Z_OK) {
        /* Perform a decompression step on the copy to exercise state */
        copy_strm.next_in = in_buf;
        copy_strm.avail_in = in_size;
        copy_strm.next_out = out_buf;
        copy_strm.avail_out = out_size;
        inflate(&copy_strm, Z_NO_FLUSH);
        inflateEnd(&copy_strm);
    }

    /* Perform a decompression step on the main stream */
    inflate(&strm, Z_NO_FLUSH);

    /* Cleanup */
    inflateEnd(&strm);
    free(in_buf);
    free(out_buf);

    return 0;
}