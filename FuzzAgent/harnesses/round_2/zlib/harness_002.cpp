#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_002 {
/*
 * Fuzzing harness for zlib inflateBack callback-based decompression APIs
 * Target: inflateBack, inflateBackInit, inflateBackEnd
 * Harness ID: 002
 * 
 * This harness focuses on testing the callback-based decompression interface,
 * which is a critical coverage gap with 0% coverage and 340+ undiscovered branches.
 * Different from harness_000 (basic compression/decompression) and 
 * harness_001 (gzip file I/O).
 */


// Context structure for callback functions
struct callback_context {
    unsigned char* input_data;  // Non-const to match vector data()
    size_t input_size;
    size_t position;
    unsigned char* output_buffer;
    size_t output_capacity;
    size_t output_position;
    bool simulate_output_error;
    int window_bits;
    unsigned char* window_buffer;
};

// Callback function for input (in_func)
// Returns number of bytes provided and pointer to that input in buf
static unsigned input_callback(void* desc, z_const unsigned char** buf) {
    // desc points to our callback context containing input data and position
    struct callback_context* ctx = (struct callback_context*)desc;
    
    if (ctx == NULL || ctx->input_data == NULL || ctx->position >= ctx->input_size) {
        return 0;  // No more input available
    }
    
    *buf = (z_const unsigned char*)(ctx->input_data + ctx->position);
    
    // Determine how many bytes to return (at least 1, at most remaining bytes)
    size_t bytes_to_return = 1;
    if (ctx->position + 1 < ctx->input_size) {
        // Randomly decide how many bytes to return (1-4 or remaining)
        bytes_to_return = (ctx->input_size - ctx->position) > 4 ? 4 : (ctx->input_size - ctx->position);
    }
    
    // Update position for next call
    ctx->position += bytes_to_return;
    return (unsigned)bytes_to_return;
}

// Callback function for output (out_func)
// Returns 0 on success, non-zero on failure
static int output_callback(void* desc, unsigned char* buf, unsigned len) {
    // desc points to our callback context
    struct callback_context* ctx = (struct callback_context*)desc;
    
    if (ctx == NULL || buf == NULL || len == 0) {
        return 0;  // Nothing to do
    }
    
    // Store decompressed data if we have space
    if (ctx->output_position + len <= ctx->output_capacity) {
        memcpy(ctx->output_buffer + ctx->output_position, buf, len);
        ctx->output_position += len;
    }
    
    // Return success (0) most of the time, but occasionally simulate output error
    // Based on a flag in the context
    if (ctx->simulate_output_error) {
        return 1;  // Simulate output error
    }
    
    return 0;  // Success
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
    bool use_custom_alloc = fdp.ConsumeBool();
    bool simulate_output_error = fdp.ConsumeBool();
    bool provide_initial_input = fdp.ConsumeBool();
    
    // Consume remaining bytes as input data for decompression
    std::vector<uint8_t> input_buffer = fdp.ConsumeRemainingBytes<uint8_t>();
    if (input_buffer.empty()) {
        return 0;
    }
    
    // Allocate window buffer (must be 2^window_bits bytes)
    size_t window_size = 1U << window_bits;
    unsigned char* window = (unsigned char*)malloc(window_size);
    if (window == NULL) {
        return 0;
    }
    
    // Initialize z_stream structure
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Set up custom allocators if requested
    if (use_custom_alloc) {
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
    }
    
    // Initialize callback context
    struct callback_context ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.input_data = input_buffer.data();
    ctx.input_size = input_buffer.size();
    ctx.position = 0;
    ctx.output_buffer = NULL;  // We'll allocate if needed
    ctx.output_capacity = 0;
    ctx.output_position = 0;
    ctx.simulate_output_error = simulate_output_error;
    ctx.window_bits = window_bits;
    ctx.window_buffer = window;
    
    // Allocate output buffer for storing decompressed data
    size_t output_capacity = input_buffer.size() * 4;  // Rough estimate
    unsigned char* output_buffer = (unsigned char*)malloc(output_capacity);
    if (output_buffer == NULL) {
        free(window);
        return 0;
    }
    
    ctx.output_buffer = output_buffer;
    ctx.output_capacity = output_capacity;
    
    // Initialize inflateBack
    int ret = inflateBackInit(&strm, window_bits, window);
    if (ret != Z_OK) {
        // Cleanup and return
        free(output_buffer);
        free(window);
        return 0;
    }
    
    // Set up initial input if requested
    if (provide_initial_input && input_buffer.size() > 0) {
        size_t initial_input_size = input_buffer.size() > 16 ? 16 : input_buffer.size();
        strm.next_in = (Bytef*)input_buffer.data();
        strm.avail_in = (uInt)initial_input_size;
        ctx.position = initial_input_size;  // Skip this data in callback
    } else {
        strm.next_in = Z_NULL;
        strm.avail_in = 0;
    }
    
    // Perform inflateBack decompression
    ret = inflateBack(&strm, input_callback, &ctx, output_callback, &ctx);
    
    // Check return values
    switch (ret) {
        case Z_STREAM_END:
            // Successful decompression
            break;
        case Z_BUF_ERROR:
            // Input or output error (expected when simulate_output_error is true)
            break;
        case Z_DATA_ERROR:
            // Data format error (possible with random input)
            break;
        case Z_STREAM_ERROR:
            // Stream state error
            break;
        default:
            // Other return values
            break;
    }
    
    // Call inflateBackEnd to clean up
    ret = inflateBackEnd(&strm);
    
    // Test error cases by calling functions with invalid parameters
    inflateBackInit(Z_NULL, window_bits, window);
    inflateBack(Z_NULL, NULL, NULL, NULL, NULL);
    inflateBackEnd(Z_NULL);
    
    // Test with different window sizes
    for (int test_bits = 8; test_bits <= 15; test_bits += 7) {
        if (test_bits != window_bits) {
            unsigned char* test_window = (unsigned char*)malloc(1U << test_bits);
            if (test_window) {
                z_stream test_strm;
                memset(&test_strm, 0, sizeof(test_strm));
                
                ret = inflateBackInit(&test_strm, test_bits, test_window);
                if (ret == Z_OK) {
                    // Set up minimal input
                    test_strm.next_in = (Bytef*)input_buffer.data();
                    test_strm.avail_in = input_buffer.size() > 16 ? 16 : (uInt)input_buffer.size();
                    
                    // Create test context
                    struct callback_context test_ctx = ctx;
                    test_ctx.position = 0;
                    test_ctx.simulate_output_error = false;
                    
                    inflateBack(&test_strm, input_callback, &test_ctx, output_callback, &test_ctx);
                    inflateBackEnd(&test_strm);
                }
                free(test_window);
            }
        }
    }
    
    // Clean up
    free(output_buffer);
    free(window);
    
    return 0;
}

} // namespace harness_002
