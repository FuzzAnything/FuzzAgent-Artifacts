#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>
#include <algorithm>

namespace harness_011 {
/*
 * Fuzzing harness for zlib inflateBack stored block processing
 * Targets inflateBack's stored block (type 0) processing with 142 blocked branches
 * Focuses on generating valid stored blocks with proper length fields
 * where (length) == (~length ^ 0xffff) to exercise stored block paths
 */


// Callback context structures
struct InputContext {
    const uint8_t* data;
    size_t size;
    size_t pos;
    bool should_fail;
    size_t max_return_size;
};

struct OutputContext {
    std::vector<uint8_t> buffer;
    size_t max_size;
    bool should_fail;
    size_t fail_after_bytes;
    size_t bytes_written;
};

// Input callback function for inflateBack
static unsigned input_callback(void* desc, z_const unsigned char FAR* FAR* buf) {
    InputContext* ctx = (InputContext*)desc;
    
    if (ctx->should_fail) {
        return 0;  // Simulate input failure
    }
    
    if (ctx->pos >= ctx->size) {
        return 0;  // No more data available
    }
    
    // Return pointer to remaining data
    *buf = (z_const unsigned char FAR*)&ctx->data[ctx->pos];
    
    // Determine how much to return (1 to max_return_size bytes at a time)
    size_t available = ctx->size - ctx->pos;
    size_t to_return = std::min(available, ctx->max_return_size);
    
    ctx->pos += to_return;
    return (unsigned)to_return;
}

// Output callback function for inflateBack
static int output_callback(void* desc, unsigned char FAR* buf, unsigned len) {
    OutputContext* ctx = (OutputContext*)desc;
    
    if (ctx->should_fail) {
        return 1;  // Simulate output failure
    }
    
    // Check if we would exceed max buffer size
    if (ctx->buffer.size() + len > ctx->max_size) {
        return 1;  // Buffer overflow
    }
    
    // Check for failure after specific number of bytes
    ctx->bytes_written += len;
    if (ctx->fail_after_bytes > 0 && ctx->bytes_written > ctx->fail_after_bytes) {
        return 1;  // Simulate mid-stream failure
    }
    
    // Append data to buffer
    ctx->buffer.insert(ctx->buffer.end(), buf, buf + len);
    return 0;  // Success
}

// Helper function to generate a valid stored block
// Stored block format: BFINAL (1 bit), BTYPE=00 (2 bits), LEN (16 bits), NLEN (16 bits), data
static void generate_stored_block(std::vector<uint8_t>& buffer, uint16_t length, bool is_final) {
    // Add block header: BFINAL (bit 0), BTYPE=00 (bits 1-2)
    uint8_t header = (is_final ? 0x01 : 0x00);
    buffer.push_back(header);
    
    // Add LEN (little-endian)
    buffer.push_back((uint8_t)(length & 0xFF));
    buffer.push_back((uint8_t)((length >> 8) & 0xFF));
    
    // Add NLEN = ~LEN (little-endian)
    uint16_t nlen = ~length;
    buffer.push_back((uint8_t)(nlen & 0xFF));
    buffer.push_back((uint8_t)((nlen >> 8) & 0xFF));
    
    // Add data bytes (if length > 0)
    for (uint16_t i = 0; i < length; i++) {
        buffer.push_back((uint8_t)(i & 0xFF));  // Simple pattern
    }
}

// Helper function to generate invalid stored block (for error testing)
static void generate_invalid_stored_block(std::vector<uint8_t>& buffer, uint16_t length, uint16_t nlen, bool is_final) {
    // Add block header: BFINAL (bit 0), BTYPE=00 (bits 1-2)
    uint8_t header = (is_final ? 0x01 : 0x00);
    buffer.push_back(header);
    
    // Add LEN (little-endian)
    buffer.push_back((uint8_t)(length & 0xFF));
    buffer.push_back((uint8_t)((length >> 8) & 0xFF));
    
    // Add NLEN (little-endian, potentially invalid)
    buffer.push_back((uint8_t)(nlen & 0xFF));
    buffer.push_back((uint8_t)((nlen >> 8) & 0xFF));
    
    // Add data bytes (if length > 0)
    for (uint16_t i = 0; i < length; i++) {
        buffer.push_back((uint8_t)(i & 0xFF));  // Simple pattern
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
    bool test_valid_stored = fdp.ConsumeBool();
    bool test_invalid_stored = fdp.ConsumeBool();
    bool test_multiple_blocks = fdp.ConsumeBool();
    bool test_early_termination = fdp.ConsumeBool();
    bool test_boundary_cases = fdp.ConsumeBool();
    
    // Consume callback behavior parameters
    bool input_failure = fdp.ConsumeBool();
    bool output_failure = fdp.ConsumeBool();
    size_t fail_after_bytes = fdp.ConsumeIntegralInRange<size_t>(0, 32768);
    
    // Consume stored block parameters
    uint16_t stored_length = fdp.ConsumeIntegral<uint16_t>();
    uint16_t invalid_nlen = fdp.ConsumeIntegral<uint16_t>();
    bool is_final_block = fdp.ConsumeBool();
    
    // Consume buffer size limits
    size_t max_output_size = fdp.ConsumeIntegralInRange<size_t>(1024, 64 * 1024);
    size_t max_input_return = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    
    // Generate test data based on configuration
    std::vector<uint8_t> test_data;
    
    if (test_valid_stored) {
        // Generate valid stored block(s)
        if (test_multiple_blocks) {
            // Generate multiple stored blocks
            int num_blocks = fdp.ConsumeIntegralInRange<int>(1, 5);
            for (int i = 0; i < num_blocks; i++) {
                bool final_block = (i == num_blocks - 1) ? true : false;
                uint16_t block_len = fdp.ConsumeIntegralInRange<uint16_t>(0, 1024);
                generate_stored_block(test_data, block_len, final_block);
            }
        } else {
            // Generate single stored block
            generate_stored_block(test_data, stored_length, is_final_block);
        }
    } else if (test_invalid_stored) {
        // Generate invalid stored block for error testing
        generate_invalid_stored_block(test_data, stored_length, invalid_nlen, is_final_block);
    } else {
        // Use remaining fuzzer input as test data
        size_t data_size = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
        if (fdp.remaining_bytes() < data_size) {
            return 0;
        }
        test_data = fdp.ConsumeBytes<uint8_t>(data_size);
    }
    
    // Test early termination by truncating data
    if (test_early_termination && test_data.size() > 10) {
        size_t truncate_to = fdp.ConsumeIntegralInRange<size_t>(1, test_data.size() - 1);
        test_data.resize(truncate_to);
    }
    
    // Test boundary cases by manipulating window size
    if (test_boundary_cases) {
        // Test with window size just enough/not enough for data
        if (stored_length > 0) {
            size_t required_window = (size_t)stored_length;
            if (required_window < (1U << windowBits)) {
                // Data fits in window
            } else {
                // Data doesn't fit, adjust windowBits
                while ((1U << windowBits) < required_window && windowBits < 15) {
                    windowBits++;
                }
            }
        }
    }
    
    // Create window buffer
    size_t window_size = 1U << windowBits;
    unsigned char* window = (unsigned char*)malloc(window_size);
    if (!window) {
        return 0;
    }
    
    // Initialize z_stream
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    
    // Initialize inflateBack
    int ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, sizeof(z_stream));
    
    if (ret == Z_OK) {
        // Set up callback contexts
        InputContext in_ctx;
        OutputContext out_ctx;
        
        in_ctx.data = test_data.data();
        in_ctx.size = test_data.size();
        in_ctx.pos = 0;
        in_ctx.should_fail = input_failure;
        in_ctx.max_return_size = max_input_return;
        
        out_ctx.max_size = max_output_size;
        out_ctx.should_fail = output_failure;
        out_ctx.fail_after_bytes = fail_after_bytes;
        out_ctx.bytes_written = 0;
        
        // Set initial input if we have data
        if (test_data.size() > 0 && !input_failure) {
            size_t initial_size = std::min(test_data.size(), (size_t)256);
            strm.next_in = (Bytef*)test_data.data();
            strm.avail_in = (uInt)initial_size;
            in_ctx.pos = initial_size;
        }
        
        // Test inflateBack with stored block data
        ret = inflateBack(&strm, input_callback, &in_ctx, output_callback, &out_ctx);
        
        // Test error recovery and retry if appropriate
        if (ret == Z_BUF_ERROR || ret == Z_DATA_ERROR) {
            // Reset and try again with different parameters
            if (!output_failure && !input_failure) {
                // Reset contexts
                in_ctx.pos = 0;
                in_ctx.should_fail = false;
                out_ctx.buffer.clear();
                out_ctx.should_fail = false;
                out_ctx.bytes_written = 0;
                
                // Reset stream
                strm.next_in = Z_NULL;
                strm.avail_in = 0;
                
                // Try again
                inflateBack(&strm, input_callback, &in_ctx, output_callback, &out_ctx);
            }
        }
        
        // Test with empty input
        if (fdp.ConsumeBool()) {
            InputContext empty_in;
            OutputContext empty_out;
            
            empty_in.data = nullptr;
            empty_in.size = 0;
            empty_in.pos = 0;
            empty_in.should_fail = false;
            
            empty_out.max_size = 1024;
            empty_out.should_fail = false;
            
            z_stream strm2;
            memset(&strm2, 0, sizeof(strm2));
            strm2.zalloc = Z_NULL;
            strm2.zfree = Z_NULL;
            strm2.opaque = Z_NULL;
            
            unsigned char small_window[256];
            inflateBackInit_(&strm2, 8, small_window, ZLIB_VERSION, sizeof(z_stream));
            inflateBack(&strm2, input_callback, &empty_in, output_callback, &empty_out);
            inflateBackEnd(&strm2);
        }
        
        // Clean up
        inflateBackEnd(&strm);
    }
    
    // Test error cases in inflateBackInit_
    if (fdp.ConsumeBool()) {
        z_stream bad_strm;
        memset(&bad_strm, 0, sizeof(bad_strm));
        
        // Test with invalid windowBits
        unsigned char dummy_window[32768];
        inflateBackInit_(&bad_strm, 7, dummy_window, ZLIB_VERSION, sizeof(z_stream));
        inflateBackInit_(&bad_strm, 16, dummy_window, ZLIB_VERSION, sizeof(z_stream));
        
        // Test with NULL parameters
        inflateBackInit_(nullptr, 15, dummy_window, ZLIB_VERSION, sizeof(z_stream));
        inflateBackInit_(&bad_strm, 15, nullptr, ZLIB_VERSION, sizeof(z_stream));
        
        // Test with wrong stream size
        inflateBackInit_(&bad_strm, 15, dummy_window, ZLIB_VERSION, sizeof(z_stream) - 1);
    }
    
    // Free window
    free(window);
    
    return 0;
}

} // namespace harness_011
