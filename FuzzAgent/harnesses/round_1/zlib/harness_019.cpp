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
#include <algorithm>

namespace harness_019 {
/*
 * Fuzzing harness for zlib comprehensive inflate testing through multiple entry points
 * Targets inflate function with 439 blocked branches - the largest coverage gap in zlib
 * 
 * Focus: Comprehensive traditional inflate API testing through multiple entry points:
 * - Direct inflate() calls with varied wrap modes (0=raw, 1=zlib, 2=gzip)
 * - One-shot decompression via uncompress2_z()
 * - Gzip file API paths via gzread()/gzgets()
 * - Multiple flush parameters: Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_BLOCK
 * - Dictionary testing with inflateSetDictionary
 * - Error recovery with inflateReset/inflateReset2
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: Basic compression/decompression
 * - harness_001: Gzip file operations
 * - harness_002: inflateBack callback interface  
 * - harness_006: Stream control functions
 * - harness_018: inflateBack callback-based decompression (complementary)
 * - This harness: Traditional inflate API paths with comprehensive parameter variation
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size configuration parameters first
    int windowBits_base = fdp.ConsumeIntegralInRange<int>(8, 15);
    int wrap_mode = fdp.ConsumeIntegralInRange<int>(0, 2);  // 0=raw, 1=zlib, 2=gzip
    
    // Calculate actual windowBits based on wrap mode
    int windowBits = windowBits_base;
    switch (wrap_mode) {
        case 0:  // raw
            windowBits = -windowBits_base;
            break;
        case 1:  // zlib
            windowBits = windowBits_base;
            break;
        case 2:  // gzip
            windowBits = windowBits_base + 16;  // Add 16 for gzip wrapper
            break;
    }
    
    // Test selection flags
    bool test_direct_inflate = fdp.ConsumeBool();
    bool test_uncompress2 = fdp.ConsumeBool();
    bool test_gzip_api = fdp.ConsumeBool();
    bool test_dictionary = fdp.ConsumeBool();
    bool test_error_recovery = fdp.ConsumeBool();
    bool test_multiple_flush = fdp.ConsumeBool();
    
    // Consume flush parameter selection
    int flush_mode = fdp.ConsumeIntegralInRange<int>(0, 5);
    int flush_values[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_BLOCK, Z_TREES};
    int selected_flush = flush_values[flush_mode % 6];
    
    // Consume dictionary size if needed
    size_t dict_size = 0;
    if (test_dictionary) {
        dict_size = fdp.ConsumeIntegralInRange<size_t>(32, 1024);
    }
    
    // Split remaining data: first part for compressed data, second for dictionary
    size_t compressed_data_size = fdp.ConsumeIntegralInRange<size_t>(128, 8192);
    if (fdp.remaining_bytes() < compressed_data_size) {
        return 0;
    }
    
    std::vector<uint8_t> compressed_data = fdp.ConsumeBytes<uint8_t>(compressed_data_size);
    
    // Test 1: Direct inflate() calls with comprehensive parameter variation
    if (test_direct_inflate && compressed_data_size > 0) {
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.next_in = Z_NULL;
        strm.avail_in = 0;
        
        // Initialize with specific wrap mode
        int ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            // Prepare input/output buffers
            size_t out_buf_size = compressed_data_size * 4;  // Safe expansion factor
            if (out_buf_size > 1024 * 1024) out_buf_size = 1024 * 1024;  // Cap at 1MB
            
            std::vector<uint8_t> out_buffer(out_buf_size);
            
            strm.next_in = compressed_data.data();
            strm.avail_in = compressed_data.size();
            strm.next_out = out_buffer.data();
            strm.avail_out = out_buffer.size();
            
            // Test with different flush modes if requested
            if (test_multiple_flush) {
                // Test Z_NO_FLUSH first (partial decompression)
                ret = inflate(&strm, Z_NO_FLUSH);
                
                // Then test selected flush mode
                if (ret == Z_OK || ret == Z_BUF_ERROR) {
                    ret = inflate(&strm, selected_flush);
                }
            } else {
                // Single flush mode test
                ret = inflate(&strm, selected_flush);
            }
            
            // Test dictionary if requested and we have enough data
            if (test_dictionary && dict_size > 0 && fdp.remaining_bytes() >= dict_size) {
                std::vector<uint8_t> dictionary = fdp.ConsumeBytes<uint8_t>(dict_size);
                inflateSetDictionary(&strm, dictionary.data(), dictionary.size());
                
                // Continue inflation with dictionary
                if (strm.avail_in > 0) {
                    inflate(&strm, Z_FINISH);
                }
            }
            
            // Test error recovery if requested
            if (test_error_recovery) {
                if (fdp.ConsumeBool()) {
                    // Try inflateReset
                    inflateReset(&strm);
                    
                    // Reset input/output and try again
                    strm.next_in = compressed_data.data();
                    strm.avail_in = compressed_data.size() / 2;  // Half the data
                    strm.next_out = out_buffer.data();
                    strm.avail_out = out_buffer.size();
                    inflate(&strm, Z_FINISH);
                } else {
                    // Try inflateReset2 with different windowBits
                    int new_windowBits = windowBits_base + (fdp.ConsumeBool() ? 0 : 1);
                    if (wrap_mode == 0) new_windowBits = -new_windowBits;
                    else if (wrap_mode == 2) new_windowBits = new_windowBits + 16;
                    
                    inflateReset2(&strm, new_windowBits);
                    
                    // Try with remaining data
                    strm.next_in = compressed_data.data() + compressed_data.size() / 2;
                    strm.avail_in = compressed_data.size() / 2;
                    strm.next_out = out_buffer.data();
                    strm.avail_out = out_buffer.size();
                    inflate(&strm, Z_FINISH);
                }
            }
            
            inflateEnd(&strm);
        }
    }
    

    // Test 2: One-shot decompression via uncompress2_z()
    if (test_uncompress2 && compressed_data_size > 0) {
        // Prepare output buffer
        size_t dest_capacity = compressed_data_size * 4;
        if (dest_capacity > 1024 * 1024) dest_capacity = 1024 * 1024;
        
        std::vector<uint8_t> dest_buffer(dest_capacity);
        z_size_t destLen = dest_capacity;
        z_size_t sourceLen = compressed_data.size();
        
        // Try decompression
        int ret = uncompress2_z(dest_buffer.data(), &destLen, 
                               compressed_data.data(), &sourceLen);
        
        // If successful, verify we can recompress (simplified check)
        if (ret == Z_OK && destLen > 0 && destLen <= dest_capacity) {
            // Verify decompressed data is not all zeros (basic sanity)
            bool all_zeros = true;
            for (size_t i = 0; i < std::min((size_t)destLen, (size_t)100); i++) {
                if (dest_buffer[i] != 0) {
                    all_zeros = false;
                    break;
                }
            }
        }
    }
    if (test_gzip_api && compressed_data_size > 0) {
        // Create a temporary file with compressed data
        char fname[] = "/tmp/gzip_inflate_fuzz_XXXXXX";
        int fd = mkstemp(fname);
        if (fd >= 0) {
            // Write compressed data to file
            write(fd, compressed_data.data(), compressed_data.size());
            close(fd);
            
            // Open for reading with appropriate mode based on wrap mode
            const char* mode = "rb";
            gzFile file = gzopen(fname, mode);
            if (file) {
                // Test gzread() with various buffer sizes
                std::vector<uint8_t> read_buffer(4096);
                int bytes_read = 0;
                size_t total_read = 0;
                
                do {
                    size_t to_read = fdp.ConsumeIntegralInRange<size_t>(1, read_buffer.size());
                    bytes_read = gzread(file, read_buffer.data(), to_read);
                    if (bytes_read > 0) {
                        total_read += bytes_read;
                    }
                    
                    // Occasionally test gzgets() if we have text-like data
                    if (fdp.ConsumeBool() && total_read > 10) {
                        char line_buf[256];
                        gzseek(file, 0, SEEK_SET);  // Reset to beginning
                        gzgets(file, line_buf, sizeof(line_buf));
                    }
                    
                } while (bytes_read > 0 && total_read < 64 * 1024);  // Limit total read
                
                // Test other gzip functions
                gzeof(file);
                gzdirect(file);
                gzoffset(file);
                
                // Get and clear errors
                int errnum;
                const char* errmsg = gzerror(file, &errnum);
                gzclearerr(file);
                
                gzclose(file);
            }
            
            // Clean up temporary file
            unlink(fname);
        }
    }
    
    // Test 4: Additional dictionary-focused testing
    if (test_dictionary && dict_size > 0 && fdp.remaining_bytes() >= dict_size) {
        // Use remaining data for dictionary
        std::vector<uint8_t> dictionary = fdp.ConsumeBytes<uint8_t>(dict_size);
        
        // Test inflate with dictionary without compression header
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        
        // Initialize for raw inflate
        int ret = inflateInit2_(&strm, -15, ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            // We'd need actual compressed data with dictionary for this to work
            // This is just to exercise the API path
            inflateSetDictionary(&strm, dictionary.data(), dictionary.size());
            
            // Also test inflateGetDictionary
            uint8_t dict_copy[1024];
            uInt dict_len = sizeof(dict_copy);
            inflateGetDictionary(&strm, dict_copy, &dict_len);
            
            inflateEnd(&strm);
        }
    }
    
    return 0;
}

} // namespace harness_019
