#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_009 {
/*
 * Fuzzing harness for comprehensive inflate() state machine testing and inflateBack()
 * Targets: inflate(), inflateBack(), inflateInit2_(), inflateEnd()
 * 
 * This harness focuses on deep coverage exploration of the inflate state machine
 * which has 316 blocked branches (50% blocked) and inflateBack() with 126 blocked
 * branches (37% blocked) as identified in coverage analysis.
 * 
 * Strategy:
 * 1. Use inflateInit2_() with varied windowBits:
 *    - Positive values (8-15): zlib header
 *    - Negative values: raw inflate (no header)
 *    - 16+ values: gzip header (if GUNZIP enabled)
 * 2. Call inflate() with crafted input data targeting specific state transitions
 * 3. Test scenarios:
 *    - Valid/invalid header checks
 *    - Various compression methods
 *    - Window size validations
 *    - Gzip flag processing
 *    - Dictionary ID handling
 * 4. Include inflateBack() testing with custom input/output callbacks
 * 5. Cleanup with inflateEnd()
 */


// Callback functions for inflateBack()
static unsigned inflate_back_input_callback(void FAR *desc,
                                           z_const unsigned char FAR * FAR *buf) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(desc);
    if (fdp->remaining_bytes() == 0) {
        return 0;  // No more input
    }
    
    // Get a pointer to the next chunk of data
    size_t chunk_size = fdp->ConsumeIntegralInRange<size_t>(1, 1024);
    if (chunk_size > fdp->remaining_bytes()) {
        chunk_size = fdp->remaining_bytes();
    }
    
    *buf = fdp->ConsumeBytes<uint8_t>(chunk_size).data();
    return chunk_size;
}

static int inflate_back_output_callback(void FAR *desc,
                                       unsigned char FAR *buf,
                                       unsigned len) {
    // Just consume the output, return 0 for success
    (void)desc;
    (void)buf;
    (void)len;
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for meaningful testing
    // Minimum: windowBits (1-2 bytes) + some compressed data
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume windowBits parameter with various configurations
    int windowBits = 0;
    switch (fdp.ConsumeIntegral<uint8_t>() % 6) {
        case 0:
            // Positive values 8-15: zlib header
            windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
            break;
        case 1:
            // Negative values: raw inflate (no header)
            windowBits = fdp.ConsumeIntegralInRange<int>(-15, -8);
            break;
        case 2:
            // 16+ values: gzip header (if GUNZIP enabled)
            windowBits = fdp.ConsumeIntegralInRange<int>(16, 31);
            break;
        case 3:
            // Zero: automatic header detection
            windowBits = 0;
            break;
        case 4:
            // Small positive values
            windowBits = fdp.ConsumeIntegralInRange<int>(1, 7);
            break;
        case 5:
            // Large negative values
            windowBits = fdp.ConsumeIntegralInRange<int>(-31, -16);
            break;
    }
    
    // Get remaining data as compressed input
    std::vector<uint8_t> compressed_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Initialize z_stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Set up output buffer
    const size_t out_buf_size = 32768;  // 32K window
    Bytef out_buf[32768];
    
    switch (test_scenario) {
        case 0: {
            // Basic inflate() testing with various windowBits
            
            // Initialize inflation
            int init_result = inflateInit2_(&strm, windowBits,
                                           ZLIB_VERSION, (int)sizeof(z_stream));
            if (init_result != Z_OK) {
                // Failed initialization, skip test
                return 0;
            }
            
            // Set up input
            strm.next_in = compressed_data.data();
            strm.avail_in = compressed_data.size();
            strm.next_out = out_buf;
            strm.avail_out = out_buf_size;
            
            // Try to inflate
            int inflate_result = inflate(&strm, Z_NO_FLUSH);
            
            // Keep inflating if we have more data and output space
            while (inflate_result == Z_OK && strm.avail_in > 0 && strm.avail_out > 0) {
                inflate_result = inflate(&strm, Z_NO_FLUSH);
            }
            
            // Try different flush modes with remaining data
            if (strm.avail_in > 0) {
                inflate(&strm, Z_SYNC_FLUSH);
                inflate(&strm, Z_FULL_FLUSH);
                inflate(&strm, Z_FINISH);
                inflate(&strm, Z_BLOCK);
            }
            
            // Clean up
            inflateEnd(&strm);
            break;
        }
        
        case 1: {
            // inflate() testing with dictionary
            
            // Initialize inflation
            int init_result = inflateInit2_(&strm, windowBits,
                                           ZLIB_VERSION, (int)sizeof(z_stream));
            if (init_result != Z_OK) {
                return 0;
            }
            
            // Create a simple dictionary from fuzzer data
            // Use first 256 bytes of compressed data as dictionary if available
            if (compressed_data.size() > 256) {
                Bytef dict_buf[256];
                size_t dict_size = 256;
                if (dict_size > compressed_data.size()) {
                    dict_size = compressed_data.size();
                }
                memcpy(dict_buf, compressed_data.data(), dict_size);
                
                // Try to set dictionary
                inflateSetDictionary(&strm, dict_buf, dict_size);
            }
            
            // Set up input/output
            strm.next_in = compressed_data.data();
            strm.avail_in = compressed_data.size();
            strm.next_out = out_buf;
            strm.avail_out = out_buf_size;
            
            // Inflate with various flush modes
            int inflate_result = inflate(&strm, Z_NO_FLUSH);
            
            // Try to get header if it's a gzip stream
            gz_header header;
            memset(&header, 0, sizeof(header));
            if (windowBits >= 16 || windowBits <= -16) {
                inflateGetHeader(&strm, &header);
            }
            
            // Continue inflation
            while (inflate_result == Z_OK && strm.avail_in > 0 && strm.avail_out > 0) {
                inflate_result = inflate(&strm, Z_NO_FLUSH);
            }
            
            // Clean up
            inflateEnd(&strm);
            break;
        }
        
        case 2: {
            // inflateBack() testing with custom callbacks
            
            // Need window buffer for inflateBack
            unsigned char window[32768];
            
            // Initialize for backward inflation
            int init_result = inflateBackInit_(&strm, 15, window,
                                              ZLIB_VERSION, (int)sizeof(z_stream));
            if (init_result != Z_OK) {
                return 0;
            }
            
            // Set up initial input if we have data
            if (!compressed_data.empty()) {
                strm.next_in = compressed_data.data();
                strm.avail_in = compressed_data.size();
            }
            
            // Call inflateBack with our callbacks
            // Use remaining fuzzer data through the input callback
            FuzzedDataProvider* callback_fdp = &fdp;
            int back_result = inflateBack(&strm,
                                         inflate_back_input_callback,
                                         callback_fdp,
                                         inflate_back_output_callback,
                                         NULL);
            
            // Clean up
            inflateBackEnd(&strm);
            break;
        }
        
        case 3: {
            // Comprehensive state machine testing with reset and multiple calls
            
            // Initialize
            int init_result = inflateInit2_(&strm, windowBits,
                                           ZLIB_VERSION, (int)sizeof(z_stream));
            if (init_result != Z_OK) {
                return 0;
            }
            
            // Split compressed data into chunks to test partial processing
            size_t chunk_size = compressed_data.size() / 4;
            if (chunk_size < 1) chunk_size = 1;
            
            for (int i = 0; i < 4 && i * chunk_size < compressed_data.size(); i++) {
                size_t offset = i * chunk_size;
                size_t this_chunk = chunk_size;
                if (offset + this_chunk > compressed_data.size()) {
                    this_chunk = compressed_data.size() - offset;
                }
                
                strm.next_in = compressed_data.data() + offset;
                strm.avail_in = this_chunk;
                strm.next_out = out_buf;
                strm.avail_out = out_buf_size;
                
                // Inflate this chunk
                int inflate_result = inflate(&strm, Z_NO_FLUSH);
                
                // Try reset between chunks to test state machine reset
                if (i % 2 == 0 && inflate_result != Z_STREAM_ERROR) {
                    inflateReset(&strm);
                }
            }
            
            // Test inflateReset2 to change windowBits
            if (compressed_data.size() > 100) {
                int new_windowBits = (windowBits > 0) ? -windowBits : -windowBits + 8;
                inflateReset2(&strm, new_windowBits);
                
                // Try inflation with new windowBits
                strm.next_in = compressed_data.data();
                strm.avail_in = compressed_data.size();
                strm.next_out = out_buf;
                strm.avail_out = out_buf_size;
                
                inflate(&strm, Z_FINISH);
            }
            
            // Clean up
            inflateEnd(&strm);
            break;
        }
    }
    
    return 0;
}

} // namespace harness_009
