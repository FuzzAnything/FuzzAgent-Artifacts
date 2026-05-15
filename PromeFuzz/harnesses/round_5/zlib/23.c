// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

static voidpf dummy_alloc(voidpf opaque, uInt items, uInt size) {
    (void)opaque;
    return malloc(items * size);
}

static void dummy_free(voidpf opaque, voidpf address) {
    (void)opaque;
    free(address);
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    z_stream strm;
    int ret;
    unsigned char out_buffer[4096];
    
    if (Size < 1) return 0;
    
    strm.zalloc = dummy_alloc;
    strm.zfree = dummy_free;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    
    int windowBits = (Data[0] % 32) - 15;
    if (windowBits == 0) windowBits = 15;
    
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) return 0;
    
    size_t data_offset = 1;
    size_t remaining = Size - data_offset;
    
    if (remaining > 0) {
        if (Data[data_offset] & 0x01) {
            int bits = (Data[data_offset] % 17);
            int value = (remaining > 1) ? Data[data_offset + 1] : 0;
            inflatePrime(&strm, bits, value);
        }
        
        if (remaining > 2 && (Data[data_offset] & 0x02)) {
            int check = Data[data_offset + 1] & 0x01;
            inflateValidate(&strm, check);
        }
    }
    
    strm.next_in = (Bytef*)(Data + data_offset);
    strm.avail_in = (uInt)(Size - data_offset < UINT_MAX ? Size - data_offset : UINT_MAX);
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    int flush = Z_NO_FLUSH;
    if (Size > 2 && (Data[data_offset] & 0x04)) {
        flush = Z_SYNC_FLUSH;
    }
    
    while (strm.avail_in > 0 && strm.avail_out > 0) {
        ret = inflate(&strm, flush);
        if (ret == Z_STREAM_ERROR || ret == Z_NEED_DICT || ret == Z_DATA_ERROR) {
            if (ret == Z_DATA_ERROR && (Data[data_offset] & 0x08)) {
                inflateSync(&strm);
            }
            break;
        }
        if (ret == Z_STREAM_END) {
            if (Size > 3 && (Data[data_offset] & 0x10)) {
                inflateResetKeep(&strm);
                strm.next_in = (Bytef*)(Data + data_offset);
                strm.avail_in = (uInt)(Size - data_offset < UINT_MAX ? Size - data_offset : UINT_MAX);
                strm.next_out = out_buffer;
                strm.avail_out = sizeof(out_buffer);
                continue;
            }
            break;
        }
        if (ret != Z_OK && ret != Z_BUF_ERROR) break;
        
        if (strm.avail_out == 0) {
            strm.next_out = out_buffer;
            strm.avail_out = sizeof(out_buffer);
        }
    }
    
    inflateEnd(&strm);
    return 0;
}