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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    z_stream strm;
    z_stream strm_copy;
    gz_header header;
    int ret;
    static Bytef out_buffer[4096];
    static Bytef dict_buffer[4096];
    
    /* Initialize structures */
    memset(&strm, 0, sizeof(z_stream));
    memset(&strm_copy, 0, sizeof(z_stream));
    memset(&header, 0, sizeof(gz_header));
    
    /* Set up input data */
    strm.next_in = (Bytef *)Data;
    strm.avail_in = Size > UINT_MAX ? UINT_MAX : (uInt)Size;
    
    /* Set up output buffer */
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    /* 1. inflateInit2_ */
    ret = inflateInit2_(&strm, 15 + 16, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        /* If initialization fails, clean up and return */
        inflateEnd(&strm);
        return 0;
    }
    
    /* 2. inflateGetHeader */
    inflateGetHeader(&strm, &header);
    
    /* 3. inflate */
    inflate(&strm, Z_NO_FLUSH);
    
    /* Prepare dictionary data - use first part of input if available */
    size_t dict_size = Size > sizeof(dict_buffer) ? sizeof(dict_buffer) : Size;
    if (dict_size > 0 && Data != NULL) {
        memcpy(dict_buffer, Data, dict_size);
    } else {
        /* Use some default data if input is empty */
        const char default_dict[] = "test dictionary";
        memcpy(dict_buffer, default_dict, sizeof(default_dict));
        dict_size = sizeof(default_dict);
    }
    
    /* 4. inflateSetDictionary (first call) */
    inflateSetDictionary(&strm, dict_buffer, dict_size > UINT_MAX ? UINT_MAX : (uInt)dict_size);
    
    /* 5. inflateSetDictionary (second call) - use different size */
    size_t dict_size2 = dict_size > 10 ? dict_size - 10 : dict_size;
    inflateSetDictionary(&strm, dict_buffer, dict_size2 > UINT_MAX ? UINT_MAX : (uInt)dict_size2);
    
    /* 6. inflateSetDictionary (third call) - use minimal size */
    size_t dict_size3 = dict_size > 1 ? 1 : dict_size;
    inflateSetDictionary(&strm, dict_buffer, dict_size3 > UINT_MAX ? UINT_MAX : (uInt)dict_size3);
    
    /* 7. inflate */
    inflate(&strm, Z_SYNC_FLUSH);
    
    /* 8. inflateCopy */
    inflateCopy(&strm_copy, &strm);
    
    /* 9. inflateEnd (on original stream) */
    inflateEnd(&strm);
    
    /* 10. inflateReset2 */
    inflateReset2(&strm_copy, 15);
    
    /* 11. inflateEnd (on copied stream) */
    inflateEnd(&strm_copy);
    
    return 0;
}