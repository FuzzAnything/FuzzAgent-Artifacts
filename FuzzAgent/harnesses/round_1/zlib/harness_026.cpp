#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_026 {
/*
 * Fuzzing harness for diverse decompression scenarios to exercise 459 blocked branches in inflate function
 * 
 * Based on coverage analysis guidance at /root/FuzzAgent/output/zlib/coverage_guidance/harness_026_guidance.md
 * 
 * Target: 459 blocked branches in inflate function (largest coverage gap)
 * 
 * Primary target: uncompress2_z for direct inflate access
 * Alternative paths: gzread, gzgets
 * Required helpers: inflateInit2_, inflateEnd
 * 
 * Test scenarios:
 * 1. zlib headers
 * 2. gzip headers with flags (extra fields, filename, comment)
 * 3. raw deflate data
 * 4. stored blocks (type 0), fixed blocks (type 1), dynamic blocks (type 2)
 * 5. error conditions (invalid headers, incorrect checksums, truncated data, invalid block types)
 * 6. different flush modes (Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FINISH, Z_BLOCK)
 * 
 * Use inflateInit2_ with different window bits (8-15) and compression methods.
 * Generate test data covering all combinations.
 * Repeat with gzip variants using gzread/gzgets with gzip headers containing different flag combinations.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration and test data
    if (size < 256) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // ==============================================
    // Consume fixed-size configuration parameters
    // ==============================================
    
    // Determine test scenario
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 6;  // 0-5 for different scenarios
    
    // Window bits for inflateInit2_ (8-15 for zlib/gzip, negative for raw)
    int window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
    
    // Determine if we should test raw deflate (negative window bits)
    bool use_raw = fdp.ConsumeBool();
    if (use_raw) {
        window_bits = -window_bits;  // Negative for raw deflate
    }
    
    // Determine flush mode
    int flush_mode = Z_NO_FLUSH;
    uint8_t flush_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (flush_choice) {
        case 0: flush_mode = Z_NO_FLUSH; break;
        case 1: flush_mode = Z_SYNC_FLUSH; break;
        case 2: flush_mode = Z_FINISH; break;
        case 3: flush_mode = Z_BLOCK; break;
    }
    
    // Determine block type for test data generation
    uint8_t block_type = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0=stored, 1=fixed, 2=dynamic
    
    // Determine if we should test error conditions
    bool test_errors = fdp.ConsumeBool();
    
    // Determine if we should use gzread/gzgets path
    bool use_gzread = fdp.ConsumeBool();
    
    // ==============================================
    // Consume variable-size test data
    // ==============================================
    
    // Get test data (compressed or raw deflate data)
    size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(64, 2048);
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
    
    // Get output buffer size
    size_t output_size = fdp.ConsumeIntegralInRange<size_t>(1024, 8192);
    
    // ==============================================
    // Scenario 0: Direct uncompress2_z testing
    // ==============================================
    if (scenario == 0) {
        // Allocate output buffer
        std::vector<uint8_t> output_buffer(output_size);
        z_size_t dest_len = output_size;
        z_size_t source_len = test_data.size();
        
        // Test uncompress2_z
        int result = uncompress2_z(output_buffer.data(), &dest_len, 
                                   test_data.data(), &source_len);
        
        // Check result and potentially exercise error paths
        if (test_errors && result != Z_OK) {
            // Try with corrupted data
            if (test_data.size() > 10) {
                test_data[5] ^= 0xFF;  // Corrupt a byte
                z_size_t dest_len2 = output_size;
                z_size_t source_len2 = test_data.size();
                uncompress2_z(output_buffer.data(), &dest_len2, 
                             test_data.data(), &source_len2);
            }
        }
    }
    
    // ==============================================
    // Scenario 1: Low-level inflate API with inflateInit2_
    // ==============================================
    else if (scenario == 1) {
        z_stream stream;
        memset(&stream, 0, sizeof(stream));
        
        // Initialize decompression
        int init_result = inflateInit2_(&stream, window_bits, ZLIB_VERSION, sizeof(z_stream));
        if (init_result != Z_OK) {
            // Clean up and return
            inflateEnd(&stream);
            return 0;
        }
        
        // Set up input/output buffers
        std::vector<uint8_t> output_buffer(output_size);
        
        stream.next_in = test_data.data();
        stream.avail_in = test_data.size();
        stream.next_out = output_buffer.data();
        stream.avail_out = output_buffer.size();
        
        // Perform inflation with specified flush mode
        int inflate_result;
        do {
            inflate_result = inflate(&stream, flush_mode);
            
            // Handle different return values
            switch (inflate_result) {
                case Z_OK:
                case Z_STREAM_END:
                case Z_BUF_ERROR:
                    // Continue or break based on logic
                    break;
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                case Z_STREAM_ERROR:
                    // Error handling
                    if (test_errors) {
                        // Try to recover or test error paths
                        inflateEnd(&stream);
                        inflateInit2_(&stream, window_bits, ZLIB_VERSION, sizeof(z_stream));
                    }
                    break;
            }
            
            // Break if we're done or have an error
            if (inflate_result == Z_STREAM_END || 
                inflate_result == Z_DATA_ERROR || 
                inflate_result == Z_MEM_ERROR || 
                inflate_result == Z_STREAM_ERROR) {
                break;
            }
            
            // Simulate partial consumption for flush testing
            if (flush_mode == Z_SYNC_FLUSH || flush_mode == Z_BLOCK) {
                // Consume some output and continue
                if (stream.avail_out == 0) {
                    stream.avail_out = output_buffer.size();
                    stream.next_out = output_buffer.data();
                }
            }
            
        } while (stream.avail_in > 0 && inflate_result == Z_OK);
        
        // Clean up
        inflateEnd(&stream);
    }
    
    // ==============================================
    // Scenario 2: gzread testing with gzip headers
    // ==============================================
    else if (scenario == 2 && use_gzread) {
        // Create a temporary file with gzip data
        FILE* tmp = tmpfile();
        if (tmp) {
            // Write test data to temp file
            fwrite(test_data.data(), 1, test_data.size(), tmp);
            fflush(tmp);
            rewind(tmp);
            
            // Open with gzopen (using fdopen to get FILE* for gzdopen)
            int fd = fileno(tmp);
            if (fd >= 0) {
                gzFile gz = gzdopen(fd, "rb");
                if (gz) {
                    // Test gzread
                    std::vector<char> read_buffer(1024);
                    int bytes_read;
                    do {
                        bytes_read = gzread(gz, read_buffer.data(), read_buffer.size());
                        // Exercise different read sizes and error conditions
                        if (test_errors && bytes_read < 0) {
                            // Error handling test
                            break;
                        }
                    } while (bytes_read > 0);
                    
                    gzclose(gz);
                }
            }
            fclose(tmp);
        }
    }
    
    // ==============================================
    // Scenario 3: gzgets testing with gzip headers
    // ==============================================
    else if (scenario == 3 && use_gzread) {
        // Similar to gzread but with line-based reading
        FILE* tmp = tmpfile();
        if (tmp) {
            // Write test data that might contain newlines
            fwrite(test_data.data(), 1, test_data.size(), tmp);
            fflush(tmp);
            rewind(tmp);
            
            int fd = fileno(tmp);
            if (fd >= 0) {
                gzFile gz = gzdopen(fd, "rb");
                if (gz) {
                    char line_buffer[256];
                    // Test gzgets
                    while (gzgets(gz, line_buffer, sizeof(line_buffer)) != NULL) {
                        // Process line
                        if (test_errors && strlen(line_buffer) == 0) {
                            // Test empty line handling
                            break;
                        }
                    }
                    
                    gzclose(gz);
                }
            }
            fclose(tmp);
        }
    }
    
    // ==============================================
    // Scenario 4: Error condition testing
    // ==============================================
    else if (scenario == 4 && test_errors) {
        // Test various error conditions
        
        // 1. Test with invalid headers
        if (test_data.size() > 10) {
            // Corrupt header bytes
            std::vector<uint8_t> corrupted_data = test_data;
            corrupted_data[0] ^= 0xFF;  // Corrupt first byte
            
            z_stream stream;
            memset(&stream, 0, sizeof(stream));
            
            inflateInit2_(&stream, window_bits, ZLIB_VERSION, sizeof(z_stream));
            
            std::vector<uint8_t> output_buffer(output_size);
            stream.next_in = corrupted_data.data();
            stream.avail_in = corrupted_data.size();
            stream.next_out = output_buffer.data();
            stream.avail_out = output_buffer.size();
            
            inflate(&stream, Z_NO_FLUSH);
            inflateEnd(&stream);
        }
        
        // 2. Test truncated data
        if (test_data.size() > 20) {
            std::vector<uint8_t> truncated_data(test_data.begin(), test_data.begin() + test_data.size() / 2);
            
            z_stream stream;
            memset(&stream, 0, sizeof(stream));
            
            inflateInit2_(&stream, window_bits, ZLIB_VERSION, sizeof(z_stream));
            
            std::vector<uint8_t> output_buffer(output_size);
            stream.next_in = truncated_data.data();
            stream.avail_in = truncated_data.size();
            stream.next_out = output_buffer.data();
            stream.avail_out = output_buffer.size();
            
            inflate(&stream, Z_FINISH);
            inflateEnd(&stream);
        }
        
        // 3. Test incorrect checksums (simulated by corrupting data)
        if (test_data.size() > 30) {
            std::vector<uint8_t> checksum_corrupted = test_data;
            // Corrupt near the end to affect checksum
            if (checksum_corrupted.size() > 10) {
                checksum_corrupted[checksum_corrupted.size() - 5] ^= 0xFF;
            }
            
            // Test with uncompress2_z
            std::vector<uint8_t> output_buffer(output_size);
            z_size_t dest_len = output_size;
            z_size_t source_len = checksum_corrupted.size();
            
            uncompress2_z(output_buffer.data(), &dest_len, 
                         checksum_corrupted.data(), &source_len);
        }
    }
    
    // ==============================================
    // Scenario 5: Mixed mode testing
    // ==============================================
    else if (scenario == 5) {
        // Test multiple approaches in sequence
        
        // First, test low-level API
        z_stream stream;
        memset(&stream, 0, sizeof(stream));
        
        inflateInit2_(&stream, window_bits, ZLIB_VERSION, sizeof(z_stream));
        
        std::vector<uint8_t> output_buffer1(output_size);
        stream.next_in = test_data.data();
        stream.avail_in = test_data.size();
        stream.next_out = output_buffer1.data();
        stream.avail_out = output_buffer1.size();
        
        inflate(&stream, flush_mode);
        inflateEnd(&stream);
        
        // Then test uncompress2_z
        if (test_data.size() > 50) {
            std::vector<uint8_t> output_buffer2(output_size);
            z_size_t dest_len = output_size;
            z_size_t source_len = test_data.size();
            
            uncompress2_z(output_buffer2.data(), &dest_len, 
                         test_data.data(), &source_len);
        }
        
        // Finally test gzread if enabled
        if (use_gzread && test_data.size() > 100) {
            FILE* tmp = tmpfile();
            if (tmp) {
                fwrite(test_data.data(), 1, test_data.size(), tmp);
                fflush(tmp);
                rewind(tmp);
                
                int fd = fileno(tmp);
                if (fd >= 0) {
                    gzFile gz = gzdopen(fd, "rb");
                    if (gz) {
                        char buffer[256];
                        gzread(gz, buffer, sizeof(buffer));
                        gzclose(gz);
                    }
                }
                fclose(tmp);
            }
        }
    }
    
    return 0;
}

} // namespace harness_026
