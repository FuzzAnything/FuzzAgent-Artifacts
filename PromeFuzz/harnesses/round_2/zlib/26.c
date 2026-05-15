// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflatePrime at inflate.c:219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCodesUsed at inflate.c:1408:23 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateCodesUsed at inflate.c:1408:23 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"

static voidpf dummy_alloc(voidpf opaque, uInt items, uInt size) {
    (void)opaque;
    return malloc(items * size);
}

static void dummy_free(voidpf opaque, voidpf address) {
    (void)opaque;
    free(address);
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    z_stream strm1, strm2;
    int ret;
    unsigned long codes;
    
    /* Initialize first stream */
    strm1.zalloc = dummy_alloc;
    strm1.zfree = dummy_free;
    strm1.opaque = Z_NULL;
    strm1.next_in = (Bytef *)Data;
    strm1.avail_in = Size > 1024 ? 1024 : (uInt)Size;
    
    /* Use first byte for windowBits parameter (range -15 to 47) */
    int windowBits = (Data[0] % 63) - 15;
    
    /* Initialize inflation */
    ret = inflateInit2_(&strm1, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    /* Set up output buffer */
    Bytef out_buf[4096];
    strm1.next_out = out_buf;
    strm1.avail_out = sizeof(out_buf);
    
    /* Use second byte for flush parameter */
    int flush = Data[1] % 5;  /* Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_BLOCK */
    
    /* Try inflate with different flush modes */
    for (int i = 0; i < 3 && strm1.avail_in > 0; i++) {
        ret = inflate(&strm1, flush);
        if (ret == Z_STREAM_END || ret == Z_NEED_DICT || ret < 0) {
            break;
        }
        
        /* After Z_BLOCK, we can use inflatePrime */
        if (ret == Z_BLOCK && Size > 10) {
            /* Use third and fourth bytes for inflatePrime parameters */
            int bits = Data[2] % 17;  /* bits must be <= 16 */
            int value = Data[3];
            inflatePrime(&strm1, bits, value);
        }
    }
    
    /* Try inflateUndermine with fifth byte as parameter */
    if (Size > 5) {
        int undermine = Data[4] % 2;
        inflateUndermine(&strm1, undermine);
    }
    
    /* Try to copy the stream state */
    memset(&strm2, 0, sizeof(z_stream));
    strm2.zalloc = dummy_alloc;
    strm2.zfree = dummy_free;
    strm2.opaque = Z_NULL;
    
    ret = inflateCopy(&strm2, &strm1);
    if (ret == Z_OK) {
        /* Use the copied stream for some operations */
        strm2.next_in = strm1.next_in;
        strm2.avail_in = strm1.avail_in;
        Bytef out_buf2[2048];
        strm2.next_out = out_buf2;
        strm2.avail_out = sizeof(out_buf2);
        
        /* Try inflate on copied stream */
        if (strm2.avail_in > 0) {
            inflate(&strm2, Z_NO_FLUSH);
        }
        
        /* Get codes used from copied stream */
        codes = inflateCodesUsed(&strm2);
        (void)codes;
        
        inflateEnd(&strm2);
    }
    
    /* Get codes used from original stream */
    codes = inflateCodesUsed(&strm1);
    (void)codes;
    
    /* Try inflatePrime before resetting (if stream is in appropriate state) */
    if (Size > 8 && strm1.avail_in > 0) {
        int bits = Data[6] % 17;
        int value = Data[7];
        inflatePrime(&strm1, bits, value);
        
        /* Try one more inflate */
        if (strm1.avail_out > 0) {
            inflate(&strm1, Z_SYNC_FLUSH);
        }
    }
    
    inflateEnd(&strm1);
    
    return 0;
}