// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateMark at inflate.c:1397:14 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    z_stream dest = {0};
    uint8_t out_buf[4096];
    int ret;

    if (Size == 0) {
        return 0;
    }

    // Initialize the source stream
    if (inflateInit(&strm) != Z_OK) {
        return 0;
    }

    // Prepare input
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = (Size > 65535) ? 65535 : (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    // 1. inflatePrime
    // Try to inject bits. Use modulo to keep bits in range [0, 16] or negative.
    int bits = (Size > 0) ? (Data[0] % 17) : 0;
    int value = (Size > 1) ? (Data[1] & 0xFF) : 0;
    if (bits == 0) bits = -1; // Clear buffer if 0
    inflatePrime(&strm, bits, value);

    // 2. inflateSync
    // Attempt to sync. This might fail if no sync point found, which is expected.
    inflateSync(&strm);

    // 3. inflate
    // Perform decompression
    ret = inflate(&strm, Z_NO_FLUSH);
    // If we hit an error or end, we might try to recover or continue
    if (ret == Z_STREAM_END || ret == Z_DATA_ERROR) {
        // Try to sync again if error occurred
        if (ret == Z_DATA_ERROR) {
            inflateSync(&strm);
        }
    }

    // 4. inflateSync (Second call as per sequence)
    inflateSync(&strm);

    // 5. inflateSyncPoint
    // Check if at a sync point
    inflateSyncPoint(&strm);

    // 6. inflateCopy
    // Copy the stream state to dest
    if (inflateCopy(&dest, &strm) == Z_OK) {
        // Use dest briefly to ensure it's active
        dest.next_out = out_buf;
        dest.avail_out = sizeof(out_buf);
        inflate(&dest, Z_NO_FLUSH);
        inflateEnd(&dest);
    }

    // 7. inflateUndermine
    // Attempt to undermine strict validation
    int subvert = (Size > 2) ? (Data[2] % 2) : 1;
    inflateUndermine(&strm, subvert);

    // 8. inflateMark
    // Get mark information
    inflateMark(&strm);

    // 9. inflateEnd
    // Cleanup
    inflateEnd(&strm);

    return 0;
}