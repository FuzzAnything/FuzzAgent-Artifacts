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

namespace harness_010 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for advanced gzip testing
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool use_gzdopen = fdp.ConsumeBool();
    bool use_non_blocking = fdp.ConsumeBool();
    bool test_multiple_streams = fdp.ConsumeBool();
    int buffer_size = fdp.ConsumeIntegralInRange<int>(128, 65536);
    int compression_level = fdp.ConsumeIntegralInRange<int>(-1, 9);
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Create primary temporary file
    char primary_filename[] = "/tmp/gz_advanced_XXXXXX";
    int primary_fd = mkstemp(primary_filename);
    if (primary_fd < 0) {
        return 0;
    }
    
    // Create secondary temporary file for multi-file testing
    char secondary_filename[] = "/tmp/gz_secondary_XXXXXX";
    int secondary_fd = mkstemp(secondary_filename);
    if (secondary_fd < 0) {
        close(primary_fd);
        unlink(primary_filename);
        return 0;
    }
    
    // Generate test content for files
    size_t content_size = fdp.ConsumeIntegralInRange<size_t>(128, 4096);
    if (content_size > fdp.remaining_bytes()) {
        content_size = fdp.remaining_bytes();
    }
    
    if (content_size > 0) {
        std::vector<uint8_t> test_content = fdp.ConsumeBytes<uint8_t>(content_size);
        
        // Write compressed data to primary file
        uLongf compr_bound = compressBound(content_size);
        if (compr_bound > 0 && compr_bound <= 10 * 1024 * 1024) {
            Bytef* compr = (Bytef*)malloc(compr_bound);
            if (compr) {
                uLongf compr_len = compr_bound;
                int compress_result = compress(compr, &compr_len, test_content.data(), content_size);
                if (compress_result == Z_OK && compr_len > 0) {
                    write(primary_fd, compr, compr_len);
                    
                    // For multiple streams test, write multiple concatenated gzip streams
                    if (test_multiple_streams) {
                        // Write a second compressed stream
                        uLongf compr_len2 = compr_bound;
                        int compress_result2 = compress(compr, &compr_len2, test_content.data(), 
                                                       content_size > 100 ? 100 : content_size);
                        if (compress_result2 == Z_OK && compr_len2 > 0) {
                            write(primary_fd, compr, compr_len2);
                        }
                    }
                }
                free(compr);
            }
        }
        
        // Write raw data to secondary file for transparent reading test
        write(secondary_fd, test_content.data(), test_content.size());
    }
    
    close(primary_fd);
    close(secondary_fd);
    
    // Test scenario selection
    gzFile gz_file1 = NULL;
    gzFile gz_file2 = NULL;
    const char* mode1 = NULL;
    const char* mode2 = NULL;
    
    switch (test_scenario) {
        case 0: // Read primary, write secondary
            mode1 = "rb";
            mode2 = "wb";
            break;
        case 1: // Write primary, read secondary  
            mode1 = "wb";
            mode2 = "rb";
            break;
        case 2: // Append to primary, read secondary
            mode1 = "ab";
            mode2 = "rb";
            break;
        case 3: // Both read with different modes
            mode1 = "rb";
            mode2 = "rT"; // Read transparent
            break;
        case 4: // Write with compression level
            {
                static char mode_with_level[8];
                if (compression_level >= 0 && compression_level <= 9) {
                    snprintf(mode_with_level, sizeof(mode_with_level), "wb%d", compression_level);
                    mode1 = mode_with_level;
                } else {
                    mode1 = "wb";
                }
                mode2 = "rb";
            }
            break;
        case 5: // Non-blocking mode test
            mode1 = use_non_blocking ? "rbN" : "rb";
            mode2 = use_non_blocking ? "wbN" : "wb";
            break;
        case 6: // Write with strategy
            {
                static char mode_with_strategy[8];
                const char* strategy_char = "f";
                switch (strategy % 4) {
                    case 0: strategy_char = "f"; break; // filtered
                    case 1: strategy_char = "h"; break; // huffman only
                    case 2: strategy_char = "R"; break; // run-length encoding
                    case 3: strategy_char = "F"; break; // fixed code
                }
                snprintf(mode_with_strategy, sizeof(mode_with_strategy), "wb6%s", strategy_char);
                mode1 = mode_with_strategy;
                mode2 = "rb";
            }
            break;
        case 7: // Default
            mode1 = "rb";
            mode2 = "wb";
            break;
    }
    
    // Open first gzip file
    if (use_gzdopen) {
        primary_fd = open(primary_filename, O_RDWR);
        if (primary_fd >= 0) {
            gz_file1 = gzdopen(primary_fd, mode1);
        }
    } else {
        gz_file1 = gzopen(primary_filename, mode1);
    }
    
    if (gz_file1 == NULL) {
        if (use_gzdopen && primary_fd >= 0) {
            close(primary_fd);
        }
        unlink(primary_filename);
        unlink(secondary_filename);
        return 0;
    }
    
    // Open second gzip file
    secondary_fd = open(secondary_filename, O_RDWR);
    if (secondary_fd >= 0) {
        gz_file2 = gzdopen(secondary_fd, mode2);
    }
    
    if (gz_file2 == NULL && secondary_fd >= 0) {
        close(secondary_fd);
        gz_file2 = gzopen(secondary_filename, mode2);
    }
    
    // Set buffer size if requested
    if (fdp.ConsumeBool()) {
        gzbuffer(gz_file1, buffer_size);
        if (gz_file2) {
            gzbuffer(gz_file2, buffer_size);
        }
    }
    
    // Test gzdirect - check if file is in gzip format
    int is_direct1 = gzdirect(gz_file1);
    int is_direct2 = gz_file2 ? gzdirect(gz_file2) : 0;
    
    // Test gzsetparams for write modes
    if (strchr(mode1, 'w') || strchr(mode1, 'a')) {
        if (fdp.ConsumeBool()) {
            gzsetparams(gz_file1, compression_level, strategy);
        }
    }
    
    if (gz_file2 && (strchr(mode2, 'w') || strchr(mode2, 'a'))) {
        if (fdp.ConsumeBool()) {
            int level2 = fdp.ConsumeIntegralInRange<int>(0, 9);
            int strategy2 = fdp.ConsumeIntegralInRange<int>(0, 4);
            gzsetparams(gz_file2, level2, strategy2);
        }
    }
    
    // Perform complex sequence of operations
    size_t num_ops = fdp.ConsumeIntegralInRange<size_t>(1, 20);
    
    for (size_t i = 0; i < num_ops && fdp.remaining_bytes() > 0; i++) {
        uint8_t op_type = fdp.ConsumeIntegral<uint8_t>() % 12;
        
        switch (op_type) {
            case 0: {  // gzread from primary file
                size_t read_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                if (read_len > 0) {
                    std::vector<char> buffer(read_len);
                    int bytes_read = gzread(gz_file1, buffer.data(), read_len);
                    // Check for errors if bytes_read is negative
                    if (bytes_read < 0 && fdp.ConsumeBool()) {
                        int err_num;
                        const char* err_msg = gzerror(gz_file1, &err_num);
                    }
                }
                break;
            }
            
            case 1: {  // gzwrite to primary file (if in write mode)
                size_t write_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                if (write_len > fdp.remaining_bytes()) {
                    write_len = fdp.remaining_bytes();
                }
                if (write_len > 0) {
                    std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_len);
                    int bytes_written = gzwrite(gz_file1, write_data.data(), write_len);
                    // bytes_written is number of bytes written, 0 on error
                }
                break;
            }
            
            case 2: {  // gzread from secondary file
                if (gz_file2) {
                    size_t read_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
                    if (read_len > 0) {
                        std::vector<char> buffer(read_len);
                        int bytes_read = gzread(gz_file2, buffer.data(), read_len);
                    }
                }
                break;
            }
            
            case 3: {  // gzwrite to secondary file
                if (gz_file2) {
                    size_t write_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
                    if (write_len > fdp.remaining_bytes()) {
                        write_len = fdp.remaining_bytes();
                    }
                    if (write_len > 0) {
                        std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_len);
                        int bytes_written = gzwrite(gz_file2, write_data.data(), write_len);
                    }
                }
                break;
            }
            
            case 4: {  // Seek operations
                int seek_whence = fdp.ConsumeIntegralInRange<int>(0, 2);
                long seek_offset = fdp.ConsumeIntegral<long>();
                z_off_t seek_result = gzseek(gz_file1, seek_offset, seek_whence);
                
                // Get current position
                z_off_t tell_result = gztell(gz_file1);
                
                // Test gzrewind
                if (fdp.ConsumeBool()) {
                    gzrewind(gz_file1);
                }
                break;
            }
            
            case 5: {  // Character operations
                if (fdp.ConsumeBool()) {
                    int ch = fdp.ConsumeIntegral<uint8_t>();
                    gzputc(gz_file1, ch);
                } else {
                    int result = gzgetc(gz_file1);
                    if (result != -1 && fdp.ConsumeBool()) {
                        gzungetc(result, gz_file1);
                    }
                }
                break;
            }
            
            case 6: {  // String operations
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                if (str_len > fdp.remaining_bytes()) {
                    str_len = fdp.remaining_bytes();
                }
                if (str_len > 0) {
                    std::string str = fdp.ConsumeBytesAsString(str_len);
                    
                    if (fdp.ConsumeBool()) {
                        // Write string
                        gzputs(gz_file1, str.c_str());
                    } else {
                        // Try to read string (if in read mode)
                        if (str_len < 256) {
                            char buffer[256];
                            char* result = gzgets(gz_file1, buffer, sizeof(buffer));
                        }
                    }
                }
                break;
            }
            
            case 7: {  // Flush operations
                int flush_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                gzflush(gz_file1, flush_type);
                break;
            }
            
            case 8: {  // Check EOF
                int eof_result = gzeof(gz_file1);
                break;
            }
            
            case 9: {  // Error checking
                int err_num;
                const char* err_msg = gzerror(gz_file1, &err_num);
                if (err_num != Z_OK && fdp.ConsumeBool()) {
                    gzclearerr(gz_file1);
                }
                break;
            }
            
            case 10: { // File offset
                z_off_t offset = gzoffset(gz_file1);
                break;
            }
            
            case 11: { // Mixed operations on both files
                if (gz_file2) {
                    // Read from one, write to another
                    size_t transfer_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                    if (transfer_len > 0) {
                        std::vector<char> buffer(transfer_len);
                        int bytes_read = gzread(gz_file1, buffer.data(), transfer_len);
                        if (bytes_read > 0) {
                            gzwrite(gz_file2, buffer.data(), bytes_read);
                        }
                    }
                }
                break;
            }
        }
    }
    
    // Close files
    gzclose(gz_file1);
    if (gz_file2) {
        gzclose(gz_file2);
    } else if (secondary_fd >= 0) {
        close(secondary_fd);
    }
    
    // Clean up temporary files
    unlink(primary_filename);
    unlink(secondary_filename);
    
    return 0;
}

} // namespace harness_010
