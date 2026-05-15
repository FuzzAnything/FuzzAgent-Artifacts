#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_005 {
/*
 * Fuzzing harness for zlib dictionary manipulation and stream state copying functions
 * Targets 8 APIs with 82+ accumulated undiscovered branches:
 * - Dictionary manipulation: deflateSetDictionary, inflateSetDictionary, deflateGetDictionary, inflateGetDictionary
 * - Stream control: deflatePrime, inflatePrime
 * - Stream state copying: deflateCopy, inflateCopy
 * Focus: Testing dictionary management, bit insertion, and stream state duplication with semantic diversity
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration and dictionary data
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);  // Window bits for stream
    int level = fdp.ConsumeIntegralInRange<int>(0, 9);        // Compression level
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);     // Compression strategy
    int memLevel = fdp.ConsumeIntegralInRange<int>(1, 9);     // Memory level
    
    // Operation selection flags
    bool test_deflate_dict = fdp.ConsumeBool();      // Test deflate dictionary operations
    bool test_inflate_dict = fdp.ConsumeBool();      // Test inflate dictionary operations
    bool test_prime_ops = fdp.ConsumeBool();         // Test prime (bit insertion) operations
    bool test_copy_ops = fdp.ConsumeBool();          // Test stream copy operations
    bool test_raw_mode = fdp.ConsumeBool();          // Test raw deflate/inflate mode
    bool test_mixed_ops = fdp.ConsumeBool();         // Test mixed operations
    
    // Prime operation parameters
    int bits = fdp.ConsumeIntegralInRange<int>(1, 16);    // Number of bits to insert
    int value = fdp.ConsumeIntegral<int>();               // Value to insert
    
    // Dictionary size parameters
    size_t dict_size = fdp.ConsumeIntegralInRange<size_t>(1, 32768);  // Dictionary size
    if (fdp.remaining_bytes() < dict_size) {
        return 0;
    }
    
    // Consume dictionary data
    std::vector<uint8_t> dictionary_data = fdp.ConsumeBytes<uint8_t>(dict_size);
    
    // Consume test data for compression/decompression
    size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
    if (fdp.remaining_bytes() < test_data_size) {
        return 0;
    }
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
    
    // Determine mode based on raw flag
    int actual_windowBits = windowBits;
    if (test_raw_mode) {
        actual_windowBits = -windowBits;  // Negative for raw mode
    }
    
    // Test deflate dictionary operations
    if (test_deflate_dict) {
        z_stream deflate_strm;
        memset(&deflate_strm, 0, sizeof(z_stream));
        deflate_strm.zalloc = Z_NULL;
        deflate_strm.zfree = Z_NULL;
        deflate_strm.opaque = Z_NULL;
        
        // Initialize deflate stream
        int ret = deflateInit2(&deflate_strm, level, Z_DEFLATED, actual_windowBits, memLevel, strategy);
        if (ret == Z_OK) {
            // Test deflateSetDictionary
            if (dictionary_data.size() > 0) {
                int dict_ret = deflateSetDictionary(&deflate_strm, dictionary_data.data(), dictionary_data.size());
                // Continue even if fails - test error paths
            }
            
            // Test deflateGetDictionary
            if (dict_size > 0) {
                Bytef dict_buffer[32768];
                uInt dict_len = 0;
                int get_dict_ret = deflateGetDictionary(&deflate_strm, dict_buffer, &dict_len);
                
                // Also test with NULL dictionary to just get length
                uInt dict_len_only = 0;
                deflateGetDictionary(&deflate_strm, Z_NULL, &dict_len_only);
                
                // Test with NULL dictLength parameter
                deflateGetDictionary(&deflate_strm, dict_buffer, Z_NULL);
            }
            
            // Test deflatePrime if enabled
            if (test_prime_ops) {
                deflatePrime(&deflate_strm, bits, value);
            }
            
            // Actually compress some data to exercise the stream
            if (test_data.size() > 0) {
                uLongf compr_size = deflateBound(&deflate_strm, test_data.size());
                Bytef* compr = (Bytef*)malloc(compr_size);
                if (compr) {
                    deflate_strm.next_in = test_data.data();
                    deflate_strm.avail_in = test_data.size();
                    deflate_strm.next_out = compr;
                    deflate_strm.avail_out = compr_size;
                    
                    deflate(&deflate_strm, Z_SYNC_FLUSH);
                    
                    // Test deflateCopy if enabled
                    if (test_copy_ops) {
                        z_stream deflate_copy_strm;
                        memset(&deflate_copy_strm, 0, sizeof(z_stream));
                        deflate_copy_strm.zalloc = Z_NULL;
                        deflate_copy_strm.zfree = Z_NULL;
                        deflate_copy_strm.opaque = Z_NULL;
                        
                        int copy_ret = deflateCopy(&deflate_copy_strm, &deflate_strm);
                        if (copy_ret == Z_OK) {
                            // Use the copied stream
                            Bytef* compr2 = (Bytef*)malloc(compr_size);
                            if (compr2) {
                                deflate_copy_strm.next_out = compr2;
                                deflate_copy_strm.avail_out = compr_size;
                                deflate(&deflate_copy_strm, Z_FINISH);
                                free(compr2);
                            }
                            deflateEnd(&deflate_copy_strm);
                        }
                    }
                    
                    free(compr);
                }
            }
            
            deflateEnd(&deflate_strm);
        }
    }
    
    // Test inflate dictionary operations
    if (test_inflate_dict) {
        z_stream inflate_strm;
        memset(&inflate_strm, 0, sizeof(z_stream));
        inflate_strm.zalloc = Z_NULL;
        inflate_strm.zfree = Z_NULL;
        inflate_strm.opaque = Z_NULL;
        
        // Initialize inflate stream
        int ret = inflateInit2(&inflate_strm, actual_windowBits);
        if (ret == Z_OK) {
            // Test inflateSetDictionary
            if (dictionary_data.size() > 0) {
                int dict_ret = inflateSetDictionary(&inflate_strm, dictionary_data.data(), dictionary_data.size());
                // Continue even if fails - test error paths
            }
            
            // Test inflateGetDictionary
            if (dict_size > 0) {
                Bytef dict_buffer[32768];
                uInt dict_len = 0;
                int get_dict_ret = inflateGetDictionary(&inflate_strm, dict_buffer, &dict_len);
                
                // Also test with NULL dictionary to just get length
                uInt dict_len_only = 0;
                inflateGetDictionary(&inflate_strm, Z_NULL, &dict_len_only);
                
                // Test with NULL dictLength parameter
                inflateGetDictionary(&inflate_strm, dict_buffer, Z_NULL);
            }
            
            // Test inflatePrime if enabled
            if (test_prime_ops) {
                inflatePrime(&inflate_strm, bits, value);
            }
            
            // Test inflateCopy if enabled
            if (test_copy_ops) {
                z_stream inflate_copy_strm;
                memset(&inflate_copy_strm, 0, sizeof(z_stream));
                inflate_copy_strm.zalloc = Z_NULL;
                inflate_copy_strm.zfree = Z_NULL;
                inflate_copy_strm.opaque = Z_NULL;
                
                int copy_ret = inflateCopy(&inflate_copy_strm, &inflate_strm);
                if (copy_ret == Z_OK) {
                    inflateEnd(&inflate_copy_strm);
                }
            }
            
            inflateEnd(&inflate_strm);
        }
    }
    
    // Test mixed operations - create compressed data then decompress with dictionary
    if (test_mixed_ops && test_data.size() > 0) {
        // First create compressed data
        z_stream deflate_strm;
        memset(&deflate_strm, 0, sizeof(z_stream));
        deflate_strm.zalloc = Z_NULL;
        deflate_strm.zfree = Z_NULL;
        deflate_strm.opaque = Z_NULL;
        
        int ret = deflateInit2(&deflate_strm, level, Z_DEFLATED, windowBits, memLevel, strategy);
        if (ret == Z_OK && dictionary_data.size() > 0) {
            // Set dictionary before compression
            deflateSetDictionary(&deflate_strm, dictionary_data.data(), dictionary_data.size());
            
            // Compress data
            uLongf compr_size = deflateBound(&deflate_strm, test_data.size());
            Bytef* compr = (Bytef*)malloc(compr_size);
            if (compr) {
                deflate_strm.next_in = test_data.data();
                deflate_strm.avail_in = test_data.size();
                deflate_strm.next_out = compr;
                deflate_strm.avail_out = compr_size;
                
                int deflate_ret = deflate(&deflate_strm, Z_FINISH);
                if (deflate_ret == Z_STREAM_END) {
                    size_t actual_compr_size = compr_size - deflate_strm.avail_out;
                    
                    // Now try to decompress with dictionary
                    z_stream inflate_strm;
                    memset(&inflate_strm, 0, sizeof(z_stream));
                    inflate_strm.zalloc = Z_NULL;
                    inflate_strm.zfree = Z_NULL;
                    inflate_strm.opaque = Z_NULL;
                    
                    int inflate_ret = inflateInit2(&inflate_strm, windowBits);
                    if (inflate_ret == Z_OK) {
                        // Try to decompress without dictionary first
                        Bytef* uncompr = (Bytef*)malloc(test_data.size());
                        if (uncompr) {
                            inflate_strm.next_in = compr;
                            inflate_strm.avail_in = actual_compr_size;
                            inflate_strm.next_out = uncompr;
                            inflate_strm.avail_out = test_data.size();
                            
                            inflate(&inflate_strm, Z_FINISH);
                            
                            // Try to set dictionary (may fail if not Z_NEED_DICT)
                            inflateSetDictionary(&inflate_strm, dictionary_data.data(), dictionary_data.size());
                            
                            free(uncompr);
                        }
                        inflateEnd(&inflate_strm);
                    }
                }
                free(compr);
            }
            deflateEnd(&deflate_strm);
        }
    }
    
    return 0;
}

} // namespace harness_005
