// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_z_stream(z_streamp strm, const uint8_t *data, size_t size) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = (Bytef*)data;
    strm->avail_in = (uInt)size;
    strm->next_out = NULL;
    strm->avail_out = 0;
    strm->total_in = 0;
    strm->total_out = 0;
    strm->msg = NULL;
    strm->data_type = 0;
    strm->adler = 0;
    strm->reserved = 0;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    z_stream strm;
    int ret;
    uint8_t *out_buffer = NULL;
    size_t out_size = 1024;
    
    /* Initialize with random windowBits from fuzzer data */
    int windowBits = (int)Data[0] % 31 - 15; /* Range: -15 to 15 */
    init_z_stream(&strm, Data + 1, Size - 1);
    
    /* Test inflateInit2_ */
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0; /* Initialization failed, skip this input */
    }
    
    /* Test inflateValidate with random check value */
    int check = Data[1] & 1;
    inflateValidate(&strm, check);
    
    /* Test inflatePrime with random bits and value */
    int bits = Data[2] % 17; /* 0-16 bits */
    int value = Data[3] | (Data[4] << 8);
    inflatePrime(&strm, bits, value);
    
    /* Allocate output buffer and attempt decompression */
    out_buffer = (uint8_t*)malloc(out_size);
    if (!out_buffer) {
        inflateEnd(&strm);
        return 0;
    }
    strm.next_out = out_buffer;
    strm.avail_out = (uInt)out_size;
    
    /* Attempt decompression to trigger potential errors */
    ret = inflate(&strm, Z_NO_FLUSH);
    
    /* Test inflateSync if we got a data error */
    if (ret == Z_DATA_ERROR) {
        inflateSync(&strm);
    }
    
    /* Test inflateSyncPoint */
    inflateSyncPoint(&strm);
    
    /* Test inflateResetKeep */
    if (inflateResetKeep(&strm) == Z_OK) {
        /* Reset input/output for another round */
        strm.next_in = Data + (Size/2);
        strm.avail_in = (uInt)(Size - Size/2);
        strm.next_out = out_buffer;
        strm.avail_out = (uInt)out_size;
        
        /* Try decompression again */
        inflate(&strm, Z_NO_FLUSH);
    }
    
    /* Cleanup */
    free(out_buffer);
    inflateEnd(&strm);
    
    return 0;
}