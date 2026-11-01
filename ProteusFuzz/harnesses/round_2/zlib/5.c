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
#include <zlib.h>

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    z_stream strm_copy = {0};
    gz_header header = {0};
    uint8_t *out_buf = NULL;
    uint8_t *dict_buf = NULL;
    int ret;
    int windowBits = -15; 

    // Prepare output buffer
    size_t out_size = Size > 0 ? Size * 2 + 1024 : 1024;
    out_buf = (uint8_t *)malloc(out_size);
    if (!out_buf) return 0;

    // Prepare dictionary buffer
    size_t dict_len = Size > 100 ? 100 : (Size > 0 ? Size : 10);
    dict_buf = (uint8_t *)malloc(dict_len);
    if (!dict_buf) {
        free(out_buf);
        return 0;
    }
    if (Size > 0) {
        memcpy(dict_buf, Data, dict_len);
    } else {
        memset(dict_buf, 0, dict_len);
    }

    // 1. inflateInit2_
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        free(dict_buf);
        return 0;
    }

    // 2. inflateGetHeader
    ret = inflateGetHeader(&strm, &header);
    // Return value ignored to allow execution flow regardless of header support

    // Setup input for first inflate call
    strm.next_in = Data;
    strm.avail_in = Size;
    strm.next_out = out_buf;
    strm.avail_out = out_size;

    // 3. inflate
    ret = inflate(&strm, Z_NO_FLUSH);

    // 4, 5, 6. inflateSetDictionary (called 3 times)
    if (dict_buf) {
        ret = inflateSetDictionary(&strm, dict_buf, (uInt)dict_len);
        ret = inflateSetDictionary(&strm, dict_buf, (uInt)dict_len);
        ret = inflateSetDictionary(&strm, dict_buf, (uInt)dict_len);
    }

    // 7. inflate (second call)
    if (strm.avail_in == 0 && Size > 0) {
        strm.next_in = Data;
        strm.avail_in = Size;
    }
    if (strm.avail_out == 0) {
        strm.next_out = out_buf;
        strm.avail_out = out_size;
    }
    ret = inflate(&strm, Z_SYNC_FLUSH);

    // 8. inflateCopy
    ret = inflateCopy(&strm_copy, &strm);

    // 9. inflateEnd
    ret = inflateEnd(&strm);

    // 10. inflateReset2
    // Called on ended stream to test error handling
    ret = inflateReset2(&strm, windowBits);

    // 11. inflateEnd
    ret = inflateEnd(&strm);

    // Cleanup strm_copy if it was successfully created
    if (strm_copy.state != NULL) {
        inflateEnd(&strm_copy);
    }

    // Final cleanup
    free(out_buf);
    free(dict_buf);

    return 0;
}