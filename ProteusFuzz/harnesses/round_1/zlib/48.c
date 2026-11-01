// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    z_stream strm = {0};
    z_stream strm_copy = {0};
    gz_header header = {0};
    Bytef *out_buf = NULL;
    int ret;
    int windowBits;
    int subvert;
    int validate;
    int header_mode;

    // Parse input for configuration
    subvert = Data[0] & 1;
    validate = (Data[0] >> 1) & 1;
    header_mode = (Data[0] >> 2) & 1;
    // windowBits range: -15 to 15. Map Data[1] to this range.
    // -15 to 15 is 31 values.
    windowBits = (int)(Data[1] % 31) - 15;

    // Allocate output buffer
    out_buf = (Bytef *)malloc(Size + 256);
    if (!out_buf) {
        return 0;
    }

    // Initialize stream
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    strm.next_out = out_buf;
    strm.avail_out = Size + 256;

    // 1. inflateInit2_
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // 2. inflateGetHeader (must be called after init, before reset/inflate)
    // Only try if header_mode is set and windowBits implies gzip (positive)
    if (header_mode && windowBits > 0) {
        ret = inflateGetHeader(&strm, &header);
        (void)ret; // Ignore error, just exercise the path
    }

    // 3. inflateValidate
    ret = inflateValidate(&strm, validate);
    (void)ret;

    // 4. inflateUndermine
    ret = inflateUndermine(&strm, subvert);
    (void)ret;

    // 5. inflateReset2
    // Try resetting with a potentially different window size
    int reset_bits = windowBits;
    if ((Data[0] >> 3) & 1) {
        // Flip sign or change value slightly to explore state
        if (reset_bits > 0) reset_bits = -reset_bits;
        else if (reset_bits < 0) reset_bits = -reset_bits;
        else reset_bits = 15;
    }
    ret = inflateReset2(&strm, reset_bits);
    if (ret != Z_OK) {
        // If reset fails, we might still proceed with inflate if state is recoverable,
        // but typically we should clean up. However, for fuzzing, we try to continue
        // to see if the library handles the error state gracefully during inflate.
    }

    // 6. inflateCopy
    // Initialize dest stream partially
    strm_copy.zalloc = Z_NULL;
    strm_copy.zfree = Z_NULL;
    strm_copy.opaque = Z_NULL;
    ret = inflateCopy(&strm_copy, &strm);
    if (ret == Z_OK) {
        // If copy succeeded, try to use the copy
        strm_copy.next_in = Data;
        strm_copy.avail_in = Size;
        strm_copy.next_out = out_buf;
        strm_copy.avail_out = Size + 256;
        
        // Perform one inflate step on the copy
        inflate(&strm_copy, Z_NO_FLUSH);
        
        // Cleanup copy
        inflateEnd(&strm_copy);
    }

    // Perform actual inflate on original stream
    strm.next_in = Data;
    strm.avail_in = Size;
    
    // Try a few inflate calls to explore state
    int flush_mode = Z_NO_FLUSH;
    if ((Data[0] >> 4) & 1) flush_mode = Z_SYNC_FLUSH;
    if ((Data[0] >> 5) & 1) flush_mode = Z_FINISH;
    if ((Data[0] >> 6) & 1) flush_mode = Z_BLOCK;
    if ((Data[0] >> 7) & 1) flush_mode = Z_TREES;

    ret = inflate(&strm, flush_mode);
    (void)ret;

    // Cleanup
    inflateEnd(&strm);
    free(out_buf);

    return 0;
}