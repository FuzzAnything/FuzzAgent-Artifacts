// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateEnd at inflate.c:1155:13 in zlib.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    z_stream strm = {0};
    z_stream copy_strm = {0};
    Bytef *out_buf = NULL;
    uInt out_size = 0;
    int ret;

    // Allocate output buffer
    out_size = Size * 2 + 256;
    if (out_size < 256) out_size = 256;
    out_buf = (Bytef *)malloc(out_size);
    if (!out_buf) {
        return 0;
    }

    // Initialize stream
    if (inflateInit(&strm) != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Set input data
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = Size;
    strm.next_out = out_buf;
    strm.avail_out = out_size;

    // 1. inflatePrime
    // Try different bit values based on input to explore states
    // Ensure we have enough data before accessing Data[1]
    int bits = 1; 
    int value = 0;
    if (Size > 0) {
        bits = (Data[0] % 17); 
        if (bits == 0) bits = 1; // Avoid 0 bits which returns immediately
    }
    if (Size > 1) {
        value = (Data[1] % 256);
    }
    
    ret = inflatePrime(&strm, bits, value);
    (void)ret; // Ignore return for fuzzing, just exercise

    // 2. inflateSync (first call)
    // This might fail if not in error state, but we exercise it
    ret = inflateSync(&strm);
    (void)ret;

    // 3. inflate
    // Run inflate with different flush modes based on input
    int flush_mode = Z_NO_FLUSH;
    if (Size > 0) {
        flush_mode = (Data[0] % 3) ? Z_NO_FLUSH : Z_SYNC_FLUSH;
    }
    ret = inflate(&strm, flush_mode);
    (void)ret;

    // 4. inflateSync (second call)
    ret = inflateSync(&strm);
    (void)ret;

    // 5. inflateSyncPoint
    // Check if we are at a sync point
    int is_sync = inflateSyncPoint(&strm);
    (void)is_sync;

    // 6. inflateCopy
    // Attempt to copy the stream state
    ret = inflateCopy(&copy_strm, &strm);
    if (ret == Z_OK) {
        // If copy succeeded, we can try to use the copy or just clean it up
        // For fuzzing, we just ensure the copy was attempted and then cleaned
        (void)inflateEnd(&copy_strm);
    }

    // 7. inflateUndermine
    // Try to undermine the stream with different flags
    int subvert = 0;
    if (Size > 0) {
        subvert = (Data[0] % 2);
    }
    ret = inflateUndermine(&strm, subvert);
    (void)ret;

    // 8. inflateMark
    // Get the mark
    long mark = inflateMark(&strm);
    (void)mark;

    // 9. inflateEnd
    // Cleanup
    ret = inflateEnd(&strm);
    (void)ret;

    free(out_buf);
    return 0;
}