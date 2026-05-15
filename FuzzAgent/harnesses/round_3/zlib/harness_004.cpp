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

namespace harness_004 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for comprehensive gzip I/O testing
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Create a temporary file for gzip operations
    char temp_filename[] = "/tmp/gz_comprehensive_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        return 0;
    }
    close(fd);

    // Determine operation mode from fuzzer input
    // 0: read-only, 1: write-only, 2: append, 3: mixed read/write
    uint8_t mode_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Build mode string for gzopen
    std::string mode_str;
    char primary_mode;
    
    switch (mode_type) {
        case 0: primary_mode = 'r'; break;  // read
        case 1: primary_mode = 'w'; break;  // write
        case 2: primary_mode = 'a'; break;  // append
        case 3: primary_mode = 'r'; break;  // start with read, but we'll handle mixed
        default: primary_mode = 'r'; break;
    }
    
    mode_str += primary_mode;
    mode_str += 'b';  // binary mode
    
    // Add compression level for write/append modes
    if (primary_mode == 'w' || primary_mode == 'a') {
        uint8_t add_level = fdp.ConsumeBool();
        if (add_level) {
            int level = fdp.ConsumeIntegralInRange<int>(0, 9);
            mode_str += std::to_string(level);
            
            // Optional: add strategy
            uint8_t add_strategy = fdp.ConsumeBool();
            if (add_strategy) {
                char strategy_char = 'f';  // filtered
                uint8_t strategy_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (strategy_choice) {
                    case 0: strategy_char = 'f'; break;  // filtered
                    case 1: strategy_char = 'h'; break;  // huffman only
                    case 2: strategy_char = 'R'; break;  // run-length encoding
                    case 3: strategy_char = 'F'; break;  // fixed code
                }
                mode_str += strategy_char;
            }
        }
    }

    // For read mode, we need to write some gzip-compressed data to the file first
    if (primary_mode == 'r' || mode_type == 3) {
        // Create some compressed data to read
        size_t compress_size = fdp.ConsumeIntegralInRange<size_t>(64, 2048);
        if (fdp.remaining_bytes() < compress_size) {
            unlink(temp_filename);
            return 0;
        }
        
        std::vector<uint8_t> compress_data = fdp.ConsumeBytes<uint8_t>(compress_size);
        
        // Write compressed data to temp file
        FILE* f = fopen(temp_filename, "wb");
        if (f) {
            fwrite(compress_data.data(), 1, compress_data.size(), f);
            fclose(f);
        }
    }

    // Open the gzip file
    gzFile gz_file = gzopen(temp_filename, mode_str.c_str());
    if (gz_file == NULL) {
        unlink(temp_filename);
        return 0;
    }

    // Test gzdirect() - check if file is in direct (raw) mode
    int is_direct = gzdirect(gz_file);
    // is_direct returns 1 if file is in direct mode, 0 if not

    // Optional: set compression parameters with gzsetparams
    uint8_t set_params = fdp.ConsumeBool();
    if (set_params) {
        int level = fdp.ConsumeIntegralInRange<int>(0, 9);
        int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
        gzsetparams(gz_file, level, strategy);
    }

    // Determine number of operations to perform
    size_t num_ops = fdp.ConsumeIntegralInRange<size_t>(1, 20);
    
    // Track if we're currently in a state suitable for reading/writing
    bool can_read = (primary_mode == 'r' || mode_type == 3);
    bool can_write = (primary_mode == 'w' || primary_mode == 'a' || mode_type == 3);
    
    // For mixed mode, we might switch between reading and writing
    // In practice, mixing read/write on the same gzFile is complex and
    // may require seeks or reopening. We'll handle this carefully.
    
    for (size_t i = 0; i < num_ops && fdp.remaining_bytes() > 0; i++) {
        // For mixed mode, decide whether to do read or write operation
        uint8_t op_category;
        if (mode_type == 3) {  // mixed mode
            op_category = fdp.ConsumeBool() ? 0 : 1;  // 0 for read, 1 for write
        } else {
            op_category = (primary_mode == 'r') ? 0 : 1;
        }
        
        if (op_category == 0 && can_read) {
            // READING OPERATIONS
            uint8_t read_op = fdp.ConsumeIntegral<uint8_t>() % 5;
            
            switch (read_op) {
                case 0: {  // gzread - read binary data
                    size_t read_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
                    if (read_size > 0) {
                        std::vector<uint8_t> buffer(read_size);
                        int bytes_read = gzread(gz_file, buffer.data(), static_cast<unsigned>(read_size));
                        // bytes_read is number of bytes read, 0 for EOF or error
                        
                        // Check for errors
                        if (bytes_read < 0) {
                            const char* error_msg = gzerror(gz_file, NULL);
                            // error_msg contains error description
                        }
                    }
                    break;
                }
                
                case 1: {  // gzfread - formatted read
                    size_t item_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                    size_t num_items = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                    size_t total_size = item_size * num_items;
                    
                    if (total_size > 0 && total_size <= 4096) {
                        std::vector<uint8_t> buffer(total_size);
                        size_t items_read = gzfread(buffer.data(), item_size, num_items, gz_file);
                        // items_read is number of full items read
                    }
                    break;
                }
                
                case 2: {  // gzgets - read string line
                    size_t buf_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                    if (buf_size > 0) {
                        std::vector<char> buffer(buf_size);
                        char* result = gzgets(gz_file, buffer.data(), static_cast<int>(buf_size));
                        // result is buffer on success, NULL on error or EOF
                    }
                    break;
                }
                
                case 3: {  // gzgetc - read single character
                    int ch = gzgetc(gz_file);
                    // ch is character read on success, -1 on error or EOF
                    
                    // Test gzungetc - push character back
                    if (ch != -1) {
                        int unget_result = gzungetc(ch, gz_file);
                        // unget_result is character pushed back on success, -1 on error
                    }
                    break;
                }
                
                case 4: {  // gzungetc with fuzzed character
                    if (fdp.remaining_bytes() >= 1) {
                        char ch = fdp.ConsumeIntegral<char>();
                        int unget_result = gzungetc(ch, gz_file);
                        // unget_result is character pushed back on success, -1 on error
                    }
                    break;
                }
            }
            
            // Test gzeof - check for end of file
            int at_eof = gzeof(gz_file);
            // at_eof returns 1 if at end of file, 0 otherwise
            
        } else if (op_category == 1 && can_write) {
            // WRITING OPERATIONS
            uint8_t write_op = fdp.ConsumeIntegral<uint8_t>() % 5;
            
            switch (write_op) {
                case 0: {  // gzwrite - write binary data
                    size_t write_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
                    if (write_size > 0 && fdp.remaining_bytes() >= write_size) {
                        std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(write_size);
                        int bytes_written = gzwrite(gz_file, buffer.data(), static_cast<unsigned>(write_size));
                        // bytes_written is number of bytes written, 0 on error
                    }
                    break;
                }
                
                case 1: {  // gzfwrite - formatted write
                    size_t item_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                    size_t num_items = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                    size_t total_size = item_size * num_items;
                    
                    if (total_size > 0 && total_size <= 4096 && fdp.remaining_bytes() >= total_size) {
                        std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(total_size);
                        size_t items_written = gzfwrite(buffer.data(), item_size, num_items, gz_file);
                        // items_written is number of full items written
                    }
                    break;
                }
                
                case 2: {  // gzputs - write string
                    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                    if (str_len > 0 && fdp.remaining_bytes() >= str_len) {
                        std::string str = fdp.ConsumeBytesAsString(str_len);
                        // Ensure null termination for string functions
                        if (!str.empty()) {
                            int result = gzputs(gz_file, str.c_str());
                            // result is non-negative on success, -1 on error
                        }
                    }
                    break;
                }
                
                case 3: {  // gzputc - write single character
                    if (fdp.remaining_bytes() >= 1) {
                        char ch = fdp.ConsumeIntegral<char>();
                        int result = gzputc(gz_file, ch);
                        // result is character written on success, -1 on error
                    }
                    break;
                }
                
                case 4: {  // gzprintf - formatted output
                    // Create format string with different specifiers
                    if (fdp.remaining_bytes() >= 20) {
                        // Choose format string type
                        uint8_t fmt_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                        
                        if (fmt_type == 0) {
                            // Simple string format
                            size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                            if (fdp.remaining_bytes() >= str_len) {
                                std::string str_val = fdp.ConsumeBytesAsString(str_len);
                                int chars_printed = gzprintf(gz_file, "%s", str_val.c_str());
                                // chars_printed is number of characters printed, negative on error
                            }
                        } else if (fmt_type == 1) {
                            // Integer format
                            int int_val = fdp.ConsumeIntegral<int>();
                            float float_val = fdp.ConsumeFloatingPoint<float>();
                            int chars_printed = gzprintf(gz_file, "int=%d float=%.2f", int_val, float_val);
                        } else {
                            // Mixed format
                            int int_val = fdp.ConsumeIntegral<int>();
                            size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                            if (fdp.remaining_bytes() >= str_len) {
                                std::string str_val = fdp.ConsumeBytesAsString(str_len);
                                int chars_printed = gzprintf(gz_file, "str=%s int=%d", str_val.c_str(), int_val);
                            }
                        }
                    }
                    break;
                }
            }
            
            // Test gzflush - flush output buffer
            uint8_t do_flush = fdp.ConsumeBool();
            if (do_flush) {
                int flush_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                int flush_val = Z_NO_FLUSH;
                switch (flush_type) {
                    case 0: flush_val = Z_NO_FLUSH; break;
                    case 1: flush_val = Z_SYNC_FLUSH; break;
                    case 2: flush_val = Z_FULL_FLUSH; break;
                    case 3: flush_val = Z_FINISH; break;
                }
                
                int flush_result = gzflush(gz_file, flush_val);
                // flush_result is Z_OK on success, Z_STREAM_ERROR if not writing,
                // Z_BUF_ERROR if no progress is possible
            }
        }
        
        // Periodically check for errors and clear them
        if (i % 3 == 0) {
            // Check for errors with gzerror
            int err_num;
            const char* error_msg = gzerror(gz_file, &err_num);
            // error_msg contains error description, err_num contains error code
            
            // Clear errors if any
            if (err_num != Z_OK) {
                gzclearerr(gz_file);
            }
        }
    }

    // Final error check
    int final_err_num;
    gzerror(gz_file, &final_err_num);
    
    // Close the gzip file based on mode
    int close_result;
    if (primary_mode == 'r' || (mode_type == 3 && !can_write)) {
        close_result = gzclose_r(gz_file);
    } else {
        close_result = gzclose_w(gz_file);
    }
    
    // Clean up temporary file
    unlink(temp_filename);
    
    return 0;
}

} // namespace harness_004
