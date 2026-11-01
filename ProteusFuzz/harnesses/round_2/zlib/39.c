// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateMark at inflate.c:1397:14 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateMark at inflate.c:1397:14 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateCodesUsed at inflate.c:1408:23 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    z_stream dest = {0};
    uint8_t *out_buf = NULL;
    int ret;

    if (Size < 2) {
        return 0;
    }

    // Allocate output buffer
    out_buf = (uint8_t *)malloc(Size + 1024);
    if (!out_buf) {
        return 0;
    }

    // Initialize source stream
    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Set input data
    strm.next_in = Data;
    strm.avail_in = Size;
    strm.next_out = out_buf;
    strm.avail_out = Size + 1024;

    // Fuzz inflateUndermine
    // Try both 0 and 1 for subvert
    inflateUndermine(&strm, Data[0] % 2);

    // Perform some decompression to reach a state where other functions are valid
    ret = inflate(&strm, Z_NO_FLUSH);
    
    // Fuzz inflateSyncPoint
    // Only call if state is valid (Z_OK, Z_STREAM_END, or Z_BUF_ERROR)
    if (ret == Z_OK || ret == Z_STREAM_END || ret == Z_BUF_ERROR) {
        inflateSyncPoint(&strm);
    }

    // Fuzz inflateSync
    // Try to recover from potential errors
    inflateSync(&strm);

    // Fuzz inflateMark
    // Returns progress info
    inflateMark(&strm);

    // Fuzz inflateCopy
    // Initialize dest stream structure before copying
    dest.zalloc = Z_NULL;
    dest.zfree = Z_NULL;
    dest.opaque = Z_NULL;
    dest.state = Z_NULL;
    
    ret = inflateCopy(&dest, &strm);
    if (ret == Z_OK) {
        // Perform operations on the copied stream to ensure it's functional
        dest.next_out = out_buf;
        dest.avail_out = Size + 1024;
        
        // Try to decompress with the copied stream
        inflate(&dest, Z_NO_FLUSH);
        
        // Test other functions on the copied stream
        inflateUndermine(&dest, Data[1] % 2);
        inflateSyncPoint(&dest);
        inflateSync(&dest);
        inflateMark(&dest);
        
        // Cleanup copied stream
        inflateEnd(&dest);
    }

    // Fuzz inflateCodesUsed
    // Check memory usage of code tables
    inflateCodesUsed(&strm);

    // Cleanup
    inflateEnd(&strm);
    free(out_buf);

    return 0;
}