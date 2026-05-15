#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_004 {
/*
 * Fuzzing harness for advanced gzip formatted I/O with dynamic parameter control in zlib
 * Target: gzip format operations with dynamic parameter control during streaming,
 * combining aspects of gzip I/O and parameter management.
 * 
 * This harness focuses on testing advanced gzip I/O with runtime parameter adjustment,
 * which is a critical coverage gap with 1400+ cumulative undiscovered branches.
 * Different from existing harnesses:
 * - harness_000 (basic compression/decompression)
 * - harness_001 (gzip file I/O operations)
 * - harness_002 (inflateBack callback-based decompression) 
 * - harness_003 (deflate parameter control)
 * 
 * This harness combines gzip file I/O with dynamic parameter control via gzsetparams,
 * testing parameter changes during active compression/decompression streams.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4); // Z_DEFAULT_STRATEGY to Z_FIXED
    bool test_dynamic_params = fdp.ConsumeBool();
    bool test_buffer_size = fdp.ConsumeBool();
    bool test_flush_operations = fdp.ConsumeBool();
    bool test_seek_operations = fdp.ConsumeBool();
    bool test_mixed_io = fdp.ConsumeBool();
    bool test_error_recovery = fdp.ConsumeBool();
    
    // Determine gzip mode from fuzzer input
    std::string mode = "wb";
    uint8_t mode_selector = fdp.ConsumeIntegral<uint8_t>() % 6;
    switch (mode_selector) {
        case 0: mode = "rb"; break;     // Read mode
        case 1: mode = "wb"; break;     // Write mode  
        case 2: mode = "ab"; break;     // Append mode
        case 3: mode = "wb6"; break;    // Write with compression level 6
        case 4: mode = "wT"; break;     // Write transparent (no compression)
        case 5: mode = "wb9f"; break;   // Write with full flush
    }

    // Create a temporary file for gzip operations
    char temp_filename[] = "/tmp/gzip_param_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd == -1) {
        return 0;
    }
    close(fd);

    // Open gzip file
    gzFile gzfile = gzopen(temp_filename, mode.c_str());
    if (gzfile == NULL) {
        // Try with a default mode if opening failed
        mode = "wb";
        gzfile = gzopen(temp_filename, mode.c_str());
        if (gzfile == NULL) {
            remove(temp_filename);
            return 0;
        }
    }

    // Set buffer size if requested
    if (test_buffer_size && (mode[0] == 'w' || mode[0] == 'a')) {
        unsigned buffer_size = fdp.ConsumeIntegralInRange<unsigned>(1024, 65536);
        gzbuffer(gzfile, buffer_size);
    }

    // Determine number of operations to perform
    uint8_t num_operations = fdp.ConsumeIntegral<uint8_t>() % 32 + 1;
    
    // Track compression parameters for dynamic changes
    int current_level = compression_level;
    int current_strategy = strategy;
    
    // Track file position for seek operations
    z_off_t file_position = 0;
    
    for (uint8_t i = 0; i < num_operations && fdp.remaining_bytes() > 0; i++) {
        // Choose operation type based on mode and remaining input
        uint8_t operation;
        if (mode[0] == 'r') {
            // Reading operations
            operation = fdp.ConsumeIntegral<uint8_t>() % 8;
        } else {
            // Writing operations
            operation = fdp.ConsumeIntegral<uint8_t>() % 12;
        }

        switch (operation) {
            case 0: {  // Dynamic parameter change during operation
                if (test_dynamic_params && (mode[0] == 'w' || mode[0] == 'a')) {
                    // Change compression parameters on the fly
                    int new_level = fdp.ConsumeIntegralInRange<int>(0, 9);
                    int new_strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
                    
                    int result = gzsetparams(gzfile, new_level, new_strategy);
                    if (result == Z_OK) {
                        current_level = new_level;
                        current_strategy = new_strategy;
                    }
                }
                break;
            }
            
            case 1: {  // gzputc - write a single character
                if (mode[0] == 'w' || mode[0] == 'a') {
                    char c = fdp.ConsumeIntegral<uint8_t>();
                    gzputc(gzfile, c);
                    file_position++;
                }
                break;
            }
            
            case 2: {  // gzputs - write a string
                if (mode[0] == 'w' || mode[0] == 'a') {
                    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                    if (fdp.remaining_bytes() < str_len) {
                        break;
                    }
                    std::string str = fdp.ConsumeRandomLengthString(str_len);
                    gzputs(gzfile, str.c_str());
                    file_position += str.length();
                }
                break;
            }
            
            case 3: {  // gzwrite - write binary data
                if (mode[0] == 'w' || mode[0] == 'a') {
                    size_t write_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                    if (fdp.remaining_bytes() < write_len) {
                        break;
                    }
                    std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(write_len);
                    gzwrite(gzfile, buffer.data(), buffer.size());
                    file_position += write_len;
                }
                break;
            }
            
            case 4: {  // gzread - read data
                if (mode[0] == 'r') {
                    size_t read_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                    std::vector<uint8_t> buffer(read_len);
                    int bytes_read = gzread(gzfile, buffer.data(), read_len);
                    if (bytes_read > 0) {
                        file_position += bytes_read;
                    }
                }
                break;
            }
            
            case 5: {  // gzgetc - read a single character
                if (mode[0] == 'r') {
                    int c = gzgetc(gzfile);
                    if (c != -1) {
                        file_position++;
                    }
                }
                break;
            }
            
            case 6: {  // gzgets - read a string
                if (mode[0] == 'r') {
                    size_t buf_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                    std::vector<char> buffer(buf_len);
                    char* result = gzgets(gzfile, buffer.data(), buf_len);
                    if (result != NULL) {
                        file_position += strlen(buffer.data());
                    }
                }
                break;
            }
            
            case 7: {  // Flush operations
                if (test_flush_operations && (mode[0] == 'w' || mode[0] == 'a')) {
                    int flush_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                    int flush_param;
                    switch (flush_type) {
                        case 0: flush_param = Z_NO_FLUSH; break;
                        case 1: flush_param = Z_SYNC_FLUSH; break;
                        case 2: flush_param = Z_FULL_FLUSH; break;
                        case 3: flush_param = Z_FINISH; break;
                        default: flush_param = Z_NO_FLUSH; break;
                    }
                    gzflush(gzfile, flush_param);
                }
                break;
            }
            
            case 8: {  // Seek operations
                if (test_seek_operations) {
                    int seek_whence = fdp.ConsumeIntegral<uint8_t>() % 3;
                    z_off_t seek_offset;
                    
                    switch (seek_whence) {
                        case 0: // SEEK_SET
                            seek_offset = fdp.ConsumeIntegralInRange<z_off_t>(0, file_position + 1024);
                            gzseek(gzfile, seek_offset, SEEK_SET);
                            file_position = seek_offset;
                            break;
                        case 1: // SEEK_CUR
                            seek_offset = fdp.ConsumeIntegralInRange<z_off_t>(-1024, 1024);
                            gzseek(gzfile, seek_offset, SEEK_CUR);
                            file_position += seek_offset;
                            if (file_position < 0) file_position = 0;
                            break;
                        case 2: // SEEK_END (not always supported for gzip)
                            // Try SEEK_END but handle potential errors
                            seek_offset = fdp.ConsumeIntegralInRange<z_off_t>(-1024, 0);
                            gzseek(gzfile, seek_offset, SEEK_END);
                            // Don't update file_position as SEEK_END is complex for compressed files
                            break;
                    }
                }
                break;
            }
            
            case 9: {  // Get current position
                if (test_seek_operations) {
                    z_off_t pos = gztell(gzfile);
                    // pos could be -1 on error, but we just use it
                }
                break;
            }
            
            case 10: {  // Check if at end of file
                if (mode[0] == 'r') {
                    gzeof(gzfile);
                }
                break;
            }
            
            case 11: {  // Check if direct (non-gzip) access
                gzdirect(gzfile);
                break;
            }
            
            default:
                // No operation
                break;
        }
        
        // Occasionally check for errors
        if (test_error_recovery && (i % 4 == 0)) {
            int errnum;
            const char* errmsg = gzerror(gzfile, &errnum);
            if (errnum != Z_OK) {
                // Clear error state to continue testing
                gzclearerr(gzfile);
            }
        }
        
        // Mixed I/O: switch between read and write modes occasionally
        if (test_mixed_io && (i % 8 == 0) && fdp.remaining_bytes() > 10) {
            // Close current file
            gzclose(gzfile);
            
            // Switch mode
            if (mode[0] == 'r') {
                mode = "ab";
            } else {
                mode = "rb";
            }
            
            // Reopen with new mode
            gzfile = gzopen(temp_filename, mode.c_str());
            if (gzfile == NULL) {
                // Fall back to write mode
                mode = "wb";
                gzfile = gzopen(temp_filename, mode.c_str());
                if (gzfile == NULL) {
                    remove(temp_filename);
                    return 0;
                }
            }
            
            file_position = 0;
        }
    }
    
    // Final parameter change before closing
    if (test_dynamic_params && (mode[0] == 'w' || mode[0] == 'a')) {
        int final_level = fdp.ConsumeIntegralInRange<int>(0, 9);
        int final_strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
        gzsetparams(gzfile, final_level, final_strategy);
    }
    
    // Close the gzip file
    int close_result = gzclose(gzfile);
    
    // Clean up temporary file
    remove(temp_filename);
    
    return 0;
}

} // namespace harness_004
