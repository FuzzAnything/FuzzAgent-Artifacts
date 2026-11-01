// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateMark at inflate.c:1397:14 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    z_stream strm = {0};
    z_stream strm_copy = {0};
    Bytef *out_buf = NULL;
    int ret;

    // Allocate output buffer
    out_buf = (Bytef *)malloc(Size + 1024);
    if (out_buf == NULL) {
        return 0;
    }

    // Initialize the stream
    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // 1. inflatePrime
    // Inject some bits. bits can be 0-16.
    int bits = (Data[0] % 17);
    int value = (Data[1] % 65536);
    inflatePrime(&strm, bits, value);

    // 2. inflateSync
    inflateSync(&strm);

    // Setup input for inflate
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = Size;
    strm.next_out = out_buf;
    strm.avail_out = Size + 1024;

    // 3. inflate
    inflate(&strm, Z_NO_FLUSH);

    // 4. inflateSync
    inflateSync(&strm);

    // 5. inflateSyncPoint
    inflateSyncPoint(&strm);

    // 6. inflateCopy
    memset(&strm_copy, 0, sizeof(strm_copy));
    ret = inflateCopy(&strm_copy, &strm);
    if (ret == Z_OK) {
        inflateEnd(&strm_copy);
    }

    // 7. inflateUndermine
    inflateUndermine(&strm, 1);

    // 8. inflateMark
    inflateMark(&strm);

    // 9. inflateEnd
    inflateEnd(&strm);

    free(out_buf);
    return 0;
}