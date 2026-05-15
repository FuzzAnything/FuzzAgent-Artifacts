#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_011 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for comprehensive gzip testing
    if (size < 256) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    bool use_64bit_offsets = fdp.ConsumeBool();
    bool test_array_functions = fdp.ConsumeBool();
    bool test_format_strings = fdp.ConsumeBool();
    bool simulate_errors = fdp.ConsumeBool();
    int compression_level = fdp.ConsumeIntegralInRange<int>(-1, 9);
    int buffer_size = fdp.ConsumeIntegralInRange<int>(128, 65536);
    
    // Create multiple temporary files for different test scenarios
    char read_filename[] = "/tmp/gz_read_XXXXXX";
    int read_fd = mkstemp(read_filename);
    if (read_fd < 0) {
        return 0;
    }
    
    char write_filename[] = "/tmp/gz_write_XXXXXX";
    int write_fd = mkstemp(write_filename);
    if (write_fd < 0) {
        close(read_fd);
        unlink(read_filename);
        return 0;
    }
    
    char append_filename[] = "/tmp/gz_append_XXXXXX";
    int append_fd = mkstemp(append_filename);
    if (append_fd < 0) {
        close(read_fd);
        close(write_fd);
        unlink(read_filename);
        unlink(write_filename);
        return 0;
    }
    
    // Generate test content for files
    size_t content_size = fdp.ConsumeIntegralInRange<size_t>(256, 8192);
    if (content_size > fdp.remaining_bytes()) {
        content_size = fdp.remaining_bytes();
    }
    
    if (content_size > 0) {
        std::vector<uint8_t> test_content = fdp.ConsumeBytes<uint8_t>(content_size);
        
        // Write compressed data to read file
        uLongf compr_bound = compressBound(content_size);
        if (compr_bound > 0 && compr_bound <= 10 * 1024 * 1024) {
            Bytef* compr = (Bytef*)malloc(compr_bound);
            if (compr) {
                uLongf compr_len = compr_bound;
                int compress_result = compress(compr, &compr_len, test_content.data(), content_size);
                if (compress_result == Z_OK && compr_len > 0) {
                    write(read_fd, compr, compr_len);
                }
                free(compr);
            }
        }
        
        // Write raw data to write and append files
        write(write_fd, test_content.data(), test_content.size());
        write(append_fd, test_content.data(), test_content.size());
    }
    
    close(read_fd);
    close(write_fd);
    close(append_fd);
    
    // Open files based on scenario
    gzFile gz_read = NULL;
    gzFile gz_write = NULL;
    gzFile gz_append = NULL;
    
    const char* read_mode = "rb";
    const char* write_mode = "wb";
    const char* append_mode = "ab";
    
    // Adjust modes for compression level if specified
    if (compression_level >= 0 && compression_level <= 9) {
        static char write_mode_with_level[8];
        snprintf(write_mode_with_level, sizeof(write_mode_with_level), "wb%d", compression_level);
        write_mode = write_mode_with_level;
    }
    
    // Open files using appropriate functions
    if (use_64bit_offsets) {
        // Use 64-bit offset functions
        gz_read = gzopen64(read_filename, read_mode);
        gz_write = gzopen64(write_filename, write_mode);
        gz_append = gzopen64(append_filename, append_mode);
    } else {
        gz_read = gzopen(read_filename, read_mode);
        gz_write = gzopen(write_filename, write_mode);
        gz_append = gzopen(append_filename, append_mode);
    }
    
    if (!gz_read) {
        unlink(read_filename);
        unlink(write_filename);
        unlink(append_filename);
        return 0;
    }
    
    // Set buffer size
    if (buffer_size > 0) {
        gzbuffer(gz_read, buffer_size);
        if (gz_write) gzbuffer(gz_write, buffer_size);
        if (gz_append) gzbuffer(gz_append, buffer_size);
    }
    
    // Test gzdirect to check file format
    int is_direct_read = gzdirect(gz_read);
    int is_direct_write = gz_write ? gzdirect(gz_write) : 0;
    int is_direct_append = gz_append ? gzdirect(gz_append) : 0;
    
    // Test scenario execution
    switch (test_scenario) {
        case 0: {  // Array function testing (gzfread/gzfwrite)
            if (test_array_functions && gz_write) {
                // Create test data for array operations
                size_t array_size = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                size_t num_items = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                size_t total_bytes = array_size * num_items;
                
                if (total_bytes > 0 && total_bytes <= fdp.remaining_bytes()) {
                    std::vector<char> array_data(total_bytes);
                    memcpy(array_data.data(), fdp.ConsumeBytes<uint8_t>(total_bytes).data(), total_bytes);
                    
                    // Write using gzfwrite
                    z_size_t items_written = gzfwrite(array_data.data(), array_size, num_items, gz_write);
                    
                    // Flush to ensure data is written
                    gzflush(gz_write, Z_SYNC_FLUSH);
                    
                    // Close and reopen for reading
                    gzclose(gz_write);
                    
                    // Reopen in read mode
                    write_fd = open(write_filename, O_RDONLY);
                    if (write_fd >= 0) {
                        gz_write = gzdopen(write_fd, "rb");
                        
                        if (gz_write) {
                            // Read back using gzfread
                            std::vector<char> read_buffer(total_bytes);
                            z_size_t items_read = gzfread(read_buffer.data(), array_size, num_items, gz_write);
                        }
                    }
                }
            }
            break;
        }
        
        case 1: {  // 64-bit offset and position testing
            if (use_64bit_offsets) {
                // Test large offset seeking
                z_off64_t offset = fdp.ConsumeIntegral<z_off64_t>();
                int whence = fdp.ConsumeIntegralInRange<int>(0, 2);
                
                z_off64_t seek_result = gzseek64(gz_read, offset, whence);
                z_off64_t tell_result = gztell64(gz_read);
                z_off64_t offset_result = gzoffset64(gz_read);
                
                // Test rewinding
                if (fdp.ConsumeBool()) {
                    gzrewind(gz_read);
                }
            }
            break;
        }
        
        case 2: {  // Format string testing (gzprintf/gzvprintf)
            if (test_format_strings && gz_write) {
                // Generate format string and arguments
                std::string format_str = fdp.ConsumeRandomLengthString(128);
                int int_arg1 = fdp.ConsumeIntegral<int>();
                int int_arg2 = fdp.ConsumeIntegral<int>();
                double double_arg = fdp.ConsumeFloatingPoint<double>();
                std::string str_arg = fdp.ConsumeRandomLengthString(64);
                
                // Try gzprintf with different argument counts
                int printf_result = 0;
                if (format_str.find("%d") != std::string::npos && 
                    format_str.find("%s") != std::string::npos &&
                    format_str.find("%f") != std::string::npos) {
                    // Complex format string
                    printf_result = gzprintf(gz_write, format_str.c_str(), 
                                           int_arg1, str_arg.c_str(), double_arg, int_arg2);
                } else if (format_str.find("%d") != std::string::npos) {
                    // Simple integer format
                    printf_result = gzprintf(gz_write, format_str.c_str(), int_arg1, int_arg2);
                } else {
                    // Simple string format
                    printf_result = gzprintf(gz_write, format_str.c_str(), str_arg.c_str());
                }
                
                // Flush after writing
                gzflush(gz_write, Z_FULL_FLUSH);
            }
            break;
        }
        
        case 3: {  // Error simulation and recovery testing
            if (simulate_errors) {
                // Perform operations that might cause errors
                size_t invalid_read_size = fdp.ConsumeIntegralInRange<size_t>(1024, 10000);
                std::vector<char> large_buffer(invalid_read_size);
                
                // Attempt to read more than available
                int read_result = gzread(gz_read, large_buffer.data(), invalid_read_size);
                
                // Check for errors
                int err_num;
                const char* err_msg = gzerror(gz_read, &err_num);
                
                if (err_num != Z_OK) {
                    // Clear error and continue
                    gzclearerr(gz_read);
                    
                    // Test EOF detection
                    int eof_status = gzeof(gz_read);
                    
                    // Try to continue with valid operations
                    if (fdp.ConsumeBool()) {
                        // Perform valid small read
                        size_t small_read = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                        if (small_read > 0) {
                            std::vector<char> small_buffer(small_read);
                            gzread(gz_read, small_buffer.data(), small_read);
                        }
                    }
                }
                
                // Test error on write file if available
                if (gz_write) {
                    // Close write file to simulate error
                    gzclose(gz_write);
                    gz_write = NULL;
                    
                    // Try to write to closed file (should handle gracefully)
                    if (fdp.ConsumeBool()) {
                        std::string test_str = "test";
                        gzputs(gz_write, test_str.c_str());
                    }
                }
            }
            break;
        }
        
        case 4: {  // Mixed character and string operations
            // Test character operations
            for (int i = 0; i < fdp.ConsumeIntegralInRange<int>(1, 50) && fdp.remaining_bytes() > 0; i++) {
                if (fdp.ConsumeBool()) {
                    // Write character
                    int ch = fdp.ConsumeIntegral<uint8_t>();
                    if (gz_write) gzputc(gz_write, ch);
                } else {
                    // Read character
                    int result = gzgetc(gz_read);
                    if (result != -1 && fdp.ConsumeBool()) {
                        // Push back character
                        gzungetc(result, gz_read);
                    }
                }
            }
            
            // Test string operations
            size_t num_strings = fdp.ConsumeIntegralInRange<size_t>(1, 10);
            for (size_t i = 0; i < num_strings && fdp.remaining_bytes() > 0; i++) {
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                if (str_len > fdp.remaining_bytes()) {
                    str_len = fdp.remaining_bytes();
                }
                
                if (str_len > 0) {
                    std::string str = fdp.ConsumeBytesAsString(str_len);
                    
                    if (gz_write) {
                        // Write string
                        gzputs(gz_write, str.c_str());
                    }
                    
                    if (fdp.ConsumeBool() && gz_read) {
                        // Try to read string
                        char buffer[512];
                        char* read_result = gzgets(gz_read, buffer, sizeof(buffer));
                    }
                }
            }
            break;
        }
        
        case 5: {  // Complex state transitions and parameter changes
            // Read some data first
            size_t initial_read = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            if (initial_read > 0 && gz_read) {
                std::vector<char> buffer(initial_read);
                gzread(gz_read, buffer.data(), initial_read);
            }
            
            // Change compression parameters on write file
            if (gz_write) {
                int new_level = fdp.ConsumeIntegralInRange<int>(0, 9);
                int new_strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
                gzsetparams(gz_write, new_level, new_strategy);
                
                // Write with new parameters
                size_t write_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                if (write_size > fdp.remaining_bytes()) {
                    write_size = fdp.remaining_bytes();
                }
                
                if (write_size > 0) {
                    std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_size);
                    gzwrite(gz_write, write_data.data(), write_size);
                }
                
                // Flush with different flush types
                int flush_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                gzflush(gz_write, flush_type);
            }
            
            // Test append operations
            if (gz_append) {
                size_t append_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
                if (append_size > fdp.remaining_bytes()) {
                    append_size = fdp.remaining_bytes();
                }
                
                if (append_size > 0) {
                    std::vector<uint8_t> append_data = fdp.ConsumeBytes<uint8_t>(append_size);
                    gzwrite(gz_append, append_data.data(), append_size);
                    
                    // Get file position
                    z_off_t append_pos = gztell(gz_append);
                }
            }
            
            // Final read to check state
            if (gz_read) {
                size_t final_read = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                if (final_read > 0) {
                    std::vector<char> final_buffer(final_read);
                    gzread(gz_read, final_buffer.data(), final_read);
                    
                    // Check EOF
                    int final_eof = gzeof(gz_read);
                }
            }
            break;
        }
    }
    
    // Close all files
    if (gz_read) gzclose(gz_read);
    if (gz_write) gzclose(gz_write);
    if (gz_append) gzclose(gz_append);
    
    // Clean up temporary files
    unlink(read_filename);
    unlink(write_filename);
    unlink(append_filename);
    
    return 0;
}

} // namespace harness_011
