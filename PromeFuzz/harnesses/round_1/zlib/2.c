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

static void initialize_stream(z_streamp strm, const uint8_t *data, size_t size) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = (Bytef*)data;
    strm->avail_in = (uInt)size;
    strm->next_out = NULL;
    strm->avail_out = 0;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    z_stream strm1, strm2;
    int ret;
    unsigned char out_buf[1024];
    
    if (Size < 2) return 0;
    
    initialize_stream(&strm1, Data, Size);
    ret = inflateInit2(&strm1, -15);
    if (ret != Z_OK) return 0;
    
    strm1.next_out = out_buf;
    strm1.avail_out = sizeof(out_buf);
    
    int bits = Data[0] % 17;
    int value = Data[1];
    inflatePrime(&strm1, bits, value);
    
    inflateSync(&strm1);
    
    inflate(&strm1, Z_NO_FLUSH);
    
    inflateSync(&strm1);
    
    inflateSyncPoint(&strm1);
    
    initialize_stream(&strm2, Data, Size);
    ret = inflateCopy(&strm2, &strm1);
    if (ret == Z_OK) {
        int undermine = Data[0] % 2;
        inflateUndermine(&strm1, undermine);
        
        inflateMark(&strm1);
        
        inflateEnd(&strm2);
    }
    
    inflateEnd(&strm1);
    
    return 0;
}