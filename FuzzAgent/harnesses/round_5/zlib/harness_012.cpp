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

namespace harness_012 {
/*
 * Fuzzing harness for systematic exploration of inflate() gzip header parsing
 * Targets: inflateInit2_(), inflateGetHeader(), inflate(), inflateEnd()
 * 
 * This harness specifically targets the 407 remaining blocked branches in inflate() 
 * gzip header parsing logic after fuzzer_011 execution. Focuses on systematic
 * exploration of gzip header field combinations, invalid compression methods,
 * unknown header flags, and field boundary cases.
 * 
 * Based on analysis of inflate.c gzip handling:
 * - Lines 558-562: Invalid compression method checking (must be Z_DEFLATED = 8)
 * - Lines 563-567: Unknown header flags checking (flags & 0xe000)
 * - Lines 596-629: Extra field processing (0x0400 flag)
 * - Lines 634-651: Filename processing (0x0800 flag)
 * - Lines 656-673: Comment processing (0x1000 flag)
 * - Lines 579-580, 590-591, 601-602: CRC16 header checks (0x0200 flag)
 * 
 * Strategy:
 * 1. Generate raw gzip header bytes directly to test inflate() parsing logic
 * 2. Use various windowBits values for gzip decoding (16+31, 32, 47, etc.)
 * 3. Test all combinations of gzip header flags (0x0200, 0x0400, 0x0800, 0x1000)
 * 4. Test invalid compression methods (0-7, 9-255)
 * 5. Test unknown header flags (0xe000 bits set)
 * 6. Test field boundary cases (max length, empty, special chars)
 * 7. Test invalid gzip headers to exercise error handling paths
 * 8. Use inflateGetHeader() to retrieve parsed header information
 */


// Build a raw gzip header based on configuration parameters
std::vector<uint8_t> build_gzip_header(
    uint8_t method,        // compression method (must be 8 for DEFLATED)
    uint8_t flags,         // FLG field (bit flags)
    uint32_t mtime,        // MTIME field (Unix timestamp)
    uint8_t xflags,        // XFL field (extra flags)
    uint8_t os,            // OS field (operating system)
    const std::vector<uint8_t>& extra_data, // extra field if FLG.FEXTRA set
    const std::string& filename,    // filename if FLG.FNAME set
    const std::string& comment,     // comment if FLG.FCOMMENT set
    bool include_crc16     // include CRC16 if FLG.FHCRC set
) {
    std::vector<uint8_t> header;
    
    // Gzip magic bytes: 0x1f, 0x8b
    header.push_back(0x1f);
    header.push_back(0x8b);
    
    // Compression method
    header.push_back(method);
    
    // Flags
    header.push_back(flags);
    
    // Modification time (4 bytes, little-endian)
    header.push_back((mtime >> 0) & 0xff);
    header.push_back((mtime >> 8) & 0xff);
    header.push_back((mtime >> 16) & 0xff);
    header.push_back((mtime >> 24) & 0xff);
    
    // Extra flags
    header.push_back(xflags);
    
    // Operating system
    header.push_back(os);
    
    // Extra field if FLG.FEXTRA (bit 2) is set
    if (flags & 0x04) {
        // Extra field length (2 bytes, little-endian)
        uint16_t extra_len = (uint16_t)extra_data.size();
        header.push_back((extra_len >> 0) & 0xff);
        header.push_back((extra_len >> 8) & 0xff);
        
        // Extra field data
        header.insert(header.end(), extra_data.begin(), extra_data.end());
    }
    
    // Filename if FLG.FNAME (bit 3) is set
    if (flags & 0x08) {
        header.insert(header.end(), filename.begin(), filename.end());
        header.push_back(0); // Null terminator
    }
    
    // Comment if FLG.FCOMMENT (bit 4) is set
    if (flags & 0x10) {
        header.insert(header.end(), comment.begin(), comment.end());
        header.push_back(0); // Null terminator
    }
    
    // CRC16 if FLG.FHCRC (bit 1) is set
    if ((flags & 0x02) && include_crc16) {
        // For simplicity, we'll use a dummy CRC16
        // In real testing, the fuzzer should explore invalid CRCs
        header.push_back(0x00); // CRC16 low byte
        header.push_back(0x00); // CRC16 high byte
    }
    
    return header;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for gzip header configuration
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // ========== CONSUME GZIP HEADER CONFIGURATION ==========
    
    // Consume compression method - test invalid values
    uint8_t method = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume flags - test all combinations including invalid ones
    uint8_t flags = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume modification time
    uint32_t mtime = fdp.ConsumeIntegral<uint32_t>();
    
    // Consume extra flags
    uint8_t xflags = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume OS field (0-20 defined by RFC1952, but test beyond)
    uint8_t os = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume extra field configuration
    uint16_t extra_len = 0;
    std::vector<uint8_t> extra_data;
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 2) {
        extra_len = fdp.ConsumeIntegralInRange<uint16_t>(0, 1024);
        if (extra_len > 0 && fdp.remaining_bytes() >= extra_len) {
            extra_data = fdp.ConsumeBytes<uint8_t>(extra_len);
        }
    }
    
    // Consume filename configuration
    std::string filename;
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        // Test boundary cases: empty, max length, special chars
        size_t max_filename_len = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
        filename = fdp.ConsumeRandomLengthString(max_filename_len);
        
        // Test with null bytes embedded (invalid but exercises parsing)
        if (fdp.ConsumeBool() && !filename.empty()) {
            size_t pos = fdp.ConsumeIntegralInRange<size_t>(0, filename.length() - 1);
            filename[pos] = '\0';
        }
    }
    
    // Consume comment configuration
    std::string comment;
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        // Test boundary cases
        size_t max_comment_len = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
        comment = fdp.ConsumeRandomLengthString(max_comment_len);
        
        // Test with null bytes embedded
        if (fdp.ConsumeBool() && !comment.empty()) {
            size_t pos = fdp.ConsumeIntegralInRange<size_t>(0, comment.length() - 1);
            comment[pos] = '\0';
        }
    }
    
    // Consume whether to include CRC16 (even if flag not set, or invalid CRC)
    bool include_crc16 = fdp.ConsumeBool();
    
    // Consume windowBits value for inflateInit2_
    // windowBits > 15 for gzip decoding, +32 for auto detection
    int windowBits = fdp.ConsumeIntegralInRange<int>(-15, 47);
    
    // ========== BUILD GZIP HEADER ==========
    
    std::vector<uint8_t> gzip_header = build_gzip_header(
        method, flags, mtime, xflags, os, 
        extra_data, filename, comment, include_crc16
    );
    
    // Add some compressed data (minimal deflate stream)
    // Simple stored block: 0x00 (final), 0x01 0x00 0xff 0xff (len=0, nlen=0)
    std::vector<uint8_t> compressed_data = {
        0x00, 0x01, 0x00, 0x00, 0xff, 0xff  // Empty stored block
    };
    
    // Combine header and compressed data
    std::vector<uint8_t> gzip_data = gzip_header;
    gzip_data.insert(gzip_data.end(), compressed_data.begin(), compressed_data.end());
    
    // ========== TEST INFLATE() WITH GZIP HEADER ==========
    
    z_stream d_stream;
    int err;
    
    // Initialize decompression stream
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in = Z_NULL;
    d_stream.avail_in = 0;
    
    // Initialize for gzip decoding
    // windowBits > 15 for gzip, +16 for gzip header/trailer
    // windowBits + 32 for automatic detection of zlib/gzip headers
    err = inflateInit2_(&d_stream, windowBits, ZLIB_VERSION, sizeof(z_stream));
    
    if (err != Z_OK) {
        // Initialization failed, but we should still test some error paths
        // For example, test with negative windowBits that might fail
        return 0;
    }
    
    // Set up gz_header structure for retrieving header info
    gz_header gzip_header_struct;
    memset(&gzip_header_struct, 0, sizeof(gzip_header_struct));
    
    // Allocate buffers for header fields if we expect to retrieve them
    if (flags & 0x04 && extra_len > 0) {
        gzip_header_struct.extra_max = extra_len + 16; // Some extra space
        gzip_header_struct.extra = (Bytef*)malloc(gzip_header_struct.extra_max);
    }
    
    if (flags & 0x08 && !filename.empty()) {
        gzip_header_struct.name_max = filename.length() + 16;
        gzip_header_struct.name = (Bytef*)malloc(gzip_header_struct.name_max);
    }
    
    if (flags & 0x10 && !comment.empty()) {
        gzip_header_struct.comm_max = comment.length() + 16;
        gzip_header_struct.comment = (Bytef*)malloc(gzip_header_struct.comm_max);
    }
    
    // Set header CRC flag based on input
    gzip_header_struct.hcrc = (flags & 0x02) ? 1 : 0;
    
    // Provide the header structure to inflate
    err = inflateGetHeader(&d_stream, &gzip_header_struct);
    if (err != Z_OK) {
        // inflateGetHeader can fail for some windowBits values
        // Continue anyway to test error paths
    }
    
    // Set input data
    d_stream.next_in = (Bytef*)gzip_data.data();
    d_stream.avail_in = (uInt)gzip_data.size();
    
    // Prepare output buffer
    size_t out_size = 4096;
    std::vector<uint8_t> out_buffer(out_size);
    d_stream.next_out = out_buffer.data();
    d_stream.avail_out = (uInt)out_size;
    
    // Call inflate() - this will parse the gzip header
    // We're mostly interested in the header parsing, not decompression
    err = inflate(&d_stream, Z_NO_FLUSH);
    
    // Check various error conditions that exercise different code paths
    // inflate returns Z_OK, Z_STREAM_END, Z_DATA_ERROR, Z_STREAM_ERROR, etc.
    
    // Test reset and retry with different parameters
    if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
        // Reset the stream and try again
        inflateEnd(&d_stream);
        
        // Reinitialize with different windowBits
        int new_windowBits = fdp.ConsumeIntegralInRange<int>(-15, 47);
        err = inflateInit2_(&d_stream, new_windowBits, ZLIB_VERSION, sizeof(z_stream));
        
        if (err == Z_OK) {
            // Try with partial input to test streaming/buffering
            size_t partial_size = fdp.ConsumeIntegralInRange<size_t>(1, gzip_data.size());
            d_stream.next_in = (Bytef*)gzip_data.data();
            d_stream.avail_in = (uInt)partial_size;
            d_stream.next_out = out_buffer.data();
            d_stream.avail_out = (uInt)out_size;
            
            // Call inflate with partial data
            inflate(&d_stream, Z_NO_FLUSH);
            
            // Provide remaining data
            if (partial_size < gzip_data.size()) {
                d_stream.next_in = (Bytef*)gzip_data.data() + partial_size;
                d_stream.avail_in = (uInt)(gzip_data.size() - partial_size);
                inflate(&d_stream, Z_NO_FLUSH);
            }
        }
    }
    
    // Test with intentionally malformed gzip data
    if (fdp.remaining_bytes() > 20 && fdp.ConsumeBool()) {
        std::vector<uint8_t> malformed_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        z_stream test_stream;
        test_stream.zalloc = (alloc_func)0;
        test_stream.zfree = (free_func)0;
        test_stream.opaque = (voidpf)0;
        
        // Try to parse malformed data as gzip
        if (inflateInit2_(&test_stream, 47, ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
            test_stream.next_in = (Bytef*)malformed_data.data();
            test_stream.avail_in = (uInt)malformed_data.size();
            
            std::vector<uint8_t> test_out(1024);
            test_stream.next_out = test_out.data();
            test_stream.avail_out = (uInt)test_out.size();
            
            // This will likely fail, exercising error handling paths
            inflate(&test_stream, Z_NO_FLUSH);
            
            inflateEnd(&test_stream);
        }
    }
    
    // Clean up allocated buffers
    if (gzip_header_struct.extra != Z_NULL) {
        free(gzip_header_struct.extra);
    }
    if (gzip_header_struct.name != Z_NULL) {
        free(gzip_header_struct.name);
    }
    if (gzip_header_struct.comment != Z_NULL) {
        free(gzip_header_struct.comment);
    }
    
    // Clean up decompression stream
    inflateEnd(&d_stream);
    
    return 0;
}

} // namespace harness_012
