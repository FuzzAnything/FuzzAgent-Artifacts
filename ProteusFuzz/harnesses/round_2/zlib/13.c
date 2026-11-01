// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    z_stream strm = {0};
    z_stream dest = {0};
    z_stream source = {0};
    
    uint8_t *in_buf = NULL;
    uint8_t *out_buf = NULL;
    size_t in_size = 0;
    size_t out_size = 0;
    int ret;

    // Parse input to determine behavior
    // First byte: operation selector
    // Second byte: windowBits for inflateInit2_
    uint8_t op = Data[0];
    int windowBits = (int)Data[1];
    
    // Allocate buffers for data
    if (Size > 2) {
        in_size = Size - 2;
        in_buf = (uint8_t *)malloc(in_size);
        if (!in_buf) return 0;
        memcpy(in_buf, Data + 2, in_size);
    }
    
    // Output buffer size
    out_size = 65536;
    out_buf = (uint8_t *)malloc(out_size);
    if (!out_buf) {
        free(in_buf);
        return 0;
    }

    // Initialize source stream for copy operations
    ret = inflateInit2_(&source, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(in_buf);
        free(out_buf);
        return 0;
    }
    
    // Feed some data to source to make it active
    source.next_in = in_buf;
    source.avail_in = in_size;
    source.next_out = out_buf;
    source.avail_out = out_size;
    
    ret = inflate(&source, Z_NO_FLUSH);
    // Ignore return value here as we just want a state to copy

    // Initialize main stream
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&source);
        free(in_buf);
        free(out_buf);
        return 0;
    }

    // Set up input for main stream
    strm.next_in = in_buf;
    strm.avail_in = in_size;
    strm.next_out = out_buf;
    strm.avail_out = out_size;

    // Execute target API functions based on operation selector
    switch (op) {
        case 0:
            // inflateUndermine
            // Try with subvert = 0 and 1
            inflateUndermine(&strm, 0);
            inflateUndermine(&strm, 1);
            break;
            
        case 1:
            // inflateSync
            // Call after potential error or just to test state
            inflateSync(&strm);
            break;
            
        case 2:
            // inflateSyncPoint
            // Check if at sync point
            (void)inflateSyncPoint(&strm);
            break;
            
        case 3:
            // inflatePrime
            // Try various bit values
            inflatePrime(&strm, 0, 0);
            inflatePrime(&strm, 1, 1);
            inflatePrime(&strm, 8, 0xFF);
            inflatePrime(&strm, 16, 0xFFFF);
            inflatePrime(&strm, -1, 0); // Clear buffer
            inflatePrime(&strm, 32, 0); // Should fail (too many bits)
            break;
            
        case 4:
            // inflateCopy
            // dest is already zeroed, source is initialized
            ret = inflateCopy(&dest, &source);
            if (ret == Z_OK) {
                inflateEnd(&dest);
            }
            break;
            
        default:
            // General inflate run to exercise state
            ret = inflate(&strm, Z_NO_FLUSH);
            if (ret == Z_STREAM_END || ret == Z_OK) {
                // Try sync point check after successful inflate
                (void)inflateSyncPoint(&strm);
            } else if (ret == Z_DATA_ERROR) {
                // Try sync recovery
                inflateSync(&strm);
            }
            break;
    }

    // Cleanup
    inflateEnd(&strm);
    inflateEnd(&source);
    free(in_buf);
    free(out_buf);

    return 0;
}