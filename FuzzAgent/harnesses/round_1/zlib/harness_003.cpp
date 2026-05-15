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

namespace harness_003 {
/*
 * Fuzzing harness for zlib gzip file reading operations targeting gzread.c module
 * Targets gzip file reading APIs with various file positions, buffer sizes, and error conditions
 * APIs: gzgets, gzungetc, gzfread, gzgetc, gzseek, gztell, gzrewind, gzeof
 * Focus: Testing gzip file reading with semantic diversity from existing harnesses
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration and test data
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int read_operation = fdp.ConsumeIntegralInRange<int>(0, 7);  // Which reading API to test
    int buffer_size = fdp.ConsumeIntegralInRange<int>(1, 4096);  // Buffer size for reading
    int seek_offset = fdp.ConsumeIntegral<int>();  // Seek offset (can be negative)
    int seek_whence = fdp.ConsumeIntegralInRange<int>(0, 2);  // SEEK_SET, SEEK_CUR, SEEK_END
    bool test_error_conditions = fdp.ConsumeBool();  // Whether to test error conditions
    bool test_boundary_conditions = fdp.ConsumeBool();  // Test edge cases
    bool test_sequential_ops = fdp.ConsumeBool();  // Test sequential operations
    
    // Consume string data for gzgets testing
    size_t string_data_size = fdp.ConsumeIntegralInRange<size_t>(16, 512);
    if (fdp.remaining_bytes() < string_data_size) {
        return 0;
    }
    std::string test_string_data = fdp.ConsumeBytesAsString(string_data_size);
    
    // Consume binary data for gzfread/gzread testing
    size_t binary_data_size = fdp.ConsumeIntegralInRange<size_t>(64, 2048);
    if (fdp.remaining_bytes() < binary_data_size) {
        return 0;
    }
    std::vector<uint8_t> test_binary_data = fdp.ConsumeBytes<uint8_t>(binary_data_size);
    
    // Create a temporary file for gzip operations
    char fname[] = "/tmp/gzread_fuzz_XXXXXX";
    int fd = mkstemp(fname);
    if (fd < 0) {
        return 0;
    }
    
    // Write test data to the temporary file
    if (write(fd, test_binary_data.data(), test_binary_data.size()) != (ssize_t)test_binary_data.size()) {
        close(fd);
        unlink(fname);
        return 0;
    }
    close(fd);
    
    // Open the file for reading with gzip
    const char* mode = "rb";  // Read binary mode
    gzFile file = gzopen(fname, mode);
    if (!file) {
        unlink(fname);
        return 0;
    }
    
    // Set buffer size for gzip operations
    if (buffer_size > 0) {
        gzbuffer(file, buffer_size);
    }
    
    // Test different reading operations based on fuzzer input
    int result = 0;
    char read_buffer[4096];
    z_off_t position = 0;
    
    switch (read_operation) {
        case 0:  // Test gzgets - line reading
        {
            // Read lines with gzgets
            char line_buffer[256];
            for (int i = 0; i < 10 && !gzeof(file); i++) {
                char* line = gzgets(file, line_buffer, sizeof(line_buffer));
                if (!line && !gzeof(file)) {
                    // Error occurred
                    int errnum;
                    const char* errmsg = gzerror(file, &errnum);
                    break;
                }
            }
            break;
        }
        
        case 1:  // Test gzungetc - push back characters
        {
            // Read a character
            int ch = gzgetc(file);
            if (ch != -1) {
                // Push it back
                int unget_result = gzungetc(ch, file);
                if (unget_result != -1) {
                    // Read it again
                    int ch2 = gzgetc(file);
                    // Should be the same character
                }
            }
            break;
        }
        
        case 2:  // Test gzfread - structured reading
        {
            // Read items of various sizes
            size_t item_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
            size_t num_items = fdp.ConsumeIntegralInRange<size_t>(1, 32);
            size_t items_read = 0;
            
            if (item_size > 0 && num_items > 0) {
                items_read = gzfread(read_buffer, item_size, num_items, file);
                
                // Check for errors
                if (items_read == 0 && !gzeof(file)) {
                    int errnum;
                    const char* errmsg = gzerror(file, &errnum);
                }
            }
            break;
        }
        
        case 3:  // Test gzgetc - character reading
        {
            // Read multiple characters
            for (int i = 0; i < 100 && !gzeof(file); i++) {
                int ch = gzgetc(file);
                if (ch == -1 && !gzeof(file)) {
                    // Error occurred
                    break;
                }
            }
            break;
        }
        
        case 4:  // Test gzseek - file positioning
        {
            // Get current position
            position = gztell(file);
            
            // Try to seek to different positions
            z_off_t new_position = gzseek(file, seek_offset, seek_whence);
            
            // If seek succeeded, read some data
            if (new_position != -1) {
                size_t read_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                if (read_size > sizeof(read_buffer)) {
                    read_size = sizeof(read_buffer);
                }
                gzread(file, read_buffer, read_size);
            }
            break;
        }
        
        case 5:  // Test gztell - position querying
        {
            // Read some data first
            size_t read_amount = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            if (read_amount > sizeof(read_buffer)) {
                read_amount = sizeof(read_buffer);
            }
            int bytes_read = gzread(file, read_buffer, read_amount);
            
            // Get position after reading
            position = gztell(file);
            
            // Try seeking back based on position
            if (position > 0) {
                z_off_t back_seek = position / 2;
                if (back_seek > 0) {
                    gzseek(file, -((z_off_t)back_seek), SEEK_CUR);
                }
            }
            break;
        }
        
        case 6:  // Test gzrewind - reset to beginning
        {
            // Read some data first
            gzread(file, read_buffer, 100);
            
            // Check if we're at EOF
            int eof_before = gzeof(file);
            
            // Rewind to beginning
            result = gzrewind(file);
            
            // Check EOF after rewind
            int eof_after = gzeof(file);
            
            // Read again from beginning
            if (result == 0) {
                gzread(file, read_buffer, 50);
            }
            break;
        }
        
        case 7:  // Test gzeof - end-of-file detection
        {
            // Read until EOF
            int total_read = 0;
            while (!gzeof(file)) {
                int bytes_read = gzread(file, read_buffer, sizeof(read_buffer));
                if (bytes_read <= 0) {
                    break;
                }
                total_read += bytes_read;
                
                // Occasionally check position
                if (total_read % 100 == 0) {
                    position = gztell(file);
                }
            }
            
            // Verify EOF is set
            int final_eof = gzeof(file);
            
            // Try to read past EOF (should return 0 or -1)
            int past_eof_read = gzread(file, read_buffer, 10);
            break;
        }
    }
    
    // Test error conditions if requested
    if (test_error_conditions) {
        // Test with zero length (valid buffer, zero bytes to read)
        gzread(file, read_buffer, 0);
        
        // Test gzgets with small buffer - ensure buffer has space for null terminator
        char small_buf[16];  // Larger buffer to avoid overflow
        gzrewind(file);
        // Use length smaller than buffer size to test truncation
        gzgets(file, small_buf, 2);  // Buffer size 16, length 2 allows 1 char + null
        
        // Test gzungetc at beginning of file (should work)
        gzrewind(file);
        gzungetc('A', file);
        
        // Test reading from closed file (simulate error)
        // First close and reopen to avoid actual crash
        gzrewind(file);
        // Read a byte to ensure file position advances
        int ch = gzgetc(file);
        // Now try to read with current position
        gzread(file, read_buffer, 10);
    }
    
    // Test boundary conditions if requested
    if (test_boundary_conditions) {
        // Test reading exactly at buffer boundaries
        gzrewind(file);
        size_t exact_read = buffer_size > 0 ? buffer_size : 1024;
        if (exact_read > sizeof(read_buffer)) {
            exact_read = sizeof(read_buffer);
        }
        gzread(file, read_buffer, exact_read);
        
        // Test seeking beyond file boundaries
        gzseek(file, 1000000L, SEEK_SET);  // Seek far beyond
        
        // Test with very large buffer request (but limited to actual buffer size)
        gzrewind(file);
        // Request large amount but don't exceed buffer size
        size_t large_request = 1000000;
        if (large_request > sizeof(read_buffer)) {
            large_request = sizeof(read_buffer);
        }
        gzread(file, read_buffer, large_request);  // Request large amount but safe buffer
    }
    // Test sequential operations if requested
    if (test_sequential_ops) {
        gzrewind(file);
        
        // Sequence: read, seek, read, ungetc, read
        int bytes1 = gzread(file, read_buffer, 100);
        if (bytes1 > 0) {
            position = gztell(file);
            gzseek(file, 50, SEEK_CUR);
            int bytes2 = gzread(file, read_buffer, 50);
            if (bytes2 > 0) {
                // Try to ungetc the last character read
                if (bytes2 > 0) {
                    gzungetc(read_buffer[bytes2 - 1], file);
                }
                int bytes3 = gzread(file, read_buffer, 50);
            }
        }
        
        // Check EOF status through the sequence
        int eof_during = gzeof(file);
    }
    
    // Always check for errors at the end
    int errnum;
    const char* errmsg = gzerror(file, &errnum);
    
    // Clear any errors
    gzclearerr(file);
    
    // Close the file
    gzclose(file);
    
    // Clean up temporary file
    unlink(fname);
    
    return 0;
}

} // namespace harness_003
