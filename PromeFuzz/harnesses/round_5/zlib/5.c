// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateMark at inflate.c:1397:14 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Initialize source stream
    z_stream src;
    src.zalloc = Z_NULL;
    src.zfree = Z_NULL;
    src.opaque = Z_NULL;
    src.next_in = (Bytef *)Data;
    src.avail_in = Size > 0 ? Size : 0;
    src.total_in = 0;
    
    // Initialize destination stream for copy
    z_stream dest;
    dest.zalloc = Z_NULL;
    dest.zfree = Z_NULL;
    dest.opaque = Z_NULL;
    
    // Initialize inflation with raw deflate format (negative windowBits)
    if (inflateInit2(&src, -15) != Z_OK) {
        return 0;
    }
    
    // Allocate output buffer
    size_t out_size = 1024;
    Bytef *out_buf = (Bytef *)malloc(out_size);
    if (!out_buf) {
        inflateEnd(&src);
        return 0;
    }
    src.next_out = out_buf;
    src.avail_out = out_size;
    src.total_out = 0;
    
    // 1. inflatePrime - use first 2 bytes for bits/value if available
    int bits = 0;
    int value = 0;
    if (Size >= 2) {
        bits = Data[0] % 17;  // bits must be <= 16
        value = Data[1];
    }
    inflatePrime(&src, bits, value);
    
    // 2. inflateSync
    inflateSync(&src);
    
    // 3. inflate with Z_NO_FLUSH
    inflate(&src, Z_NO_FLUSH);
    
    // 4. inflateSync again
    inflateSync(&src);
    
    // 5. inflateSyncPoint
    inflateSyncPoint(&src);
    
    // 6. inflateCopy
    inflateCopy(&dest, &src);
    
    // 7. inflateUndermine - use 0 or 1 based on data
    int undermine = 0;
    if (Size > 0) {
        undermine = Data[0] & 1;
    }
    inflateUndermine(&src, undermine);
    
    // 8. inflateMark
    inflateMark(&src);
    
    // 9. inflateEnd for both streams
    inflateEnd(&src);
    inflateEnd(&dest);
    
    // Cleanup
    free(out_buf);
    
    return 0;
}

#ifdef __cplusplus
}
#endif