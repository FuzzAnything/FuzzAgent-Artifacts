// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
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

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    z_stream inflate_stream;
    z_stream deflate_stream;
    int ret;
    uLong crc1, crc2;
    
    // Initialize inflate stream
    memset(&inflate_stream, 0, sizeof(inflate_stream));
    inflate_stream.zalloc = dummy_alloc;
    inflate_stream.zfree = dummy_free;
    
    // Call inflateInit2_
    ret = inflateInit2_(&inflate_stream, 15, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Call crc32 (first time)
    crc1 = crc32(0L, Z_NULL, 0);
    if (Size > 0) {
        crc1 = crc32(crc1, Data, Size > 1024 ? 1024 : (uInt)Size);
    }
    
    // Prepare inflate input
    if (Size > 0) {
        inflate_stream.next_in = (Bytef*)Data;
        inflate_stream.avail_in = Size > 1024 ? 1024 : (uInt)Size;
        
        // Prepare output buffer
        Bytef out_buf[1024];
        inflate_stream.next_out = out_buf;
        inflate_stream.avail_out = sizeof(out_buf);
        
        // Call inflate
        ret = inflate(&inflate_stream, Z_NO_FLUSH);
        // Ignore return value for fuzzing
    }
    
    // Call crc32 (second time) - use output if available
    crc2 = crc32(0L, Z_NULL, 0);
    if (inflate_stream.total_out > 0 && inflate_stream.next_out != NULL) {
        size_t out_size = inflate_stream.total_out > 1024 ? 1024 : inflate_stream.total_out;
        crc2 = crc32(crc2, inflate_stream.next_out - out_size, (uInt)out_size);
    }
    
    // Call inflateEnd
    ret = inflateEnd(&inflate_stream);
    
    // Initialize deflate stream
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    deflate_stream.zalloc = dummy_alloc;
    deflate_stream.zfree = dummy_free;
    
    // Call deflateInit2_
    ret = deflateInit2_(&deflate_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        15, 8, Z_DEFAULT_STRATEGY,
                        ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Call deflateSetDictionary
    if (Size > 0) {
        size_t dict_size = Size > 256 ? 256 : Size;
        ret = deflateSetDictionary(&deflate_stream, Data, (uInt)dict_size);
    }
    
    // Call deflatePrime
    if (Size > 2) {
        int bits = Data[0] & 0x0F;  // 0-15 bits
        int value = Data[1];
        if (bits > 0) {
            ret = deflatePrime(&deflate_stream, bits, value);
        }
    }
    
    // Clean up deflate stream
    deflateEnd(&deflate_stream);
    
    return 0;
}