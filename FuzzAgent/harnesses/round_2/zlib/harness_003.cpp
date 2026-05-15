#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_003 {
/*
 * Fuzzing harness for zlib deflate parameter control APIs
 * Target: deflateParams, deflateSetDictionary, deflateBound, deflateTune, 
 *         deflateGetDictionary, deflatePending, deflatePrime,
 *         deflateSetHeader, deflateReset, deflateCopy
 * 
 * This harness focuses on testing dynamic parameter adjustment during compression,
 * which is a critical coverage gap with 340+ undiscovered branches.
 * Different from existing harnesses:
 * - harness_000 (basic compression/decompression)
 * - harness_001 (gzip file I/O) 
 * - harness_002 (inflateBack callback-based decompression)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
    int mem_level = fdp.ConsumeIntegralInRange<int>(1, 9);
    int initial_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4); // Z_DEFAULT_STRATEGY to Z_FIXED
    bool use_raw_deflate = fdp.ConsumeBool();
    bool use_gzip_header = fdp.ConsumeBool();
    bool test_dictionary = fdp.ConsumeBool();
    bool test_param_changes = fdp.ConsumeBool();
    bool test_tuning = fdp.ConsumeBool();
    bool test_copy = fdp.ConsumeBool();
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(1, 8192);
    if (fdp.remaining_bytes() < input_size) {
        return 0;
    }

    // Consume input data for compression
    std::vector<uint8_t> input_buffer = fdp.ConsumeBytes<uint8_t>(input_size);

    // Consume dictionary data if needed
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

    // Initialize z_stream for compression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));

    // Declare gz_header at function scope to ensure it stays valid
    gz_header gzheader;
    std::vector<uint8_t> gzheader_extra;  // To keep extra field data alive

    // Set up allocation functions (use default)
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    // Determine compression method and window bits
    int method = Z_DEFLATED;
    int actual_window_bits = window_bits;
    
    if (use_raw_deflate) {
        actual_window_bits = -window_bits;
    }
    
    if (use_gzip_header) {
        actual_window_bits += 16; // Add 16 for gzip header
    }

    // Initialize deflate
    int ret = deflateInit2(&strm, initial_level, method, actual_window_bits, 
                          mem_level, strategy);
    if (ret != Z_OK) {
        // Clean up and return if initialization failed
        return 0;
    }

    // Set gzip header if requested
    if (use_gzip_header) {
        memset(&gzheader, 0, sizeof(gzheader));
        gzheader.text = fdp.ConsumeBool() ? 1 : 0;
        gzheader.time = fdp.ConsumeIntegral<uint32_t>();
        gzheader.os = fdp.ConsumeIntegral<uint8_t>();
        
        // Set extra field if available
        if (fdp.remaining_bytes() > 2) {
            uint16_t xlen = fdp.ConsumeIntegral<uint16_t>();
            if (fdp.remaining_bytes() >= xlen) {
                gzheader_extra = fdp.ConsumeBytes<uint8_t>(xlen);
                gzheader.extra = gzheader_extra.data();
                gzheader.extra_len = xlen;
            }
        }
        
        deflateSetHeader(&strm, &gzheader);
    }

    // Set dictionary if requested
    if (test_dictionary && !dictionary_data.empty()) {
        // For raw deflate, we need to be at block boundary
        if (!use_raw_deflate) {
            deflateSetDictionary(&strm, dictionary_data.data(), dictionary_data.size());
        }
    }

    // Test deflateTune if requested
    if (test_tuning && fdp.remaining_bytes() >= 16) {
        int good_length = fdp.ConsumeIntegralInRange<int>(0, 258);
        int max_lazy = fdp.ConsumeIntegralInRange<int>(0, 258);
        int nice_length = fdp.ConsumeIntegralInRange<int>(0, 258);
        int max_chain = fdp.ConsumeIntegralInRange<int>(0, 4096);
        
        deflateTune(&strm, good_length, max_lazy, nice_length, max_chain);
    }

    // Calculate bound for output buffer
    uLong bound = deflateBound(&strm, input_buffer.size());
    if (bound == 0 || bound > 10 * 1024 * 1024) {
        deflateEnd(&strm);
        return 0;
    }

    // Allocate output buffer
    std::vector<uint8_t> output_buffer(bound);

    // Set up input/output pointers
    strm.next_in = input_buffer.data();
    strm.avail_in = input_buffer.size();
    strm.next_out = output_buffer.data();
    strm.avail_out = output_buffer.size();

    // Test deflatePrime if requested (only for raw deflate)
    if (use_raw_deflate && fdp.remaining_bytes() >= 2) {
        int bits = fdp.ConsumeIntegralInRange<int>(1, 16);
        int value = fdp.ConsumeIntegralInRange<int>(0, (1 << bits) - 1);
        deflatePrime(&strm, bits, value);
    }

    // Perform initial compression with partial flush
    ret = deflate(&strm, Z_PARTIAL_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }

    // Test parameter changes during compression
    if (test_param_changes && fdp.remaining_bytes() >= 4) {
        // Consume new parameters
        int new_level = fdp.ConsumeIntegralInRange<int>(0, 9);
        int new_strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
        
        // Try to change parameters dynamically
        deflateParams(&strm, new_level, new_strategy);
        
        // Continue compression with new parameters
        if (strm.avail_in > 0) {
            ret = deflate(&strm, Z_SYNC_FLUSH);
        }
    }

    // Test deflatePending
    if (fdp.remaining_bytes() >= 1) {
        unsigned bits;
        int pending_bits;
        int pending_ret = deflatePending(&strm, &bits, &pending_bits);
    }
    // Test deflateGetDictionary
    if (test_dictionary && fdp.remaining_bytes() >= 2) {
        uInt dict_length = 0;
        // First get dictionary length
        deflateGetDictionary(&strm, Z_NULL, &dict_length);
        
        if (dict_length > 0 && dict_length <= 32768) {
            std::vector<uint8_t> retrieved_dict(dict_length);
            deflateGetDictionary(&strm, retrieved_dict.data(), &dict_length);
        }
    }

    // Test stream copying if requested
    if (test_copy && fdp.remaining_bytes() >= 1) {
        z_stream strm_copy;
        strm_copy.zalloc = Z_NULL;
        strm_copy.zfree = Z_NULL;
        strm_copy.opaque = Z_NULL;
        
        ret = deflateCopy(&strm_copy, &strm);
        if (ret == Z_OK) {
            // Use the copy for a bit more compression
            if (strm_copy.avail_in > 0) {
                deflate(&strm_copy, Z_FINISH);
            }
            deflateEnd(&strm_copy);
        }
    }

    // Finish compression
    while (strm.avail_in > 0) {
        ret = deflate(&strm, Z_NO_FLUSH);
        if (ret != Z_OK && ret != Z_BUF_ERROR) {
            break;
        }
        
        // If output buffer is full, we can't continue
        if (strm.avail_out == 0) {
            break;
        }
    }

    // Final flush
    ret = deflate(&strm, Z_FINISH);
    
    // Test deflateReset
    if (fdp.remaining_bytes() >= 1) {
        deflateReset(&strm);
        
        // Try to use the reset stream
        if (fdp.remaining_bytes() >= 128) {
            std::vector<uint8_t> more_input = fdp.ConsumeBytes<uint8_t>(128);
            strm.next_in = more_input.data();
            strm.avail_in = more_input.size();
            strm.next_out = output_buffer.data();
            strm.avail_out = output_buffer.size();
            
            deflate(&strm, Z_FINISH);
        }
    }

    // Clean up
    deflateEnd(&strm);

    return 0;
}

} // namespace harness_003
