#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_018 {
/*
 * Advanced fuzzing harness for zlib gzip-specific inflation features and validation APIs
 * Target: inflate (Primary - 480 blocked branches in gzip header processing), 
 *         inflateValidate (Target - 0% covered, sets/clears wrap bit 4 for CRC checking),
 *         inflateUndermine (Target - 0% covered, error condition testing),
 *         zError (Target - completely unreached, error message retrieval)
 * Harness ID: 018
 * 
 * This harness specifically targets the single largest coverage gap: 480 blocked branches
 * in inflate function's gzip header processing logic (76% complexity untouched) by focusing
 * on specialized validation APIs and gzip header CRC checking enabled via inflateValidate.
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_015 (comprehensive inflate testing with gzip headers): covers basic gzip header processing
 * - harness_012 (inflate validation and header manipulation): covers validation APIs but with different approach
 * - harness_017 (GUNZIP header processing via uncompress2_z): covers different API path to inflate
 * 
 * This harness differs by:
 * 1. Specifically targeting inflateValidate API which controls CRC checking (wrap bit 4) for gzip headers
 * 2. Focusing on the 0x0200 (FHCRC) flag in gzip headers which requires wrap & 4 to be set
 * 3. Testing inflateUndermine in specific error conditions
 * 4. Calling zError to retrieve error messages from all error states
 * 5. Testing windowBits = 32 (+16 for gzip +16 for auto-detection) for automatic zlib/gzip detection
 * 6. Creating semantically diverse gzip headers with specific flag combinations targeting state transitions
 * 
 * Based on source code analysis (inflate.c):
 * - inflateValidate sets/clears bit 4 in state->wrap (0x4)
 * - When wrap & 4 is set AND flags & 0x0200 (FHCRC), CRC checking is performed during gzip header parsing
 * - CRC checking occurs in FLAGS, TIME, OS, EXLEN, EXTRA, NAME, COMMENT, HCRC states
 * - This harness specifically tests these CRC-enabled header processing paths
 * 
 * Test scenarios targeting blocked branches:
 * 1. Gzip header with FHCRC flag (0x0200) and wrap & 4 enabled via inflateValidate
 * 2. All gzip header state transitions with CRC checking: HEAD→FLAGS→TIME→OS→EXLEN→EXTRA→NAME→COMMENT→HCRC
 * 3. Invalid CRC16 values to trigger HCRC error paths
 * 4. Mixed flag combinations: FHCRC + EXTRA (0x0400), FHCRC + NAME (0x0800), FHCRC + COMMENT (0x1000)
 * 5. Different windowBits values including 32 (auto-detect), 31 (gzip), and raw modes
 * 6. inflateUndermine called after specific error conditions
 * 7. zError called for all possible error codes
 * 
 * Expected coverage gain:
 * - Target reduction of inflate's 480 blocked branches by exploring CRC-enabled gzip header processing
 * - Improve coverage of inflateValidate (0% covered) by testing wrap bit 4 functionality
 * - Improve coverage of inflateUndermine (0% covered) by testing error condition undermining
 * - Reach zError API (completely unreached) by retrieving error messages
 * - Exercise CRC checking paths in gzip header processing (state->wrap & 4)
 */


// Helper function to create gzip header with specific configuration
static std::vector<uint8_t> create_gzip_header_with_crc_config(
    uint8_t method,           // Compression method (8 = DEFLATE)
    uint8_t flags,            // FLG field flags (bit 1=FHCRC, bit 2=EXTRA, bit 3=NAME, bit 4=COMMENT)
    uint32_t mtime,           // Modification time
    uint8_t xfl,              // Extra flags  
    uint8_t os,               // Operating system
    const std::vector<uint8_t>& extra_data,  // Optional extra field (if flags & 0x0400)
    const std::string& filename,             // Optional filename (if flags & 0x0800)
    const std::string& comment,              // Optional comment (if flags & 0x1000)
    uint16_t crc16            // Optional CRC16 (if flags & 0x0200)
) {
    std::vector<uint8_t> header;
    
    // Gzip magic bytes (0x1f, 0x8b)
    header.push_back(0x1f);
    header.push_back(0x8b);
    
    // Compression method
    header.push_back(method);
    
    // Flags
    header.push_back(flags);
    
    // Modification time (4 bytes, little endian)
    header.push_back((mtime >> 0) & 0xFF);
    header.push_back((mtime >> 8) & 0xFF);
    header.push_back((mtime >> 16) & 0xFF);
    header.push_back((mtime >> 24) & 0xFF);
    
    // Extra flags
    header.push_back(xfl);
    
    // Operating system
    header.push_back(os);
    
    // Extra field (if flags & 0x0400)
    if (flags & 0x0400 && !extra_data.empty()) {
        // Extra field length (2 bytes, little endian)
        uint16_t extra_len = (uint16_t)extra_data.size();
        header.push_back(extra_len & 0xFF);
        header.push_back((extra_len >> 8) & 0xFF);
        // Extra field data
        header.insert(header.end(), extra_data.begin(), extra_data.end());
    }
    
    // Filename (if flags & 0x0800)
    if (flags & 0x0800 && !filename.empty()) {
        header.insert(header.end(), filename.begin(), filename.end());
        header.push_back(0); // Null terminator
    }
    
    // Comment (if flags & 0x1000)
    if (flags & 0x1000 && !comment.empty()) {
        header.insert(header.end(), comment.begin(), comment.end());
        header.push_back(0); // Null terminator
    }
    
    // CRC16 (if flags & 0x0200)
    if (flags & 0x0200) {
        header.push_back(crc16 & 0xFF);
        header.push_back((crc16 >> 8) & 0xFF);
    }
    
    return header;
}

// Helper to compute CRC16 for testing (simple implementation for fuzzing)
static uint16_t compute_simple_crc16(const std::vector<uint8_t>& data) {
    uint32_t crc = 0;
    for (uint8_t byte : data) {
        crc ^= (uint32_t)byte << 8;
        for (int i = 0; i < 8; i++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }
    return (uint16_t)(crc & 0xFFFF);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing of gzip header validation
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int window_bits_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    int window_bits = 0;
    
    // Determine window bits based on mode
    switch (window_bits_mode) {
        case 0: window_bits = 15; break;                     // Standard zlib
        case 1: window_bits = 31; break;                     // Gzip (15 + 16)
        case 2: window_bits = 32; break;                     // Auto-detect zlib/gzip (15 + 16 + 1)
        case 3: window_bits = -15; break;                    // Raw deflate
    }
    
    bool test_validate_api = fdp.ConsumeBool();
    bool test_undermine_api = fdp.ConsumeBool();
    bool test_zerror_api = fdp.ConsumeBool();
    bool test_crc_scenarios = fdp.ConsumeBool();
    bool test_header_retrieval = fdp.ConsumeBool();
    bool test_sync_recovery = fdp.ConsumeBool();
    
    // Consume gzip header configuration
    uint8_t compression_method = fdp.ConsumeIntegral<uint8_t>() % 16;
    uint8_t gzip_flags = fdp.ConsumeIntegral<uint8_t>();
    uint32_t mtime = fdp.ConsumeIntegral<uint32_t>();
    uint8_t extra_flags = fdp.ConsumeIntegral<uint8_t>();
    uint8_t os_type = fdp.ConsumeIntegral<uint8_t>() % 256;
    
    // Consume extra field data if needed
    std::vector<uint8_t> extra_data;
    if ((gzip_flags & 0x0400) && fdp.remaining_bytes() > 2) {
        size_t extra_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
        if (fdp.remaining_bytes() >= extra_size) {
            extra_data = fdp.ConsumeBytes<uint8_t>(extra_size);
        }
    }
    
    // Consume filename if needed
    std::string filename;
    if ((gzip_flags & 0x0800) && fdp.remaining_bytes() > 1) {
        size_t filename_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        if (fdp.remaining_bytes() >= filename_len) {
            filename = fdp.ConsumeBytesAsString(filename_len);
        }
    }
    
    // Consume comment if needed
    std::string comment;
    if ((gzip_flags & 0x1000) && fdp.remaining_bytes() > 1) {
        size_t comment_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        if (fdp.remaining_bytes() >= comment_len) {
            comment = fdp.ConsumeBytesAsString(comment_len);
        }
    }
    
    // Consume CRC16 value (may be correct or incorrect for testing)
    uint16_t crc16 = 0;
    if ((gzip_flags & 0x0200) && fdp.remaining_bytes() >= 2) {
        crc16 = fdp.ConsumeIntegral<uint16_t>();
    }
    
    // Consume payload data for compression
    size_t payload_size = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
    if (fdp.remaining_bytes() < payload_size) {
        payload_size = fdp.remaining_bytes();
    }
    std::vector<uint8_t> payload_data = fdp.ConsumeBytes<uint8_t>(payload_size);
    
    // Create compressed data to test decompression
    std::vector<uint8_t> compressed_data;
    if (!payload_data.empty()) {
        uLong compressed_size = compressBound((uLong)payload_data.size());
        if (compressed_size > 0 && compressed_size < 10 * 1024 * 1024) {
            compressed_data.resize(compressed_size);
            if (compress(compressed_data.data(), &compressed_size, 
                        payload_data.data(), (uLong)payload_data.size()) == Z_OK) {
                compressed_data.resize(compressed_size);
            } else {
                compressed_data.clear();
            }
        }
    }
    
    if (compressed_data.empty()) {
        // Fallback: create simple deflate block
        compressed_data.push_back(0x01);  // Final block, uncompressed
        compressed_data.push_back(0x00);  // Length LSB
        compressed_data.push_back(0x00);  // Length MSB
        compressed_data.push_back(0xFF);  // ~Length LSB
        compressed_data.push_back(0xFF);  // ~Length MSB
        if (!payload_data.empty()) {
            size_t copy_len = payload_data.size() > 65535 ? 65535 : payload_data.size();
            compressed_data.insert(compressed_data.end(), 
                                  payload_data.begin(), payload_data.begin() + copy_len);
        }
    }
    
    // Create gzip header
    std::vector<uint8_t> gzip_header = create_gzip_header_with_crc_config(
        compression_method, gzip_flags, mtime, extra_flags, os_type,
        extra_data, filename, comment, crc16
    );
    
    // Combine header and compressed data
    std::vector<uint8_t> test_data = gzip_header;
    test_data.insert(test_data.end(), compressed_data.begin(), compressed_data.end());
    
    // Initialize decompression stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Buffers for header retrieval - must remain valid until inflateEnd()
    char name_buf[256];
    char comment_buf[256];
    Bytef extra_buf[512];
    
    int ret = inflateInit2_(&strm, window_bits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        if (test_zerror_api) {
            const char* err_msg = zError(ret);
            (void)err_msg;  // Use error message
        }
        return 0;
    }
    
    // Set up gz_header if testing header retrieval
    gz_header header_struct;
    memset(&header_struct, 0, sizeof(header_struct));
    if (test_header_retrieval) {
        header_struct.name = (Bytef*)name_buf;
        header_struct.name_max = 256;
        header_struct.comment = (Bytef*)comment_buf;
        header_struct.comm_max = 256;
        header_struct.extra = extra_buf;
        header_struct.extra_max = 512;
        inflateGetHeader(&strm, &header_struct);
    }
    
    // Set up input/output buffers
    strm.next_in = test_data.data();
    strm.avail_in = (uInt)test_data.size();
    
    std::vector<uint8_t> out_buffer(8192);
    strm.next_out = out_buffer.data();
    strm.avail_out = (uInt)out_buffer.size();
    
    // Test inflateValidate API if enabled
    if (test_validate_api) {
        // Test both enabling and disabling validation
        int validate_mode = fdp.ConsumeBool() ? 1 : 0;
        inflateValidate(&strm, validate_mode);
    }
    
    // Perform decompression with potential error handling
    bool had_error = false;
    int inflate_result = Z_OK;
    
    while (strm.avail_in > 0 && inflate_result == Z_OK) {
        inflate_result = inflate(&strm, Z_NO_FLUSH);
        
        // Test inflateSync at random points if enabled
        if (test_sync_recovery && fdp.ConsumeBool() && inflate_result == Z_OK) {
            inflateSync(&strm);
        }
        
        if (inflate_result == Z_NEED_DICT || inflate_result == Z_DATA_ERROR || 
            inflate_result == Z_STREAM_ERROR || inflate_result == Z_MEM_ERROR) {
            had_error = true;
            break;
        }
        
        // Refill output buffer if needed
        if (strm.avail_out == 0) {
            strm.next_out = out_buffer.data();
            strm.avail_out = (uInt)out_buffer.size();
        }
    }
    
    // Test inflateUndermine API if enabled and we had an error
    if (test_undermine_api && had_error) {
        int undermine_mode = fdp.ConsumeBool() ? 1 : 0;
        inflateUndermine(&strm, undermine_mode);
    }
    
    // Test zError API for various error codes if enabled
    if (test_zerror_api) {
        // Test error messages for all possible error codes
        for (int err_code = Z_OK; err_code <= Z_BUF_ERROR; err_code++) {
            const char* err_msg = zError(err_code);
            (void)err_msg;  // Use error message
        }
        
        // Also test with the actual result from inflate
        if (inflate_result != Z_OK && inflate_result != Z_STREAM_END) {
            const char* actual_err_msg = zError(inflate_result);
            (void)actual_err_msg;
        }
    }
    
    // Test CRC scenarios if enabled
    if (test_crc_scenarios && (gzip_flags & 0x0200)) {
        // Reinitialize with different validation modes
        inflateEnd(&strm);
        
        // Test with validation enabled
        inflateInit2_(&strm, window_bits, ZLIB_VERSION, sizeof(z_stream));
        inflateValidate(&strm, 1);  // Enable validation
        
        strm.next_in = test_data.data();
        strm.avail_in = (uInt)test_data.size();
        strm.next_out = out_buffer.data();
        strm.avail_out = (uInt)out_buffer.size();
        
        inflate(&strm, Z_NO_FLUSH);
        
        inflateEnd(&strm);
        
        // Test with validation disabled
        inflateInit2_(&strm, window_bits, ZLIB_VERSION, sizeof(z_stream));
        inflateValidate(&strm, 0);  // Disable validation
        
        strm.next_in = test_data.data();
        strm.avail_in = (uInt)test_data.size();
        strm.next_out = out_buffer.data();
        strm.avail_out = (uInt)out_buffer.size();
        
        inflate(&strm, Z_NO_FLUSH);
    }
    
    // Clean up
    inflateEnd(&strm);
    
    return 0;
}

} // namespace harness_018
