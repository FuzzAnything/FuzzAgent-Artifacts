// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflate at inflate.c:474:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

static void cleanup_stream(z_stream *strm) {
    if (strm != NULL) {
        // inflateEnd handles freeing the internal state and window buffer.
        // We must NOT manually free strm->next_out here because:
        // 1. It was allocated by the fuzzer driver (malloc), not zlib.
        // 2. inflateEnd might have already freed internal allocations if the stream was valid.
        // 3. The previous double-free occurred because we were freeing the same buffer 
        //    allocated for 'out_buf' via strm->next_out in both 'dest' and 'strm' cleanup.
        
        if (strm->state != NULL) {
            inflateEnd(strm);
        }
        // Reset pointers to avoid dangling references, but do not free user-allocated buffers here.
        strm->next_in = Z_NULL;
        strm->next_out = Z_NULL;
        strm->zalloc = Z_NULL;
        strm->zfree = Z_NULL;
        strm->opaque = Z_NULL;
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    z_stream strm = {0};
    z_stream dest = {0};
    int ret;
    int subvert_flag;
    int bits;
    int value;
    int check;

    // Allocate output buffer. 
    // We use a single buffer for both streams to avoid complex memory management issues.
    // The buffer is owned by the fuzzer driver, not zlib.
    uint8_t *out_buf = (uint8_t *)malloc(1024);
    if (out_buf == NULL) {
        return 0;
    }

    // Initialize source stream
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    strm.avail_out = 0;
    strm.next_out = Z_NULL;

    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Set up input data from fuzzer
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = Size;

    // Set up output buffer for source stream
    strm.next_out = out_buf;
    strm.avail_out = 1024;

    // Perform some initial inflate to get into a valid state
    // We use Z_NO_FLUSH to try to process data without forcing a block end
    inflate(&strm, Z_NO_FLUSH);

    // Fuzz inflateResetKeep
    ret = inflateResetKeep(&strm);
    (void)ret;

    // Fuzz inflateUndermine
    subvert_flag = (Size > 0) ? (Data[0] % 2) : 0;
    ret = inflateUndermine(&strm, subvert_flag);
    (void)ret;

    // Fuzz inflateSync
    ret = inflateSync(&strm);
    (void)ret;

    // Fuzz inflateCopy
    dest.zalloc = Z_NULL;
    dest.zfree = Z_NULL;
    dest.opaque = Z_NULL;
    dest.next_in = Z_NULL;
    dest.next_out = Z_NULL;
    
    ret = inflateCopy(&dest, &strm);
    if (ret == Z_OK) {
        // Try to use the copied stream
        // Reuse the same output buffer for simplicity, as we are just testing state
        dest.next_in = strm.next_in;
        dest.avail_in = strm.avail_in;
        dest.next_out = out_buf;
        dest.avail_out = 1024;
        
        inflate(&dest, Z_NO_FLUSH);
        
        // Cleanup the destination stream
        // Note: We do NOT free out_buf here, as it is shared and will be freed at the end.
        cleanup_stream(&dest);
    }

    // Fuzz inflatePrime
    bits = (Size > 1) ? (Data[1] % 17) : 5;
    value = (Size > 2) ? (Data[2] % 256) : 0;
    ret = inflatePrime(&strm, bits, value);
    (void)ret;

    // Fuzz inflateValidate
    check = (Size > 3) ? (Data[3] % 2) : 1;
    ret = inflateValidate(&strm, check);
    (void)ret;

    // Cleanup source stream
    cleanup_stream(&strm);

    // Free the output buffer allocated by the driver
    free(out_buf);

    return 0;
}