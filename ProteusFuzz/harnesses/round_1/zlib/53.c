// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateCodesUsed at inflate.c:1408:23 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    z_stream strm = {0};
    z_stream copy_strm = {0};
    Bytef *in_buf = NULL;
    Bytef *out_buf = NULL;

    // Allocate input and output buffers
    in_buf = (Bytef *)malloc(Size);
    out_buf = (Bytef *)malloc(Size + 1024); // Extra space for decompression

    if (!in_buf || !out_buf) {
        free(in_buf);
        free(out_buf);
        return 0;
    }

    memcpy(in_buf, Data, Size);

    // Initialize the stream
    if (inflateInit(&strm) != Z_OK) {
        free(in_buf);
        free(out_buf);
        return 0;
    }

    // Set up input
    strm.next_in = in_buf;
    strm.avail_in = Size;
    strm.next_out = out_buf;
    strm.avail_out = Size + 1024;

    // Fuzz inflateUndermine
    // Try both 0 and 1 for subvert
    inflateUndermine(&strm, 0);
    inflateUndermine(&strm, 1);

    // Fuzz inflateSync
    // Call it even if not in error state to test robustness
    inflateSync(&strm);

    // Fuzz inflateSyncPoint
    // Check if at sync point
    inflateSyncPoint(&strm);

    // Fuzz inflateResetKeep
    // Reset while keeping dictionary
    inflateResetKeep(&strm);

    // Fuzz inflateCopy
    // Copy the stream to a new one
    if (inflateCopy(&copy_strm, &strm) == Z_OK) {
        // Perform some operations on the copy
        copy_strm.next_in = in_buf;
        copy_strm.avail_in = Size;
        copy_strm.next_out = out_buf;
        copy_strm.avail_out = Size + 1024;

        inflateUndermine(&copy_strm, 1);
        inflateSync(&copy_strm);
        inflateSyncPoint(&copy_strm);
        inflateResetKeep(&copy_strm);
        
        // Clean up copy
        inflateEnd(&copy_strm);
    }

    // Fuzz inflateCodesUsed
    // Get the number of codes used
    inflateCodesUsed(&strm);

    // Cleanup
    inflateEnd(&strm);
    free(in_buf);
    free(out_buf);

    return 0;
}