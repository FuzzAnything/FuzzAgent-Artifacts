#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_015 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful deflate control API testing
    // Need enough data for parameters, dictionary, and test content
    if (size < 256) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first for deflateInit2
    int level = fdp.ConsumeIntegralInRange<int>(0, 9);
    int method = Z_DEFLATED;  // Always use deflate method
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
    int memLevel = fdp.ConsumeIntegralInRange<int>(1, 9);
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Determine if we're doing gzip format (windowBits > 15)
    bool use_gzip_format = fdp.ConsumeBool();
    if (use_gzip_format) {
        windowBits = fdp.ConsumeIntegralInRange<int>(16, 31);
    }
    
    // Consume parameters for deflateTune
    int good_length = fdp.ConsumeIntegralInRange<int>(0, 255);
    int max_lazy = fdp.ConsumeIntegralInRange<int>(0, 255);
    int nice_length = fdp.ConsumeIntegralInRange<int>(0, 255);
    int max_chain = fdp.ConsumeIntegralInRange<int>(0, 65535);
    
    // Consume parameters for deflatePrime
    int prime_bits = fdp.ConsumeIntegralInRange<int>(0, 16);
    int prime_value = fdp.ConsumeIntegral<uint16_t>();
    
    // Prepare test data for compression
    size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(128, 4096);
    if (test_data_size > fdp.remaining_bytes()) {
        test_data_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> test_data;
    if (test_data_size > 0) {
        test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
    }
    
    // Prepare dictionary data
    size_t dict_size = fdp.ConsumeIntegralInRange<size_t>(0, 32768);
    if (dict_size > fdp.remaining_bytes()) {
        dict_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> dictionary_data;
    if (dict_size > 0) {
        dictionary_data = fdp.ConsumeBytes<uint8_t>(dict_size);
    }
    
    // Initialize source deflate stream
    z_stream src_strm;
    src_strm.zalloc = Z_NULL;
    src_strm.zfree = Z_NULL;
    src_strm.opaque = Z_NULL;
    
    int init_result = deflateInit2(&src_strm, level, method, windowBits, memLevel, strategy);
    if (init_result != Z_OK) {
        // Failed to initialize, return early
        return 0;
    }
    
    // Step 1: Call deflateBound to query compressed size bound
    uLong bound_result = deflateBound(&src_strm, test_data.size());
    // Also test deflateBound_z variant
    z_size_t bound_z_result = deflateBound_z(&src_strm, test_data.size());
    
    // Step 2: Call deflateSetDictionary if we have dictionary data
    if (dict_size > 0) {
        int dict_result = deflateSetDictionary(&src_strm, dictionary_data.data(), dict_size);
        // Result may be Z_OK or Z_STREAM_ERROR depending on state
    }
    
    // Step 3: Call deflatePrime for raw deflate streams
    // Only call deflatePrime for raw deflate (positive windowBits <= 15)
    if (windowBits > 0 && windowBits <= 15) {
        int prime_result = deflatePrime(&src_strm, prime_bits, prime_value);
        // Result may be Z_OK, Z_BUF_ERROR, or Z_STREAM_ERROR
    }
    
    // Step 4: Setup buffers and call deflate with Z_NO_FLUSH
    uLongf compr_bound = bound_result;
    if (compr_bound == 0 || compr_bound > 10 * 1024 * 1024) {
        // Avoid excessive memory usage
        deflateEnd(&src_strm);
        return 0;
    }
    
    Bytef* compr_buffer = (Bytef*)malloc(compr_bound);
    if (!compr_buffer) {
        deflateEnd(&src_strm);
        return 0;
    }
    
    src_strm.next_in = test_data.data();
    src_strm.avail_in = test_data.size();
    src_strm.next_out = compr_buffer;
    src_strm.avail_out = compr_bound;
    
    int deflate_result = deflate(&src_strm, Z_NO_FLUSH);
    // Result may be Z_OK, Z_BUF_ERROR, etc.
    
    // Step 5: Call deflateGetDictionary to retrieve current dictionary
    Bytef dict_buffer[32768];  // Maximum dictionary size
    uInt dict_len = 32768;
    int get_dict_result = deflateGetDictionary(&src_strm, dict_buffer, &dict_len);
    // Also test with NULL buffer to just get length
    uInt dict_len_only = 0;
    deflateGetDictionary(&src_strm, Z_NULL, &dict_len_only);
    
    // Step 6: Call deflatePending to check pending output
    unsigned pending_bytes = 0;
    int pending_bits = 0;
    int pending_result = deflatePending(&src_strm, &pending_bytes, &pending_bits);
    // Also test with NULL parameters
    deflatePending(&src_strm, Z_NULL, Z_NULL);
    
    // Step 7: Call deflateUsed to check deflate bits used
    int used_bits = 0;
    int used_result = deflateUsed(&src_strm, &used_bits);
    // Also test with NULL parameter
    deflateUsed(&src_strm, Z_NULL);
    
    // Step 8: Call deflateSetHeader for gzip streams
    gz_header header;
    memset(&header, 0, sizeof(header));
    
    if (windowBits > 15) {  // gzip format
        // Set up gzip header fields
        header.text = fdp.ConsumeBool() ? 1 : 0;
        header.time = fdp.ConsumeIntegral<uLong>();
        header.xflags = fdp.ConsumeIntegral<int>() % 256;
        header.os = fdp.ConsumeIntegral<int>() % 256;
        header.hcrc = fdp.ConsumeBool() ? 1 : 0;
        
        // Prepare optional name and comment strings
        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
        std::string name_str;
        if (name_len > 0 && name_len <= fdp.remaining_bytes()) {
            name_str = fdp.ConsumeBytesAsString(name_len);
            header.name = (Bytef*)name_str.c_str();
        }
        
        size_t comment_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
        std::string comment_str;
        if (comment_len > 0 && comment_len <= fdp.remaining_bytes()) {
            comment_str = fdp.ConsumeBytesAsString(comment_len);
            header.comment = (Bytef*)comment_str.c_str();
        }
        
        int header_result = deflateSetHeader(&src_strm, &header);
        // Result may be Z_OK or Z_STREAM_ERROR
    }
    
    // Step 9: Call deflateTune to adjust compression parameters
    int tune_result = deflateTune(&src_strm, good_length, max_lazy, nice_length, max_chain);
    // Result may be Z_OK or Z_STREAM_ERROR
    
    // Step 10: Prepare destination stream for deflateCopy
    z_stream dest_strm;
    dest_strm.zalloc = Z_NULL;
    dest_strm.zfree = Z_NULL;
    dest_strm.opaque = Z_NULL;
    
    // Step 11: Call deflateCopy to duplicate stream state
    int copy_result = deflateCopy(&dest_strm, &src_strm);
    
    if (copy_result == Z_OK) {
        // If copy succeeded, we have two streams to manage
        
        // Continue compression with the source stream using Z_FINISH
        src_strm.next_out = compr_buffer + src_strm.total_out;
        src_strm.avail_out = compr_bound - src_strm.total_out;
        deflate(&src_strm, Z_FINISH);
        
        // Also test the copied stream
        Bytef* dest_compr = (Bytef*)malloc(compr_bound);
        if (dest_compr) {
            dest_strm.next_in = test_data.data();
            dest_strm.avail_in = test_data.size();
            dest_strm.next_out = dest_compr;
            dest_strm.avail_out = compr_bound;
            
            deflate(&dest_strm, Z_FINISH);
            
            free(dest_compr);
        }
        
        // Clean up the copied stream
        deflateEnd(&dest_strm);
    }
    
    // Step 12: Complete compression with source stream if not already done
    if (copy_result != Z_OK) {
        // Only source stream exists
        src_strm.next_out = compr_buffer + src_strm.total_out;
        src_strm.avail_out = compr_bound - src_strm.total_out;
        deflate(&src_strm, Z_FINISH);
    }
    
    // Step 13: Clean up source stream
    deflateEnd(&src_strm);
    
    // Free compression buffer
    free(compr_buffer);
    
    return 0;
}

} // namespace harness_015
