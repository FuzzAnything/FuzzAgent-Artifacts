#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <limits>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_009 {
/*
 * Fuzzing harness for zlib gzfread API - Comprehensive structured gzip file reading
 * Target: gzfread (structured reading with size_t parameters like fread)
 * 
 * This is the final harness in the campaign targeting 90% API coverage threshold.
 * Focuses on comprehensive testing of gzfread with various buffer sizes, item counts,
 * data patterns, and error conditions.
 * 
 * Semantic diversity from harness_003 (which targeted gzip reading operations):
 * 1. Dedicated focus on gzfread with extensive parameter combinations
 * 2. Tests size*nitems overflow conditions (Z_STREAM_ERROR path)
 * 3. Tests partial item reading at EOF (size != 1 edge cases)
 * 4. Tests with different gzip file states (seek positions, buffer sizes)
 * 5. Tests with concurrent error conditions and state transitions
 * 6. Tests all gzfread error paths described in documentation
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration and test data
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // ==============================================
    // Consume fixed-size configuration parameters
    // ==============================================
    
    // Test scenario selection
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // gzfread parameters
    size_t item_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
    size_t num_items = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
    
    // File operation parameters
    int buffer_size = fdp.ConsumeIntegralInRange<int>(0, 65536);
    int seek_offset = fdp.ConsumeIntegral<int>();
    int seek_whence = fdp.ConsumeIntegralInRange<int>(0, 2);  // SEEK_SET, SEEK_CUR, SEEK_END
    
    // Error injection flags
    bool test_overflow = fdp.ConsumeBool();
    bool test_partial_eof = fdp.ConsumeBool();
    bool test_error_recovery = fdp.ConsumeBool();
    bool test_concurrent_ops = fdp.ConsumeBool();
    
    // Consume binary data for the gzip file content
    size_t file_data_size = fdp.ConsumeIntegralInRange<size_t>(128, 4096);
    if (fdp.remaining_bytes() < file_data_size) {
        return 0;
    }
    std::vector<uint8_t> file_data = fdp.ConsumeBytes<uint8_t>(file_data_size);
    

    
    // ==============================================
    // Create temporary gzip file for testing
    // ==============================================
    
    char temp_filename[] = "/tmp/gzfread_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        return 0;
    }
    
    // Write test data to temporary file
    if (write(fd, file_data.data(), file_data.size()) != (ssize_t)file_data.size()) {
        close(fd);
        unlink(temp_filename);
        return 0;
    }
    close(fd);
    
    // ==============================================
    // Open gzip file for reading
    // ==============================================
    
    const char* mode = "rb";
    gzFile gzfile = gzopen(temp_filename, mode);
    if (!gzfile) {
        unlink(temp_filename);
        return 0;
    }
    
    // Set buffer size if requested
    if (buffer_size > 0) {
        gzbuffer(gzfile, buffer_size);
    }
    
    // ==============================================
    // Test different gzfread scenarios
    // ==============================================
    
    int result = 0;
    size_t items_read = 0;
    z_off_t position = 0;
    
    switch (test_scenario) {
        case 0:  // Basic gzfread with various size/nitems combinations
        {
            // Test different combinations of item_size and num_items
            if (item_size > 0 && num_items > 0) {
                // Ensure buffer is large enough
                size_t buffer_needed = item_size * num_items;
                size_t buffer_size_actual = (buffer_needed < 65536) ? buffer_needed : 65536;
                
                // Adjust num_items if buffer would be too large
                if (buffer_needed > 65536) {
                    num_items = 65536 / item_size;
                    if (num_items == 0) num_items = 1;
                }
                
                std::vector<uint8_t> read_buffer(item_size * num_items);
                items_read = gzfread(read_buffer.data(), item_size, num_items, gzfile);
                
                // Check for errors
                if (items_read == 0 && !gzeof(gzfile)) {
                    int errnum;
                    const char* errmsg = gzerror(gzfile, &errnum);
                    // Error path tested
                }
            }
            break;
        }
        
        case 1:  // Test size*nitems overflow (Z_STREAM_ERROR path)
        {
            if (test_overflow) {
                // Try to trigger overflow by using large values
                size_t overflow_size = std::numeric_limits<size_t>::max() / 2 + 1;
                size_t overflow_nitems = 2;  // This should cause overflow
                
                char small_buffer[16];
                items_read = gzfread(small_buffer, overflow_size, overflow_nitems, gzfile);
                
                // Should return 0 and set error state to Z_STREAM_ERROR
                if (items_read == 0) {
                    int errnum;
                    const char* errmsg = gzerror(gzfile, &errnum);
                    // Error should be Z_STREAM_ERROR
                }
            } else {
                // Normal operation without overflow
                if (item_size > 0 && num_items > 0) {
                    size_t safe_size = (item_size > 1024) ? 1024 : item_size;
                    size_t safe_nitems = (num_items > 1024) ? 1024 : num_items;
                    
                    std::vector<uint8_t> read_buffer(safe_size * safe_nitems);
                    items_read = gzfread(read_buffer.data(), safe_size, safe_nitems, gzfile);
                }
            }
            break;
        }
        
        case 2:  // Test partial item reading at EOF (size != 1 edge case)
        {
            if (test_partial_eof) {
                // Read to near EOF first
                char temp_buffer[1024];
                size_t temp_read = 0;
                
                // Read some data to advance position
                while (!gzeof(gzfile) && temp_read < sizeof(temp_buffer)) {
                    temp_read += gzread(gzfile, temp_buffer + temp_read, sizeof(temp_buffer) - temp_read);
                }
                
                // Now try gzfread with size > 1 when near EOF
                // This tests the partial item reading behavior
                if (item_size > 1) {
                    std::vector<uint8_t> read_buffer(item_size * 2);  // Ask for 2 items
                    items_read = gzfread(read_buffer.data(), item_size, 2, gzfile);
                    
                    // Could be 0, 1, or 2 items read depending on remaining data
                    // Documentation says partial item may be read but not counted
                }
            } else {
                // Normal reading from beginning
                if (item_size > 0 && num_items > 0) {
                    std::vector<uint8_t> read_buffer(item_size * num_items);
                    items_read = gzfread(read_buffer.data(), item_size, num_items, gzfile);
                }
            }
            break;
        }
        
        case 3:  // Test gzfread with file positioning (seek operations)
        {
            // Get current position
            position = gztell(gzfile);
            
            // Try seeking to different positions
            z_off_t new_position = gzseek(gzfile, seek_offset, seek_whence);
            
            // If seek succeeded, perform gzfread
            if (new_position != -1) {
                if (item_size > 0 && num_items > 0) {
                    std::vector<uint8_t> read_buffer(item_size * num_items);
                    items_read = gzfread(read_buffer.data(), item_size, num_items, gzfile);
                    
                    // Check position after reading
                    z_off_t after_position = gztell(gzfile);
                }
            }
            break;
        }
        
        case 4:  // Test gzfread error recovery scenarios
        {
            if (test_error_recovery) {
                // First, try to read with invalid parameters to potentially cause error
                char small_buffer[16];
                
                // Try with size=0 (should return 0 immediately)
                items_read = gzfread(small_buffer, 0, num_items, gzfile);
                
                // Try with nitems=0 (should return 0 immediately)
                items_read = gzfread(small_buffer, item_size, 0, gzfile);
                
                // Clear any error state
                int errnum;
                const char* errmsg = gzerror(gzfile, &errnum);
                
                // Now try valid read
                if (item_size > 0 && num_items > 0) {
                    size_t safe_size = (item_size > 1024) ? 1024 : item_size;
                    size_t safe_nitems = (num_items > 1024) ? 1024 : num_items;
                    
                    std::vector<uint8_t> read_buffer(safe_size * safe_nitems);
                    items_read = gzfread(read_buffer.data(), safe_size, safe_nitems, gzfile);
                }
            } else {
                // Normal reading without error injection
                if (item_size > 0 && num_items > 0) {
                    std::vector<uint8_t> read_buffer(item_size * num_items);
                    items_read = gzfread(read_buffer.data(), item_size, num_items, gzfile);
                }
            }
            break;
        }
        
        case 5:  // Test gzfread with concurrent operations (mixed API calls)
        {
            if (test_concurrent_ops) {
                // Mix gzfread with other gzip reading operations
                
                // Read some data with gzread first
                char temp_buf[256];
                int bytes_read = gzread(gzfile, temp_buf, sizeof(temp_buf));
                
                // Then use gzfread
                if (item_size > 0 && num_items > 0) {
                    std::vector<uint8_t> read_buffer(item_size * num_items);
                    items_read = gzfread(read_buffer.data(), item_size, num_items, gzfile);
                }
                
                // Check EOF status
                int eof_status = gzeof(gzfile);
                
                // Read more with gzgetc
                if (!eof_status) {
                    int ch = gzgetc(gzfile);
                    if (ch != -1) {
                        // Try gzungetc
                        gzungetc(ch, gzfile);
                        // Read it back
                        int ch2 = gzgetc(gzfile);
                    }
                }
                
                // Another gzfread
                if (item_size > 0 && num_items > 0) {
                    std::vector<uint8_t> read_buffer2(item_size * (num_items / 2 + 1));
                    items_read = gzfread(read_buffer2.data(), item_size, num_items / 2 + 1, gzfile);
                }
            } else {
                // Just gzfread
                if (item_size > 0 && num_items > 0) {
                    std::vector<uint8_t> read_buffer(item_size * num_items);
                    items_read = gzfread(read_buffer.data(), item_size, num_items, gzfile);
                }
            }
            break;
        }
        
        case 6:  // Test gzfread with different buffer allocation strategies
        {
            // Test with stack-allocated buffer
            char stack_buffer[4096];
            if (item_size > 0 && num_items > 0) {
                size_t buffer_needed = item_size * num_items;
                if (buffer_needed <= sizeof(stack_buffer)) {
                    items_read = gzfread(stack_buffer, item_size, num_items, gzfile);
                } else {
                    // Use heap allocation
                    std::vector<uint8_t> heap_buffer(buffer_needed);
                    items_read = gzfread(heap_buffer.data(), item_size, num_items, gzfile);
                }
            }
            
            // Test with aligned buffers (power of 2 sizes)
            if (item_size > 0) {
                // Round item_size to nearest power of 2
                size_t aligned_size = 1;
                while (aligned_size < item_size && aligned_size < 2048) {
                    aligned_size <<= 1;
                }
                
                std::vector<uint8_t> aligned_buffer(aligned_size * 4);  // Read 4 items
                items_read = gzfread(aligned_buffer.data(), aligned_size, 4, gzfile);
            }
            break;
        }
        
        case 7:  // Comprehensive gzfread stress test
        {
            // Multiple sequential gzfread operations with varying parameters
            for (int i = 0; i < 10 && !gzeof(gzfile); i++) {
                // Vary parameters for each read
                size_t current_item_size = (item_size + i) % 256 + 1;
                size_t current_num_items = (num_items + i * 2) % 32 + 1;
                
                size_t buffer_needed = current_item_size * current_num_items;
                if (buffer_needed > 65536) {
                    current_num_items = 65536 / current_item_size;
                    if (current_num_items == 0) current_num_items = 1;
                }
                
                std::vector<uint8_t> read_buffer(current_item_size * current_num_items);
                size_t current_read = gzfread(read_buffer.data(), current_item_size, current_num_items, gzfile);
                
                items_read += current_read;
                
                // Break if nothing was read and not at EOF (possible error)
                if (current_read == 0 && !gzeof(gzfile)) {
                    int errnum;
                    const char* errmsg = gzerror(gzfile, &errnum);
                    break;
                }
            }
            break;
        }
    }
    
    // ==============================================
    // Additional comprehensive error testing
    // ==============================================
    
    // Test gzfread with NULL buffer (should be handled by gzfread implementation)
    // Note: We don't actually call with NULL as it would be undefined behavior,
    // but we test edge cases around it
    
    // Test gzfread after gzclearerr (error state clearing)
    if (test_error_recovery) {
        // Check current error state
        int errnum;
        const char* errmsg = gzerror(gzfile, &errnum);
        
        // If there's an error, try to clear it (though gzclearerr doesn't exist in zlib)
        // Instead, we just note the error and continue
    }
    
    // Test gzfread at exact EOF boundary
    if (test_partial_eof) {
        // Try to read past EOF
        char eof_buffer[16];
        size_t eof_read = gzfread(eof_buffer, 1, sizeof(eof_buffer), gzfile);
        
        // Should return 0 and set EOF
        if (eof_read == 0) {
            int eof_flag = gzeof(gzfile);
        }
    }
    
    // ==============================================
    // Cleanup
    // ==============================================
    
    // Close gzip file
    result = gzclose(gzfile);
    
    // Remove temporary file
    unlink(temp_filename);
    
    return 0;
}

} // namespace harness_009
