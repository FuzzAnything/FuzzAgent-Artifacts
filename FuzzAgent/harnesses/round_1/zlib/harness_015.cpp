#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_015 {
/*
 * Fuzzing harness for zlib deflate/inflate tuning and diagnostic operations
 * Targets completely uncovered APIs: deflateTune, inflateCodesUsed, inflateUndermine
 * Targets partially covered APIs: inflateCopy (50%), inflatePrime (50%)
 * Also targets: deflatePrime for compression bit insertion
 * Focuses on tuning/diagnostic API layer rather than core compression
 * Ensures semantic diversity from existing harnesses (000-014)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);  // Window bits for deflate/inflate
    int memLevel = fdp.ConsumeIntegralInRange<int>(1, 9);     // Memory level
    int level = fdp.ConsumeIntegralInRange<int>(0, 9);        // Compression level
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);     // Compression strategy
    
    // Parameters for deflateTune
    int good_length = fdp.ConsumeIntegralInRange<int>(0, 258);
    int max_lazy = fdp.ConsumeIntegralInRange<int>(0, 258);
    int nice_length = fdp.ConsumeIntegralInRange<int>(0, 258);
    int max_chain = fdp.ConsumeIntegralInRange<int>(0, 4096);
    
    // Parameters for inflatePrime/deflatePrime
    int prime_bits = fdp.ConsumeIntegralInRange<int>(0, 16);  // Bits to insert (0-16)
    int prime_value = fdp.ConsumeIntegral<int>();             // Value to insert
    
    // Parameters for inflateUndermine
    int undermine_value = fdp.ConsumeIntegral<int>();         // Undermine parameter
    
    // Test selection flags
    bool test_deflate_tune = fdp.ConsumeBool();
    bool test_inflate_copy = fdp.ConsumeBool();
    bool test_inflate_codes_used = fdp.ConsumeBool();
    bool test_inflate_undermine = fdp.ConsumeBool();
    bool test_inflate_prime = fdp.ConsumeBool();
    bool test_deflate_prime = fdp.ConsumeBool();
    bool test_actual_operations = fdp.ConsumeBool();  // Test actual compression/decompression
    
    // Consume test data for compression/decompression
    size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
    if (fdp.remaining_bytes() < test_data_size) {
        return 0;
    }
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
    
    // Test 1: Deflate stream operations with tuning
    if (test_deflate_tune || test_deflate_prime || test_actual_operations) {
        z_stream deflate_strm;
        deflate_strm.zalloc = Z_NULL;
        deflate_strm.zfree = Z_NULL;
        deflate_strm.opaque = Z_NULL;
        
        // Initialize deflate stream with deflateInit2_
        int ret = deflateInit2_(&deflate_strm, level, Z_DEFLATED, windowBits, memLevel, strategy,
                               ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            // Test deflateTune if requested
            if (test_deflate_tune) {
                ret = deflateTune(&deflate_strm, good_length, max_lazy, nice_length, max_chain);
                // Result can be Z_OK or Z_STREAM_ERROR
            }
            
            // Test deflatePrime if requested (must be before first deflate() call)
            if (test_deflate_prime && prime_bits >= 0 && prime_bits <= 16) {
                ret = deflatePrime(&deflate_strm, prime_bits, prime_value);
                // Result can be Z_OK or Z_STREAM_ERROR
            }
            
            // Perform actual compression if requested
            if (test_actual_operations && test_data_size > 0) {
                // Prepare buffers
                size_t compr_bound = deflateBound(&deflate_strm, (uLong)test_data_size);
                Bytef* compr = (Bytef*)malloc(compr_bound);
                Bytef* uncompr = (Bytef*)malloc(test_data_size);
                
                if (compr && uncompr) {
                    // Compress the data
                    deflate_strm.next_in = test_data.data();
                    deflate_strm.avail_in = (uInt)test_data_size;
                    deflate_strm.next_out = compr;
                    deflate_strm.avail_out = (uInt)compr_bound;
                    
                    ret = deflate(&deflate_strm, Z_FINISH);
                    // Result can be Z_STREAM_END, Z_OK, etc.
                    
                    // Reset for possible reuse
                    deflateReset(&deflate_strm);
                    
                    free(compr);
                    free(uncompr);
                }
            }
            
            // Clean up
            deflateEnd(&deflate_strm);
        }
    }
    
    // Test 2: Inflate stream operations with diagnostic APIs
    if (test_inflate_copy || test_inflate_codes_used || test_inflate_undermine || 
        test_inflate_prime || test_actual_operations) {
        
        // First create a compressed stream to work with
        z_stream deflate_strm;
        deflate_strm.zalloc = Z_NULL;
        deflate_strm.zfree = Z_NULL;
        deflate_strm.opaque = Z_NULL;
        
        // Initialize deflate to create compressed data
        int ret = deflateInit2(&deflate_strm, level, Z_DEFLATED, windowBits, memLevel, strategy);
        if (ret == Z_OK && test_data_size > 0) {
            // Create compressed data
            size_t compr_bound = deflateBound(&deflate_strm, (uLong)test_data_size);
            Bytef* compr = (Bytef*)malloc(compr_bound);
            
            if (compr) {
                deflate_strm.next_in = test_data.data();
                deflate_strm.avail_in = (uInt)test_data_size;
                deflate_strm.next_out = compr;
                deflate_strm.avail_out = (uInt)compr_bound;
                
                ret = deflate(&deflate_strm, Z_FINISH);
                size_t compressed_size = compr_bound - deflate_strm.avail_out;
                
                // Now test inflate operations on this compressed data
                if (ret == Z_STREAM_END && compressed_size > 0) {
                    // Create first inflate stream
                    z_stream inflate_strm1;
                    inflate_strm1.zalloc = Z_NULL;
                    inflate_strm1.zfree = Z_NULL;
                    inflate_strm1.opaque = Z_NULL;
                    
                    ret = inflateInit2_(&inflate_strm1, windowBits, ZLIB_VERSION, sizeof(z_stream));
                    if (ret == Z_OK) {
                        // Test inflateCopy if requested
                        z_stream inflate_strm2;
                        if (test_inflate_copy) {
                            // Initialize second stream structure
                            inflate_strm2.zalloc = Z_NULL;
                            inflate_strm2.zfree = Z_NULL;
                            inflate_strm2.opaque = Z_NULL;
                            
                            // Copy the stream state
                            ret = inflateCopy(&inflate_strm2, &inflate_strm1);
                            // Result can be Z_OK or Z_STREAM_ERROR
                            
                            // If copy succeeded, we should clean up the copy
                            if (ret == Z_OK) {
                                inflateEnd(&inflate_strm2);
                            }
                        }
                        
                        // Test inflateCodesUsed if requested
                        if (test_inflate_codes_used) {
                            unsigned long codes_used = inflateCodesUsed(&inflate_strm1);
                            // Diagnostic value, no error check needed
                        }
                        
                        // Test inflateUndermine if requested
                        if (test_inflate_undermine) {
                            ret = inflateUndermine(&inflate_strm1, undermine_value);
                            // Result can be Z_OK, Z_STREAM_ERROR, or Z_DATA_ERROR
                        }
                        
                        // Test inflatePrime if requested
                        if (test_inflate_prime && prime_bits >= 0 && prime_bits <= 16) {
                            ret = inflatePrime(&inflate_strm1, prime_bits, prime_value);
                            // Result can be Z_OK or Z_STREAM_ERROR
                        }
                        
                        // Perform actual decompression if requested
                        if (test_actual_operations) {
                            // Prepare output buffer
                            Bytef* uncompr = (Bytef*)malloc(test_data_size);
                            if (uncompr) {
                                inflate_strm1.next_in = compr;
                                inflate_strm1.avail_in = (uInt)compressed_size;
                                inflate_strm1.next_out = uncompr;
                                inflate_strm1.avail_out = (uInt)test_data_size;
                                
                                ret = inflate(&inflate_strm1, Z_FINISH);
                                // Result can be Z_STREAM_END, Z_OK, Z_BUF_ERROR, etc.
                                
                                free(uncompr);
                            }
                        }
                        
                        // Clean up
                        inflateEnd(&inflate_strm1);
                    }
                }
                
                free(compr);
            }
            
            deflateEnd(&deflate_strm);
        }
    }
    
    // Test 3: Combined operations - create streams, tune them, copy them, and exercise
    // This tests the interaction between multiple tuning/diagnostic operations
    if (test_actual_operations && fdp.ConsumeBool()) {
        // Create compression stream
        z_stream c_stream;
        c_stream.zalloc = Z_NULL;
        c_stream.zfree = Z_NULL;
        c_stream.opaque = Z_NULL;
        
        if (deflateInit2(&c_stream, level, Z_DEFLATED, windowBits, memLevel, strategy) == Z_OK) {
            // Tune the compression parameters
            deflateTune(&c_stream, good_length, max_lazy, nice_length, max_chain);
            
            // Create decompression stream  
            z_stream d_stream;
            d_stream.zalloc = Z_NULL;
            d_stream.zfree = Z_NULL;
            d_stream.opaque = Z_NULL;
            
            if (inflateInit2(&d_stream, windowBits) == Z_OK) {
                // Test diagnostic functions on decompression stream
                inflateCodesUsed(&d_stream);
                
                // Test undermine with various values
                for (int i = 0; i < 3; i++) {
                    inflateUndermine(&d_stream, fdp.ConsumeIntegral<int>());
                }
                
                // Clean up
                inflateEnd(&d_stream);
            }
            
            deflateEnd(&c_stream);
        }
    }
    
    return 0;
}

} // namespace harness_015
