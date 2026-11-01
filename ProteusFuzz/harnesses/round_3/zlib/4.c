// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    z_stream strm_copy = {0};
    gz_header header = {0};
    Bytef *out_buf = NULL;
    Bytef *dict_buf = NULL;
    int ret;
    int windowBits;

    if (Size < 2) {
        return 0;
    }

    // Prepare input buffer
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = Size;

    // Prepare output buffer
    out_buf = (Bytef *)malloc(Size + 1024);
    if (!out_buf) return 0;
    strm.next_out = out_buf;
    strm.avail_out = Size + 1024;

    // Determine windowBits from input data to explore different states
    // Range: -15 to 15
    windowBits = (int)Data[0] % 32 - 15;

    // 1. inflateInit2_
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // 2. inflateGetHeader
    ret = inflateGetHeader(&strm, &header);
    (void)ret;

    // 3. inflate
    ret = inflate(&strm, Z_NO_FLUSH);
    (void)ret;

    // 4, 5, 6. inflateSetDictionary (called 3 times as requested)
    if (Size > 10) {
        dict_buf = (Bytef *)malloc(10);
        if (dict_buf) {
            memcpy(dict_buf, Data + 1, 10);
            ret = inflateSetDictionary(&strm, dict_buf, 10);
            (void)ret;
            ret = inflateSetDictionary(&strm, dict_buf, 10);
            (void)ret;
            ret = inflateSetDictionary(&strm, dict_buf, 10);
            (void)ret;
            free(dict_buf);
        }
    }

    // 7. inflate (second call)
    ret = inflate(&strm, Z_NO_FLUSH);
    (void)ret;

    // 8. inflateCopy
    ret = inflateCopy(&strm_copy, &strm);
    if (ret == Z_OK) {
        strm_copy.next_out = out_buf;
        strm_copy.avail_out = Size + 1024;
        ret = inflate(&strm_copy, Z_NO_FLUSH);
        (void)ret;
    }

    // 9. inflateEnd (for original stream)
    ret = inflateEnd(&strm);
    (void)ret;

    // 10. inflateReset2 (on the copy stream)
    int newWindowBits = (int)Data[1] % 32 - 15;
    ret = inflateReset2(&strm_copy, newWindowBits);
    (void)ret;

    // 11. inflateEnd (for the copy stream)
    ret = inflateEnd(&strm_copy);
    (void)ret;

    free(out_buf);
    return 0;
}