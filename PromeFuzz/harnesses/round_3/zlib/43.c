// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
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

static void* custom_zalloc(void* opaque, unsigned items, unsigned size) {
    (void)opaque;
    return malloc(items * size);
}

static void custom_zfree(void* opaque, void* address) {
    (void)opaque;
    free(address);
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    z_stream strm;
    unsigned char out_buffer[4096];
    int ret;
    
    strm.zalloc = custom_zalloc;
    strm.zfree = custom_zfree;
    strm.opaque = Z_NULL;
    strm.next_in = (Bytef*)Data;
    strm.avail_in = Size > 1024 ? 1024 : (uInt)Size;
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    int windowBits = (Data[0] % 31) + 8;
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    int check = Data[1] & 1;
    inflateValidate(&strm, check);
    
    int bits = Data[2] % 17;
    int value = Data[3];
    inflatePrime(&strm, bits, value);
    
    int flush = Data[4] % 5;
    ret = inflate(&strm, flush);
    
    if (ret == Z_DATA_ERROR) {
        inflateSync(&strm);
    }
    
    if (ret == Z_OK || ret == Z_BUF_ERROR || ret == Z_STREAM_END) {
        inflateResetKeep(&strm);
        
        strm.next_in = (Bytef*)Data + (Size > 512 ? 512 : Size/2);
        strm.avail_in = Size > 512 ? (uInt)(Size - 512) : (uInt)(Size/2);
        strm.next_out = out_buffer;
        strm.avail_out = sizeof(out_buffer);
        
        bits = Data[5] % 17;
        value = Data[6];
        inflatePrime(&strm, bits, value);
        
        flush = Data[7] % 5;
        inflate(&strm, flush);
    }
    
    inflateEnd(&strm);
    return 0;
}