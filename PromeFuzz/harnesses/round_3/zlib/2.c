// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
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

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    z_stream strm, dest_strm;
    gz_header header;
    int ret;
    
    /* Initialize z_stream structures */
    memset(&strm, 0, sizeof(z_stream));
    memset(&dest_strm, 0, sizeof(z_stream));
    memset(&header, 0, sizeof(gz_header));
    
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)(Size > 0xFFFFFFFF ? 0xFFFFFFFF : Size);
    
    dest_strm.zalloc = Z_NULL;
    dest_strm.zfree = Z_NULL;
    dest_strm.opaque = Z_NULL;
    
    /* 1. inflateInit2_ */
    ret = inflateInit2_(&strm, 15 + 16, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&strm);
        return 0;
    }
    
    /* 2. inflateGetHeader */
    header.extra_max = 65536;
    header.name_max = 65536;
    header.comm_max = 65536;
    inflateGetHeader(&strm, &header);
    
    /* 3. inflate */
    Bytef out_buf[4096];
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);
    inflate(&strm, Z_NO_FLUSH);
    
    /* 4-6. inflateSetDictionary (three times) */
    static const Bytef dict1[] = "test dictionary 1";
    static const Bytef dict2[] = "test dictionary 2";
    static const Bytef dict3[] = "test dictionary 3";
    
    inflateSetDictionary(&strm, dict1, sizeof(dict1) - 1);
    inflateSetDictionary(&strm, dict2, sizeof(dict2) - 1);
    inflateSetDictionary(&strm, dict3, sizeof(dict3) - 1);
    
    /* 7. inflate */
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);
    inflate(&strm, Z_FINISH);
    
    /* 8. inflateCopy */
    inflateCopy(&dest_strm, &strm);
    
    /* 9. inflateEnd */
    inflateEnd(&strm);
    
    /* 10. inflateReset2 */
    inflateReset2(&dest_strm, 15);
    
    /* 11. inflateEnd */
    inflateEnd(&dest_strm);
    
    return 0;
}