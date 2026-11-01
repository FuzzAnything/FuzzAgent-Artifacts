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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    z_stream strm_copy = {0};
    gz_header header = {0};
    uint8_t *out_buf = NULL;
    uint8_t *dict_buf = NULL;
    int ret;

    if (Size < 2) return 0;

    // Allocate output buffer
    out_buf = (uint8_t *)malloc(Size + 1024);
    if (!out_buf) return 0;

    // Allocate dictionary buffer if enough data
    if (Size > 10) {
        dict_buf = (uint8_t *)malloc(Size - 10);
        if (dict_buf) {
            memcpy(dict_buf, Data + 10, Size - 10);
        }
    }

    // 1. inflateInit2_
    // Use -15 to disable zlib header (raw deflate) for broader fuzzing coverage
    ret = inflateInit2_(&strm, -15, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        free(dict_buf);
        return 0;
    }

    // 2. inflateGetHeader
    // Note: This might fail if not gzip format, which is expected behavior for fuzzing
    inflateGetHeader(&strm, &header);

    // Setup input
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(Size + 1024);

    // 3. inflate
    ret = inflate(&strm, Z_NO_FLUSH);

    // 4, 5, 6. inflateSetDictionary (called 3 times as requested)
    // This is likely to fail if state is not DICT, but we try to exercise the path
    if (dict_buf) {
        inflateSetDictionary(&strm, dict_buf, (uInt)(Size - 10));
        inflateSetDictionary(&strm, dict_buf, (uInt)(Size - 10));
        inflateSetDictionary(&strm, dict_buf, (uInt)(Size - 10));
    }

    // 7. inflate (second call)
    ret = inflate(&strm, Z_NO_FLUSH);

    // 8. inflateCopy
    // Initialize dest stream partially to ensure it's not null, though inflateCopy handles init
    strm_copy.zalloc = strm.zalloc;
    strm_copy.zfree = strm.zfree;
    strm_copy.opaque = strm.opaque;
    ret = inflateCopy(&strm_copy, &strm);

    // 9. inflateEnd (for original stream)
    inflateEnd(&strm);

    // 10. inflateReset2
    // Try to reset the copied stream. If copy failed, this might error, which is fine.
    inflateReset2(&strm_copy, -15);

    // 11. inflateEnd (for copied stream)
    inflateEnd(&strm_copy);

    free(out_buf);
    free(dict_buf);

    return 0;
}