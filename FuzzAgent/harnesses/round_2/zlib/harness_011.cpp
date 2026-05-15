#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_011 {
/*
 * Fuzzing harness for zlib inflate header validation and gzip processing paths
 * Target: inflate (primary - 319 blocked branches), uncompress2/uncompress, 
 *         inflateEnd, inflateInit2_/inflateInit_
 * Harness ID: 011
 * 
 * This harness specifically targets the blocked header validation and gzip processing
 * paths in inflate() which has 319 blocked branches (50.6% of its total).
 * 
 * Different from existing harnesses:
 * - harness_000 (basic compression/decompression): covers basic compress/uncompress
 * - harness_002 (inflateBack callback-based decompression): covers inflateBack APIs
 * - harness_006 (size_t variant compression/decompression): covers _z variants  
 * - harness_007 (advanced inflate state manipulation): covers inflate* state APIs
 * 
 * This harness focuses on header validation error paths:
 * 1. Gzip header validation (magic 0x8b1f)
 * 2. Zlib header validation (checksum % 31 == 0)
 * 3. Invalid compression method handling (!= Z_DEFLATED)
 * 4. Invalid window size validation (len > 15 or inconsistent with wbits)
 * 5. Mixed valid/invalid header patterns
 * 6. Both uncompress2() entry point and stream API approaches
 * 
 * The harness creates semantically diverse header validation scenarios:
 * - Valid and invalid gzip headers with various flag combinations
 * - Valid and invalid zlib headers that pass/fail checksum validation
 * - Headers with compression method != 8 (Z_DEFLATED)
 * - Headers with window size > 15 or inconsistent with wbits
 * - Error recovery from invalid headers
 * - State consistency after header validation failures
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing of header validation
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    bool use_uncompress2 = fdp.ConsumeBool();
    bool use_stream_api = fdp.ConsumeBool();
    bool test_gzip_headers = fdp.ConsumeBool();
    bool test_zlib_headers = fdp.ConsumeBool();
    bool test_invalid_methods = fdp.ConsumeBool();
    bool test_invalid_window = fdp.ConsumeBool();
    bool test_mixed_headers = fdp.ConsumeBool();
    
    int window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
    
    // Determine buffer size for compressed data
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
    
    // Check if we have enough input data
    if (fdp.remaining_bytes() < buffer_size) {
        return 0;
    }
    
    // Consume the input data for header crafting
    std::vector<uint8_t> input_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
    
    // Create output buffer for decompression
    size_t output_capacity = buffer_size * 4; // Allow for expansion
    uint8_t* output_buffer = (uint8_t*)malloc(output_capacity);
    if (!output_buffer) {
        return 0;
    }
    
    int result = Z_OK;
    
    // Test different header validation scenarios
    switch (test_scenario) {
        case 0: {
            // Test 1: Gzip header validation (magic 0x8b1f)
            if (test_gzip_headers) {
                // Craft a gzip header with magic 0x8b1f
                std::vector<uint8_t> crafted_data;
                
                // Add gzip magic bytes (0x1f, 0x8b) - little endian
                crafted_data.push_back(0x1f);
                crafted_data.push_back(0x8b);
                
                // Compression method (8 = Z_DEFLATED, or invalid)
                if (test_invalid_methods) {
                    // Use invalid compression method (!= 8)
                    uint8_t method = fdp.ConsumeIntegral<uint8_t>();
                    if (method == 8) method = 7; // Ensure != 8
                    crafted_data.push_back(method);
                } else {
                    crafted_data.push_back(8); // Z_DEFLATED
                }
                
                // Flags
                crafted_data.push_back(fdp.ConsumeIntegral<uint8_t>());
                
                // Modification time (4 bytes)
                uint32_t mtime = fdp.ConsumeIntegral<uint32_t>();
                crafted_data.push_back((mtime >> 0) & 0xFF);
                crafted_data.push_back((mtime >> 8) & 0xFF);
                crafted_data.push_back((mtime >> 16) & 0xFF);
                crafted_data.push_back((mtime >> 24) & 0xFF);
                
                // Extra flags
                crafted_data.push_back(fdp.ConsumeIntegral<uint8_t>());
                
                // OS (3 = Unix, but any value allowed)
                crafted_data.push_back(fdp.ConsumeIntegral<uint8_t>() % 256);
                
                // Add some actual compressed data (or invalid data)
                size_t data_to_add = fdp.ConsumeIntegralInRange<size_t>(16, 256);
                if (data_to_add > input_buffer.size()) {
                    data_to_add = input_buffer.size();
                }
                crafted_data.insert(crafted_data.end(), 
                                   input_buffer.begin(), 
                                   input_buffer.begin() + data_to_add);
                
                // Test with uncompress2
                if (use_uncompress2) {
                    uLongf dest_len = output_capacity;
                    uLongf source_len = crafted_data.size();
                    result = uncompress2(output_buffer, &dest_len,
                                        crafted_data.data(), &source_len);
                } 
                // Test with stream API
                else if (use_stream_api) {
                    z_stream strm;
                    memset(&strm, 0, sizeof(strm));
                    strm.zalloc = Z_NULL;
                    strm.zfree = Z_NULL;
                    strm.opaque = Z_NULL;
                    
                    // Initialize for gzip (window_bits + 16)
                    result = inflateInit2_(&strm, window_bits + 16, 
                                          ZLIB_VERSION, sizeof(z_stream));
                    
                    if (result == Z_OK) {
                        strm.next_in = crafted_data.data();
                        strm.avail_in = crafted_data.size();
                        strm.next_out = output_buffer;
                        strm.avail_out = output_capacity;
                        
                        result = inflate(&strm, Z_NO_FLUSH);
                        
                        inflateEnd(&strm);
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Test 2: Zlib header validation (checksum % 31 == 0)
            if (test_zlib_headers) {
                // Craft a zlib header
                std::vector<uint8_t> crafted_data;
                
                // CMF byte: compression method and info
                // Bits 0-3: CM (compression method, 8 = Z_DEFLATED)
                // Bits 4-7: CINFO (window size, 0-7 = 256 bytes to 32K)
                uint8_t cmf = 0;
                
                if (test_invalid_methods) {
                    // Set invalid compression method in bits 0-3
                    uint8_t method = fdp.ConsumeIntegral<uint8_t>() & 0x0F;
                    if ((method & 0x0F) == 8) method = 7; // Ensure != 8
                    cmf |= (method & 0x0F);
                } else {
                    cmf |= 8; // Z_DEFLATED
                }
                
                // Set window size
                uint8_t cinfo = fdp.ConsumeIntegralInRange<uint8_t>(0, 7);
                if (test_invalid_window && cinfo > 7) {
                    cinfo = 8; // Invalid window size (> 7)
                }
                cmf |= (cinfo << 4);
                crafted_data.push_back(cmf);
                
                // FLG byte: check bits, compression level, etc.
                // Must satisfy: (cmf * 256 + flg) % 31 == 0
                uint8_t flg = 0;
                
                // Set compression level bits (6-7)
                uint8_t level = fdp.ConsumeIntegral<uint8_t>() % 4;
                flg |= (level << 6);
                
                // Set preset dictionary flag (bit 5)
                if (fdp.ConsumeBool()) {
                    flg |= 0x20;
                }
                
                // Now adjust flg to make checksum valid or invalid
                uint16_t header = (cmf << 8) | flg;
                uint8_t remainder = header % 31;
                
                if (fdp.ConsumeBool()) {
                    // Make valid checksum
                    if (remainder != 0) {
                        flg = (flg + (31 - remainder)) & 0xFF;
                    }
                } else {
                    // Make invalid checksum
                    if (remainder == 0) {
                        flg = (flg + 1) & 0xFF;
                    }
                }
                
                crafted_data.push_back(flg);
                
                // Add some actual compressed data
                size_t data_to_add = fdp.ConsumeIntegralInRange<size_t>(16, 256);
                if (data_to_add > input_buffer.size()) {
                    data_to_add = input_buffer.size();
                }
                crafted_data.insert(crafted_data.end(),
                                   input_buffer.begin(),
                                   input_buffer.begin() + data_to_add);
                
                // Test with uncompress2
                if (use_uncompress2) {
                    uLongf dest_len = output_capacity;
                    uLongf source_len = crafted_data.size();
                    result = uncompress2(output_buffer, &dest_len,
                                        crafted_data.data(), &source_len);
                }
                // Test with stream API
                else if (use_stream_api) {
                    z_stream strm;
                    memset(&strm, 0, sizeof(strm));
                    strm.zalloc = Z_NULL;
                    strm.zfree = Z_NULL;
                    strm.opaque = Z_NULL;
                    
                    // Initialize for zlib (window_bits)
                    result = inflateInit2_(&strm, window_bits,
                                          ZLIB_VERSION, sizeof(z_stream));
                    
                    if (result == Z_OK) {
                        strm.next_in = crafted_data.data();
                        strm.avail_in = crafted_data.size();
                        strm.next_out = output_buffer;
                        strm.avail_out = output_capacity;
                        
                        result = inflate(&strm, Z_NO_FLUSH);
                        
                        inflateEnd(&strm);
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Test 3: Invalid compression methods (!= Z_DEFLATED)
            if (test_invalid_methods) {
                // Craft header with invalid compression method
                std::vector<uint8_t> crafted_data;
                
                // Create a basic header but with invalid method
                // Try both gzip and zlib formats
                uint8_t format_choice = fdp.ConsumeIntegral<uint8_t>() % 2;
                
                if (format_choice == 0) {
                    // Gzip format with invalid method
                    crafted_data.push_back(0x1f); // gzip magic
                    crafted_data.push_back(0x8b); // gzip magic
                    crafted_data.push_back(fdp.ConsumeIntegralInRange<uint8_t>(0, 7)); // Invalid method (0-7, not 8)
                    crafted_data.push_back(fdp.ConsumeIntegral<uint8_t>()); // flags
                    
                    // Add minimal header continuation
                    for (int i = 0; i < 6; i++) {
                        crafted_data.push_back(fdp.ConsumeIntegral<uint8_t>());
                    }
                } else {
                    // Zlib format with invalid method
                    uint8_t cmf = fdp.ConsumeIntegralInRange<uint8_t>(0, 7); // Invalid method in bits 0-3
                    cmf |= (fdp.ConsumeIntegralInRange<uint8_t>(0, 7) << 4); // window size
                    crafted_data.push_back(cmf);
                    
                    // FLG byte - try to make checksum invalid
                    uint8_t flg = fdp.ConsumeIntegral<uint8_t>();
                    crafted_data.push_back(flg);
                }
                
                // Add some data
                size_t data_to_add = fdp.ConsumeIntegralInRange<size_t>(16, 128);
                if (data_to_add > input_buffer.size()) {
                    data_to_add = input_buffer.size();
                }
                crafted_data.insert(crafted_data.end(),
                                   input_buffer.begin(),
                                   input_buffer.begin() + data_to_add);
                
                // Test decompression - should fail with "unknown compression method"
                uLongf dest_len = output_capacity;
                uLongf source_len = crafted_data.size();
                result = uncompress2(output_buffer, &dest_len,
                                    crafted_data.data(), &source_len);
            }
            break;
        }
        
        case 3: {
            // Test 4: Invalid window sizes (len > 15 or inconsistent)
            if (test_invalid_window) {
                // Craft zlib header with invalid window size
                std::vector<uint8_t> crafted_data;
                
                // CMF byte with invalid window size (> 7 in CINFO bits)
                uint8_t cmf = 8; // Z_DEFLATED
                cmf |= (fdp.ConsumeIntegralInRange<uint8_t>(8, 15) << 4); // Invalid window size
                crafted_data.push_back(cmf);
                
                // FLG byte - make checksum valid to isolate window size error
                uint8_t flg = 0;
                uint16_t header = (cmf << 8) | flg;
                uint8_t remainder = header % 31;
                if (remainder != 0) {
                    flg = (31 - remainder) & 0xFF;
                }
                crafted_data.push_back(flg);
                
                // Add some data
                size_t data_to_add = fdp.ConsumeIntegralInRange<size_t>(16, 128);
                if (data_to_add > input_buffer.size()) {
                    data_to_add = input_buffer.size();
                }
                crafted_data.insert(crafted_data.end(),
                                   input_buffer.begin(),
                                   input_buffer.begin() + data_to_add);
                
                // Test with stream API to specify window bits
                if (use_stream_api) {
                    z_stream strm;
                    memset(&strm, 0, sizeof(strm));
                    strm.zalloc = Z_NULL;
                    strm.zfree = Z_NULL;
                    strm.opaque = Z_NULL;
                    
                    // Initialize with window bits that might be inconsistent
                    int wbits = fdp.ConsumeIntegralInRange<int>(8, 15);
                    result = inflateInit2_(&strm, wbits,
                                          ZLIB_VERSION, sizeof(z_stream));
                    
                    if (result == Z_OK) {
                        strm.next_in = crafted_data.data();
                        strm.avail_in = crafted_data.size();
                        strm.next_out = output_buffer;
                        strm.avail_out = output_capacity;
                        
                        result = inflate(&strm, Z_NO_FLUSH);
                        
                        inflateEnd(&strm);
                    }
                } else {
                    // Test with uncompress2
                    uLongf dest_len = output_capacity;
                    uLongf source_len = crafted_data.size();
                    result = uncompress2(output_buffer, &dest_len,
                                        crafted_data.data(), &source_len);
                }
            }
            break;
        }
        
        case 4: {
            // Test 5: Mixed valid/invalid header patterns
            if (test_mixed_headers) {
                // Create a buffer with alternating valid/invalid headers
                std::vector<uint8_t> crafted_data;
                
                int num_segments = fdp.ConsumeIntegralInRange<int>(2, 8);
                for (int i = 0; i < num_segments && fdp.remaining_bytes() > 4; i++) {
                    bool make_valid = fdp.ConsumeBool();
                    
                    if (make_valid) {
                        // Add valid zlib header
                        uint8_t cmf = 0x78; // Common valid header (8K window, Z_DEFLATED)
                        uint8_t flg = 0x9C; // Makes checksum valid: 0x789C % 31 = 0
                        crafted_data.push_back(cmf);
                        crafted_data.push_back(flg);
                    } else {
                        // Add invalid header
                        uint8_t cmf = fdp.ConsumeIntegral<uint8_t>();
                        uint8_t flg = fdp.ConsumeIntegral<uint8_t>();
                        crafted_data.push_back(cmf);
                        crafted_data.push_back(flg);
                    }
                    
                    // Add a small amount of data
                    size_t seg_size = fdp.ConsumeIntegralInRange<size_t>(4, 32);
                    if (seg_size > input_buffer.size()) {
                        seg_size = input_buffer.size();
                    }
                    if (seg_size > 0) {
                        crafted_data.insert(crafted_data.end(),
                                           input_buffer.begin(),
                                           input_buffer.begin() + seg_size);
                    }
                }
                
                // Test decompression
                uLongf dest_len = output_capacity;
                uLongf source_len = crafted_data.size();
                result = uncompress2(output_buffer, &dest_len,
                                    crafted_data.data(), &source_len);
            }
            break;
        }
        
        case 5: {
            // Test 6: Raw deflate stream (no header) with different window bits
            if (use_stream_api) {
                // Create raw deflate data (no header)
                std::vector<uint8_t> crafted_data = input_buffer;
                
                z_stream strm;
                memset(&strm, 0, sizeof(strm));
                strm.zalloc = Z_NULL;
                strm.zfree = Z_NULL;
                strm.opaque = Z_NULL;
                
                // Initialize for raw deflate (negative window bits)
                int raw_wbits = -window_bits;
                result = inflateInit2_(&strm, raw_wbits,
                                      ZLIB_VERSION, sizeof(z_stream));
                
                if (result == Z_OK) {
                    strm.next_in = crafted_data.data();
                    strm.avail_in = crafted_data.size();
                    strm.next_out = output_buffer;
                    strm.avail_out = output_capacity;
                    
                    result = inflate(&strm, Z_NO_FLUSH);
                    
                    inflateEnd(&strm);
                }
            }
            break;
        }
    }
    
    // Clean up
    free(output_buffer);
    
    return 0;
}

} // namespace harness_011
