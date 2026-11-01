// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflateMark at inflate.c:1397:14 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateMark at inflate.c:1397:14 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    z_stream strm_copy = {0};
    uint8_t *out_buf = NULL;
    int ret;

    if (Size < 1) {
        return 0;
    }

    // Allocate output buffer
    out_buf = (uint8_t *)malloc(1024);
    if (!out_buf) {
        return 0;
    }

    // Initialize stream
    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Set input data
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = 1024;

    // 1. inflatePrime
    // Inject some bits to manipulate the state
    inflatePrime(&strm, 8, 0xFF);
    inflatePrime(&strm, -1, 0); // Clear buffer

    // 2. inflateSync
    // Attempt to sync (may return error if not corrupted, which is fine for fuzzing)
    inflateSync(&strm);

    // 3. inflate
    // Perform decompression
    ret = inflate(&strm, Z_NO_FLUSH);
    // Handle potential errors by trying sync again or continuing
    if (ret == Z_DATA_ERROR) {
        inflateSync(&strm);
    }

    // 4. inflateSync
    // Try syncing again after inflate
    inflateSync(&strm);

    // 5. inflateSyncPoint
    // Check if at a sync point
    (void)inflateSyncPoint(&strm);

    // 6. inflateCopy
    // Copy the stream state to a new stream
    ret = inflateCopy(&strm_copy, &strm);
    if (ret == Z_OK) {
        // Perform operations on the copy to explore state
        inflatePrime(&strm_copy, 4, 0x0F);
        inflateSync(&strm_copy);
        inflate(&strm_copy, Z_NO_FLUSH);
        inflateSyncPoint(&strm_copy);
        inflateMark(&strm_copy);
        inflateEnd(&strm_copy);
    }

    // 7. inflateUndermine
    // Attempt to undermine strict checks (may fail if not compiled with specific macro)
    inflateUndermine(&strm, 1);
    inflateUndermine(&strm, 0);

    // 8. inflateMark
    // Get mark information
    (void)inflateMark(&strm);

    // 9. inflateEnd
    // Cleanup original stream
    inflateEnd(&strm);

    free(out_buf);
    return 0;
}