#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_017 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing: need enough for parameters and compressed data
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);  // Window bits for inflateInit2
    int flush = fdp.ConsumeIntegralInRange<int>(0, 4);        // Flush mode for inflate
    bool use_gzip = fdp.ConsumeBool();                        // Whether to use gzip format
    
    // Adjust windowBits for gzip if needed
    if (use_gzip) {
        windowBits = windowBits + 16;  // Add 16 for gzip format
    }
    
    // Determine source data size for compression
    size_t src_len = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
    if (src_len > fdp.remaining_bytes()) {
        src_len = fdp.remaining_bytes();
    }
    
    if (src_len < 64) {
        return 0;  // Not enough data
    }
    
    // Consume source data for compression
    std::vector<uint8_t> src_data = fdp.ConsumeBytes<uint8_t>(src_len);
    
    // Step 1: First compress some data to create valid compressed input for inflation
    uLongf compr_bound = compressBound(src_len);
    if (compr_bound == 0 || compr_bound > 10 * 1024 * 1024) {
        return 0;  // Avoid excessive memory usage
    }
    
    Bytef* compr = (Bytef*)malloc(compr_bound);
    if (!compr) {
        return 0;
    }
    
    uLongf compr_len = compr_bound;
    int compress_ret = compress2(compr, &compr_len, src_data.data(), src_len, Z_DEFAULT_COMPRESSION);
    
    if (compress_ret != Z_OK) {
        free(compr);
        return 0;  // Compression failed
    }
    
    // Step 2: Initialize inflate stream
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    
    int init_ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (init_ret != Z_OK) {
        free(compr);
        return 0;
    }
    
    // Step 3: Setup compressed data for inflation
    strm.next_in = compr;
    strm.avail_in = compr_len;
    
    // Allocate output buffer for inflation
    size_t out_buf_size = src_len * 2;  // Slightly larger than original
    Bytef* out_buf = (Bytef*)malloc(out_buf_size);
    if (!out_buf) {
        inflateEnd(&strm);
        free(compr);
        return 0;
    }
    
    strm.next_out = out_buf;
    strm.avail_out = out_buf_size;
    
    // Step 4: Perform some inflation to create active state
    int inflate_ret = inflate(&strm, Z_NO_FLUSH);
    
    // Step 5: Test inflateGetDictionary - get dictionary from inflate stream
    if (inflate_ret == Z_OK || inflate_ret == Z_STREAM_END || inflate_ret == Z_NEED_DICT) {
        uInt dict_len = 0;
        Bytef dict_buffer[32768];  // Maximum dictionary size
        
        // First call to get dictionary length
        int dict_ret = inflateGetDictionary(&strm, Z_NULL, &dict_len);
        
        if (dict_ret == Z_OK && dict_len > 0 && dict_len <= sizeof(dict_buffer)) {
            // Second call to actually get the dictionary
            inflateGetDictionary(&strm, dict_buffer, &dict_len);
        }
    }
    
    // Step 6: Test inflateMark - get current position marker
    long mark_result = inflateMark(&strm);
    // mark_result can be used with inflateReset or inflateBack
    
    // Step 7: Test inflateValidate - validate the stream
    int validate_check = fdp.ConsumeIntegralInRange<int>(0, 1);
    int validate_ret = inflateValidate(&strm, validate_check);
    
    // Step 8: Test inflateGetHeader - get gzip header if using gzip format
    if (use_gzip && windowBits > 15) {  // gzip format has windowBits > 15
        gz_header header;
        memset(&header, 0, sizeof(header));
        header.extra_max = 65536;
        header.name_max = 65536;
        header.comm_max = 65536;
        
        int header_ret = inflateGetHeader(&strm, &header);
        // header structure now contains gzip header info if available
    }
    
    // Step 9: Test inflateUndermine - undermine stream integrity check
    int undermine_check = fdp.ConsumeIntegralInRange<int>(0, 1);
    int undermine_ret = inflateUndermine(&strm, undermine_check);
    
    // Step 10: Test inflateCodesUsed - get number of codes used
    unsigned long codes_used = inflateCodesUsed(&strm);
    // codes_used contains number of codes used in current block
    
    // Step 11: Test error reporting with zError
    if (inflate_ret < 0) {  // Negative return codes are errors
        const char* error_msg = zError(inflate_ret);
        // error_msg contains error string
    }
    
    // Step 12: Continue inflation with different flush modes if there's more input
    if (strm.avail_in > 0 && inflate_ret == Z_OK) {
        // Try with a different flush mode
        strm.avail_out = out_buf_size - (strm.next_out - out_buf);
        inflate(&strm, flush);
        
        // Test query functions again after more inflation
        inflateMark(&strm);
        inflateValidate(&strm, validate_check);
        inflateCodesUsed(&strm);
    }
    
    // Step 13: Cleanup
    inflateEnd(&strm);
    free(out_buf);
    free(compr);
    
    return 0;
}

} // namespace harness_017
