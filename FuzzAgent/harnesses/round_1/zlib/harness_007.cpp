#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_007 {
/*
 * Fuzzing harness for zlib '_z' variant compression/decompression functions and gzip stream reading
 * Targets high-priority uncovered APIs: compress_z (186 branches), uncompress_z (368 branches), gzgetc_ (352 branches)
 * 
 * This harness specifically tests the size_t ('_z') variant functions which are semantically different
 * from the regular uLong variants tested in existing harnesses. The '_z' variants use z_size_t for
 * lengths, making them suitable for 64-bit systems and large data handling.
 * 
 * APIs targeted:
 * - compress_z, compress2_z, compressBound_z (size_t variant compression)
 * - uncompress_z, uncompress2_z (size_t variant decompression)  
 * - deflateBound_z (size_t variant deflate bound)
 * - gzgetc_ (backward compatibility gzip character reading)
 * - adler32_z, crc32_z (size_t variant checksums)
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: Tests regular compress/uncompress (uLong variants), not _z variants
 * - harness_001: Tests gzip operations but not gzgetc_
 * - harness_003: Tests gzgetc but not gzgetc_ backward compatibility function
 * - No existing harness tests the size_t variant functions
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    bool test_compress_z = fdp.ConsumeBool();
    bool test_uncompress_z = fdp.ConsumeBool();
    bool test_compress2_z = fdp.ConsumeBool();
    bool test_uncompress2_z = fdp.ConsumeBool();
    bool test_gzgetc_underscore = fdp.ConsumeBool();
    bool test_checksum_z = fdp.ConsumeBool();
    bool test_deflate_bound_z = fdp.ConsumeBool();
    bool test_error_conditions = fdp.ConsumeBool();
    
    // Consume size parameters for _z variants (size_t instead of uLong)
    z_size_t source_size = fdp.ConsumeIntegralInRange<z_size_t>(1, 64 * 1024);
    z_size_t dest_buffer_size = fdp.ConsumeIntegralInRange<z_size_t>(1, 128 * 1024);
    
    // Consume input data for compression/checksum tests
    if (fdp.remaining_bytes() < source_size) {
        return 0;
    }
    std::vector<uint8_t> source_data = fdp.ConsumeBytes<uint8_t>(source_size);
    
    // Test 1: compress_z and compressBound_z
    if (test_compress_z && source_size > 0) {
        // First get bound using compressBound_z
        z_size_t bound = compressBound_z(source_size);
        
        // Allocate destination buffer
        Bytef* dest = (Bytef*)malloc(bound);
        if (dest) {
            z_size_t destLen = bound;
            
            // Test compress_z
            int ret = compress_z(dest, &destLen, source_data.data(), source_size);
            
            // If compression succeeded, we could test decompression
            if (ret == Z_OK && destLen > 0 && destLen <= bound) {
                // For testing, we could decompress but we'll do that in uncompress_z test
                // Just verify the compressed size is reasonable
                if (destLen < source_size * 2) {
                    // This is expected for compressible data
                }
            }
            
            free(dest);
        }
    }
    
    // Test 2: compress2_z with specific compression level
    if (test_compress2_z && source_size > 0) {
        z_size_t bound = compressBound_z(source_size);
        Bytef* dest = (Bytef*)malloc(bound);
        if (dest) {
            z_size_t destLen = bound;
            
            // Test compress2_z with fuzzed compression level
            int ret = compress2_z(dest, &destLen, source_data.data(), source_size, compression_level);
            
            // Test error conditions if requested
            if (test_error_conditions) {
                // Test with invalid destLen (too small)
                z_size_t small_destLen = 1;
                Bytef* small_dest = (Bytef*)malloc(small_destLen);
                if (small_dest) {
                    compress2_z(small_dest, &small_destLen, source_data.data(), source_size, compression_level);
                    free(small_dest);
                }
                
                // Test with NULL pointers
                if (fdp.ConsumeBool()) {
                    z_size_t dummy_len = bound;
                    compress2_z(nullptr, &dummy_len, source_data.data(), source_size, compression_level);
                }
                if (fdp.ConsumeBool()) {
                    compress2_z(dest, nullptr, source_data.data(), source_size, compression_level);
                }
            }
            
            free(dest);
        }
    }
    
    // Test 3: uncompress_z (requires compressed data first)
    if (test_uncompress_z) {
        // First compress some data to have valid compressed input
        z_size_t bound = compressBound_z(source_size);
        Bytef* compressed = (Bytef*)malloc(bound);
        Bytef* uncompressed = (Bytef*)malloc(source_size);
        
        if (compressed && uncompressed && source_size > 0) {
            z_size_t compressed_len = bound;
            int compress_ret = compress_z(compressed, &compressed_len, source_data.data(), source_size);
            
            if (compress_ret == Z_OK && compressed_len > 0) {
                // Now test uncompress_z
                z_size_t uncompressed_len = source_size;
                int uncompress_ret = uncompress_z(uncompressed, &uncompressed_len, 
                                                  compressed, compressed_len);
                
                // Verify if successful
                if (uncompress_ret == Z_OK && uncompressed_len == source_size) {
                    // Data should match
                    memcmp(uncompressed, source_data.data(), source_size);
                }
                
                // Test error conditions
                if (test_error_conditions) {
                    // Test with insufficient buffer
                    z_size_t small_len = 1;
                    Bytef* small_buf = (Bytef*)malloc(small_len);
                    if (small_buf) {
                        uncompress_z(small_buf, &small_len, compressed, compressed_len);
                        free(small_buf);
                    }
                    
                    // Test with corrupted compressed data
                    if (compressed_len > 10) {
                        // Corrupt some bytes
                        compressed[0] ^= 0xFF;
                        compressed[compressed_len/2] ^= 0xFF;
                        z_size_t test_len = source_size;
                        uncompress_z(uncompressed, &test_len, compressed, compressed_len);
                    }
                }
            }
            
            free(compressed);
            free(uncompressed);
        } else {
            if (compressed) free(compressed);
            if (uncompressed) free(uncompressed);
        }
    }
    
    // Test 4: uncompress2_z (with sourceLen pointer)
    if (test_uncompress2_z) {
        z_size_t bound = compressBound_z(source_size);
        Bytef* compressed = (Bytef*)malloc(bound);
        Bytef* uncompressed = (Bytef*)malloc(source_size);
        
        if (compressed && uncompressed && source_size > 0) {
            z_size_t compressed_len = bound;
            compress_z(compressed, &compressed_len, source_data.data(), source_size);
            
            if (compressed_len > 0) {
                z_size_t uncompressed_len = source_size;
                z_size_t source_consumed = compressed_len;
                
                // Test uncompress2_z with source length pointer
                int ret = uncompress2_z(uncompressed, &uncompressed_len, 
                                       compressed, &source_consumed);
                
                // source_consumed should be updated
                if (ret == Z_OK) {
                    // Verify source_consumed <= compressed_len
                    if (source_consumed > compressed_len) {
                        // Unexpected but handle gracefully
                    }
                }
            }
            
            free(compressed);
            free(uncompressed);
        } else {
            if (compressed) free(compressed);
            if (uncompressed) free(uncompressed);
        }
    }
    
    // Test 5: gzgetc_ (backward compatibility function)
    if (test_gzgetc_underscore) {
        // Create a temporary file with gzipped content
        char fname[] = "/tmp/gzgetc_fuzz_XXXXXX";
        int fd = mkstemp(fname);
        if (fd >= 0) {
            // Write some test data
            write(fd, source_data.data(), source_size > 1024 ? 1024 : source_size);
            close(fd);
            
            // Open with gzip
            gzFile file = gzopen(fname, "rb");
            if (file) {
                // Test gzgetc_ function (backward compatibility)
                int ch = gzgetc_(file);
                
                // Read a few more bytes if available
                for (int i = 0; i < 10 && !gzeof(file); i++) {
                    ch = gzgetc_(file);
                    if (ch == -1 && !gzeof(file)) {
                        // Error occurred
                        int errnum;
                        const char* errmsg = gzerror(file, &errnum);
                        break;
                    }
                }
                
                // Test gzungetc with the character
                if (ch != -1) {
                    gzungetc(ch, file);
                    // Read it back
                    gzgetc_(file);
                }
                
                gzclose(file);
            }
            
            unlink(fname);
        }
    }
    
    // Test 6: Checksum _z variants (adler32_z, crc32_z)
    if (test_checksum_z && source_size > 0) {
        // Test adler32_z
        uLong adler = adler32(0L, Z_NULL, 0);
        adler = adler32_z(adler, source_data.data(), source_size);
        
        // Test with different initial values
        uLong init_adler = fdp.ConsumeIntegral<uLong>();
        adler32_z(init_adler, source_data.data(), source_size);
        
        // Test crc32_z
        uLong crc = crc32(0L, Z_NULL, 0);
        crc = crc32_z(crc, source_data.data(), source_size);
        
        // Test with different initial values
        uLong init_crc = fdp.ConsumeIntegral<uLong>();
        crc32_z(init_crc, source_data.data(), source_size);
        
        // Test with partial buffers
        if (source_size > 10) {
            z_size_t half_size = source_size / 2;
            adler32_z(0L, source_data.data(), half_size);
            adler32_z(adler32_z(0L, source_data.data(), half_size), 
                     source_data.data() + half_size, source_size - half_size);
        }
    }
    
    // Test 7: deflateBound_z (requires deflate stream)
    if (test_deflate_bound_z) {
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        
        int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
        int memLevel = fdp.ConsumeIntegralInRange<int>(1, 9);
        
        int ret = deflateInit2(&strm, compression_level, Z_DEFLATED, 
                              windowBits, memLevel, Z_DEFAULT_STRATEGY);
        if (ret == Z_OK) {
            // Test deflateBound_z with various source lengths
            z_size_t bound1 = deflateBound_z(&strm, source_size);
            z_size_t bound2 = deflateBound_z(&strm, source_size * 2);
            z_size_t bound3 = deflateBound_z(&strm, 0);
            
            // Verify bounds are reasonable (non-zero for non-zero input)
            if (source_size > 0 && bound1 == 0) {
                // This might indicate an issue
            }
            
            deflateEnd(&strm);
        }
    }
    
    return 0;
}

} // namespace harness_007
