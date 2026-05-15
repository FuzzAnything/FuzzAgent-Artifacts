#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <memory>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_012 {
/* Comprehensive inflateBack harness targeting different block types and callback scenarios
 * Primary target: inflateBack (210 blocked branches, API coverage 100%, branch coverage 72.34%)
 * Focus: Testing stored blocks, dynamic blocks, fixed Huffman blocks, and realistic callback scenarios
 * 
 * This harness complements harness_010 (error conditions) by focusing on:
 * 1. Different deflate block types: stored (uncompressed), fixed Huffman, dynamic Huffman
 * 2. Various window sizes and callback behaviors
 * 3. Realistic decompression scenarios with generated deflate data
 * 4. Testing callback edge cases and input/output patterns
 * 
 * Block type coverage:
 * - Stored blocks (BTYPE=00): Simple uncompressed blocks
 * - Fixed Huffman blocks (BTYPE=01): Predefined Huffman codes
 * - Dynamic Huffman blocks (BTYPE=10): Custom Huffman codes in block header
 * - Reserved blocks (BTYPE=11): Should generate errors
 * 
 * Distinct from existing harnesses:
 * - harness_002: Basic inflate and inflateBack testing
 * - harness_010: Error condition testing for inflateBack
 * - harness_012: Block type and callback scenario testing (this harness)
 */


// Minimum input size requirement
const size_t MIN_INPUT_SIZE = 32;

// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_WINDOW_SIZE = 32768;  // 32KB (2^15, max window size)
const size_t MAX_COMPRESSED_DATA = 1024 * 1024;  // 1MB

// Callback context structure for realistic testing
struct CallbackContext {
    const uint8_t* data;
    size_t size;
    size_t position;
    bool use_stream_input;  // Whether to use stream->next_in initially
    size_t chunk_size;      // Maximum bytes to return per callback call
    int callback_mode;      // 0: normal, 1: partial data, 2: delayed data
};

// Realistic in() callback function for testing
unsigned real_in_callback(void FAR* desc, z_const unsigned char FAR* FAR* buf) {
    CallbackContext* ctx = reinterpret_cast<CallbackContext*>(desc);
    
    if (!ctx || ctx->position >= ctx->size) {
        return 0;  // No more data available
    }
    
    // Provide pointer to remaining data
    *buf = const_cast<unsigned char FAR*>(ctx->data + ctx->position);
    
    // Calculate how much to provide based on callback mode
    size_t available = ctx->size - ctx->position;
    size_t to_provide;
    
    switch (ctx->callback_mode) {
        case 0:  // Normal: provide all available
            to_provide = available;
            break;
        case 1:  // Partial: provide small chunks
            to_provide = ctx->chunk_size > 0 ? ctx->chunk_size : 1;
            if (to_provide > available) to_provide = available;
            break;
        case 2:  // Delayed: sometimes return 0 to simulate no data available
            if (ctx->position % 3 == 0) {
                return 0;  // Simulate no data available yet
            }
            to_provide = ctx->chunk_size > 0 ? ctx->chunk_size : 1;
            if (to_provide > available) to_provide = available;
            break;
        default:
            to_provide = available;
    }
    
    // Update position
    ctx->position += to_provide;
    return static_cast<unsigned>(to_provide);
}

// Realistic out() callback function for testing
int real_out_callback(void FAR* desc, unsigned char FAR* buf, unsigned len) {
    // For realistic testing, we accept the output but don't do anything with it
    // This simulates a real application that processes decompressed data
    (void)desc;
    (void)buf;
    (void)len;
    
    // Always succeed - real applications would process the data here
    return 0;
}

// Generate simple deflate data for testing different block types
std::vector<uint8_t> generate_deflate_data(FuzzedDataProvider& fdp, int block_type) {
    std::vector<uint8_t> result;
    
    // Consume some data for the block
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(data_size);
    
    if (test_data.empty()) {
        // Use a simple default if no data available
        test_data = {'T', 'e', 's', 't', ' ', 'd', 'a', 't', 'a'};
    }
    
    // Very simple deflate header simulation for testing
    // Note: This is not proper deflate encoding, but should trigger
    // different code paths in inflateBack based on block type
    
    switch (block_type) {
        case 0: { // Stored block (BTYPE=00)
            // BFINAL=1, BTYPE=00 (stored), then LEN and NLEN
            result.push_back(0x01);  // BFINAL=1, BTYPE=00
            // LEN (little-endian)
            uint16_t len = static_cast<uint16_t>(test_data.size());
            result.push_back(len & 0xFF);
            result.push_back((len >> 8) & 0xFF);
            // NLEN (one's complement of LEN)
            uint16_t nlen = ~len;
            result.push_back(nlen & 0xFF);
            result.push_back((nlen >> 8) & 0xFF);
            // Data
            result.insert(result.end(), test_data.begin(), test_data.end());
            break;
        }
            
        case 1: { // Fixed Huffman block (BTYPE=01)
            // Simple fixed block with end-of-block code
            result.push_back(0x03);  // BFINAL=1, BTYPE=01
            // Add end-of-block code (0000000) and some simple literals
            // This is a very simplified representation
            result.push_back(0x00);
            result.push_back(0x00);
            // Add some test data as literals
            for (size_t i = 0; i < std::min(test_data.size(), static_cast<size_t>(10)); i++) {
                result.push_back(test_data[i]);
            }
            break;
        }
            
        case 2: { // Dynamic Huffman block (BTYPE=10)
            // Try to trigger dynamic Huffman code path
            result.push_back(0x07);  // BFINAL=1, BTYPE=10
            // Simplified header (not valid, but should trigger parsing)
            result.push_back(0x00);
            result.push_back(0x00);
            break;
        }
            
        case 3: { // Reserved block (BTYPE=11) - should cause error
            result.push_back(0x0F);  // BFINAL=1, BTYPE=11 (reserved)
            break;
        }
    }
    
    return result;
}

// Test inflateBack with different block types and callback scenarios
void test_inflateBack_block_types(FuzzedDataProvider& fdp) {
    // Consume test parameters
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
    int block_type = fdp.ConsumeIntegralInRange<int>(0, 3);
    int callback_mode = fdp.ConsumeIntegralInRange<int>(0, 2);
    bool use_stream_input = fdp.ConsumeBool();
    size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    
    // Generate deflate data for testing
    std::vector<uint8_t> deflate_data = generate_deflate_data(fdp, block_type);
    if (deflate_data.empty()) {
        return;
    }
    
    // Initialize stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Allocate window buffer
    size_t window_size = 1U << windowBits;
    if (window_size > MAX_WINDOW_SIZE) window_size = MAX_WINDOW_SIZE;
    std::vector<unsigned char> window(window_size);
    
    // Initialize inflateBack
    int result = inflateBackInit_(&strm, windowBits, window.data(),
                                  ZLIB_VERSION, sizeof(z_stream));
    if (result != Z_OK) {
        // Initialization failed, clean up and return
        return;
    }
    
    // Set up callback context
    CallbackContext ctx;
    ctx.data = deflate_data.data();
    ctx.size = deflate_data.size();
    ctx.position = 0;
    ctx.use_stream_input = use_stream_input;
    ctx.chunk_size = chunk_size;
    ctx.callback_mode = callback_mode;
    
    // Set up initial input if requested
    if (use_stream_input && deflate_data.size() > 0) {
        size_t initial_input = std::min(deflate_data.size(), static_cast<size_t>(128));
        strm.next_in = const_cast<Bytef*>(deflate_data.data());
        strm.avail_in = static_cast<uInt>(initial_input);
        ctx.position = initial_input;
    } else {
        strm.next_in = Z_NULL;
        strm.avail_in = 0;
    }
    
    // Call inflateBack with our callbacks
    result = inflateBack(&strm, real_in_callback, &ctx,
                        real_out_callback, &ctx);
    
    // Note: We don't check the result because we're testing various scenarios
    // including invalid data that should produce errors
    
    // Clean up
    inflateBackEnd(&strm);
}

// Test multiple inflateBack operations in sequence
void test_inflateBack_sequence(FuzzedDataProvider& fdp) {
    // Consume parameters for sequential testing
    int num_operations = fdp.ConsumeIntegralInRange<int>(1, 5);
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
    
    // Allocate window buffer
    size_t window_size = 1U << windowBits;
    if (window_size > MAX_WINDOW_SIZE) window_size = MAX_WINDOW_SIZE;
    std::vector<unsigned char> window(window_size);
    
    // Initialize stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflateBack
    int result = inflateBackInit_(&strm, windowBits, window.data(),
                                  ZLIB_VERSION, sizeof(z_stream));
    if (result != Z_OK) {
        return;
    }
    
    // Perform multiple operations
    for (int i = 0; i < num_operations && fdp.remaining_bytes() > 16; i++) {
        // Generate data for this operation
        int block_type = fdp.ConsumeIntegralInRange<int>(0, 3);
        std::vector<uint8_t> deflate_data = generate_deflate_data(fdp, block_type);
        
        if (deflate_data.empty()) {
            continue;
        }
        
        // Set up callback context
        CallbackContext ctx;
        ctx.data = deflate_data.data();
        ctx.size = deflate_data.size();
        ctx.position = 0;
        ctx.use_stream_input = false;
        ctx.chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        ctx.callback_mode = fdp.ConsumeIntegralInRange<int>(0, 2);
        
        // Reset stream input for new operation
        strm.next_in = Z_NULL;
        strm.avail_in = 0;
        
        // Call inflateBack
        result = inflateBack(&strm, real_in_callback, &ctx,
                            real_out_callback, &ctx);
        
        // Don't check result - we're testing various scenarios
    }
    
    // Clean up
    inflateBackEnd(&strm);
}

// Test inflateBack with mixed valid and invalid data
void test_inflateBack_mixed_data(FuzzedDataProvider& fdp) {
    // Generate mixed data: some valid deflate, some random
    size_t mixed_size = fdp.ConsumeIntegralInRange<size_t>(64, 1024);
    std::vector<uint8_t> mixed_data = fdp.ConsumeBytes<uint8_t>(mixed_size);
    
    if (mixed_data.empty()) {
        return;
    }
    
    // Initialize stream with reasonable window
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    int windowBits = 15;  // Max window for general deflate streams
    size_t window_size = 1U << windowBits;
    std::vector<unsigned char> window(window_size);
    
    // Initialize inflateBack
    int result = inflateBackInit_(&strm, windowBits, window.data(),
                                  ZLIB_VERSION, sizeof(z_stream));
    if (result != Z_OK) {
        return;
    }
    
    // Set up callback context
    CallbackContext ctx;
    ctx.data = mixed_data.data();
    ctx.size = mixed_data.size();
    ctx.position = 0;
    ctx.use_stream_input = false;
    ctx.chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    ctx.callback_mode = fdp.ConsumeIntegralInRange<int>(0, 2);
    
    // Call inflateBack with mixed data
    result = inflateBack(&strm, real_in_callback, &ctx,
                        real_out_callback, &ctx);
    
    // Clean up
    inflateBackEnd(&strm);
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;  // Insufficient input
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test type selector
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (test_type) {
        case 0:
            // Test different block types
            test_inflateBack_block_types(fdp);
            break;
            
        case 1:
            // Test sequential operations
            test_inflateBack_sequence(fdp);
            break;
            
        case 2:
            // Test with mixed valid/invalid data
            test_inflateBack_mixed_data(fdp);
            break;
    }
    
    return 0;
}

} // namespace harness_012
