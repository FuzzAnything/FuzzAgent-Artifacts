// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

static int fuzz_inflate_functions(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    z_stream dest = {0};
    int ret;
    uint8_t *output_buf = NULL;
    size_t output_size = 0;

    // Allocate output buffer if input data exists
    if (Size > 0) {
        output_size = Size * 4 + 256; // Heuristic for decompression expansion
        output_buf = (uint8_t *)malloc(output_size);
        if (!output_buf) return -1;
    }

    // Initialize the stream
    if (inflateInit(&strm) != Z_OK) {
        free(output_buf);
        return -1;
    }

    // Set input data
    if (Size > 0) {
        strm.next_in = (const Bytef *)Data;
        strm.avail_in = (uInt)Size;
    }
    strm.next_out = output_buf;
    strm.avail_out = (uInt)output_size;

    // 1. Test inflatePrime
    // Try various bit counts and values
    ret = inflatePrime(&strm, 0, 0); // No-op
    (void)ret;
    ret = inflatePrime(&strm, -1, 0); // Clear buffer
    (void)ret;
    ret = inflatePrime(&strm, 16, 0xFFFF); // Max bits
    (void)ret;
    ret = inflatePrime(&strm, 1, 1); // Single bit
    (void)ret;
    ret = inflatePrime(&strm, 33, 0); // Invalid bits (should fail)
    (void)ret;

    // 2. Test inflateCopy
    // Initialize dest before copy
    if (inflateInit(&dest) == Z_OK) {
        // Attempt to copy from source to dest
        ret = inflateCopy(&dest, &strm);
        if (ret == Z_OK) {
            // Perform some operations on the copied stream to ensure independence
            dest.next_out = output_buf;
            dest.avail_out = (uInt)output_size;
            ret = inflate(&dest, Z_NO_FLUSH);
            (void)ret;
        }
        // Ensure dest is cleaned up regardless of inflateCopy or inflate success/failure
        inflateEnd(&dest);
    }

    // 3. Test inflateUndermine
    // Try subverting (0) and not subverting (1)
    ret = inflateUndermine(&strm, 0); // Disable sanity checks if supported
    (void)ret;
    ret = inflateUndermine(&strm, 1); // Enable sanity checks
    (void)ret;

    // 4. Test inflateResetKeep
    // Reset while keeping dictionary/window
    ret = inflateResetKeep(&strm);
    (void)ret;

    // 5. Test inflateSync
    // Try to sync (might fail if no sync point found, which is expected)
    ret = inflateSync(&strm);
    (void)ret;

    // 6. Test inflateSyncPoint
    // Check if at a sync point
    int is_sync = inflateSyncPoint(&strm);
    (void)is_sync;

    // Cleanup
    inflateEnd(&strm);
    free(output_buf);

    return 0;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Handle empty input gracefully
    if (Size == 0) {
        return 0;
    }

    // Run the fuzzing logic
    int result = fuzz_inflate_functions(Data, Size);
    
    // Return 0 to indicate no crash
    (void)result;
    return 0;
}