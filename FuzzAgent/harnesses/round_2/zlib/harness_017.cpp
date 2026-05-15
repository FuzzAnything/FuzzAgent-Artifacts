#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_017 {
/*
 * Advanced fuzzing harness for zlib GUNZIP header processing via uncompress2_z API
 * Target: uncompress2_z (primary for direct inflate access), inflate (target function with 480 blocked branches),
 *         inflateValidate (secondary - 0% covered, 6 branches), inflateUndermine (secondary - 0% covered, 2 branches)
 * Harness ID: 017
 * 
 * This harness specifically targets the single most critical coverage gap: 480 blocked branches
 * in inflate function (76% of remaining branch complexity) by testing comprehensive GUNZIP format
 * scenarios through the uncompress2_z API which directly calls inflate.
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000 (basic compression/decompression): covers basic compress/uncompress
 * - harness_001 (gzip file I/O): covers gz* APIs
 * - harness_002 (inflateBack callback-based decompression): covers inflateBack APIs
 * - harness_003 (deflate parameter control): covers deflate* APIs
 * - harness_004 (advanced gzip I/O with dynamic parameter control): covers gz* with params
 * - harness_005 (gzip formatted I/O): covers gzip-specific APIs
 * - harness_006 (size_t variant compression/decompression): covers _z variants
 * - harness_007 (advanced inflate state manipulation): covers some inflate validation APIs
 * - harness_011 (header validation): covers basic header validation paths
 * - harness_012 (inflate validation and header manipulation): covers inflate validation APIs
 * - harness_013 (gzip header processing): covers GZIP header and some dictionary scenarios
 * - harness_014 (dictionary-based decompression): covers inflateSetDictionary specifically
 * - harness_015 (comprehensive inflate testing with gzip headers): covers inflate directly with GZIP headers
 * - harness_016 (advanced inflateBack callback-based decompression): covers inflateBack APIs
 * 
 * This harness differs from harness_015 by:
 * 1. Using uncompress2_z API instead of direct inflate calls (different call path to reach inflate)
 * 2. Targeting different error conditions and state transitions
 * 3. Including inflateValidate and inflateUndermine calls on stream state
 * 4. Testing more edge-case GUNZIP header configurations
 * 5. Focusing on the size_t variant API (uncompress2_z vs standard uncompress2)
 * 
 * Based on coverage guidance analysis:
 * - inflate function has 480 blocked branches representing massive undiscovered complexity
 * - Current fuzzing has focused on standard zlib compression but misses GUNZIP-specific header processing
 * - By targeting uncompress2_z (which directly calls inflate), we bypass gzip wrapper layers
 *   and directly test the inflate state machine with controlled GUNZIP headers
 * 
 * Test scenarios targeting blocked branches:
 * 1. GUNZIP header processing paths (HEAD→FLAGS→TIME→OS→EXLEN→EXTRA→NAME→COMMENT→HCRC states)
 * 2. Complex flag combinations in inflate state machine
 * 3. Header validation logic for zlib vs gzip formats with invalid headers
 * 4. Extra data, name, and comment sections in gzip headers
 * 5. CRC16/CRC32 validation in gzip format
 * 6. Window size combinations and error conditions
 * 7. Unknown compression method error paths
 * 8. State transitions requiring specific header configurations
 * 
 * Expected coverage gain:
 * - Target reduction of inflate's 480 blocked branches by exploring specific GUNZIP header processing
 * - Improve coverage of inflateValidate and inflateUndermine (currently 0% covered)
 * - Exercise error paths in GUNZIP header validation not covered by harness_015
 * - Test size_t variant API (uncompress2_z) which may have different code paths
 */


// Helper function to create gzip header with specific configuration
static std::vector<uint8_t> create_gzip_header_with_config(
    uint8_t method,          // Compression method (8 = DEFLATE)
    uint8_t flags,           // FLG field flags
    uint32_t mtime,          // Modification time
    uint8_t xfl,             // Extra flags
    uint8_t os,              // Operating system
    const std::vector<uint8_t>& extra_data,  // Optional extra field
    const std::string& filename,             // Optional filename
    const std::string& comment,              // Optional comment
    bool include_crc16       // Include CRC16 if FLG.FHCRC is set
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
    
    // Extra field if FLG.FEXTRA is set
    if (flags & 0x04) {  // FEXTRA flag
        if (!extra_data.empty()) {
            uint16_t xlen = (uint16_t)extra_data.size();
            header.push_back(xlen & 0xFF);
            header.push_back((xlen >> 8) & 0xFF);
            header.insert(header.end(), extra_data.begin(), extra_data.end());
        } else {
            // Empty extra field
            header.push_back(0);
            header.push_back(0);
        }
    }
    
    // Filename if FLG.FNAME is set
    if (flags & 0x08) {  // FNAME flag
        if (!filename.empty()) {
            header.insert(header.end(), filename.begin(), filename.end());
        }
        header.push_back(0);  // Null-terminator
    }
    
    // Comment if FLG.FCOMMENT is set
    if (flags & 0x10) {  // FCOMMENT flag
        if (!comment.empty()) {
            header.insert(header.end(), comment.begin(), comment.end());
        }
        header.push_back(0);  // Null-terminator
    }
    
    // CRC16 if FLG.FHCRC is set
    if (flags & 0x02) {  // FHCRC flag
        if (include_crc16) {
            // Placeholder CRC16 - in real implementation would calculate actual CRC
            header.push_back(0x00);
            header.push_back(0x00);
        }
    }
    
    return header;
}

// Helper function to create simple deflate compressed data
static std::vector<uint8_t> create_deflate_data(const std::vector<uint8_t>& input_data, int level) {
    std::vector<uint8_t> compressed;
    
    uLong compressed_size = compressBound((uLong)input_data.size());
    if (compressed_size == 0 || compressed_size > 10 * 1024 * 1024) {
        return compressed;
    }
    
    compressed.resize(compressed_size);
    
    int result = compress2(compressed.data(), &compressed_size,
                          input_data.data(), (uLong)input_data.size(),
                          level);
    
    if (result == Z_OK) {
        compressed.resize(compressed_size);
    } else {
        compressed.clear();
    }
    
    return compressed;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for comprehensive GUNZIP testing
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    uint8_t compression_method = fdp.ConsumeIntegral<uint8_t>() % 16;
    uint8_t gzip_flags = fdp.ConsumeIntegral<uint8_t>();
    uint32_t modification_time = fdp.ConsumeIntegral<uint32_t>();
    uint8_t extra_flags = fdp.ConsumeIntegral<uint8_t>();
    uint8_t os_type = fdp.ConsumeIntegral<uint8_t>() % 20;
    bool include_crc16 = fdp.ConsumeBool();
    bool test_invalid_headers = fdp.ConsumeBool();
    bool test_edge_cases = fdp.ConsumeBool();
    bool use_size_t_variant = true;  // Always use _z variant as per guidance
    bool test_validation_apis = fdp.ConsumeBool();
    bool test_undermine_api = fdp.ConsumeBool();
    
    // Window bits for deflate initialization
    int window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
    
    // Consume input data for compression
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
    if (fdp.remaining_bytes() < input_size) {
        return 0;
    }
    
    std::vector<uint8_t> input_data = fdp.ConsumeBytes<uint8_t>(input_size);
    
    // Consume extra field data if needed
    std::vector<uint8_t> extra_field_data;
    if (gzip_flags & 0x04) {  // FEXTRA flag
        size_t extra_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
        if (fdp.remaining_bytes() < extra_size) {
            extra_size = fdp.remaining_bytes();
        }
        if (extra_size > 0) {
            extra_field_data = fdp.ConsumeBytes<uint8_t>(extra_size);
        }
    }
    
    // Consume filename if needed
    std::string filename;
    if (gzip_flags & 0x08) {  // FNAME flag
        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(0, 128);
        filename = fdp.ConsumeRandomLengthString(name_len);
    }
    
    // Consume comment if needed
    std::string comment;
    if (gzip_flags & 0x10) {  // FCOMMENT flag
        size_t comment_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
        comment = fdp.ConsumeRandomLengthString(comment_len);
    }
    
    // Create compressed data
    std::vector<uint8_t> deflate_data = create_deflate_data(input_data, Z_DEFAULT_COMPRESSION);
    if (deflate_data.empty()) {
        return 0;
    }
    
    // Test different header scenarios
    for (int scenario = 0; scenario < 4; scenario++) {
        uint8_t test_method = compression_method;
        uint8_t test_flags = gzip_flags;
        
        // Modify parameters for different test scenarios
        switch (scenario) {
            case 0:
                // Standard valid gzip
                test_method = 8;  // DEFLATE
                break;
            case 1:
                // Invalid compression method
                test_method = (compression_method == 8) ? 9 : compression_method;
                break;
            case 2:
                // Test with various flag combinations
                test_flags ^= 0x1F;  // Flip all flag bits
                break;
            case 3:
                // Edge case: reserved flags set
                test_flags |= 0xE0;  // Set reserved bits
                break;
        }
        
        // Skip invalid method test if not requested
        if (test_method != 8 && !test_invalid_headers) {
            continue;
        }
        
        // Create gzip header
        std::vector<uint8_t> gzip_header = create_gzip_header_with_config(
            test_method, test_flags, modification_time, extra_flags, os_type,
            extra_field_data, filename, comment, include_crc16);
        
        // Combine header with compressed data
        std::vector<uint8_t> gzip_data;
        gzip_data.insert(gzip_data.end(), gzip_header.begin(), gzip_header.end());
        gzip_data.insert(gzip_data.end(), deflate_data.begin(), deflate_data.end());
        
        // Add trailing data (CRC32 and ISIZE)
        if (!test_edge_cases || fdp.ConsumeBool()) {
            // Add placeholder CRC32 (4 bytes)
            for (int i = 0; i < 4; i++) {
                gzip_data.push_back(fdp.ConsumeIntegral<uint8_t>());
            }
            
            // Add ISIZE (input size modulo 2^32, 4 bytes little-endian)
            uint32_t isize = (uint32_t)input_data.size();
            gzip_data.push_back(isize & 0xFF);
            gzip_data.push_back((isize >> 8) & 0xFF);
            gzip_data.push_back((isize >> 16) & 0xFF);
            gzip_data.push_back((isize >> 24) & 0xFF);
        }
        
        // Prepare output buffer
        size_t output_capacity = input_data.size() * 2 + 1024;  // Extra space
        if (output_capacity > 10 * 1024 * 1024) {
            output_capacity = 10 * 1024 * 1024;
        }
        
        std::vector<uint8_t> output_buffer(output_capacity);
        
        // Test uncompress2_z API
        z_size_t dest_len = output_capacity;
        z_size_t source_len = gzip_data.size();
        
        int result = uncompress2_z(output_buffer.data(), &dest_len,
                                  gzip_data.data(), &source_len);
        
        // If we have a valid stream, test advanced APIs on inflate state
        if (result == Z_OK || result == Z_STREAM_END) {
            // Create a separate stream to test validation APIs
            z_stream validation_stream;
            memset(&validation_stream, 0, sizeof(validation_stream));
            validation_stream.zalloc = Z_NULL;
            validation_stream.zfree = Z_NULL;
            validation_stream.opaque = Z_NULL;
            
            // Initialize with appropriate window bits for gzip
            int init_result = inflateInit2(&validation_stream, window_bits + 16);
            if (init_result == Z_OK) {
                // Set up the stream with our gzip data
                validation_stream.next_in = gzip_data.data();
                validation_stream.avail_in = (uInt)gzip_data.size();
                validation_stream.next_out = output_buffer.data();
                validation_stream.avail_out = (uInt)output_capacity;
                
                // Do a partial inflate to create valid state
                int inflate_result = inflate(&validation_stream, Z_NO_FLUSH);
                
                if (inflate_result == Z_OK || inflate_result == Z_STREAM_END) {
                    // Test inflateValidate if requested
                    if (test_validation_apis) {
                        int validation_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
                        inflateValidate(&validation_stream, validation_mode);
                    }
                    
                    // Test inflateUndermine if requested
                    if (test_undermine_api) {
                        int undermine_mode = fdp.ConsumeIntegralInRange<int>(0, 2);
                        inflateUndermine(&validation_stream, undermine_mode);
                    }
                }
                
                // Clean up validation stream
                inflateEnd(&validation_stream);
            }
        }
        
        // Test additional edge cases with remaining fuzzer input
        if (test_edge_cases && fdp.remaining_bytes() > 0) {
            // Create malformed headers with remaining data
            size_t malformed_size = fdp.ConsumeIntegralInRange<size_t>(16, 256);
            if (fdp.remaining_bytes() < malformed_size) {
                malformed_size = fdp.remaining_bytes();
            }
            
            std::vector<uint8_t> malformed_data = fdp.ConsumeBytes<uint8_t>(malformed_size);
            
            // Try to decompress malformed data
            z_size_t malformed_dest_len = output_capacity;
            z_size_t malformed_source_len = malformed_data.size();
            
            // Use a separate output buffer to avoid corrupting previous results
            std::vector<uint8_t> malformed_output(output_capacity);
            
            uncompress2_z(malformed_output.data(), &malformed_dest_len,
                         malformed_data.data(), &malformed_source_len);
        }
    }
    
    // Test raw deflate format (non-gzip) with uncompress2_z
    if (fdp.remaining_bytes() > 0) {
        // Create raw deflate data using deflateInit2 with negative window bits
        z_stream c_strm;
        memset(&c_strm, 0, sizeof(c_strm));
        c_strm.zalloc = Z_NULL;
        c_strm.zfree = Z_NULL;
        c_strm.opaque = Z_NULL;
        
        // Use remaining data for raw compression
        size_t raw_input_size = fdp.ConsumeIntegralInRange<size_t>(64, 1024);
        if (fdp.remaining_bytes() < raw_input_size) {
            raw_input_size = fdp.remaining_bytes();
        }
        
        std::vector<uint8_t> raw_input = fdp.ConsumeBytes<uint8_t>(raw_input_size);
        
        int deflate_init_result = deflateInit2(&c_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                                              -window_bits, 8, Z_DEFAULT_STRATEGY);
        
        if (deflate_init_result == Z_OK) {
            uLong raw_compressed_size = compressBound((uLong)raw_input.size());
            std::vector<uint8_t> raw_compressed(raw_compressed_size);
            
            c_strm.next_in = raw_input.data();
            c_strm.avail_in = (uInt)raw_input.size();
            c_strm.next_out = raw_compressed.data();
            c_strm.avail_out = (uInt)raw_compressed_size;
            
            int deflate_result = deflate(&c_strm, Z_FINISH);
            
            if (deflate_result == Z_STREAM_END) {
                size_t actual_compressed_size = raw_compressed_size - c_strm.avail_out;
                raw_compressed.resize(actual_compressed_size);
                
                // Test uncompress2_z on raw deflate data
                std::vector<uint8_t> raw_output(raw_input.size() * 2);
                z_size_t raw_dest_len = raw_output.size();
                z_size_t raw_source_len = raw_compressed.size();
                
                uncompress2_z(raw_output.data(), &raw_dest_len,
                             raw_compressed.data(), &raw_source_len);
            }
            
            deflateEnd(&c_strm);
        }
    }
    
    return 0;
}

} // namespace harness_017
