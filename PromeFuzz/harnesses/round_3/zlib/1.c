// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    z_stream deflate_strm, inflate_strm;
    int ret;
    
    /* Initialize streams to zero */
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    /* Set default allocators */
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    
    /* 1. deflateInit_ */
    ret = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION,
                      ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        /* If initialization fails, clean up and return */
        deflateEnd(&deflate_strm);
        return 0;
    }
    
    /* 2. deflateEnd */
    deflateEnd(&deflate_strm);
    
    /* 3. inflateInit_ */
    ret = inflateInit_(&inflate_strm, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&inflate_strm);
        return 0;
    }
    
    /* 4. deflateReset - Note: deflate_strm was already ended, may fail */
    deflateReset(&deflate_strm);
    
    /* 5. inflateReset */
    inflateReset(&inflate_strm);
    
    /* 6. deflateReset - Second call */
    deflateReset(&deflate_strm);
    
    /* 7. inflateEnd */
    inflateEnd(&inflate_strm);
    
    /* 8. deflateEnd - Second call */
    deflateEnd(&deflate_strm);
    
    /* Additional test: Try with different initialization states */
    if (Size > 0) {
        /* Reinitialize with different compression level based on input */
        int level = Data[0] % 10; /* 0-9 */
        ret = deflateInit_(&deflate_strm, level,
                          ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            deflateEnd(&deflate_strm);
        }
        
        /* Write dummy file for potential file-based operations */
        write_dummy_file(Data, Size);
    }
    
    return 0;
}