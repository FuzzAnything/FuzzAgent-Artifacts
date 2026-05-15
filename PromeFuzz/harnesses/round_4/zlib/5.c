// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
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

static void init_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
    strm->next_out = Z_NULL;
    strm->avail_out = 0;
    strm->msg = Z_NULL;
    strm->data_type = 0;
    strm->adler = 0;
    strm->reserved = 0;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    z_stream stream;
    gz_header header;
    z_stream dest_stream;
    
    /* Initialize structures */
    memset(&stream, 0, sizeof(stream));
    memset(&header, 0, sizeof(header));
    memset(&dest_stream, 0, sizeof(dest_stream));
    
    init_stream(&stream);
    init_stream(&dest_stream);
    
    /* Prepare input data for decompression */
    Bytef *in_data = (Bytef*)malloc(Size);
    if (!in_data) return 0;
    memcpy(in_data, Data, Size);
    
    Bytef out_buf[4096];
    
    stream.next_in = in_data;
    stream.avail_in = (uInt)(Size > UINT_MAX ? UINT_MAX : Size);
    stream.next_out = out_buf;
    stream.avail_out = sizeof(out_buf);
    
    /* 1. inflateInit2_ */
    int windowBits = (Size > 0) ? (Data[0] % 31) - 15 : 15;
    if (windowBits == 0) windowBits = 15;
    int ret = inflateInit2_(&stream, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(in_data);
        return 0;
    }
    
    /* 2. inflateGetHeader */
    header.extra = Z_NULL;
    header.name = Z_NULL;
    header.comment = Z_NULL;
    header.extra_max = 0;
    header.name_max = 0;
    header.comm_max = 0;
    inflateGetHeader(&stream, &header);
    
    /* 3. inflate */
    int flush = (Size > 1) ? (Data[1] % 4) : Z_NO_FLUSH;
    inflate(&stream, flush);
    
    /* 4-6. inflateSetDictionary (three times) */
    const Bytef *dict = (const Bytef*)Data;
    uInt dict_len = (uInt)(Size > 100 ? 100 : Size);
    if (dict_len > 0) {
        inflateSetDictionary(&stream, dict, dict_len);
        inflateSetDictionary(&stream, dict, dict_len/2);
        inflateSetDictionary(&stream, dict, dict_len/4);
    }
    
    /* 7. inflate */
    flush = (Size > 2) ? (Data[2] % 4) : Z_NO_FLUSH;
    inflate(&stream, flush);
    
    /* 8. inflateCopy */
    inflateCopy(&dest_stream, &stream);
    
    /* 9. inflateEnd */
    inflateEnd(&stream);
    
    /* 10. inflateReset2 */
    windowBits = (Size > 3) ? (Data[3] % 31) - 15 : 15;
    if (windowBits == 0) windowBits = 15;
    inflateReset2(&dest_stream, windowBits);
    
    /* 11. inflateEnd */
    inflateEnd(&dest_stream);
    
    /* Cleanup */
    free(in_data);
    return 0;
}