#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>
#include <cassert>

namespace harness_011 {
/*
 * Fuzzing harness for gzip-specific decompression using inflateBack() API
 * Targets: deflateInit2_(), deflate(), deflateEnd(), inflateBackInit_(), inflateBack(), inflateBackEnd()
 * 
 * This harness specifically targets the 175 blocked branches in inflateBack() gzip header parsing logic
 * as identified in coverage analysis report. While harness_010.cpp covers inflate() with gzip headers,
 * this harness focuses on inflateBack() API which has similar but distinct gzip header processing paths.
 * 
 * Strategy:
 * 1. Setup gzip compression with deflateInit2_() with windowBits = 31 for gzip output
 * 2. Generate gzip data with various header configurations (extra fields, filename, comment, CRC16)
 * 3. Setup gzip decompression using inflateBackInit_() with windowBits = 31 + 16 for gzip
 * 4. Implement custom input/output callbacks for inflateBack() that handle gzip format data
 * 5. Call inflateBack() to decompress gzip data through callback interface
 * 6. Call inflateBackEnd() for cleanup
 * 7. Test edge cases specific to inflateBack(): partial input, callback errors, malformed gzip headers
 * 8. Include comprehensive error handling to reach blocked branches in error paths
 */


// Structure to hold input data for inflateBack callback
struct InflateBackInput {
    const uint8_t* data;
    size_t size;
    size_t position;
};

// Structure to hold output tracking for inflateBack callback  
struct InflateBackOutput {
    std::vector<uint8_t> buffer;
    size_t total_written;
};

// Input callback function for inflateBack - provides gzip data
static unsigned inflate_back_input_callback(void* desc, unsigned char** buf) {
    InflateBackInput* input = static_cast<InflateBackInput*>(desc);
    
    if (input->position >= input->size) {
        return 0;  // No more input
    }
    
    // Provide chunk of data (1 byte to remaining)
    size_t remaining = input->size - input->position;
    size_t chunk_size = 1 + (input->position % 1024);  // Varying chunk sizes
    if (chunk_size > remaining) {
        chunk_size = remaining;
    }
    
    *buf = (unsigned char*)(input->data + input->position);
    input->position += chunk_size;
    
    return static_cast<unsigned>(chunk_size);
}

// Output callback function for inflateBack - collects decompressed data
static int inflate_back_output_callback(void* desc, unsigned char* buf, unsigned len) {
    InflateBackOutput* output = static_cast<InflateBackOutput*>(desc);
    
    // Append to output buffer
    size_t old_size = output->buffer.size();
    output->buffer.resize(old_size + len);
    memcpy(output->buffer.data() + old_size, buf, len);
    output->total_written += len;
    
    return 0;  // Success
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for gzip header configuration + some content
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume gzip header configuration parameters
    uint8_t header_flags = fdp.ConsumeIntegral<uint8_t>();
    uint32_t mtime = fdp.ConsumeIntegral<uint32_t>();
    uint8_t os_type = fdp.ConsumeIntegral<uint8_t>() % 20;  // RFC1952 defines 0-20
    uint8_t xflags = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume extra field data if needed
    uint16_t extra_len = 0;
    std::vector<uint8_t> extra_data;
    if (header_flags & 0x04) {  // FEXTRA flag in gzip header
        extra_len = fdp.ConsumeIntegralInRange<uint16_t>(0, 1024);
        if (extra_len > 0 && fdp.remaining_bytes() >= extra_len) {
            extra_data = fdp.ConsumeBytes<uint8_t>(extra_len);
        } else {
            extra_len = 0;
        }
    }
    
    // Consume filename if needed
    std::string filename;
    if (header_flags & 0x08) {  // FNAME flag in gzip header
        filename = fdp.ConsumeRandomLengthString(256);
    }
    
    // Consume comment if needed
    std::string comment;
    if (header_flags & 0x10) {  // FCOMMENT flag in gzip header
        comment = fdp.ConsumeRandomLengthString(256);
    }
    
    // Consume the actual data to compress
    size_t data_to_compress_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    if (data_to_compress_size > fdp.remaining_bytes()) {
        data_to_compress_size = fdp.remaining_bytes();
    }
    std::vector<uint8_t> data_to_compress = fdp.ConsumeBytes<uint8_t>(data_to_compress_size);
    
    // ========== GZIP COMPRESSION PHASE (to generate test data for inflateBack) ==========
    z_stream c_stream;
    int err;
    
    // Initialize compression stream
    c_stream.zalloc = (alloc_func)0;
    c_stream.zfree = (free_func)0;
    c_stream.opaque = (voidpf)0;
    
    // Use windowBits = 31 for gzip with header
    int windowBits = 31;
    
    err = deflateInit2_(&c_stream, 
                       Z_DEFAULT_COMPRESSION,  // level
                       Z_DEFLATED,             // method
                       windowBits,             // windowBits for gzip
                       8,                      // memLevel
                       Z_DEFAULT_STRATEGY,     // strategy
                       ZLIB_VERSION,
                       sizeof(z_stream));
    
    if (err != Z_OK) {
        return 0;
    }
    
    // Prepare gzip header
    gz_header gzip_header;
    memset(&gzip_header, 0, sizeof(gzip_header));
    gzip_header.text = (header_flags & 0x01) ? 1 : 0;  // FTEXT flag
    gzip_header.time = mtime;
    gzip_header.xflags = xflags;
    gzip_header.os = os_type;
    
    // Set extra field if present
    if (!extra_data.empty()) {
        gzip_header.extra = extra_data.data();
        gzip_header.extra_len = extra_len;
        gzip_header.extra_max = extra_len;
    }
    
    // Set filename if present
    if (!filename.empty()) {
        gzip_header.name = (Bytef*)filename.c_str();
        gzip_header.name_max = filename.length() + 1;
    }
    
    // Set comment if present
    if (!comment.empty()) {
        gzip_header.comment = (Bytef*)comment.c_str();
        gzip_header.comm_max = comment.length() + 1;
    }
    
    gzip_header.hcrc = (header_flags & 0x02) ? 1 : 0;  // FHCRC flag
    
    err = deflateSetHeader(&c_stream, &gzip_header);
    if (err != Z_OK) {
        deflateEnd(&c_stream);
        return 0;
    }
    
    // Set input data for compression
    c_stream.next_in = data_to_compress.data();
    c_stream.avail_in = data_to_compress.size();
    
    // Allocate output buffer for compressed data
    size_t compressed_size = data_to_compress.size() * 2 + 512;  // Safe estimate
    std::vector<uint8_t> compressed_data(compressed_size);
    c_stream.next_out = compressed_data.data();
    c_stream.avail_out = compressed_size;
    
    // Perform compression
    err = deflate(&c_stream, Z_FINISH);
    if (err != Z_STREAM_END && err != Z_OK) {
        deflateEnd(&c_stream);
        return 0;
    }
    
    // Get actual compressed size
    compressed_size = compressed_size - c_stream.avail_out;
    
    // Clean up compression stream
    deflateEnd(&c_stream);
    
    // ========== GZIP DECOMPRESSION USING inflateBack() ==========
    z_stream d_stream;
    
    // Initialize decompression stream for inflateBack
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in = Z_NULL;
    d_stream.avail_in = 0;
    
    // For inflateBack with gzip, we need windowBits = 31 + 16 = 47
    // According to zlib.h: Add 16 to windowBits to decode gzip format
    int decomp_windowBits = 31 + 16;  // gzip format for inflateBack
    
    // Allocate window buffer for inflateBack
    size_t window_size = 1U << (decomp_windowBits & 0x1F);  // Window size based on lower bits
    if (window_size > 32768) {
        window_size = 32768;
    }
    unsigned char* window = static_cast<unsigned char*>(malloc(window_size));
    if (window == nullptr) {
        return 0;
    }
    
    // Initialize inflateBack with gzip support
    err = inflateBackInit_(&d_stream, decomp_windowBits, window, ZLIB_VERSION, sizeof(z_stream));
    if (err != Z_OK) {
        free(window);
        return 0;
    }
    
    // Setup input callback data structure
    InflateBackInput input_cb_data;
    input_cb_data.data = compressed_data.data();
    input_cb_data.size = compressed_size;
    input_cb_data.position = 0;
    
    // Setup output callback data structure
    InflateBackOutput output_cb_data;
    output_cb_data.total_written = 0;
    
    // Call inflateBack with our callbacks
    err = inflateBack(&d_stream, 
                     inflate_back_input_callback,
                     &input_cb_data,
                     inflate_back_output_callback,
                     &output_cb_data);
    
    // Check various error conditions that exercise different code paths
    // inflateBack returns Z_STREAM_END on success, Z_BUF_ERROR for callback issues,
    // Z_DATA_ERROR for format errors, or Z_STREAM_ERROR for initialization errors
    if (err != Z_STREAM_END && err != Z_BUF_ERROR && err != Z_DATA_ERROR) {
        // Unexpected error, clean up
        inflateBackEnd(&d_stream);
        free(window);
        return 0;
    }
    
    // Test with partial initial data in z_stream (alternative inflateBack usage)
    if (compressed_size > 10 && fdp.ConsumeBool()) {
        // Reset for another test
        inflateBackEnd(&d_stream);
        
        // Re-initialize with fresh window
        err = inflateBackInit_(&d_stream, decomp_windowBits, window, ZLIB_VERSION, sizeof(z_stream));
        if (err == Z_OK) {
            // Provide some initial data in the stream
            size_t initial_data = fdp.ConsumeIntegralInRange<size_t>(1, compressed_size / 2);
            d_stream.next_in = compressed_data.data();
            d_stream.avail_in = initial_data;
            
            // Reset input callback data for remaining data
            input_cb_data.data = compressed_data.data() + initial_data;
            input_cb_data.size = compressed_size - initial_data;
            input_cb_data.position = 0;
            
            // Reset output
            output_cb_data.buffer.clear();
            output_cb_data.total_written = 0;
            
            // Call inflateBack with mixed stream+callback input
            err = inflateBack(&d_stream,
                            inflate_back_input_callback,
                            &input_cb_data,
                            inflate_back_output_callback,
                            &output_cb_data);
            
            // Don't check error - exercising the code path is enough
            inflateBackEnd(&d_stream);
        }
    } else {
        inflateBackEnd(&d_stream);
    }
    
    // Test with intentionally malformed gzip data to exercise error paths
    if (fdp.remaining_bytes() > 20 && fdp.ConsumeBool()) {
        std::vector<uint8_t> malformed_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        z_stream test_stream;
        test_stream.zalloc = (alloc_func)0;
        test_stream.zfree = (free_func)0;
        test_stream.opaque = (voidpf)0;
        test_stream.next_in = Z_NULL;
        test_stream.avail_in = 0;
        
        // Use smaller window for malformed data test
        unsigned char* test_window = static_cast<unsigned char*>(malloc(4096));
        if (test_window != nullptr) {
            if (inflateBackInit_(&test_stream, 15 + 16, test_window, ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
                InflateBackInput malformed_input;
                malformed_input.data = malformed_data.data();
                malformed_input.size = malformed_data.size();
                malformed_input.position = 0;
                
                InflateBackOutput malformed_output;
                malformed_output.total_written = 0;
                
                // This will likely fail, exercising error handling paths
                inflateBack(&test_stream,
                          inflate_back_input_callback,
                          &malformed_input,
                          inflate_back_output_callback,
                          &malformed_output);
                
                inflateBackEnd(&test_stream);
            }
            free(test_window);
        }
    }
    
    // Clean up window buffer
    free(window);
    
    return 0;
}

} // namespace harness_011
