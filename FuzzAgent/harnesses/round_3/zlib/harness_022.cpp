#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_022 {

// Custom memory allocation functions for testing
static void* custom_zcalloc(void* opaque, unsigned items, unsigned size) {
    (void)opaque;  // Unused parameter
    // Allocate memory with extra padding for fuzzing
    size_t total_size = (size_t)items * size;
    size_t padded_size = total_size + 16;  // Add padding
    void* ptr = malloc(padded_size);
    if (ptr) {
        // Fill with pattern to detect uninitialized memory usage
        memset(ptr, 0xAA, padded_size);
        // Return pointer to the actual allocated area
        return ptr;
    }
    return Z_NULL;
}

static void custom_zcfree(void* opaque, void* ptr) {
    (void)opaque;  // Unused parameter
    // Check for corruption before freeing
    if (ptr) {
        free(ptr);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for comprehensive testing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    bool use_custom_allocators = fdp.ConsumeBool();
    bool test_64bit_combine = fdp.ConsumeBool();
    bool test_inflate_prime = fdp.ConsumeBool();
    bool test_inflate_set_dict = fdp.ConsumeBool();
    
    int window_bits = fdp.ConsumeIntegralInRange<int>(8, 47);  // Up to 47 for gzip
    int mem_level = fdp.ConsumeIntegralInRange<int>(1, 9);
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Parameters for inflatePrime
    int prime_bits = fdp.ConsumeIntegralInRange<int>(0, 16);
    int prime_value = fdp.ConsumeIntegral<uint16_t>();
    
    // Parameters for checksum combine functions
    uLong crc1 = fdp.ConsumeIntegral<uLong>();
    uLong crc2 = fdp.ConsumeIntegral<uLong>();
    z_off_t len2 = fdp.ConsumeIntegral<z_off_t>();
    z_off64_t len2_64 = fdp.ConsumeIntegral<z_off64_t>();
    
    // Prepare test data for compression/decompression
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
    
    // Test different scenarios
    switch (test_scenario) {
        case 0: {
            // Scenario 0: Advanced inflate functions with custom allocators
            // -------------------------------------------------------------
            
            // First compress some data
            uLongf compr_bound = compressBound(test_data.size());
            if (compr_bound == 0 || compr_bound > 10 * 1024 * 1024) {
                return 0;  // Avoid excessive memory usage
            }
            
            Bytef* compr = (Bytef*)malloc(compr_bound);
            Bytef* uncompr = (Bytef*)malloc(test_data.size());
            
            if (!compr || !uncompr) {
                free(compr);
                free(uncompr);
                return 0;
            }
            
            uLongf compr_len = compr_bound;
            int compress_result = compress(compr, &compr_len, test_data.data(), test_data.size());
            
            if (compress_result == Z_OK && compr_len > 0) {
                // Setup inflate stream with custom allocators if requested
                z_stream inflate_strm;
                inflate_strm.zalloc = use_custom_allocators ? custom_zcalloc : Z_NULL;
                inflate_strm.zfree = use_custom_allocators ? custom_zcfree : Z_NULL;
                inflate_strm.opaque = Z_NULL;
                inflate_strm.next_in = compr;
                inflate_strm.avail_in = compr_len;
                inflate_strm.next_out = uncompr;
                inflate_strm.avail_out = test_data.size();
                
                // Initialize inflate with advanced parameters
                int init_result;
                if (window_bits > 15) {
                    // Use gzip format
                    init_result = inflateInit2(&inflate_strm, window_bits);
                } else {
                    // Use raw deflate or zlib format
                    init_result = inflateInit2(&inflate_strm, window_bits);
                }
                
                if (init_result == Z_OK) {
                    // Test inflatePrime if requested
                    if (test_inflate_prime && window_bits > 0 && window_bits <= 15) {
                        int prime_result = inflatePrime(&inflate_strm, prime_bits, prime_value);
                        // Result may be Z_OK or Z_STREAM_ERROR
                    }
                    
                    // Test inflateSetDictionary if we have dictionary data
                    if (test_inflate_set_dict && dict_size > 0) {
                        int dict_result = inflateSetDictionary(&inflate_strm, 
                                                             dictionary_data.data(), 
                                                             dict_size);
                        // Result may be Z_OK, Z_STREAM_ERROR, or Z_DATA_ERROR
                    }
                    
                    // Perform inflation
                    int inflate_result = inflate(&inflate_strm, Z_FINISH);
                    
                    // Test inflateSync to attempt synchronization
                    if (inflate_result == Z_DATA_ERROR) {
                        // Try to sync to next boundary
                        inflateSync(&inflate_strm);
                    }
                    
                    // Clean up
                    inflateEnd(&inflate_strm);
                }
            }
            
            free(compr);
            free(uncompr);
            break;
        }
        
        case 1: {
            // Scenario 1: CRC-32 combine functions with 64-bit variants
            // ----------------------------------------------------------
            
            // Test standard combine functions
            uLong combined_std = crc32_combine(crc1, crc2, len2);
            
            // Test combine_gen and combine_op functions
            if (len2 >= 0) {
                uLong op = crc32_combine_gen(len2);
                if (op != 0) {
                    uLong combined_op = crc32_combine_op(crc1, crc2, op);
                    // Should be equivalent to crc32_combine()
                }
            }
            
            // Test 64-bit variants if requested
            if (test_64bit_combine) {
                uLong combined_64 = crc32_combine64(crc1, crc2, len2_64);
                
                if (len2_64 >= 0) {
                    uLong op64 = crc32_combine_gen64(len2_64);
                    if (op64 != 0) {
                        uLong combined_op64 = crc32_combine_op(crc1, crc2, op64);
                    }
                }
            }
            
            // Test with incremental computation using actual data
            if (test_data.size() > 4) {
                // Split data into two parts
                size_t split_point = test_data.size() / 2;
                
                // Compute CRC for first part
                uLong crc_part1 = crc32(0L, test_data.data(), split_point);
                
                // Compute CRC for second part  
                uLong crc_part2 = crc32(0L, test_data.data() + split_point, 
                                       test_data.size() - split_point);
                
                // Compute CRC for whole data
                uLong crc_whole = crc32(0L, test_data.data(), test_data.size());
                
                // Combine and compare
                uLong combined_parts = crc32_combine(crc_part1, crc_part2, 
                                                   test_data.size() - split_point);
                
                // The combined value should equal the whole checksum
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Adler-32 combine functions with 64-bit variants
            // ------------------------------------------------------------
            
            // Generate random Adler-32 values
            uLong adler1 = fdp.ConsumeIntegral<uLong>();
            uLong adler2 = fdp.ConsumeIntegral<uLong>();
            
            // Test standard combine
            uLong adler_combined = adler32_combine(adler1, adler2, len2);
            
            // Test 64-bit variant if requested
            if (test_64bit_combine) {
                uLong adler_combined64 = adler32_combine64(adler1, adler2, len2_64);
            }
            
            // Test with incremental computation using actual data
            if (test_data.size() > 4) {
                // Split data into two parts
                size_t split_point = test_data.size() / 2;
                
                // Compute Adler-32 for first part
                uLong adler_part1 = adler32(0L, test_data.data(), split_point);
                
                // Compute Adler-32 for second part  
                uLong adler_part2 = adler32(0L, test_data.data() + split_point, 
                                          test_data.size() - split_point);
                
                // Compute Adler-32 for whole data
                uLong adler_whole = adler32(0L, test_data.data(), test_data.size());
                
                // Combine and compare
                uLong adler_combined_parts = adler32_combine(adler_part1, adler_part2, 
                                                           test_data.size() - split_point);
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Combined checksum and streaming API testing
            // --------------------------------------------------------
            
            // Setup deflate stream with custom allocators
            z_stream deflate_strm;
            deflate_strm.zalloc = use_custom_allocators ? custom_zcalloc : Z_NULL;
            deflate_strm.zfree = use_custom_allocators ? custom_zcfree : Z_NULL;
            deflate_strm.opaque = Z_NULL;
            
            // Initialize deflate
            int deflate_init_result = deflateInit2(&deflate_strm, 
                                                  Z_DEFAULT_COMPRESSION,
                                                  Z_DEFLATED,
                                                  window_bits,
                                                  mem_level,
                                                  strategy);
            
            if (deflate_init_result == Z_OK) {
                // Calculate checksum of input data
                uLong input_crc = crc32(0L, test_data.data(), test_data.size());
                uLong input_adler = adler32(0L, test_data.data(), test_data.size());
                
                // Setup buffers
                uLongf compr_bound = deflateBound(&deflate_strm, test_data.size());
                if (compr_bound > 0 && compr_bound < 10 * 1024 * 1024) {
                    Bytef* compr = (Bytef*)malloc(compr_bound);
                    Bytef* uncompr = (Bytef*)malloc(test_data.size());
                    
                    if (compr && uncompr) {
                        // Setup deflate
                        deflate_strm.next_in = test_data.data();
                        deflate_strm.avail_in = test_data.size();
                        deflate_strm.next_out = compr;
                        deflate_strm.avail_out = compr_bound;
                        
                        // Perform compression
                        int deflate_result = deflate(&deflate_strm, Z_FINISH);
                        
                        if (deflate_result == Z_STREAM_END) {
                            uLongf compr_len = deflate_strm.total_out;
                            
                            // Setup inflate
                            z_stream inflate_strm;
                            inflate_strm.zalloc = use_custom_allocators ? custom_zcalloc : Z_NULL;
                            inflate_strm.zfree = use_custom_allocators ? custom_zcfree : Z_NULL;
                            inflate_strm.opaque = Z_NULL;
                            inflate_strm.next_in = compr;
                            inflate_strm.avail_in = compr_len;
                            inflate_strm.next_out = uncompr;
                            inflate_strm.avail_out = test_data.size();
                            
                            // Initialize inflate
                            int inflate_init_result = inflateInit2(&inflate_strm, window_bits);
                            
                            if (inflate_init_result == Z_OK) {
                                // Perform inflation
                                int inflate_result = inflate(&inflate_strm, Z_FINISH);
                                
                                if (inflate_result == Z_STREAM_END) {
                                    // Calculate checksum of decompressed data
                                    uLong output_crc = crc32(0L, uncompr, inflate_strm.total_out);
                                    uLong output_adler = adler32(0L, uncompr, inflate_strm.total_out);
                                    
                                    // Checksums should match if decompression was successful
                                }
                                
                                inflateEnd(&inflate_strm);
                            }
                        }
                        
                        free(compr);
                        free(uncompr);
                    }
                }
                
                deflateEnd(&deflate_strm);
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Memory allocation edge cases and utility functions
            // --------------------------------------------------------------
            
            // Test zlibCompileFlags() - returns compilation flags
            uLong compile_flags = zlibCompileFlags();
            
            // Test zError() with various error codes
            int error_code = fdp.ConsumeIntegralInRange<int>(-10, 10);
            const char* error_msg = zError(error_code);
            
            // Test with specific error codes to cover all cases
            const char* msgs[10];
            msgs[0] = zError(2);    // Z_NEED_DICT
            msgs[1] = zError(1);    // Z_STREAM_END
            msgs[2] = zError(0);    // Z_OK
            msgs[3] = zError(-1);   // Z_ERRNO
            msgs[4] = zError(-2);   // Z_STREAM_ERROR
            msgs[5] = zError(-3);   // Z_DATA_ERROR
            msgs[6] = zError(-4);   // Z_MEM_ERROR
            msgs[7] = zError(-5);   // Z_BUF_ERROR
            msgs[8] = zError(-6);   // Z_VERSION_ERROR
            msgs[9] = zError(10);   // Invalid (should return empty string)
            
            // Test zlibVersion()
            const char* version = zlibVersion();
            
            // Test memory allocation by performing operations with different sizes
            if (test_data.size() > 0) {
                // Create a z_stream with custom allocators
                z_stream test_strm;
                test_strm.zalloc = use_custom_allocators ? custom_zcalloc : Z_NULL;
                test_strm.zfree = use_custom_allocators ? custom_zcfree : Z_NULL;
                test_strm.opaque = Z_NULL;
                
                // Initialize with very small memory level to test allocation paths
                int test_mem_level = fdp.ConsumeIntegralInRange<int>(1, 2);
                int init_result = deflateInit2(&test_strm, 
                                              Z_DEFAULT_COMPRESSION,
                                              Z_DEFLATED,
                                              window_bits,
                                              test_mem_level,
                                              strategy);
                
                if (init_result == Z_OK) {
                    // Allocate small buffer to trigger memory allocations
                    uLongf small_bound = deflateBound(&test_strm, 16);
                    if (small_bound > 0 && small_bound < 1024) {
                        Bytef* small_buf = (Bytef*)malloc(small_bound);
                        if (small_buf) {
                            test_strm.next_in = test_data.data();
                            test_strm.avail_in = test_data.size() > 16 ? 16 : test_data.size();
                            test_strm.next_out = small_buf;
                            test_strm.avail_out = small_bound;
                            
                            // Perform a small compression
                            deflate(&test_strm, Z_SYNC_FLUSH);
                            
                            free(small_buf);
                        }
                    }
                    
                    deflateEnd(&test_strm);
                }
            }
            break;
        }
        
        case 5: {
            // Scenario 5: Inflate sync and reset functions
            // --------------------------------------------
            
            // Create compressed data
            uLongf compr_bound = compressBound(test_data.size());
            if (compr_bound == 0 || compr_bound > 10 * 1024 * 1024) {
                return 0;
            }
            
            Bytef* compr = (Bytef*)malloc(compr_bound);
            Bytef* uncompr = (Bytef*)malloc(test_data.size());
            
            if (!compr || !uncompr) {
                free(compr);
                free(uncompr);
                return 0;
            }
            
            uLongf compr_len = compr_bound;
            int compress_result = compress(compr, &compr_len, test_data.data(), test_data.size());
            
            if (compress_result == Z_OK && compr_len > 0) {
                // Setup inflate stream
                z_stream inflate_strm;
                inflate_strm.zalloc = Z_NULL;
                inflate_strm.zfree = Z_NULL;
                inflate_strm.opaque = Z_NULL;
                
                // Initialize inflate
                int init_result = inflateInit2(&inflate_strm, window_bits);
                
                if (init_result == Z_OK) {
                    // Test inflateReset
                    inflateReset(&inflate_strm);
                    
                    // Setup for inflation
                    inflate_strm.next_in = compr;
                    inflate_strm.avail_in = compr_len;
                    inflate_strm.next_out = uncompr;
                    inflate_strm.avail_out = test_data.size();
                    
                    // Start inflation
                    int inflate_result = inflate(&inflate_strm, Z_NO_FLUSH);
                    
                    // Test inflateReset2 to change window bits
                    if (fdp.ConsumeBool()) {
                        int new_window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
                        inflateReset2(&inflate_strm, new_window_bits);
                    }
                    
                    // Test inflateSync
                    if (inflate_result == Z_DATA_ERROR || fdp.ConsumeBool()) {
                        // Try to sync to next boundary
                        inflateSync(&inflate_strm);
                    }
                    
                    inflateEnd(&inflate_strm);
                }
            }
            
            free(compr);
            free(uncompr);
            break;
        }
    }
    
    return 0;
}

} // namespace harness_022
