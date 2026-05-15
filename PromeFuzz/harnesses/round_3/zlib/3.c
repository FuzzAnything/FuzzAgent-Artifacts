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
// inflateMark at inflate.c:1397:14 in zlib.h
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

static void init_stream(z_streamp strm, uint8_t *in_buf, size_t in_size, uint8_t *out_buf, size_t out_size) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = in_buf;
    strm->avail_in = in_size;
    strm->next_out = out_buf;
    strm->avail_out = out_size;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    z_stream strm1, strm2;
    int ret;
    long mark_result;
    uint8_t *in_buf = NULL;
    uint8_t out_buf1[4096];
    uint8_t out_buf2[4096];
    int strm1_initialized = 0;
    int strm2_initialized = 0;
    
    if (Size < 4) return 0;
    
    in_buf = (uint8_t *)malloc(Size);
    if (!in_buf) return 0;
    memcpy(in_buf, Data, Size);
    
    memset(&strm1, 0, sizeof(z_stream));
    memset(&strm2, 0, sizeof(z_stream));
    
    ret = inflateInit2(&strm1, -15);
    if (ret != Z_OK) {
        free(in_buf);
        return 0;
    }
    strm1_initialized = 1;
    
    init_stream(&strm1, in_buf, Size, out_buf1, sizeof(out_buf1));
    
    int bits = (Data[0] % 17);
    int value = (Data[1] << 8) | Data[2];
    inflatePrime(&strm1, bits, value);
    
    inflateSync(&strm1);
    
    int flush = Data[3] % 6;
    inflate(&strm1, flush);
    
    inflateSync(&strm1);
    
    inflateSyncPoint(&strm1);
    
    ret = inflateCopy(&strm2, &strm1);
    if (ret == Z_OK) {
        strm2_initialized = 1;
        init_stream(&strm2, strm1.next_in, strm1.avail_in, out_buf2, sizeof(out_buf2));
        
        int undermine = Data[0] % 2;
        inflateUndermine(&strm2, undermine);
        
        mark_result = inflateMark(&strm2);
        
        inflateEnd(&strm2);
        strm2_initialized = 0;
    }
    
    mark_result = inflateMark(&strm1);
    
    if (strm1_initialized) {
        inflateEnd(&strm1);
        strm1_initialized = 0;
    }
    
    free(in_buf);
    return 0;
}