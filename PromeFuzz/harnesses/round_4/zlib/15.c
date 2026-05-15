// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Initialize z_stream
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;

    // Initialize decompression with default window bits (15)
    const char *version = ZLIB_VERSION;
    int init_result = inflateInit_(&strm, version, sizeof(z_stream));
    if (init_result != Z_OK) {
        return 0;
    }

    // Set up input data for the stream
    // Use first half of data for initial input, second half for potential additional operations
    size_t half_size = Size / 2;
    const uint8_t *input_data = Data;
    size_t input_size = half_size;
    
    // If we have at least some data, set it as input
    if (input_size > 0) {
        strm.next_in = (Bytef *)input_data;
        strm.avail_in = (uInt)input_size;
        
        // Set up output buffer
        Bytef out_buffer[4096];
        strm.next_out = out_buffer;
        strm.avail_out = sizeof(out_buffer);
        
        // Try to inflate a bit to get stream into different state
        int inflate_result = inflate(&strm, Z_NO_FLUSH);
        // We don't care about the result - just want to change stream state
    }

    // Use remaining data to control which functions to call and with what parameters
    if (Size - half_size >= 4) {
        const uint8_t *control_data = Data + half_size;
        size_t control_size = Size - half_size;
        
        // Use first byte to determine which operations to perform
        uint8_t operation_mask = control_data[0];
        
        // Use second and third bytes for parameters
        int bits_param = (control_data[1] % 17); // bits must be 0-16
        int value_param = control_data[2];
        int check_param = control_data[3] % 2; // 0 or 1 for inflateValidate
        
        // Reset stream to initial state
        if (operation_mask & 0x01) {
            inflateReset(&strm);
        }
        
        // Reset while keeping dictionary/window
        if (operation_mask & 0x02) {
            inflateResetKeep(&strm);
        }
        
        // Insert bits into input stream
        if (operation_mask & 0x04) {
            // Try both positive and negative bits values
            inflatePrime(&strm, bits_param, value_param);
            // Also try negative bits to clear buffer
            if (bits_param > 0) {
                inflatePrime(&strm, -bits_param, value_param);
            }
        }
        
        // Enable/disable CRC validation
        if (operation_mask & 0x08) {
            inflateValidate(&strm, check_param);
            // Try the opposite value too
            inflateValidate(&strm, 1 - check_param);
        }
        
        // Try to sync after potential error
        if (operation_mask & 0x10) {
            // First, try to put stream in a state where sync might be needed
            // by attempting to inflate some more data
            if (control_size > 4) {
                strm.next_in = (Bytef *)(control_data + 4);
                strm.avail_in = (uInt)(control_size - 4);
                Bytef dummy_out[1024];
                strm.next_out = dummy_out;
                strm.avail_out = sizeof(dummy_out);
                
                int result = inflate(&strm, Z_SYNC_FLUSH);
                // If we got a data error, try sync
                if (result == Z_DATA_ERROR) {
                    inflateSync(&strm);
                }
            }
        }
        
        // Try inflateSync even without explicit error
        if (operation_mask & 0x20) {
            inflateSync(&strm);
        }
    }

    // Clean up
    inflateEnd(&strm);
    
    return 0;
}

#ifdef __cplusplus
}
#endif