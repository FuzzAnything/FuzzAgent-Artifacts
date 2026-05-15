#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_015 {
/*
 * Fuzzing harness for comprehensive inflate testing with gzip headers and complex decompression state transitions
 * Target: inflate (primary - 437 blocked branches in gzip header processing), inflateInit2_, inflateReset,
 *         inflateEnd, inflateGetHeader, inflateSetDictionary
 * Harness ID: 015
 * 
 * This harness specifically targets the 437 blocked branches in inflate function's gzip header processing
 * (69.4% blocked) focusing on GUNZIP-specific code paths and complex state machine transitions.
 * 
 * Different from existing harnesses:
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
 * 
 * This harness follows the exact recommended invocation sequence from coverage guidance:
 * 1. Call inflateInit2_ with different window bits (including negative values for gzip/zlib auto-detection)
 * 2. Prepare compressed test data including: raw deflate, zlib-wrapped, gzip-wrapped data with various header flags
 * 3. Call inflate repeatedly with varying avail_in/avail_out to test different buffer sizes
 * 4. Test inflate with Z_BLOCK mode to examine header processing incrementally
 * 5. Use inflateReset between test cases
 * 6. Test inflate with dictionary when applicable
 * 
 * Specialized test scenarios targeting blocked branches:
 * 1. Gzip header with FLAGS field containing text, extra, name, comment bits (state transitions: FLAGS→TIME→OS→EXLEN→EXTRA→NAME→COMMENT)
 * 2. Gzip header with CRC16 flag enabled (HCRC state)
 * 3. Invalid window size combinations triggering error paths
 * 4. Unknown compression method error paths
 * 5. Header check failures (incorrect header check)
 * 6. State transitions through TIME, OS, EXLEN, EXTRA, NAME, COMMENT gzip header sections
 * 7. Partial header reads requiring multiple inflate calls
 * 8. Z_BLOCK mode for incremental header examination
 * 9. Complex decompression state transitions (TYPE→TYPEDO→STORED→COPY_→COPY→TYPE)
 * 
 * Expected coverage gain:
 * - Target reduction of inflate's 437 blocked branches by exploring specific gzip header processing states
 * - Test complex state transitions that require specific header configurations
 * - Exercise error paths in gzip header validation
 * - Improve coverage of inflate's state machine logic
 */


// Helper function to create gzip header with specific flags
static std::vector<uint8_t> create_gzip_header(uint8_t flags, uint32_t mtime, uint8_t xfl, uint8_t os) {
    std::vector<uint8_t> header;
    
    // Gzip magic bytes (0x1f, 0x8b)
    header.push_back(0x1f);
    header.push_back(0x8b);
    
    // Compression method (8 = DEFLATE)
    header.push_back(8);
    
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
    
    return header;
}

// Helper function to create simple deflate block for testing
static std::vector<uint8_t> create_simple_deflate_block(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> compressed;
    
    // For simplicity, we'll use compress() to create actual deflate data
    // In a real harness, we'd craft specific deflate blocks, but for fuzzing
    // we'll use the library's own compression
    uLong compressed_size = compressBound((uLong)data.size());
    if (compressed_size == 0 || compressed_size > 10 * 1024 * 1024) {
        return compressed;
    }
    
    compressed.resize(compressed_size);
    int result = compress(compressed.data(), &compressed_size, 
                         data.data(), (uLong)data.size());
    
    if (result == Z_OK) {
        compressed.resize(compressed_size);
    } else {
        compressed.clear();
    }
    
    return compressed;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for comprehensive gzip header processing
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int window_bits = fdp.ConsumeIntegralInRange<int>(-15, 47); // Full range including gzip (+16)
    bool test_gzip_headers = fdp.ConsumeBool();
    bool test_zlib_wrapped = fdp.ConsumeBool();
    bool test_raw_deflate = fdp.ConsumeBool();
    bool use_z_block_mode = fdp.ConsumeBool();
    bool test_header_flags = fdp.ConsumeBool();
    bool test_crc16_flag = fdp.ConsumeBool();
    bool test_invalid_scenarios = fdp.ConsumeBool();
    bool test_partial_reads = fdp.ConsumeBool();
    bool test_dictionary = fdp.ConsumeBool();
    bool test_header_retrieval = fdp.ConsumeBool();
    
    // Gzip specific flags
    uint8_t gzip_flags = 0;
    if (test_header_flags) {
        // Set specific gzip header flags from fuzzer input
        if (fdp.ConsumeBool()) gzip_flags |= 0x01;  // FTEXT - text flag
        if (fdp.ConsumeBool()) gzip_flags |= 0x02;  // FHCRC - CRC16 flag
        if (fdp.ConsumeBool()) gzip_flags |= 0x04;  // FEXTRA - extra field flag
        if (fdp.ConsumeBool()) gzip_flags |= 0x08;  // FNAME - original filename flag
        if (fdp.ConsumeBool()) gzip_flags |= 0x10;  // FCOMMENT - comment flag
        if (test_crc16_flag) gzip_flags |= 0x02;    // Force CRC16 flag for testing
    }
    
    // Get input data for compression/decompression testing
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(256, 8192);
    if (fdp.remaining_bytes() < input_size) {
        return 0;
    }
    
    std::vector<uint8_t> input_data = fdp.ConsumeBytes<uint8_t>(input_size);
    
    // Get dictionary data if testing dictionary APIs
    std::vector<uint8_t> dictionary_data;
    if (test_dictionary && fdp.remaining_bytes() > 0) {
        size_t dict_size = fdp.ConsumeIntegralInRange<size_t>(1, 32768);
        if (fdp.remaining_bytes() < dict_size) {
            dict_size = fdp.remaining_bytes();
        }
        if (dict_size > 0) {
            dictionary_data = fdp.ConsumeBytes<uint8_t>(dict_size);
        }
    }
    
    // Initialize z_stream structure
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Variables for header retrieval
    gz_header gzheader;
    std::vector<uint8_t> extra_field_data;
    std::string filename_data;
    std::string comment_data;
    
    if (test_header_retrieval) {
        memset(&gzheader, 0, sizeof(gzheader));
        
        // Prepare buffers for header retrieval
        if (gzip_flags & 0x04) { // FEXTRA
            size_t extra_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            extra_field_data.resize(extra_len);
            for (size_t i = 0; i < extra_len; i++) {
                extra_field_data[i] = fdp.ConsumeIntegral<uint8_t>();
            }
            gzheader.extra = extra_field_data.data();
            gzheader.extra_len = (uInt)extra_len;
        }
        
        if (gzip_flags & 0x08) { // FNAME
            size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            filename_data = fdp.ConsumeRandomLengthString(name_len);
            filename_data.push_back('\0'); // Null-terminate
            gzheader.name = (Bytef*)filename_data.data();
        }
        
        if (gzip_flags & 0x10) { // FCOMMENT
            size_t comment_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            comment_data = fdp.ConsumeRandomLengthString(comment_len);
            comment_data.push_back('\0'); // Null-terminate
            gzheader.comment = (Bytef*)comment_data.data();
        }
        
        gzheader.time = fdp.ConsumeIntegral<uint32_t>();
        gzheader.xflags = fdp.ConsumeIntegral<uint8_t>();
        gzheader.os = fdp.ConsumeIntegral<uint8_t>();
        gzheader.extra_max = 0; // Let inflate allocate
        gzheader.name_max = 0;
        gzheader.comm_max = 0;
        gzheader.hcrc = 0;
        gzheader.done = 0;
    }
    
    // Test different compression formats
    int result = Z_OK;
    std::vector<uint8_t> compressed_data;
    if (test_gzip_headers) {
        // Create gzip header
        uint32_t mtime = fdp.ConsumeIntegral<uint32_t>();
        uint8_t xfl = fdp.ConsumeIntegral<uint8_t>();
        uint8_t os = fdp.ConsumeIntegral<uint8_t>();
        
        std::vector<uint8_t> gzip_header = create_gzip_header(gzip_flags, mtime, xfl, os);
        
        // Add extra field if flag is set
        if (gzip_flags & 0x04) { // FEXTRA
            uint16_t extra_len = fdp.ConsumeIntegral<uint16_t>();
            gzip_header.push_back(extra_len & 0xFF);
            gzip_header.push_back((extra_len >> 8) & 0xFF);
            
            for (uint16_t i = 0; i < extra_len && fdp.remaining_bytes() > 0; i++) {
                gzip_header.push_back(fdp.ConsumeIntegral<uint8_t>());
            }
        }
        
        // Add filename if flag is set
        if (gzip_flags & 0x08) { // FNAME
            std::string filename = fdp.ConsumeRandomLengthString(128);
            gzip_header.insert(gzip_header.end(), filename.begin(), filename.end());
            gzip_header.push_back(0); // Null terminator
        }
        
        // Add comment if flag is set
        if (gzip_flags & 0x10) { // FCOMMENT
            std::string comment = fdp.ConsumeRandomLengthString(128);
            gzip_header.insert(gzip_header.end(), comment.begin(), comment.end());
            gzip_header.push_back(0); // Null terminator
        }
        
        // Add CRC16 if flag is set
        if (gzip_flags & 0x02) { // FHCRC
            // For testing, we'll add a dummy CRC16 (actual CRC would be computed)
            gzip_header.push_back(0x00);
            gzip_header.push_back(0x00);
        }
        
        // Create deflate data
        std::vector<uint8_t> deflate_data = create_simple_deflate_block(input_data);
        if (!deflate_data.empty()) {
            // Combine header and deflate data
            compressed_data = gzip_header;
            compressed_data.insert(compressed_data.end(), 
                                  deflate_data.begin(), deflate_data.end());
            
            // Add CRC32 and ISIZE (simplified - in real gzip this would be computed)
            uint32_t crc = fdp.ConsumeIntegral<uint32_t>();
            uint32_t isize = (uint32_t)input_data.size();
            
            compressed_data.push_back((crc >> 0) & 0xFF);
            compressed_data.push_back((crc >> 8) & 0xFF);
            compressed_data.push_back((crc >> 16) & 0xFF);
            compressed_data.push_back((crc >> 24) & 0xFF);
            
            compressed_data.push_back((isize >> 0) & 0xFF);
            compressed_data.push_back((isize >> 8) & 0xFF);
            compressed_data.push_back((isize >> 16) & 0xFF);
            compressed_data.push_back((isize >> 24) & 0xFF);
        }
    } else if (test_zlib_wrapped) {
        // Use zlib's compress to create zlib-wrapped data
        compressed_data.resize(compressBound((uLong)input_data.size()));
        uLong compressed_size = (uLong)compressed_data.size();
        result = compress(compressed_data.data(), &compressed_size,
                         input_data.data(), (uLong)input_data.size());
        if (result == Z_OK) {
            compressed_data.resize(compressed_size);
        } else {
            compressed_data.clear();
        }
    } else if (test_raw_deflate) {
        // Get raw deflate data
        compressed_data = create_simple_deflate_block(input_data);
    } else {
        // Default: use the input as is (might be pre-compressed data)
        compressed_data = input_data;
    }
    
    if (compressed_data.empty()) {
        return 0;
    }
    
    // Initialize inflate with appropriate window bits
    // Adjust window_bits for gzip if needed
    int actual_window_bits = window_bits;
    if (test_gzip_headers && window_bits > 0) {
        actual_window_bits += 16; // Add 16 for gzip wrapper
    }
    
    result = inflateInit2(&strm, actual_window_bits);
    if (result != Z_OK) {
        // Try with default window bits if specific init failed
        result = inflateInit2(&strm, MAX_WBITS + 16); // Default to gzip
        if (result != Z_OK) {
            return 0;
        }
    }
    
    // Set up header retrieval if requested
    if (test_header_retrieval) {
        inflateGetHeader(&strm, &gzheader);
    }
    
    // Set up dictionary if requested and available
    if (test_dictionary && !dictionary_data.empty()) {
        // Note: inflateSetDictionary should only be called when inflate returns Z_NEED_DICT
        // We'll attempt to set it, but proper usage requires specific state
        inflateSetDictionary(&strm, dictionary_data.data(), (uInt)dictionary_data.size());
    }
    
    // Prepare output buffer
    size_t output_capacity = input_data.size() * 4; // Allow for expansion
    std::vector<uint8_t> output_buffer(output_capacity);
    
    strm.next_in = compressed_data.data();
    strm.avail_in = (uInt)compressed_data.size();
    strm.next_out = output_buffer.data();
    strm.avail_out = (uInt)output_capacity;
    
    // Test inflate with different flush modes and partial reads
    int flush_mode = use_z_block_mode ? Z_BLOCK : Z_FINISH;
    
    if (test_partial_reads) {
        // Test partial reads to exercise state transitions
        size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, compressed_data.size() / 4);
        if (chunk_size == 0) chunk_size = 1;
        
        while (strm.avail_in > 0 && result != Z_STREAM_END && result >= Z_OK) {
            // Save current position
            Bytef* saved_next_in = strm.next_in;
            uInt saved_avail_in = strm.avail_in;
            
            // Limit the amount of input available for this inflate call
            size_t process_size = (saved_avail_in > chunk_size) ? chunk_size : saved_avail_in;
            strm.avail_in = (uInt)process_size;
            
            result = inflate(&strm, Z_NO_FLUSH);
            
            // After inflate returns, next_in and avail_in are already updated
            // We need to restore the remaining input beyond what was processed
            size_t bytes_consumed = process_size - strm.avail_in;
            strm.next_in = saved_next_in + bytes_consumed;
            strm.avail_in = saved_avail_in - (uInt)bytes_consumed;
            
            // Occasionally change flush mode
            if (fdp.ConsumeBool()) {
                flush_mode = fdp.ConsumeIntegralInRange<int>(Z_NO_FLUSH, Z_BLOCK);
            }
            
            // Check if we need to reset due to error
            if (result == Z_STREAM_ERROR || result == Z_DATA_ERROR || result == Z_MEM_ERROR) {
                if (test_invalid_scenarios) {
                    // This is expected for invalid scenarios, try to reset
                    inflateReset(&strm);
                    result = Z_OK;
                } else {
                    break;
                }
            }
        }
        
        // Finish with the requested flush mode
        if (result >= Z_OK) {
            result = inflate(&strm, flush_mode);
        }
    } else {
        // Standard single-call inflate
        result = inflate(&strm, flush_mode);
        
        // If Z_BLOCK mode returned, we might need to continue
        if (result == Z_OK && use_z_block_mode) {
            // Continue processing blocks
            while (result == Z_OK && strm.avail_in > 0) {
                result = inflate(&strm, Z_BLOCK);
            }
        }
    }
    
    // Test inflateReset and re-inflation if we have remaining data
    if (result == Z_STREAM_END || (result >= Z_OK && strm.avail_in == 0)) {
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
            // Reset stream and try with remaining fuzzer data
            inflateReset(&strm);
            
            // Use remaining fuzzer data as new input
            std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
            if (!remaining_data.empty()) {
                strm.next_in = remaining_data.data();
                strm.avail_in = (uInt)remaining_data.size();
                strm.next_out = output_buffer.data();
                strm.avail_out = (uInt)output_capacity;
                
                result = inflate(&strm, Z_FINISH);
            }
        }
    }
    
    // Test dictionary scenario more thoroughly
    if (test_dictionary && !dictionary_data.empty() && result == Z_NEED_DICT) {
        // This is the proper state to call inflateSetDictionary
        inflateSetDictionary(&strm, dictionary_data.data(), (uInt)dictionary_data.size());
        
        // Continue inflation
        if (result == Z_NEED_DICT) {
            result = inflate(&strm, Z_FINISH);
        }
    }
    
    // Clean up
    inflateEnd(&strm);
    
    return 0;
}

} // namespace harness_015
