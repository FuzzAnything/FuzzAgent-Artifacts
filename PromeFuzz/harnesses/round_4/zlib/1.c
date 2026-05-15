// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
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
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_z_stream(z_streamp strm, const uint8_t *data, size_t size) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = (Bytef*)data;
    strm->avail_in = size;
    strm->next_out = Z_NULL;
    strm->avail_out = 0;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    z_stream strm1, strm2;
    int ret;
    long mark_result;
    
    // Initialize first stream for raw inflate
    init_z_stream(&strm1, Data, Size);
    ret = inflateInit2(&strm1, -15); // Raw inflate with negative window bits
    if (ret != Z_OK) {
        return 0;
    }
    
    // Allocate output buffer
    Bytef out_buf[4096];
    strm1.next_out = out_buf;
    strm1.avail_out = sizeof(out_buf);
    
    // 1. inflatePrime
    int bits = (Size > 0) ? (Data[0] & 0x0F) : 0; // Bits ≤ 16
    int value = (Size > 1) ? Data[1] : 0;
    inflatePrime(&strm1, bits, value);
    
    // 2. inflateSync (before inflate)
    inflateSync(&strm1);
    
    // 3. inflate
    inflate(&strm1, Z_NO_FLUSH);
    
    // 4. inflateSync (after inflate)
    inflateSync(&strm1);
    
    // 5. inflateSyncPoint
    inflateSyncPoint(&strm1);
    
    // 6. inflateCopy
    memset(&strm2, 0, sizeof(z_stream));
    strm2.zalloc = Z_NULL;
    strm2.zfree = Z_NULL;
    strm2.opaque = Z_NULL;
    inflateCopy(&strm2, &strm1);
    
    // 7. inflateUndermine
    int undermine = (Size > 2) ? (Data[2] & 1) : 0;
    inflateUndermine(&strm1, undermine);
    
    // 8. inflateMark
    mark_result = inflateMark(&strm1);
    
    // 9. inflateEnd (both streams)
    inflateEnd(&strm1);
    inflateEnd(&strm2);
    
    return 0;
}