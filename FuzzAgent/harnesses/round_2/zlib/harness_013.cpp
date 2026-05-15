#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_013 {
/*
 * Fuzzing harness for deep coverage of inflate() GZIP header processing and dictionary handling in zlib
 * Target: inflate() (353 blocked branches), inflateSetDictionary (16 undiscovered branches), 
 *         inflateGetHeader (supporting), deflateSetDictionary (helper), inflateInit2_ (with MAX_WBITS + 16)
 * Harness ID: 013
 * 
 * This harness specifically targets the critical coverage gap in inflate() function with 353 blocked branches
 * (56% blocked) focusing on GUNZIP-specific code paths and complex state machine transitions.
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
 * 
 * This harness follows the exact recommended invocation sequence:
 * 1. inflateInit2_ with MAX_WBITS + 16 for gzip wrapper support
 * 2. Set up gzip header scenarios with various flags (text, crc, extra, name, comment)
 * 3. inflateSetDictionary with crafted dictionaries
 * 4. inflateGetHeader to retrieve gzip header information
 * 5. inflate with varied input to exercise different state transitions
 * 6. inflateEnd for cleanup
 * 
 * Specialized test scenarios:
 * 1. Gzip headers with all flag combinations (text=1, crc=1, extra=1, name=1, comment=1)
 * 2. Invalid header checks to trigger error paths
 * 3. Window size validation edge cases
 * 4. Dictionary matching with partial and full overlaps
 * 5. Back-referencing patterns that trigger specific inflate modes
 * 6. Sync points and validation failures
 * 
 * Expected coverage gain:
 * - Target reduction of inflate's 353 blocked branches by 30-40%
 * - Uncover 100+ new branches in inflate.c
 * - Improve gzlib.c and infback.c coverage
 * 
 * Memory Lifetime Fix: Header retrieval vectors now declared at function scope to ensure
 * pointers remain valid throughout entire inflate() operation, preventing use-after-free.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing of GZIP header processing
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
    int compression_level = fdp.ConsumeIntegralInRange<int>(-1, 9);
    int mem_level = fdp.ConsumeIntegralInRange<int>(1, 9);
    int strategy = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // GZIP header flags from fuzzer input
    bool gzip_text_flag = fdp.ConsumeBool();
    bool gzip_crc_flag = fdp.ConsumeBool();
    bool gzip_extra_flag = fdp.ConsumeBool();
    bool gzip_name_flag = fdp.ConsumeBool();
    bool gzip_comment_flag = fdp.ConsumeBool();
    
    // Test scenario selection
    bool test_valid_gzip = fdp.ConsumeBool();
    bool test_invalid_gzip = fdp.ConsumeBool();
    bool test_dictionary = fdp.ConsumeBool();
    bool test_header_retrieval = fdp.ConsumeBool();
    bool test_sync_points = fdp.ConsumeBool();
    
    // Get input data for compression
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(256, 16384);
    if (fdp.remaining_bytes() < input_size) {
        return 0;
    }
    
    std::vector<uint8_t> input_buffer = fdp.ConsumeBytes<uint8_t>(input_size);
    
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
    
    // Get extra field data for gzip header
    std::vector<uint8_t> extra_field_data;
    if (gzip_extra_flag && fdp.remaining_bytes() > 0) {
        size_t extra_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        if (fdp.remaining_bytes() < extra_size) {
            extra_size = fdp.remaining_bytes();
        }
        if (extra_size > 0) {
            extra_field_data = fdp.ConsumeBytes<uint8_t>(extra_size);
        }
    }
    
    // Get filename for gzip header
    std::string filename;
    if (gzip_name_flag && fdp.remaining_bytes() > 0) {
        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        if (fdp.remaining_bytes() < name_len) {
            name_len = fdp.remaining_bytes();
        }
        if (name_len > 0) {
            std::vector<uint8_t> name_bytes = fdp.ConsumeBytes<uint8_t>(name_len);
            filename.assign(reinterpret_cast<char*>(name_bytes.data()), name_bytes.size());
        }
    }
    
    // Get comment for gzip header
    std::string comment;
    if (gzip_comment_flag && fdp.remaining_bytes() > 0) {
        size_t comment_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        if (fdp.remaining_bytes() < comment_len) {
            comment_len = fdp.remaining_bytes();
        }
        if (comment_len > 0) {
            std::vector<uint8_t> comment_bytes = fdp.ConsumeBytes<uint8_t>(comment_len);
            comment.assign(reinterpret_cast<char*>(comment_bytes.data()), comment_bytes.size());
        }
    }
    
    // Prepare compression stream to generate test data
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    
    // Initialize deflate with gzip wrapper (MAX_WBITS + 16)
    if (deflateInit2(&deflate_stream, compression_level, Z_DEFLATED,
                     MAX_WBITS + 16, mem_level, strategy) != Z_OK) {
        return 0;
    }
    
    // Set up gzip header if requested
    gz_header gzip_header;
    memset(&gzip_header, 0, sizeof(gzip_header));
    
    if (test_valid_gzip) {
        gzip_header.text = gzip_text_flag ? 1 : 0;
        gzip_header.time = fdp.ConsumeIntegral<uint32_t>();
        gzip_header.xflags = fdp.ConsumeIntegral<uint8_t>();
        gzip_header.os = fdp.ConsumeIntegral<uint8_t>() % 256;
        gzip_header.hcrc = gzip_crc_flag ? 1 : 0;
        gzip_header.done = 0;
        
        if (gzip_extra_flag && !extra_field_data.empty()) {
            gzip_header.extra = extra_field_data.data();
            gzip_header.extra_len = static_cast<uInt>(extra_field_data.size());
            gzip_header.extra_max = gzip_header.extra_len;
        }
        
        if (gzip_name_flag && !filename.empty()) {
            gzip_header.name = (Bytef*)filename.c_str();
            gzip_header.name_max = static_cast<uInt>(filename.size() + 1);
        }
        
        if (gzip_comment_flag && !comment.empty()) {
            gzip_header.comment = (Bytef*)comment.c_str();
            gzip_header.comm_max = static_cast<uInt>(comment.size() + 1);
        }
        
        deflateSetHeader(&deflate_stream, &gzip_header);
    }
    
    // Set dictionary if provided
    if (test_dictionary && !dictionary_data.empty()) {
        deflateSetDictionary(&deflate_stream, dictionary_data.data(), 
                            static_cast<uInt>(dictionary_data.size()));
    }
    
    // Compress input data
    size_t compressed_size = deflate_stream.total_in * 2 + 1024;
    std::vector<uint8_t> compressed_buffer(compressed_size);
    
    deflate_stream.next_in = input_buffer.data();
    deflate_stream.avail_in = static_cast<uInt>(input_buffer.size());
    deflate_stream.next_out = compressed_buffer.data();
    deflate_stream.avail_out = static_cast<uInt>(compressed_size);
    
    int deflate_result = deflate(&deflate_stream, Z_FINISH);
    if (deflate_result != Z_STREAM_END && deflate_result != Z_OK) {
        deflateEnd(&deflate_stream);
        return 0;
    }
    
    size_t actual_compressed_size = deflate_stream.total_out;
    deflateEnd(&deflate_stream);
    
    // Now test inflation with various scenarios
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(inflate_stream));
    
    // Initialize inflate with gzip wrapper support
    if (inflateInit2(&inflate_stream, MAX_WBITS + 16) != Z_OK) {
        return 0;
    }
    
    // Set up header retrieval if requested
    gz_header retrieved_header;
    memset(&retrieved_header, 0, sizeof(retrieved_header));
    
    // Allocate space for retrieved header fields (must stay alive for entire inflate operation)
    std::vector<uint8_t> retrieved_extra;
    std::vector<char> retrieved_name;
    std::vector<char> retrieved_comment;
    
    if (test_header_retrieval) {
        retrieved_extra.resize(1024);
        retrieved_name.resize(256);
        retrieved_comment.resize(1024);
        
        retrieved_header.extra = retrieved_extra.data();
        retrieved_header.extra_max = 1024;
        retrieved_header.name = (Bytef*)retrieved_name.data();
        retrieved_header.name_max = 256;
        retrieved_header.comment = (Bytef*)retrieved_comment.data();
        retrieved_header.comm_max = 1024;
        
        inflateGetHeader(&inflate_stream, &retrieved_header);
    }
    // Prepare output buffer for decompression
    std::vector<uint8_t> decompressed_buffer(input_buffer.size() * 2);
    
    inflate_stream.next_in = compressed_buffer.data();
    inflate_stream.avail_in = static_cast<uInt>(actual_compressed_size);
    inflate_stream.next_out = decompressed_buffer.data();
    inflate_stream.avail_out = static_cast<uInt>(decompressed_buffer.size());
    
    // Test various inflation scenarios based on fuzzer input
    int inflate_flush = Z_NO_FLUSH;
    if (test_sync_points) {
        uint8_t flush_selector = fdp.ConsumeIntegral<uint8_t>() % 7;
        switch (flush_selector) {
            case 0: inflate_flush = Z_NO_FLUSH; break;
            case 1: inflate_flush = Z_PARTIAL_FLUSH; break;
            case 2: inflate_flush = Z_SYNC_FLUSH; break;
            case 3: inflate_flush = Z_FULL_FLUSH; break;
            case 4: inflate_flush = Z_BLOCK; break;
            case 5: inflate_flush = Z_TREES; break;
            case 6: inflate_flush = Z_FINISH; break;
        }
    }
    
    // Perform inflation
    int inflate_result = inflate(&inflate_stream, inflate_flush);
    
    // Test dictionary setting if inflation needs dictionary
    if (inflate_result == Z_NEED_DICT && test_dictionary && !dictionary_data.empty()) {
        inflateSetDictionary(&inflate_stream, dictionary_data.data(),
                            static_cast<uInt>(dictionary_data.size()));
        
        // Continue inflation after setting dictionary
        inflate_result = inflate(&inflate_stream, inflate_flush);
    }
    
    // Test sync functionality for invalid data scenarios
    if (test_invalid_gzip && inflate_result == Z_DATA_ERROR) {
        // Try to sync to next valid point
        inflateSync(&inflate_stream);
        
        // Continue inflation if synced successfully
        if (inflate_stream.avail_in > 0) {
            inflate(&inflate_stream, inflate_flush);
        }
    }
    
    // Test multiple inflation calls to exercise state transitions
    if (inflate_result == Z_OK && inflate_stream.avail_in > 0) {
        // Continue inflation with potentially different flush mode
        int second_flush = fdp.ConsumeBool() ? Z_SYNC_FLUSH : Z_FINISH;
        inflate(&inflate_stream, second_flush);
    }
    
    // Test reset functionality
    if (fdp.ConsumeBool()) {
        inflateReset(&inflate_stream);
        
        // Re-initialize with potentially different window bits
        int alt_window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
        inflateEnd(&inflate_stream);
        
        // Clear retrieved header pointers since stream is reinitialized
        memset(&retrieved_header, 0, sizeof(retrieved_header));
        
        if (inflateInit2(&inflate_stream, alt_window_bits + 16) == Z_OK) {
            // If header retrieval was requested, set it up again for the new stream
            if (test_header_retrieval) {
                if (retrieved_extra.empty()) retrieved_extra.resize(1024);
                if (retrieved_name.empty()) retrieved_name.resize(256);
                if (retrieved_comment.empty()) retrieved_comment.resize(1024);
                
                retrieved_header.extra = retrieved_extra.data();
                retrieved_header.extra_max = 1024;
                retrieved_header.name = (Bytef*)retrieved_name.data();
                retrieved_header.name_max = 256;
                retrieved_header.comment = (Bytef*)retrieved_comment.data();
                retrieved_header.comm_max = 1024;
                
                inflateGetHeader(&inflate_stream, &retrieved_header);
            }
            
            // Try inflation again with reset stream
            inflate_stream.next_in = compressed_buffer.data();
            inflate_stream.avail_in = static_cast<uInt>(actual_compressed_size);
            inflate_stream.next_out = decompressed_buffer.data();
            inflate_stream.avail_out = static_cast<uInt>(decompressed_buffer.size());
            
            inflate(&inflate_stream, Z_FINISH);
        }
    } else {
        inflateEnd(&inflate_stream);
    }
    
    return 0;
}

} // namespace harness_013
