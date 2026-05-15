#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <memory>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_005 {
/* Comprehensive fuzzing harness for advanced zlib stream control, header manipulation, and state query functions
 * Targets advanced stream control functions that are currently 0% covered:
 * - deflateSetHeader: Set gzip header information for compression
 * - inflateGetHeader: Retrieve gzip header information from decompression
 * - deflateUsed: Query most recent number of deflate bits used
 * - deflateTune: Fine-tune compression parameters (good_length, max_lazy, nice_length, max_chain)
 * - inflateUndermine: Test stream integrity by undermining decompression
 * - inflateCodesUsed: Query number of inflate codes used
 * - zlibCompileFlags: Query compilation flags of the zlib library
 * - zError: Map error codes to descriptive strings
 * - zlibVersion: Query zlib library version
 * 
 * This harness targets 54+ undiscovered branches in deflate.c and inflate.c
 * focusing on advanced stream control, header manipulation, and state query functions.
 * 
 * Distinct from existing harnesses:
 * - harness_000: Basic compression/decompression APIs
 * - harness_001: Gzip file I/O operations
 * - harness_002: Callback-based inflation with custom I/O
 * - harness_003: Advanced gzip I/O operations
 * - harness_004: Advanced stream state inspection/control
 * - harness_005: Advanced stream control, header manipulation, state query (this harness)
 */


// Minimum input size requirement
const size_t MIN_INPUT_SIZE = 64;

// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_INPUT_DATA = 1024 * 1024;  // 1MB
const size_t MAX_WINDOW_BITS = 15;  // Maximum window bits for deflate/inflate
const size_t MAX_GZIP_EXTRA = 1024;  // Maximum gzip extra field size
const size_t MAX_GZIP_NAME = 256;    // Maximum gzip filename length
const size_t MAX_GZIP_COMMENT = 256; // Maximum gzip comment length

// Main fuzzing function for advanced stream control and header manipulation APIs
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Check minimum input size
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Query library capabilities (Guidance step 1)
    const char* version = zlibVersion();
    unsigned long compile_flags = zlibCompileFlags();
    
    // Consume parameters for stream initialization
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    int window_bits = fdp.ConsumeIntegralInRange<int>(8, MAX_WINDOW_BITS);
    int mem_level = fdp.ConsumeIntegralInRange<int>(1, 9);
    int strategy = fdp.PickValueInArray({
        Z_DEFAULT_STRATEGY,
        Z_FILTERED,
        Z_HUFFMAN_ONLY,
        Z_RLE,
        Z_FIXED
    });
    
    // Consume gzip header parameters
    bool gzip_text = fdp.ConsumeBool();
    uLong gzip_time = fdp.ConsumeIntegral<uLong>();
    int gzip_xflags = fdp.ConsumeIntegral<int>();
    int gzip_os = fdp.ConsumeIntegral<int>();
    bool gzip_hcrc = fdp.ConsumeBool();
    
    // Consume fine-tuning parameters for deflateTune
    int good_length = fdp.ConsumeIntegralInRange<int>(0, 258);
    int max_lazy = fdp.ConsumeIntegralInRange<int>(0, 258);
    int nice_length = fdp.ConsumeIntegralInRange<int>(0, 258);
    int max_chain = fdp.ConsumeIntegralInRange<int>(0, 32768);
    
    // Consume data for compression
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    std::vector<uint8_t> input_data = fdp.ConsumeBytes<uint8_t>(input_size);
    if (input_data.empty()) {
        return 0;
    }
    
    // Consume gzip extra field data
    size_t extra_len = fdp.ConsumeIntegralInRange<size_t>(0, MAX_GZIP_EXTRA);
    std::vector<uint8_t> extra_data = fdp.ConsumeBytes<uint8_t>(extra_len);
    
    // Consume gzip filename and comment
    std::string gzip_name = fdp.ConsumeRandomLengthString(MAX_GZIP_NAME);
    std::string gzip_comment = fdp.ConsumeRandomLengthString(MAX_GZIP_COMMENT);
    
    // Calculate maximum compressed size
    size_t compressed_bound = compressBound(input_data.size());
    if (compressed_bound > MAX_INPUT_DATA) {
        return 0;
    }
    
    // Allocate buffers
    std::vector<uint8_t> compressed_buffer(compressed_bound);
    std::vector<uint8_t> decompressed_buffer(input_data.size());
    
    // Step 2: Initialize deflate stream with deflateInit2_ (Guidance step 2)
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(z_stream));
    
    // Use window_bits + 16 to request gzip format for header support
    int gzip_window_bits = window_bits + 16;
    int ret = deflateInit2(&deflate_stream,
                          compression_level,
                          Z_DEFLATED,
                          gzip_window_bits,
                          mem_level,
                          strategy);
    if (ret != Z_OK) {
        // Test zError for error mapping (Guidance step 13)
        const char* error_msg = zError(ret);
        return 0;
    }
    
    // Step 3: Set gzip header with deflateSetHeader (Guidance step 3)
    gz_header gzip_header;
    memset(&gzip_header, 0, sizeof(gz_header));
    
    gzip_header.text = gzip_text ? 1 : 0;
    gzip_header.time = gzip_time;
    gzip_header.xflags = gzip_xflags;
    gzip_header.os = gzip_os;
    gzip_header.hcrc = gzip_hcrc ? 1 : 0;
    
    // Set extra field if we have data
    if (!extra_data.empty()) {
        gzip_header.extra = extra_data.data();
        gzip_header.extra_len = static_cast<uInt>(extra_data.size());
    }
    
    // Set name if we have one
    if (!gzip_name.empty()) {
        gzip_header.name = reinterpret_cast<Bytef*>(const_cast<char*>(gzip_name.c_str()));
    }
    
    // Set comment if we have one
    if (!gzip_comment.empty()) {
        gzip_header.comment = reinterpret_cast<Bytef*>(const_cast<char*>(gzip_comment.c_str()));
    }
    
    ret = deflateSetHeader(&deflate_stream, &gzip_header);
    if (ret != Z_OK) {
        const char* error_msg = zError(ret);
        deflateEnd(&deflate_stream);
        return 0;
    }
    
    // Step 4: Fine-tune compression with deflateTune (Guidance step 4)
    ret = deflateTune(&deflate_stream, good_length, max_lazy, nice_length, max_chain);
    if (ret != Z_OK) {
        const char* error_msg = zError(ret);
        deflateEnd(&deflate_stream);
        return 0;
    }
    
    // Setup compression
    deflate_stream.next_in = input_data.data();
    deflate_stream.avail_in = static_cast<uInt>(input_data.size());
    deflate_stream.next_out = compressed_buffer.data();
    deflate_stream.avail_out = static_cast<uInt>(compressed_buffer.size());
    
    // Step 5: Perform compression with deflate (Guidance step 5)
    ret = deflate(&deflate_stream, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR) {
        const char* error_msg = zError(ret);
        deflateEnd(&deflate_stream);
        return 0;
    }
    
    size_t compressed_size = compressed_buffer.size() - deflate_stream.avail_out;
    
    // Step 6: Query pending output with deflateUsed (Guidance step 6)
    int bits_used = 0;
    ret = deflateUsed(&deflate_stream, &bits_used);
    if (ret != Z_OK) {
        const char* error_msg = zError(ret);
    }
    
    // Step 7: End deflate stream with deflateEnd (Guidance step 7)
    ret = deflateEnd(&deflate_stream);
    if (ret != Z_OK) {
        const char* error_msg = zError(ret);
    }
    
    // Step 8: Initialize inflate stream with inflateInit2_ (Guidance step 8)
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(z_stream));
    
    ret = inflateInit2(&inflate_stream, gzip_window_bits);
    if (ret != Z_OK) {
        const char* error_msg = zError(ret);
        return 0;
    }
    
    // Step 9: Request header retrieval with inflateGetHeader (Guidance step 9)
    gz_header inflate_header;
    memset(&inflate_header, 0, sizeof(gz_header));
    
    // Allocate buffers for header retrieval
    std::vector<uint8_t> inflate_extra_buffer(MAX_GZIP_EXTRA);
    std::vector<char> inflate_name_buffer(MAX_GZIP_NAME);
    std::vector<char> inflate_comment_buffer(MAX_GZIP_COMMENT);
    
    inflate_header.extra_max = MAX_GZIP_EXTRA;
    inflate_header.extra = inflate_extra_buffer.data();
    inflate_header.name_max = MAX_GZIP_NAME;
    inflate_header.name = reinterpret_cast<Bytef*>(inflate_name_buffer.data());
    inflate_header.comm_max = MAX_GZIP_COMMENT;
    inflate_header.comment = reinterpret_cast<Bytef*>(inflate_comment_buffer.data());
    
    ret = inflateGetHeader(&inflate_stream, &inflate_header);
    if (ret != Z_OK) {
        const char* error_msg = zError(ret);
        inflateEnd(&inflate_stream);
        return 0;
    }
    
    // Setup decompression
    inflate_stream.next_in = compressed_buffer.data();
    inflate_stream.avail_in = static_cast<uInt>(compressed_size);
    inflate_stream.next_out = decompressed_buffer.data();
    inflate_stream.avail_out = static_cast<uInt>(decompressed_buffer.size());
    
    // Step 10: Perform decompression with inflate (Guidance step 10)
    ret = inflate(&inflate_stream, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR && ret != Z_DATA_ERROR) {
        const char* error_msg = zError(ret);
        // Continue anyway to test other APIs
    }
    
    // Step 11: Test stream integrity with inflateUndermine (Guidance step 11)
    // Note: inflateUndermine is a special API that may return Z_DATA_ERROR
    // when the stream is undermined - this is expected behavior
    int undermine_value = fdp.ConsumeIntegralInRange<int>(0, 1);
    ret = inflateUndermine(&inflate_stream, undermine_value);
    // Don't check return value as inflateUndermine may intentionally fail
    
    // Step 12: Query code usage with inflateCodesUsed (Guidance step 12)
    unsigned long codes_used = inflateCodesUsed(&inflate_stream);
    
    // Step 13: Map error codes to strings with zError for error handling (Guidance step 13)
    // Test zError with various error codes
    for (int err_code = Z_ERRNO; err_code <= Z_VERSION_ERROR; err_code++) {
        const char* err_msg = zError(err_code);
    }
    
    // Step 14: End inflate stream with inflateEnd (Guidance step 14)
    ret = inflateEnd(&inflate_stream);
    if (ret != Z_OK) {
        const char* error_msg = zError(ret);
    }
    
    return 0;
}

} // namespace harness_005
