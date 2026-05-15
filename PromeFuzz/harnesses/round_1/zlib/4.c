// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
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

static voidpf dummy_alloc(voidpf opaque, uInt items, uInt size) {
    (void)opaque;
    return malloc(items * size);
}

static void dummy_free(voidpf opaque, voidpf address) {
    (void)opaque;
    free(address);
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    z_stream strm1, strm2;
    gz_header head;
    int ret;
    unsigned int dict_len;
    const Bytef *dict_data;
    Bytef out_buf[4096];

    /* Initialize structures */
    memset(&strm1, 0, sizeof(z_stream));
    memset(&strm2, 0, sizeof(z_stream));
    memset(&head, 0, sizeof(gz_header));

    /* Setup custom allocators */
    strm1.zalloc = dummy_alloc;
    strm1.zfree = dummy_free;
    strm1.opaque = Z_NULL;

    /* Split input for dictionary and compressed data */
    dict_len = Size / 4;
    if (dict_len > 32768) dict_len = 32768;
    dict_data = Data + (Size - dict_len);

    /* Set input buffer */
    strm1.next_in = (Bytef*)Data;
    strm1.avail_in = Size - dict_len;
    strm1.next_out = out_buf;
    strm1.avail_out = sizeof(out_buf);

    /* 1. inflateInit2_ */
    ret = inflateInit2_(&strm1, 15 + 16, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&strm1);
        return 0;
    }

    /* 2. inflateGetHeader */
    head.extra = Z_NULL;
    head.name = Z_NULL;
    head.comment = Z_NULL;
    inflateGetHeader(&strm1, &head);

    /* 3. inflate */
    inflate(&strm1, Z_NO_FLUSH);

    /* 4-6. inflateSetDictionary (three times with different lengths) */
    if (dict_len >= 10) {
        inflateSetDictionary(&strm1, dict_data, dict_len / 3);
        inflateSetDictionary(&strm1, dict_data, dict_len / 2);
        inflateSetDictionary(&strm1, dict_data, dict_len);
    }

    /* 7. inflate */
    strm1.avail_in = Size - dict_len;
    strm1.next_in = (Bytef*)Data;
    inflate(&strm1, Z_SYNC_FLUSH);

    /* 8. inflateCopy */
    strm2.zalloc = dummy_alloc;
    strm2.zfree = dummy_free;
    strm2.opaque = Z_NULL;
    ret = inflateCopy(&strm2, &strm1);
    if (ret != Z_OK) {
        inflateEnd(&strm1);
        return 0;
    }

    /* 9. inflateEnd (on original stream) */
    inflateEnd(&strm1);

    /* 10. inflateReset2 */
    inflateReset2(&strm2, 15);

    /* 11. inflateEnd (on copied stream) */
    inflateEnd(&strm2);

    return 0;
}