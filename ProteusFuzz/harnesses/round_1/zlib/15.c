// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateMark at inflate.c:1397:14 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void cleanup_stream(z_stream *strm) {
    if (strm) {
        inflateEnd(strm);
        memset(strm, 0, sizeof(z_stream));
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    z_stream strm = {0};
    z_stream copy_strm = {0};
    int ret;

    // Allocate output buffer
    Bytef *out_buf = (Bytef *)malloc(Size + 256);
    if (!out_buf) {
        return 0;
    }

    // Initialize the stream
    if (inflateInit(&strm) != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Set input data
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(Size + 256);

    // Perform initial inflation to reach a state where sync functions might be useful
    ret = inflate(&strm, Z_NO_FLUSH);

    // Fuzz inflateUndermine
    // Try both 0 and 1 for the subvert flag
    inflateUndermine(&strm, 0);
    inflateUndermine(&strm, 1);

    // Fuzz inflateSync
    // This is useful if we hit a data error or just to test recovery logic
    inflateSync(&strm);

    // Fuzz inflateSyncPoint
    // Check if we are at a sync point
    inflateSyncPoint(&strm);

    // Fuzz inflateCopy
    // Try to copy the current state to a new stream
    ret = inflateCopy(&copy_strm, &strm);
    if (ret == Z_OK) {
        // If copy succeeded, try to operate on the copied stream
        copy_strm.next_out = out_buf;
        copy_strm.avail_out = (uInt)(Size + 256);
        inflate(&copy_strm, Z_NO_FLUSH);
        inflateUndermine(&copy_strm, 1);
        inflateSync(&copy_strm);
        inflateSyncPoint(&copy_strm);
        cleanup_stream(&copy_strm);
    }

    // Fuzz inflatePrime
    // Inject bits directly into the decoder
    // Try various bit counts and values
    inflatePrime(&strm, 0, 0);
    inflatePrime(&strm, 1, 1);
    inflatePrime(&strm, 8, 0xFF);
    inflatePrime(&strm, 16, 0xFFFF);
    inflatePrime(&strm, -1, 0); // Clear buffer
    inflatePrime(&strm, 32, 0); // Invalid bits count (should fail)

    // Fuzz inflateMark
    // Get the mark to track progress
    inflateMark(&strm);

    // Try more inflation to consume remaining data or trigger errors
    ret = inflate(&strm, Z_FINISH);
    if (ret == Z_DATA_ERROR) {
        // Attempt recovery again after a finish attempt
        inflateSync(&strm);
    }

    // Cleanup
    inflateEnd(&strm);
    free(out_buf);

    return 0;
}