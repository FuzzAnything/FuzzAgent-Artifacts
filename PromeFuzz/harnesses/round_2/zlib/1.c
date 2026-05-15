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

static void dummy_free(voidpf opaque, voidpf address) {
    (void)opaque;
    free(address);
}

static voidpf dummy_alloc(voidpf opaque, uInt items, uInt size) {
    (void)opaque;
    return calloc(items, size);
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;
    
    z_stream strm1, strm2;
    int ret;
    
    strm1.zalloc = dummy_alloc;
    strm1.zfree = dummy_free;
    strm1.opaque = Z_NULL;
    strm1.next_in = Z_NULL;
    strm1.avail_in = 0;
    
    ret = inflateInit2(&strm1, -15);
    if (ret != Z_OK) return 0;
    
    size_t data_used = 0;
    uint8_t out_buf[4096];
    
    strm1.next_in = (Bytef*)Data;
    strm1.avail_in = Size > 1024 ? 1024 : Size;
    data_used = strm1.avail_in;
    
    strm1.next_out = out_buf;
    strm1.avail_out = sizeof(out_buf);
    
    int bits = Data[0] % 17;
    int value = (Data[1] << 8) | Data[2];
    inflatePrime(&strm1, bits, value);
    
    inflateSync(&strm1);
    
    inflate(&strm1, Z_NO_FLUSH);
    
    inflateSync(&strm1);
    
    inflateSyncPoint(&strm1);
    
    memset(&strm2, 0, sizeof(strm2));
    strm2.zalloc = dummy_alloc;
    strm2.zfree = dummy_free;
    strm2.opaque = Z_NULL;
    inflateCopy(&strm2, &strm1);
    
    int undermine = Data[3] & 1;
    inflateUndermine(&strm1, undermine);
    
    inflateMark(&strm1);
    
    if (strm2.state != Z_NULL) {
        inflateEnd(&strm2);
    }
    inflateEnd(&strm1);
    
    return 0;
}