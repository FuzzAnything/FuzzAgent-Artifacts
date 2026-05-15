#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <memory>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_002 {
/* Comprehensive fuzzing harness for zlib callback-based inflation API family
 * Targets infback.c module which has 0% coverage with 340+ undiscovered branches:
 * - inflateBackInit_: Initialization for callback-based inflation
 * - inflateBack: Core callback-based inflation with custom I/O functions
 * - inflateBackEnd: Cleanup for callback-based inflation state
 * 
 * This harness implements simple in() and out() callback functions for custom I/O
 * and tests the complete lifecycle of callback-based inflation.
 * 
 * Distinct from existing harnesses:
 * - harness_000: Core compression/decompression APIs
 * - harness_001: Gzip file I/O operations
 * - harness_002: Callback-based inflation with custom I/O (this harness)
 */


// Minimum input size requirement
const size_t MIN_INPUT_SIZE = 32;

// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_WINDOW_SIZE = 32768;  // 32KB (2^15, max window size)
const size_t MAX_INPUT_DATA = 1024 * 1024;  // 1MB

// Callback context structures for in() and out() functions
struct InCallbackContext {
    const uint8_t* data;
    size_t size;
    size_t position;
};

struct OutCallbackContext {
    std::vector<uint8_t> output;
    size_t total_written;
};

// Simple in() callback function for inflateBack
// Returns number of bytes provided and pointer to that input in buf
unsigned in_callback(void FAR* desc, z_const unsigned char FAR* FAR* buf) {
    InCallbackContext* ctx = reinterpret_cast<InCallbackContext*>(desc);
    
    if (!ctx || ctx->position >= ctx->size) {
        return 0;  // No more data available
    }
    
    // Provide pointer to remaining data
    *buf = const_cast<unsigned char FAR*>(ctx->data + ctx->position);
    
    // Calculate how much to provide (at least 1 byte, up to remaining)
    size_t available = ctx->size - ctx->position;
    
    // Always provide at least some data if available
    if (available > 0) {
        // Provide a chunk of data (1 byte to all remaining)
        size_t to_provide = available;
        
        // Update position
        ctx->position += to_provide;
        return static_cast<unsigned>(to_provide);
    }
    
    return 0;
}

// Simple out() callback function for inflateBack
// Returns zero on success, non-zero on failure
int out_callback(void FAR* desc, unsigned char FAR* buf, unsigned len) {
    OutCallbackContext* ctx = reinterpret_cast<OutCallbackContext*>(desc);
    
    if (!ctx || !buf || len == 0) {
        return 0;  // Nothing to do
    }
    
    try {
        // Append the data to output buffer
        ctx->output.insert(ctx->output.end(), buf, buf + len);
        ctx->total_written += len;
        return 0;  // Success
    } catch (...) {
        return 1;  // Failure (e.g., memory allocation failed)
    }
}

// Main fuzzing function for callback-based inflation APIs
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Check minimum input size
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume parameters for inflateBack
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);  // Valid range 8..15
    size_t window_size = 1U << windowBits;  // 2^windowBits
    
    // Ensure we have enough input for window buffer and test data
    if (fdp.remaining_bytes() < window_size + 16) {
        return 0;
    }
    
    // Consume compressed data for testing (simulate deflate stream)
    size_t compressed_size = fdp.ConsumeIntegralInRange<size_t>(16, 4096);
    if (fdp.remaining_bytes() < compressed_size) {
        compressed_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> compressed_data = fdp.ConsumeBytes<uint8_t>(compressed_size);
    if (compressed_data.empty()) {
        return 0;
    }
    
    // Allocate window buffer (required by inflateBackInit_)
    std::vector<unsigned char> window_buffer(window_size);
    
    // Initialize zlib stream structure
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Set default memory allocation functions if needed
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Initialize for callback-based inflation
    int init_result = inflateBackInit_(&strm, windowBits, window_buffer.data(),
                                       ZLIB_VERSION, sizeof(z_stream));
    
    if (init_result != Z_OK) {
        // Initialization failed, clean up and exit
        inflateBackEnd(&strm);
        return 0;
    }
    
    // Set up callback contexts
    InCallbackContext in_ctx;
    in_ctx.data = compressed_data.data();
    in_ctx.size = compressed_data.size();
    in_ctx.position = 0;
    
    OutCallbackContext out_ctx;
    out_ctx.output.reserve(window_size * 2);  // Reserve some space
    out_ctx.total_written = 0;
    
    // Provide initial input through strm (optional)
    // If we don't set this, inflateBack will call in() immediately
    strm.next_in = const_cast<Bytef*>(compressed_data.data());
    strm.avail_in = static_cast<uInt>(compressed_data.size());
    
    // Perform callback-based inflation
    int inflate_result = inflateBack(&strm, 
                                     reinterpret_cast<in_func>(in_callback), 
                                     &in_ctx,
                                     reinterpret_cast<out_func>(out_callback), 
                                     &out_ctx);
    
    // Check various return values to exercise different code paths
    // Z_STREAM_END, Z_BUF_ERROR, Z_DATA_ERROR, Z_MEM_ERROR, Z_STREAM_ERROR are possible
    
    // Test edge case: try inflateBack again with no input (should return Z_BUF_ERROR)
    if (inflate_result == Z_STREAM_END) {
        // Reset stream state for another possible inflation
        // Note: inflateBack doesn't have an inflateBackReset, but we can test
        // with empty input
        strm.next_in = Z_NULL;
        strm.avail_in = 0;
        
        // Clear output context
        out_ctx.output.clear();
        out_ctx.total_written = 0;
        
        // Reset input context
        in_ctx.position = 0;
        
        // Try inflateBack with no input (should call in() which returns 0)
        inflate_result = inflateBack(&strm,
                                     reinterpret_cast<in_func>(in_callback),
                                     &in_ctx,
                                     reinterpret_cast<out_func>(out_callback),
                                     &out_ctx);
    }
    
    // Test with different window sizes and configurations
    if (fdp.remaining_bytes() > 16) {
        // Try with different window bits
        int alt_windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
        
        // Only proceed if we have enough space
        if (fdp.remaining_bytes() >= (1U << alt_windowBits)) {
            size_t alt_window_size = 1U << alt_windowBits;
            std::vector<unsigned char> alt_window_buffer(alt_window_size);
            
            // Re-initialize with different window size
            inflateBackEnd(&strm);  // Clean up previous
            
            memset(&strm, 0, sizeof(strm));
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            
            init_result = inflateBackInit_(&strm, alt_windowBits, alt_window_buffer.data(),
                                           ZLIB_VERSION, sizeof(z_stream));
            
            if (init_result == Z_OK) {
                // Consume more data for this test
                size_t more_data_size = fdp.ConsumeIntegralInRange<size_t>(16, 1024);
                if (fdp.remaining_bytes() >= more_data_size) {
                    std::vector<uint8_t> more_data = fdp.ConsumeBytes<uint8_t>(more_data_size);
                    
                    InCallbackContext alt_in_ctx;
                    alt_in_ctx.data = more_data.data();
                    alt_in_ctx.size = more_data.size();
                    alt_in_ctx.position = 0;
                    
                    OutCallbackContext alt_out_ctx;
                    alt_out_ctx.output.reserve(alt_window_size * 2);
                    alt_out_ctx.total_written = 0;
                    
                    strm.next_in = const_cast<Bytef*>(more_data.data());
                    strm.avail_in = static_cast<uInt>(more_data.size());
                    
                    inflate_result = inflateBack(&strm,
                                                 reinterpret_cast<in_func>(in_callback),
                                                 &alt_in_ctx,
                                                 reinterpret_cast<out_func>(out_callback),
                                                 &alt_out_ctx);
                }
            }
        }
    }
    
    // Final cleanup
    inflateBackEnd(&strm);
    
    return 0;
}

} // namespace harness_002
