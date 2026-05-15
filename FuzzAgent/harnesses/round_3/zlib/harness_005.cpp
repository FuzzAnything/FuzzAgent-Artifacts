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

namespace harness_005 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for comprehensive separate read/write gzip testing
    if (size < 256) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // PHASE 1: READING OPERATIONS (Separate file lifecycle)
    // ------------------------------------------------------
    
    // Create first temporary file for reading operations
    char read_filename[] = "/tmp/gz_read_fuzz_XXXXXX";
    int read_fd = mkstemp(read_filename);
    if (read_fd < 0) {
        return 0;
    }
    close(read_fd);
    
    // Write some compressed data to the read file
    size_t compress_size = fdp.ConsumeIntegralInRange<size_t>(128, 4096);
    if (fdp.remaining_bytes() < compress_size) {
        unlink(read_filename);
        return 0;
    }
    
    std::vector<uint8_t> compress_data = fdp.ConsumeBytes<uint8_t>(compress_size);
    
    // Write compressed data to read file
    FILE* read_f = fopen(read_filename, "wb");
    if (read_f) {
        fwrite(compress_data.data(), 1, compress_data.size(), read_f);
        fclose(read_f);
    }
    
    // Open the gzip file for reading
    gzFile gz_read_file = gzopen(read_filename, "rb");
    if (gz_read_file == NULL) {
        unlink(read_filename);
        return 0;
    }
    
    // Perform read operations
    size_t read_ops = fdp.ConsumeIntegralInRange<size_t>(1, 15);
    
    for (size_t i = 0; i < read_ops && fdp.remaining_bytes() > 0; i++) {
        uint8_t read_op = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (read_op) {
            case 0: {  // gzread - read binary data
                size_t read_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                if (read_size > 0) {
                    std::vector<uint8_t> buffer(read_size);
                    int bytes_read = gzread(gz_read_file, buffer.data(), static_cast<unsigned>(read_size));
                    // bytes_read is number of bytes read, 0 for EOF or error
                    
                    // Check for errors
                    if (bytes_read < 0) {
                        int err_num;
                        const char* error_msg = gzerror(gz_read_file, &err_num);
                        // error_msg contains error description
                    }
                }
                break;
            }
            
            case 1: {  // gzgets - read string line
                size_t buf_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
                if (buf_size > 0) {
                    std::vector<char> buffer(buf_size);
                    char* result = gzgets(gz_read_file, buffer.data(), static_cast<int>(buf_size));
                    // result is buffer on success, NULL on error or EOF
                }
                break;
            }
            
            case 2: {  // gzgetc - read single character
                int ch = gzgetc(gz_read_file);
                // ch is character read on success, -1 on error or EOF
                
                // Test gzungetc - push character back if we read successfully
                if (ch != -1 && fdp.remaining_bytes() >= 1) {
                    // Optionally push back different character
                    uint8_t use_original = fdp.ConsumeBool();
                    if (use_original) {
                        gzungetc(ch, gz_read_file);
                    } else {
                        char new_ch = fdp.ConsumeIntegral<char>();
                        gzungetc(new_ch, gz_read_file);
                    }
                }
                break;
            }
            
            case 3: {  // gzungetc with fuzzed character
                if (fdp.remaining_bytes() >= 1) {
                    char ch = fdp.ConsumeIntegral<char>();
                    int unget_result = gzungetc(ch, gz_read_file);
                    // unget_result is character pushed back on success, -1 on error
                }
                break;
            }
        }
        
        // Test gzeof - check for end of file
        int at_eof = gzeof(gz_read_file);
        // at_eof returns 1 if at end of file, 0 otherwise
        
        // Periodically check for errors
        if (i % 3 == 0) {
            int err_num;
            const char* error_msg = gzerror(gz_read_file, &err_num);
            // error_msg contains error description, err_num contains error code
            
            // Clear errors if any
            if (err_num != Z_OK) {
                gzclearerr(gz_read_file);
            }
        }
    }
    
    // Final error check for read file
    int read_err_num;
    gzerror(gz_read_file, &read_err_num);
    
    // Close read file with gzclose_r
    int close_read_result = gzclose_r(gz_read_file);
    
    // Clean up read temporary file
    unlink(read_filename);
    
    // PHASE 2: WRITING OPERATIONS (Separate file lifecycle)
    // ------------------------------------------------------
    
    // Create second temporary file for writing operations
    char write_filename[] = "/tmp/gz_write_fuzz_XXXXXX";
    int write_fd = mkstemp(write_filename);
    if (write_fd < 0) {
        return 0;
    }
    close(write_fd);
    
    // Determine write mode with compression level
    std::string write_mode = "wb";
    int write_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    write_mode += std::to_string(write_level);
    
    // Optional: add compression strategy
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
        write_mode += strategy_char;
    }
    
    // Open the gzip file for writing
    gzFile gz_write_file = gzopen(write_filename, write_mode.c_str());
    if (gz_write_file == NULL) {
        unlink(write_filename);
        return 0;
    }
    
    // Set compression parameters with gzsetparams (optional modification)
    uint8_t set_params = fdp.ConsumeBool();
    if (set_params && fdp.remaining_bytes() >= 2) {
        int param_level = fdp.ConsumeIntegralInRange<int>(0, 9);
        int param_strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
        gzsetparams(gz_write_file, param_level, param_strategy);
    }
    
    // Perform write operations
    size_t write_ops = fdp.ConsumeIntegralInRange<size_t>(1, 15);
    
    for (size_t i = 0; i < write_ops && fdp.remaining_bytes() > 0; i++) {
        uint8_t write_op = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (write_op) {
            case 0: {  // gzwrite - write binary data
                size_t write_size = fdp.ConsumeIntegralInRange<size_t>(1, 2048);
                if (write_size > 0 && fdp.remaining_bytes() >= write_size) {
                    std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(write_size);
                    int bytes_written = gzwrite(gz_write_file, buffer.data(), static_cast<unsigned>(write_size));
                    // bytes_written is number of bytes written, 0 on error
                }
                break;
            }
            
            case 1: {  // gzputs - write string
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
                if (str_len > 0 && fdp.remaining_bytes() >= str_len) {
                    std::string str = fdp.ConsumeBytesAsString(str_len);
                    // Ensure null termination for string functions
                    if (!str.empty()) {
                        int result = gzputs(gz_write_file, str.c_str());
                        // result is non-negative on success, -1 on error
                    }
                }
                break;
            }
            
            case 2: {  // gzprintf - formatted output
                if (fdp.remaining_bytes() >= 20) {
                    // Choose format string type
                    uint8_t fmt_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                    
                    if (fmt_type == 0) {
                        // Simple string format
                        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                        if (fdp.remaining_bytes() >= str_len) {
                            std::string str_val = fdp.ConsumeBytesAsString(str_len);
                            int chars_printed = gzprintf(gz_write_file, "String: %s", str_val.c_str());
                            // chars_printed is number of characters printed, negative on error
                        }
                    } else if (fmt_type == 1) {
                        // Integer format
                        int int_val = fdp.ConsumeIntegral<int>();
                        unsigned int uint_val = fdp.ConsumeIntegral<unsigned int>();
                        int chars_printed = gzprintf(gz_write_file, "int=%d uint=%u", int_val, uint_val);
                    } else if (fmt_type == 2) {
                        // Floating point format
                        float float_val = fdp.ConsumeFloatingPoint<float>();
                        double double_val = fdp.ConsumeFloatingPoint<double>();
                        int chars_printed = gzprintf(gz_write_file, "float=%.3f double=%.6f", float_val, double_val);
                    } else {
                        // Mixed format
                        int int_val = fdp.ConsumeIntegral<int>();
                        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                        if (fdp.remaining_bytes() >= str_len) {
                            std::string str_val = fdp.ConsumeBytesAsString(str_len);
                            float float_val = fdp.ConsumeFloatingPoint<float>();
                            int chars_printed = gzprintf(gz_write_file, "str=%s int=%d float=%.2f", 
                                                         str_val.c_str(), int_val, float_val);
                        }
                    }
                }
                break;
            }
            
            case 3: {  // gzputc - write single character
                if (fdp.remaining_bytes() >= 1) {
                    char ch = fdp.ConsumeIntegral<char>();
                    int result = gzputc(gz_write_file, ch);
                    // result is character written on success, -1 on error
                }
                break;
            }
        }
        
        // Periodically flush and check for errors
        if (i % 4 == 0) {
            // Test gzflush
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
                
                int flush_result = gzflush(gz_write_file, flush_val);
                // flush_result is Z_OK on success, Z_STREAM_ERROR if not writing,
                // Z_BUF_ERROR if no progress is possible
            }
            
            // Check for errors
            int err_num;
            const char* error_msg = gzerror(gz_write_file, &err_num);
            // error_msg contains error description, err_num contains error code
            
            // Clear errors if any
            if (err_num != Z_OK) {
                gzclearerr(gz_write_file);
            }
        }
    }
    
    // Test gzdirect() - check if file is in direct (raw) mode
    int is_direct = gzdirect(gz_write_file);
    // is_direct returns 1 if file is in direct mode, 0 if not
    
    // Final error check for write file
    int write_err_num;
    gzerror(gz_write_file, &write_err_num);
    
    // Close write file with gzclose_w
    int close_write_result = gzclose_w(gz_write_file);
    
    // Clean up write temporary file
    unlink(write_filename);
    
    return 0;
}

} // namespace harness_005
