#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_023 {
/*
 * Fuzzing harness for zlib gzip header compression logic in deflate() function
 * Target: Unlock 103 blocked branches in deflate() function focusing on gzip header logic
 * 
 * Based on coverage analysis guidance at /root/FuzzAgent/output/zlib/coverage_guidance/harness_023_guidance.md
 * 
 * Focus: EXTRA_STATE, NAME_STATE, COMMENT_STATE code paths in deflate() that currently have 0 coverage.
 * These code paths handle gzip header fields (extra data, filename, comment) and are only executed 
 * when s->gzhead != NULL.
 * 
 * Target APIs:
 * - deflateInit2_ with windowBits = MAX_WBITS + 16 for gzip encoding
 * - deflateSetHeader to set gzip header with various configurations
 * - deflate with multiple calls and various flush modes (Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FINISH)
 * - deflateEnd for cleanup
 * 
 * Configurations:
 * - Header with extra field only
 * - Header with filename only  
 * - Header with comment only
 * - Header with all fields (extra + filename + comment)
 * - hcrc enabled/disabled
 * - text flag enabled/disabled
 * 
 * Test patterns:
 * - Small inputs with Z_NO_FLUSH
 * - Larger inputs with Z_SYNC_FLUSH  
 * - Final Z_FINISH flush
 * - Edge cases like zero-length input, repeated calls after Z_FINISH
 * Key insight: The condition at deflate.c:1075 (if (s->gzhead == Z_NULL)) shows 49 hits on true branch 
 * (gzhead is NULL) and 0 hits on false branch (gzhead is not NULL). This harness aims to 
 * exercise the false branch and related gzip header writing logic.
 *
 * FIXED: 2025-01-01 - Use actual vector/string sizes (extra_data.size(), name_str.size(), comment_str.size())
 * instead of requested sizes to match zlib API contract and prevent buffer overruns.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration and test data
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int level = fdp.ConsumeIntegralInRange<int>(0, 9);
    int memLevel = fdp.ConsumeIntegralInRange<int>(1, 9);
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Operation selection flags
    uint8_t header_config = fdp.ConsumeIntegral<uint8_t>() % 6;
    bool test_hcrc = fdp.ConsumeBool();
    bool test_text = fdp.ConsumeBool();
    
    // Flush mode selection
    uint8_t flush_mode_idx = fdp.ConsumeIntegral<uint8_t>() % 4;
    int flush_mode;
    switch (flush_mode_idx) {
        case 0: flush_mode = Z_NO_FLUSH; break;
        case 1: flush_mode = Z_SYNC_FLUSH; break;
        case 2: flush_mode = Z_FULL_FLUSH; break;
        case 3: flush_mode = Z_FINISH; break;
        default: flush_mode = Z_NO_FLUSH; break;
    }
    
    // Consume test data for compression
    size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(0, 4096);
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
    
    // Consume header field data
    size_t extra_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::vector<uint8_t> extra_data = fdp.ConsumeBytes<uint8_t>(extra_size);
    
    size_t name_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string name_str = fdp.ConsumeBytesAsString(name_size);
    if (name_size > 0 && name_str.back() != '\0') {
        name_str += '\0';  // Ensure null termination for filename
    }
    
    size_t comment_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string comment_str = fdp.ConsumeBytesAsString(comment_size);
    if (comment_size > 0 && comment_str.back() != '\0') {
        comment_str += '\0';  // Ensure null termination for comment
    }
    
    // Initialize z_stream for deflate
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    // Use windowBits = MAX_WBITS + 16 for gzip encoding
    int windowBits = 15 + 16;  // MAX_WBITS (15) + 16 for gzip
    
    // Initialize for gzip compression
    int init_ret = deflateInit2(&deflate_strm, level, Z_DEFLATED, windowBits, memLevel, strategy);
    if (init_ret != Z_OK) {
        return 0;  // Initialization failed
    }
    
    // Setup gzip header based on configuration
    gz_header header;
    memset(&header, 0, sizeof(header));
    
    switch (header_config) {
        case 0:  // Header with extra field only
            if (extra_size > 0) {
                header.extra = extra_data.data();
                header.extra_len = (uInt)extra_data.size();
                header.extra_max = (uInt)extra_data.size();
            }
            break;
        case 1:  // Header with filename only
            if (name_size > 0) {
                header.name = (Bytef*)name_str.c_str();
                header.name_max = (uInt)name_str.size();  // Use actual string size including null terminator
            }
            break;
        case 2:  // Header with comment only
            if (comment_size > 0) {
                header.comment = (Bytef*)comment_str.c_str();
                header.comm_max = (uInt)comment_str.size();  // Use actual string size including null terminator
            }
            break;
        case 3:  // Header with extra + filename
            if (extra_size > 0) {
                header.extra = extra_data.data();
                header.extra_len = (uInt)extra_data.size();
                header.extra_max = (uInt)extra_data.size();
            }
            if (name_size > 0) {
                header.name = (Bytef*)name_str.c_str();
                header.name_max = (uInt)name_str.size();
            }
            break;
            
        case 4:  // Header with extra + comment
            if (extra_size > 0) {
                header.extra = extra_data.data();
                header.extra_len = (uInt)extra_data.size();
                header.extra_max = (uInt)extra_data.size();
            }
            if (comment_size > 0) {
                header.comment = (Bytef*)comment_str.c_str();
                header.comm_max = (uInt)comment_str.size();
            }
            break;
            
        case 5:  // Header with all fields (extra + filename + comment)
            if (extra_size > 0) {
                header.extra = extra_data.data();
                header.extra_len = (uInt)extra_data.size();
                header.extra_max = (uInt)extra_data.size();
            }
            if (name_size > 0) {
                header.name = (Bytef*)name_str.c_str();
                header.name_max = (uInt)name_str.size();
            }
            if (comment_size > 0) {
                header.comment = (Bytef*)comment_str.c_str();
                header.comm_max = (uInt)comment_str.size();
            }
            break;
    }
    
    // Set header flags
    header.text = test_text ? 1 : 0;
    header.hcrc = test_hcrc ? 1 : 0;
    header.time = fdp.ConsumeIntegral<uLong>();
    header.os = fdp.ConsumeIntegralInRange<int>(0, 255);
    header.xflags = fdp.ConsumeIntegralInRange<int>(0, 255);
    
    // Set the header before starting compression
    int header_ret = deflateSetHeader(&deflate_strm, &header);
    if (header_ret != Z_OK) {
        deflateEnd(&deflate_strm);
        return 0;
    }
    
    // Prepare output buffer
    size_t output_size = deflateBound(&deflate_strm, (uLong)test_data.size());
    if (output_size == 0) {
        output_size = 4096;  // Fallback size
    }
    
    Bytef* output_buffer = (Bytef*)malloc(output_size);
    if (!output_buffer) {
        deflateEnd(&deflate_strm);
        return 0;
    }
    
    // Set up input/output pointers
    deflate_strm.next_in = test_data.empty() ? Z_NULL : test_data.data();
    deflate_strm.avail_in = (uInt)test_data.size();
    deflate_strm.next_out = output_buffer;
    deflate_strm.avail_out = (uInt)output_size;
    
    // Perform compression with specified flush mode
    int deflate_ret = deflate(&deflate_strm, flush_mode);
    
    // Handle different flush modes and edge cases
    if (deflate_ret == Z_OK || deflate_ret == Z_BUF_ERROR) {
        // Try additional operations based on remaining input
        
        // If we used Z_NO_FLUSH and have input left, try Z_SYNC_FLUSH
        if (flush_mode == Z_NO_FLUSH && deflate_strm.avail_in > 0) {
            deflate_ret = deflate(&deflate_strm, Z_SYNC_FLUSH);
        }
        
        // Try Z_FINISH to complete the stream
        if (deflate_ret == Z_OK || deflate_ret == Z_BUF_ERROR) {
            deflate_strm.avail_in = 0;
            deflate_ret = deflate(&deflate_strm, Z_FINISH);
        }
        
        // Edge case: try calling deflate again after Z_FINISH
        if (deflate_ret == Z_STREAM_END) {
            // Should return Z_STREAM_ERROR or Z_BUF_ERROR when called after finish
            int post_finish_ret = deflate(&deflate_strm, Z_NO_FLUSH);
            (void)post_finish_ret;  // Ignore, just testing edge case
        }
        
        // Test zero-length input case
        if (test_data.empty()) {
            // Reset stream for zero-length test
            deflateReset(&deflate_strm);
            deflate_strm.next_in = Z_NULL;
            deflate_strm.avail_in = 0;
            deflate_strm.next_out = output_buffer;
            deflate_strm.avail_out = (uInt)output_size;
            
            int zero_ret = deflate(&deflate_strm, Z_FINISH);
            (void)zero_ret;  // Just testing
        }
    }
    
    // Test with different header configurations in a separate stream
    // to ensure multiple gzip header variations are tested
    if (fdp.remaining_bytes() > 100) {
        z_stream alt_strm;
        memset(&alt_strm, 0, sizeof(z_stream));
        alt_strm.zalloc = Z_NULL;
        alt_strm.zfree = Z_NULL;
        alt_strm.opaque = Z_NULL;
        
        // Try with negative windowBits for raw deflate (should not accept header)
        int raw_init = deflateInit2(&alt_strm, level, Z_DEFLATED, -windowBits, memLevel, strategy);
        if (raw_init == Z_OK) {
            gz_header raw_header;
            memset(&raw_header, 0, sizeof(raw_header));
            raw_header.text = 1;
            raw_header.name = (Bytef*)"test.txt";
            
            // This should fail with raw deflate
            int raw_header_ret = deflateSetHeader(&alt_strm, &raw_header);
            (void)raw_header_ret;  // Test the error case
            
            deflateEnd(&alt_strm);
        }
    }
    
    // Clean up
    free(output_buffer);
    deflateEnd(&deflate_strm);
    
    return 0;
}

} // namespace harness_023
