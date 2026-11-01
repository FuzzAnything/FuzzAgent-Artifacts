// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

static void cleanup_stream(z_stream *strm, uint8_t *out_buf) {
    if (strm != NULL) {
        if (strm->state != NULL) {
            inflateEnd(strm);
        }
    }
    // Only free out_buf if it hasn't been freed by inflateEnd or elsewhere
    // We pass the pointer to ensure we free it exactly once in the main function
    if (out_buf != NULL) {
        free(out_buf);
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Need enough data to safely access indices up to 7
    if (Size < 8) {
        return 0;
    }

    z_stream strm = {0};
    z_stream dest = {0};
    int ret;

    // Allocate output buffer
    uint8_t *out_buf = (uint8_t *)malloc(1024);
    if (out_buf == NULL) {
        return 0;
    }

    // Initialize stream
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = Size;
    strm.next_out = out_buf;
    strm.avail_out = 1024;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    if (inflateInit(&strm) != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Fuzz inflatePrime
    // bits: 0 to 16, or negative to clear
    int bits = (Data[0] % 18) - 2; // Range: -2 to 15
    int value = Data[1];
    ret = inflatePrime(&strm, bits, value);
    (void)ret; // Ignore return for fuzzing state exploration

    // Fuzz inflateValidate
    int check = Data[2] % 2;
    ret = inflateValidate(&strm, check);
    (void)ret;

    // Fuzz inflateUndermine
    int subvert = Data[3] % 2;
    ret = inflateUndermine(&strm, subvert);
    (void)ret;

    // Fuzz inflateCopy
    dest.zalloc = Z_NULL;
    dest.zfree = Z_NULL;
    dest.opaque = Z_NULL;
    ret = inflateCopy(&dest, &strm);
    if (ret == Z_OK) {
        // Perform some operations on dest to exercise its state
        uint8_t *dest_out_buf = (uint8_t *)malloc(1024);
        if (dest_out_buf != NULL) {
            dest.next_out = dest_out_buf;
            dest.avail_out = 1024;
            dest.next_in = strm.next_in;
            dest.avail_in = strm.avail_in;
            
            // Try inflating a bit on the copy
            inflate(&dest, Z_NO_FLUSH);
            
            // Fuzz inflateValidate on dest
            ret = inflateValidate(&dest, Data[4] % 2);
            (void)ret;
            
            // Fuzz inflateUndermine on dest
            ret = inflateUndermine(&dest, Data[5] % 2);
            (void)ret;
            
            // Fuzz inflatePrime on dest
            int bits_dest = (Data[6] % 18) - 2;
            int value_dest = Data[7];
            ret = inflatePrime(&dest, bits_dest, value_dest);
            (void)ret;

            free(dest_out_buf);
            dest.next_out = NULL;
        }
        // Cleanup dest
        inflateEnd(&dest);
    }

    // Fuzz inflateResetKeep
    ret = inflateResetKeep(&strm);
    (void)ret;

    // Fuzz inflateSync
    // Try to sync, which might fail or succeed depending on data
    ret = inflateSync(&strm);
    (void)ret;

    // Cleanup
    // inflateEnd handles freeing the internal state. 
    // We must NOT free strm.next_out here because inflateEnd might have 
    // already handled it or we might have passed ownership implicitly.
    // However, in our setup, strm.next_out points to out_buf which we allocated.
    // inflateEnd does NOT free the user-provided next_out buffer.
    // So we must free out_buf manually, but ensure we don't double free.
    
    if (strm.state != NULL) {
        inflateEnd(&strm);
    }
    
    // Free the output buffer we allocated
    free(out_buf);

    return 0;
}