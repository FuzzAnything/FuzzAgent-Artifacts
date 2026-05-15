// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    z_stream strm;
    z_stream strm_copy;
    gz_header header;
    int ret;
    Bytef *out_buffer;
    uInt out_size = 4096;
    const Bytef *dict;
    uInt dict_len;
    int windowBits = 15 + 32; /* Enable gzip decoding */

    /* Initialize output buffer */
    out_buffer = (Bytef *)malloc(out_size);
    if (!out_buffer) return 0;

    /* Initialize stream structure */
    memset(&strm, 0, sizeof(z_stream));
    memset(&strm_copy, 0, sizeof(z_stream));
    memset(&header, 0, sizeof(gz_header));

    /* Set up input data */
    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)(Size > UINT_MAX ? UINT_MAX : Size);
    strm.next_out = out_buffer;
    strm.avail_out = out_size;

    /* Initialize decompression */
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buffer);
        return 0;
    }

    /* Set up header retrieval */
    ret = inflateGetHeader(&strm, &header);
    /* Continue regardless of return value */

    /* First inflate call */
    ret = inflate(&strm, Z_NO_FLUSH);
    /* Continue regardless of return value */

    /* Prepare dictionary data (use part of input if available) */
    if (Size > 0) {
        dict = Data;
        dict_len = (uInt)(Size > 256 ? 256 : Size);
    } else {
        dict = (const Bytef *)"";
        dict_len = 0;
    }

    /* First inflateSetDictionary call */
    ret = inflateSetDictionary(&strm, dict, dict_len);
    /* Continue regardless of return value */

    /* Second inflateSetDictionary call (different dictionary) */
    if (Size > 10) {
        dict = Data + (Size / 2);
        dict_len = (uInt)(Size / 4 > 256 ? 256 : Size / 4);
        ret = inflateSetDictionary(&strm, dict, dict_len);
    }

    /* Third inflateSetDictionary call (empty dictionary) */
    ret = inflateSetDictionary(&strm, (const Bytef *)"", 0);

    /* Second inflate call */
    strm.next_out = out_buffer;
    strm.avail_out = out_size;
    ret = inflate(&strm, Z_SYNC_FLUSH);

    /* Copy the stream state */
    ret = inflateCopy(&strm_copy, &strm);
    if (ret == Z_OK) {
        /* End original stream */
        ret = inflateEnd(&strm);

        /* Reset the copied stream */
        ret = inflateReset2(&strm_copy, windowBits);

        /* End the copied stream */
        ret = inflateEnd(&strm_copy);
    } else {
        /* If copy failed, still end original stream */
        ret = inflateEnd(&strm);
    }

    free(out_buffer);
    return 0;
}