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

namespace harness_012 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for comprehensive gzip API testing
    if (size < 256) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool use_gzdopen = fdp.ConsumeBool();
    bool test_error_handling = fdp.ConsumeBool();
    bool test_formatted_output = fdp.ConsumeBool();
    bool test_direct_io = fdp.ConsumeBool();
    int buffer_size = fdp.ConsumeIntegralInRange<int>(128, 65536);
    int compression_level = fdp.ConsumeIntegralInRange<int>(-1, 9);
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Create temporary files for testing
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
        
        // Write compressed data to read file for gzread testing
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
    
    // Open files with different methods
    gzFile gz_read = NULL;
    gzFile gz_write = NULL;
    gzFile gz_append = NULL;
    
    if (use_gzdopen) {
        // Re-open file descriptors for gzdopen
        read_fd = open(read_filename, O_RDONLY);
        write_fd = open(write_filename, O_WRONLY);
        append_fd = open(append_filename, O_WRONLY | O_APPEND);
        
        if (read_fd >= 0) gz_read = gzdopen(read_fd, "rb");
        if (write_fd >= 0) gz_write = gzdopen(write_fd, "wb");
        if (append_fd >= 0) gz_append = gzdopen(append_fd, "ab");
    } else {
        gz_read = gzopen(read_filename, "rb");
        gz_write = gzopen(write_filename, "wb");
        gz_append = gzopen(append_filename, "ab");
    }
    
    // Set buffer size if requested
    if (gz_read && buffer_size > 0) {
        gzbuffer(gz_read, buffer_size);
    }
    if (gz_write && buffer_size > 0) {
        gzbuffer(gz_write, buffer_size);
    }
    
    // Test scenario selection
    switch (test_scenario) {
        case 0: {  // Comprehensive read operations
            if (gz_read) {
                // Test gzfread for structured reading
                size_t item_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                size_t num_items = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                if (item_size * num_items > 0 && item_size * num_items <= 65536) {
                    std::vector<char> buffer(item_size * num_items);
                    size_t items_read = gzfread(buffer.data(), item_size, num_items, gz_read);
                }
                
                // Test gzgetc and gzungetc
                int ch = gzgetc(gz_read);
                if (ch != -1) {
                    gzungetc(ch, gz_read);
                }
                
                // Test gzdirect
                int is_direct = gzdirect(gz_read);
                
                // Test gzoffset
                z_off_t offset = gzoffset(gz_read);
                
                // Test gzseek with different whence values
                if (offset > 0) {
                    z_off_t seek_pos = fdp.ConsumeIntegralInRange<z_off_t>(0, offset);
                    int whence = fdp.ConsumeIntegralInRange<int>(0, 2);
                    gzseek(gz_read, seek_pos, whence);
                }
                
                // Test gzrewind
                gzrewind(gz_read);
            }
            break;
        }
        
        case 1: {  // Comprehensive write operations
            if (gz_write) {
                // Test gzfwrite for structured writing
                size_t item_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                size_t num_items = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                if (item_size * num_items > 0 && item_size * num_items <= 65536) {
                    std::vector<char> buffer(item_size * num_items);
                    // Fill with some data
                    for (size_t i = 0; i < buffer.size() && fdp.remaining_bytes() > 0; i++) {
                        buffer[i] = fdp.ConsumeIntegral<char>();
                    }
                    size_t items_written = gzfwrite(buffer.data(), item_size, num_items, gz_write);
                }
                
                // Test gzputc
                int char_to_write = fdp.ConsumeIntegral<char>();
                gzputc(gz_write, char_to_write);
                
                // Test formatted output if enabled
                if (test_formatted_output && fdp.remaining_bytes() >= 100) {
                    int int_val = fdp.ConsumeIntegral<int>();
                    double double_val = fdp.ConsumeFloatingPoint<double>();
                    std::string str_val = fdp.ConsumeRandomLengthString(50);
                    
                    gzprintf(gz_write, "int=%d, double=%.2f, str=%s", int_val, double_val, str_val.c_str());
                }
                
                // Test gzflush with different flush types
                int flush_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                gzflush(gz_write, flush_type);
            }
            break;
        }
        
        case 2: {  // Error handling and status testing
            // Test gzerror and gzclearerr
            if (gz_read) {
                int errnum = 0;
                const char* error_msg = gzerror(gz_read, &errnum);
                
                // Clear any error
                gzclearerr(gz_read);
                
                // Test gzeof after reading to end
                if (gz_read) {
                    // Try to read past end
                    char buffer[1024];
                    while (gzread(gz_read, buffer, sizeof(buffer)) > 0) {
                        // Read until EOF
                    }
                    int eof_status = gzeof(gz_read);
                }
            }
            break;
        }
        
        case 3: {  // Mixed read-write operations (requires rb+ mode)
            // Re-open read file in read-write mode
            if (gz_read) gzclose(gz_read);
            gz_read = gzopen(read_filename, "rb+");
            
            if (gz_read) {
                // Read some data
                size_t read_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
                if (read_size > 0) {
                    std::vector<char> buffer(read_size);
                    gzread(gz_read, buffer.data(), read_size);
                    
                    // Seek and write
                    z_off_t seek_pos = fdp.ConsumeIntegralInRange<z_off_t>(0, 1000);
                    gzseek(gz_read, seek_pos, SEEK_SET);
                    
                    size_t write_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                    if (write_size > 0 && write_size <= fdp.remaining_bytes()) {
                        std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_size);
                        gzwrite(gz_read, write_data.data(), write_size);
                    }
                    
                    // Get current position
                    z_off_t current_pos = gztell(gz_read);
                }
            }
            break;
        }
        
        case 4: {  // String operations
            if (gz_read && gz_write) {
                // Test gzgets with various buffer sizes
                size_t num_strings = fdp.ConsumeIntegralInRange<size_t>(1, 20);
                for (size_t i = 0; i < num_strings && fdp.remaining_bytes() > 0; i++) {
                    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                    if (str_len > fdp.remaining_bytes()) {
                        str_len = fdp.remaining_bytes();
                    }
                    
                    if (str_len > 0) {
                        std::string str = fdp.ConsumeBytesAsString(str_len);
                        
                        // Write string
                        gzputs(gz_write, str.c_str());
                        
                        // Try to read string (if we have data in read file)
                        char buffer[512];
                        char* read_result = gzgets(gz_read, buffer, sizeof(buffer));
                    }
                }
            }
            break;
        }
        
        case 5: {  // Compression parameter changes
            if (gz_write) {
                // Change compression parameters
                int new_level = fdp.ConsumeIntegralInRange<int>(-1, 9);
                int new_strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
                gzsetparams(gz_write, new_level, new_strategy);
                
                // Write data with new parameters
                size_t write_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                if (write_size > fdp.remaining_bytes()) {
                    write_size = fdp.remaining_bytes();
                }
                
                if (write_size > 0) {
                    std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_size);
                    gzwrite(gz_write, write_data.data(), write_size);
                }
                
                // Test gzclose_w specifically
                if (test_error_handling) {
                    // Note: gzclose_w is called internally by gzclose
                    // We'll just call gzclose which should handle it
                }
            }
            break;
        }
        
        case 6: {  // Direct I/O testing
            if (gz_read) {
                // Test gzdirect
                int direct_status = gzdirect(gz_read);
                
                if (test_direct_io && direct_status) {
                    // If file is direct, test raw read operations
                    char buffer[1024];
                    gzread(gz_read, buffer, sizeof(buffer));
                }
            }
            break;
        }
        
        case 7: {  // Complex file position operations
            if (gz_read) {
                // Test gzseek with all whence values
                z_off_t current_pos = gztell(gz_read);
                
                // SEEK_SET
                z_off_t seek_set_pos = fdp.ConsumeIntegralInRange<z_off_t>(0, 1000);
                gzseek(gz_read, seek_set_pos, SEEK_SET);
                
                // SEEK_CUR
                z_off_t seek_cur_offset = fdp.ConsumeIntegralInRange<z_off_t>(-500, 500);
                gzseek(gz_read, seek_cur_offset, SEEK_CUR);
                
                // SEEK_END (requires knowing file size)
                // We'll just test with small negative offset
                gzseek(gz_read, -100, SEEK_END);
                
                // Get final position
                z_off_t final_pos = gztell(gz_read);
                
                // Test gzoffset
                z_off_t file_offset = gzoffset(gz_read);
            }
            break;
        }
    }
    
    // Close all files with appropriate close functions
    if (gz_read) {
        if (test_error_handling) {
            // Test gzclose_r specifically
            gzclose_r(gz_read);
        } else {
            gzclose(gz_read);
        }
    }
    
    if (gz_write) {
        if (test_error_handling) {
            // Test gzclose_w specifically
            gzclose_w(gz_write);
        } else {
            gzclose(gz_write);
        }
    }
    
    if (gz_append) {
        gzclose(gz_append);
    }
    
    // Clean up file descriptors if using gzdopen
    if (use_gzdopen) {
        if (read_fd >= 0) close(read_fd);
        if (write_fd >= 0) close(write_fd);
        if (append_fd >= 0) close(append_fd);
    }
    
    // Clean up temporary files
    unlink(read_filename);
    unlink(write_filename);
    unlink(append_filename);
    
    return 0;
}

} // namespace harness_012
