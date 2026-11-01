// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    z_stream strm_copy = {0};
    gz_header header = {0};
    Bytef *output_buf = NULL;
    Bytef *dict_buf = NULL;
    uInt dict_len = 0;
    int ret;

    if (Size < 1) return 0;

    // Allocate output buffer
    size_t out_size = Size > 65536 ? 65536 : Size;
    output_buf = (Bytef *)malloc(out_size);
    if (!output_buf) return 0;

    // Allocate dictionary buffer if we have enough data
    if (Size > 2) {
        dict_buf = (Bytef *)malloc(Size / 2);
        if (dict_buf) {
            memcpy(dict_buf, Data, Size / 2);
            dict_len = Size / 2;
        }
    }

    // 1. inflateInit2_
    int windowBits = 15;
    if (Size > 0) {
        int val = Data[0] % 50;
        if (val < 10) windowBits = -15;
        else if (val < 20) windowBits = 15;
        else if (val < 30) windowBits = 31;
        else if (val < 40) windowBits = -15;
        else windowBits = 15;
    }

    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(output_buf);
        free(dict_buf);
        return 0;
    }

    // 2. inflateGetHeader
    ret = inflateGetHeader(&strm, &header);

    // 3. inflate (First call)
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = Size;
    strm.next_out = output_buf;
    strm.avail_out = out_size;

    ret = inflate(&strm, Z_NO_FLUSH);

    // 4, 5, 6. inflateSetDictionary (Three times)
    if (dict_buf && dict_len > 0) {
        ret = inflateSetDictionary(&strm, dict_buf, dict_len);
        ret = inflateSetDictionary(&strm, dict_buf, dict_len);
        ret = inflateSetDictionary(&strm, dict_buf, dict_len);
    }

    // 7. inflate (Second call)
    if (strm.avail_in > 0) {
        ret = inflate(&strm, Z_SYNC_FLUSH);
    } else {
        strm.next_in = (const Bytef *)Data;
        strm.avail_in = Size;
        ret = inflate(&strm, Z_NO_FLUSH);
    }

    // 8. inflateCopy
    ret = inflateCopy(&strm_copy, &strm);
    if (ret == Z_OK) {
        strm_copy.next_in = (const Bytef *)Data;
        strm_copy.avail_in = Size > 10 ? 10 : Size;
        strm_copy.next_out = output_buf;
        strm_copy.avail_out = out_size;
        ret = inflate(&strm_copy, Z_NO_FLUSH);
        inflateEnd(&strm_copy);
    }

    // 9. inflateEnd (First call)
    inflateEnd(&strm);

    // Re-initialize for reset test
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // 10. inflateReset2
        int resetBits = 15;
        if (Size > 1) {
            int val = Data[1] % 50;
            if (val < 10) resetBits = -15;
            else if (val < 20) resetBits = 15;
            else if (val < 30) resetBits = 31;
            else if (val < 40) resetBits = -15;
            else resetBits = 15;
        }
        ret = inflateReset2(&strm, resetBits);

        // 11. inflateEnd (Second call)
        inflateEnd(&strm);
    }

    free(output_buf);
    free(dict_buf);

    return 0;
}