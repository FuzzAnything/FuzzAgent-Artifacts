// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBack at infback.c:191:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
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

/* Structure to track input state for inflateBack callback */
typedef struct {
    const unsigned char *data;
    size_t size;
    size_t consumed;
} input_desc_t;

/* Callback functions for inflateBack */
static unsigned in_callback(void FAR *desc, z_const unsigned char FAR **buf) {
    input_desc_t *id = (input_desc_t *)desc;
    
    /* Check if we have consumed all data */
    if (id->consumed >= id->size) {
        *buf = NULL;
        return 0;
    }
    
    /* Return pointer to remaining data */
    *buf = (unsigned char FAR *)(id->data + id->consumed);
    
    /* Calculate how much to return - ensure we don't overflow */
    size_t remaining = id->size - id->consumed;
    unsigned to_return;
    
    if (remaining > 0xFFFFFFFF) {
        to_return = 0xFFFFFFFF; /* Maximum value for unsigned */
    } else {
        to_return = (unsigned)remaining;
    }
    
    /* Update consumed count */
    id->consumed += to_return;
    
    return to_return;
}

static int out_callback(void FAR *desc, unsigned char FAR *buf, unsigned len) {
    /* Simple callback that discards output */
    (void)desc;
    (void)buf;
    (void)len;
    return 0; /* Return 0 to indicate success */
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    z_stream strm, strm_back;
    int windowBits = 15; /* Valid window size between 8-15 */
    unsigned char *window = NULL;
    int ret;
    input_desc_t input_desc;
    
    /* If we have no data, just return */
    if (Size == 0) {
        return 0;
    }
    
    /* Initialize for inflate */
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    /* Step 1: inflateInit2_ */
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        /* Clean up and return if initialization fails */
        inflateEnd(&strm);
        return 0;
    }
    
    /* Set up input data for inflate */
    strm.next_in = (Bytef *)Data;
    /* Ensure we don't overflow uInt */
    strm.avail_in = (uInt)(Size > UINT_MAX ? UINT_MAX : Size);
    
    /* Allocate output buffer */
    #define OUT_BUF_SIZE 4096
    unsigned char out_buf[OUT_BUF_SIZE];
    strm.next_out = out_buf;
    strm.avail_out = OUT_BUF_SIZE;
    
    /* Step 2: inflate */
    ret = inflate(&strm, Z_NO_FLUSH);
    /* We don't care about the return value for fuzzing purposes */
    
    /* Step 3: inflateEnd */
    inflateEnd(&strm);
    
    /* Initialize for inflateBack */
    memset(&strm_back, 0, sizeof(strm_back));
    strm_back.zalloc = Z_NULL;
    strm_back.zfree = Z_NULL;
    strm_back.opaque = Z_NULL;
    
    /* Allocate window for inflateBack */
    window = (unsigned char *)malloc(1 << windowBits);
    if (window == NULL) {
        return 0;
    }
    
    /* Step 4: inflateBackInit_ */
    ret = inflateBackInit_(&strm_back, windowBits, window, 
                          ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(window);
        return 0;
    }
    
    /* Set up input descriptor for inflateBack callback */
    input_desc.data = Data;
    input_desc.size = Size;
    input_desc.consumed = 0;
    
    /* Step 5: inflateBack */
    /* Note: We use input_desc as the input descriptor for the callback */
    ret = inflateBack(&strm_back, in_callback, (void FAR *)&input_desc,
                     out_callback, Z_NULL);
    /* We don't care about the return value for fuzzing purposes */
    
    /* Step 6: inflateBackEnd */
    inflateBackEnd(&strm_back);
    
    /* Clean up allocated memory */
    free(window);
    
    return 0;
}