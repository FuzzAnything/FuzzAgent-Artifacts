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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    z_stream copy_strm = {0};
    uint8_t out_buf[4096];
    int ret;
    int bits;
    int value;
    int subvert;
    int flush_mode;

    if (Size < 4) {
        return 0;
    }

    // Extract parameters from input to diversify execution
    bits = (int)(Data[0] % 17); // 0 to 16
    value = (int)(Data[1] % 65536); // 0 to 65535
    subvert = (int)(Data[2] % 2); // 0 or 1
    flush_mode = (int)(Data[3] % 10); // 0 to 9

    // Initialize the stream
    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        return 0;
    }

    // 1. inflatePrime
    // Inject bits into the bit buffer
    inflatePrime(&strm, bits, value);

    // Setup input and output buffers
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    // 2. inflateSync
    // Attempt to sync (may fail if not at sync point, which is expected for fuzzing)
    inflateSync(&strm);

    // 3. inflate
    // Perform decompression
    inflate(&strm, flush_mode);

    // 4. inflateSync
    // Try to sync again after some processing
    inflateSync(&strm);

    // 5. inflateSyncPoint
    // Check if we are at a sync point
    inflateSyncPoint(&strm);

    // 6. inflateCopy
    // Copy the stream state to a new stream
    ret = inflateCopy(&copy_strm, &strm);
    if (ret == Z_OK) {
        // Cleanup the copied stream
        inflateEnd(&copy_strm);
    }

    // 7. inflateUndermine
    // Disable strict distance validation (for debugging/malformed data handling)
    inflateUndermine(&strm, subvert);

    // 8. inflateMark
    // Get the number of bytes consumed since the last sync point
    inflateMark(&strm);

    // 9. inflateEnd
    // Cleanup the original stream
    inflateEnd(&strm);

    return 0;
}