#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>
#include <algorithm>

namespace harness_022 {
/*
 * Fuzzing harness for zlib inflateBack dynamic block decompression
 * Target: Unlock 134+ undiscovered branches in inflateBack dynamic block paths
 * 
 * Based on coverage analysis guidance at /root/FuzzAgent/output/zlib/coverage_guidance/harness_022_guidance.md
 * 
 * Focus: Dynamic block paths (case 2 in TYPE switch) which have near-zero coverage
 * while handling complex Huffman table construction. Craft inputs with dynamic block
 * headers to exercise code length tables and distance tables building logic that
 * current fuzzers miss.
 * 
 * Target APIs:
 * - inflateBack (340 branches, 134 undiscovered)
 * - inflateBackInit_ (Required Helper: Initialization)
 * - inflateBackEnd (Required Helper: Cleanup)
 * 
 * Invocation sequence:
 * 1. Call inflateBackInit_ to initialize stream with custom window size
 * 2. Implement in_func callback to feed compressed data with dynamic block headers
 * 3. Implement out_func callback to receive decompressed output
 * 4. Call inflateBack with the callbacks to process dynamic blocks
 * 5. Call inflateBackEnd to clean up resources
 * 
 * Block type focus: Dynamic blocks (case 2) which involve:
 * - Building code length tables (19 symbols)
 * - Building literal/length tables (up to 286 symbols)
 * - Building distance tables (up to 30 symbols)
 * - Complex Huffman table construction with repetition codes (16, 17, 18)
 */


// ==============================================
// Simple input callback context
struct DynamicInputContext {
    const uint8_t* data;
    size_t size;
    size_t pos;
    
    // For tracking block types
    bool expecting_dynamic_block;
    size_t bytes_provided;
    
    DynamicInputContext(const uint8_t* d, size_t s) : 
        data(d), size(s), pos(0), expecting_dynamic_block(false), bytes_provided(0) {}
};

// Simple output callback context  
struct DynamicOutputContext {
    std::vector<uint8_t> output_buffer;
    size_t total_written;
    bool should_fail;
    
    DynamicOutputContext() : total_written(0), should_fail(false) {}
};

// ==============================================
// Input callback function for inflateBack
static unsigned dynamic_in_func(void* desc, z_const unsigned char FAR* FAR* buf) {
    DynamicInputContext* ctx = (DynamicInputContext*)desc;
    
    // Determine how many bytes to provide
    size_t available = ctx->size - ctx->pos;
    if (available == 0) {
        *buf = NULL;
        return 0;
    }
    
    // Provide data in chunks to test partial reads
    size_t chunk_size = available;
    if (chunk_size > 1024) chunk_size = 1024;
    
    *buf = (z_const unsigned char FAR*)&ctx->data[ctx->pos];
    ctx->pos += chunk_size;
    ctx->bytes_provided += chunk_size;
    
    return (unsigned)chunk_size;
}

// ==============================================
// Output callback function for inflateBack
static int dynamic_out_func(void* desc, unsigned char FAR* buf, unsigned len) {
    DynamicOutputContext* ctx = (DynamicOutputContext*)desc;
    
    if (ctx->should_fail) {
        return -1; // Simulate output failure
    }
    
    // Store output for verification
    ctx->output_buffer.insert(ctx->output_buffer.end(), buf, buf + len);
    ctx->total_written += len;
    
    return 0; // Success
}

// ==============================================
// Helper function to create deflate stream with dynamic block
// Based on RFC 1951 deflate format specification
std::vector<uint8_t> create_dynamic_block_deflate_stream(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> stream;
    
    // Consume parameters for dynamic block
    bool last_block = fdp.ConsumeBool();
    uint8_t block_type = 2; // Dynamic block
    
    // Block header: BFINAL (1 bit) + BTYPE (2 bits)
    uint8_t block_header = (last_block ? 0x01 : 0x00) | (block_type << 1);
    stream.push_back(block_header);
    
    // For dynamic blocks, we need to write:
    // 1. HLIT: number of literal/length codes - 257 (5 bits)
    // 2. HDIST: number of distance codes - 1 (5 bits)
    // 3. HCLEN: number of code length codes - 4 (4 bits)
    
    // First 14 bits contain HLIT, HDIST, HCLEN
    uint16_t hlit = fdp.ConsumeIntegralInRange<uint16_t>(0, 31);  // 0-31 -> 257-288 codes
    uint16_t hdist = fdp.ConsumeIntegralInRange<uint16_t>(0, 31); // 0-31 -> 1-32 distance codes
    uint16_t hclen = fdp.ConsumeIntegralInRange<uint16_t>(0, 15); // 0-15 -> 4-19 code length codes
    
    uint16_t first_chunk = (hclen << 10) | (hdist << 5) | hlit;
    stream.push_back(first_chunk & 0xFF);
    stream.push_back((first_chunk >> 8) & 0xFF);
    
    // Code length alphabet permutation order (RFC 1951 section 3.2.7)
    static const unsigned char order[19] = {
        16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
    };
    
    // Write code length code lengths (3 bits each)
    for (int i = 0; i < hclen + 4; i++) {
        uint8_t code_len = fdp.ConsumeIntegralInRange<uint8_t>(0, 7);
        stream.push_back(code_len);
    }
    
    // Write literal/length and distance code lengths
    // These are encoded using the code length Huffman code we just defined
    // For simplicity in fuzzing, we'll write raw lengths and let inflateBack decode
    int total_codes = (hlit + 257) + (hdist + 1);
    
    for (int i = 0; i < total_codes; i++) {
        // Code lengths: 0-15 are literal lengths, 16=repeat previous, 17=repeat zero 3-10 times, 18=repeat zero 11-138 times
        uint8_t code_len_sym = fdp.ConsumeIntegralInRange<uint8_t>(0, 18);
        stream.push_back(code_len_sym);
        
        // If it's a repeat symbol, need extra bits
        if (code_len_sym == 16) {
            // Repeat previous code length 3-6 times (2 extra bits)
            uint8_t extra = fdp.ConsumeIntegralInRange<uint8_t>(0, 3);
            stream.push_back(extra);
        } else if (code_len_sym == 17) {
            // Repeat zero 3-10 times (3 extra bits)
            uint8_t extra = fdp.ConsumeIntegralInRange<uint8_t>(0, 7);
            stream.push_back(extra);
        } else if (code_len_sym == 18) {
            // Repeat zero 11-138 times (7 extra bits)
            uint8_t extra = fdp.ConsumeIntegralInRange<uint8_t>(0, 127);
            stream.push_back(extra);
        }
    }
    
    // After code tables, write actual compressed data
    // For simplicity, create a small stored block inside the dynamic block
    // This ensures we have some data to decompress
    uint16_t stored_len = fdp.ConsumeIntegralInRange<uint16_t>(1, 100);
    std::vector<uint8_t> stored_data = fdp.ConsumeBytes<uint8_t>(stored_len);
    
    // Write end-of-block code (256) using minimal Huffman coding
    // For simplicity, use a fixed literal
    stream.push_back(0x00); // Some literal
    
    // Add padding if needed
    while (stream.size() % 8 != 0) {
        stream.push_back(0x00);
    }
    
    return stream;
}

// ==============================================
// Alternative: Generate raw deflate data with dynamic blocks using deflate
std::vector<uint8_t> generate_dynamic_block_data(FuzzedDataProvider& fdp) {
    // Use deflate to generate valid dynamic blocks
    z_stream c_stream;
    c_stream.zalloc = Z_NULL;
    c_stream.zfree = Z_NULL;
    c_stream.opaque = Z_NULL;
    
    // Initialize with maximum compression to encourage dynamic blocks
    int ret = deflateInit(&c_stream, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        return std::vector<uint8_t>();
    }
    
    // Create some test data to compress
    size_t test_size = fdp.ConsumeIntegralInRange<size_t>(10, 1000);
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
    if (test_data.empty()) {
        test_data.push_back('A');
    }
    
    // Compress with multiple flush operations to create multiple blocks
    std::vector<uint8_t> compressed(test_data.size() * 2 + 256);
    
    c_stream.next_in = test_data.data();
    c_stream.avail_in = test_data.size();
    c_stream.next_out = compressed.data();
    c_stream.avail_out = compressed.size();
    
    // Use Z_FULL_FLUSH to force block boundaries
    ret = deflate(&c_stream, Z_FULL_FLUSH);
    if (ret != Z_OK) {
        deflateEnd(&c_stream);
        return std::vector<uint8_t>();
    }
    
    size_t compressed_size = compressed.size() - c_stream.avail_out;
    deflateEnd(&c_stream);
    
    compressed.resize(compressed_size);
    return compressed;
}

// ==============================================
// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) return 0; // Minimum size for meaningful testing
    
    FuzzedDataProvider fdp(data, size);
    
    // Generate test data with dynamic blocks
    std::vector<uint8_t> test_stream;
    
    // Choose generation method based on input
    uint8_t method = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (method) {
        case 0:
            // Method 1: Manual dynamic block construction
            test_stream = create_dynamic_block_deflate_stream(fdp);
            break;
        case 1:
            // Method 2: Use deflate to generate (likely produces dynamic blocks)
            test_stream = generate_dynamic_block_data(fdp);
            break;
        case 2:
            // Method 3: Use raw fuzzer input as deflate stream
            // Let inflateBack parse it - might contain dynamic blocks
            test_stream = fdp.ConsumeRemainingBytes<uint8_t>();
            break;
    }
    
    if (test_stream.empty()) {
        return 0;
    }
    
    // Setup window buffer for inflateBack
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
    size_t window_size = 1U << windowBits;
    std::vector<unsigned char> window_buffer(window_size);
    
    // Initialize inflateBack
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    int ret = inflateBackInit_(&strm, windowBits, window_buffer.data(),
                              ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Setup callback contexts
    DynamicInputContext in_ctx(test_stream.data(), test_stream.size());
    DynamicOutputContext out_ctx;
    
    // Configure output failure based on input
    out_ctx.should_fail = (fdp.ConsumeIntegral<uint8_t>() % 100) < 5; // 5% chance
    
    // Provide initial input
    strm.next_in = test_stream.data();
    strm.avail_in = test_stream.size();
    
    // Call inflateBack with our callbacks
    ret = inflateBack(&strm, dynamic_in_func, &in_ctx,
                     dynamic_out_func, &out_ctx);
    
    // Check return codes - we expect various outcomes
    // Z_STREAM_END: Successful decompression
    // Z_BUF_ERROR: Input/output error from callbacks
    // Z_DATA_ERROR: Invalid deflate data
    // All are valid test outcomes
    
    // Clean up
    inflateBackEnd(&strm);
    
    return 0;
}

} // namespace harness_022
