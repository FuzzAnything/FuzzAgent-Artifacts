#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>
#include <memory>

namespace harness_013 {
/*
 * Fuzzing harness for comprehensive gz file reading operations with error handling
 * Targets: gzungetc (256 undiscovered branches, 0 hits), gzgets (253 undiscovered branches, 1 hit),
 *          gzfread (238 undiscovered branches, 1 hit), gzgetc (237 undiscovered branches, 1 hit),
 *          gzread (239 undiscovered branches, 5 hits)
 * Helper APIs: gzopen, gzclose, gzdopen, gzdirect, gzclearerr, gzeof, gzerror
 * 
 * This harness specifically targets the gz reading functions identified as having
 * significant undiscovered branch coverage. It exercises various buffer sizes,
 * error conditions, read patterns, and edge cases to maximize branch coverage.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    uint8_t mode_selector = fdp.ConsumeIntegral<uint8_t>() % 6;
    int buffer_size_power = fdp.ConsumeIntegralInRange<int>(4, 12); // 16 to 4096 bytes
    size_t buffer_size = 1 << buffer_size_power;
    int num_operations = fdp.ConsumeIntegralInRange<int>(1, 10);
    int error_injection = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Generate file mode strings for different scenarios
    const char* mode_options[] = {"rb", "rb+", "wb", "wb+", "ab", "ab+"};
    const char* mode = mode_options[mode_selector % 6];
    
    // Generate filename from fuzzer input
    std::string filename_str = fdp.ConsumeRandomLengthString(32);
    const char* filename = filename_str.empty() ? "fuzz_gz_read_test.gz" : filename_str.c_str();
    
    // Generate test data for the file
    size_t test_data_len = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
    if (test_data_len > fdp.remaining_bytes()) {
        test_data_len = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_len);
    std::string text_data = fdp.ConsumeRandomLengthString(512);
    
    gzFile file = nullptr;
    
    // Create test file first with some data
    gzFile write_file = gzopen("temp_gz_read_test.gz", "wb");
    if (write_file) {
        if (!test_data.empty()) {
            gzwrite(write_file, test_data.data(), test_data.size());
        }
        if (!text_data.empty()) {
            gzprintf(write_file, "%s\n", text_data.c_str());
            gzputs(write_file, text_data.c_str());
        }
        gzclose(write_file);
    }
    
    // Now open for reading based on test scenario
    switch (test_scenario) {
        case 0:
            file = gzopen("temp_gz_read_test.gz", "rb");
            break;
        case 1:
            // Test gzdopen (simulated)
            file = gzopen("temp_gz_read_test.gz", "rb");
            break;
        case 2:
            file = gzopen("temp_gz_read_test.gz", "rb+");
            break;
        case 3:
            // Try to open non-existent file
            file = gzopen("nonexistent_file.gz", "rb");
            break;
        case 4:
            file = gzopen("temp_gz_read_test.gz", "wb");
            if (file) {
                // Write then reopen for reading
                gzwrite(file, test_data.data(), test_data.size());
                gzclose(file);
                file = gzopen("temp_gz_read_test.gz", "rb");
            }
            break;
        case 5: {
            // Test with empty file
            gzFile empty_file = gzopen("empty_gz_test.gz", "wb");
            if (empty_file) gzclose(empty_file);
            file = gzopen("empty_gz_test.gz", "rb");
            break;
        }
        case 6:
            file = gzopen("temp_gz_read_test.gz", "ab");
            break;
        case 7:
            // Test with NULL filename
            file = gzopen(NULL, "rb");
            break;
    }
    
    if (file == nullptr) {
        // Clean up temporary files
        remove("temp_gz_read_test.gz");
        remove("empty_gz_test.gz");
        return 0;
    }
    
    // Set buffer size
    gzbuffer(file, buffer_size);
    
    // Check if file is in transparent mode
    int is_direct = gzdirect(file);
    
    // Perform comprehensive reading operations
    for (int op = 0; op < num_operations; op++) {
        // Consume operation type from remaining fuzzer data
        if (fdp.remaining_bytes() < 1) break;
        uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
        
        switch (operation) {
            case 0: {
                // Test gzread with various buffer sizes
                size_t read_size = fdp.ConsumeIntegralInRange<size_t>(1, buffer_size * 2);
                std::vector<char> read_buffer(read_size);
                int bytes_read = gzread(file, read_buffer.data(), read_size);
                
                // Check error status
                int err_num;
                const char* error_msg = gzerror(file, &err_num);
                
                // Check EOF status
                int eof_status = gzeof(file);
                
                // Test gzclearerr if there was an error
                if (err_num != Z_OK) {
                    gzclearerr(file);
                }
                break;
            }
            
            case 1: {
                // Test gzgets with various line lengths
                int line_len = fdp.ConsumeIntegralInRange<int>(1, 256);
                std::vector<char> line_buffer(line_len);
                char* line_result = gzgets(file, line_buffer.data(), line_len);
                
                // If we got a line, test gzungetc with characters from it
                if (line_result != NULL && line_len > 1) {
                    for (int i = 0; i < std::min(3, line_len - 1); i++) {
                        int unget_result = gzungetc(line_buffer[i], file);
                        // Try to read it back
                        if (unget_result != -1) {
                            int read_back = gzgetc(file);
                        }
                    }
                }
                break;
            }
            
            case 2: {
                // Test gzgetc and gzungetc extensively
                int chars_to_read = fdp.ConsumeIntegralInRange<int>(1, 10);
                std::vector<int> read_chars;
                
                for (int i = 0; i < chars_to_read; i++) {
                    int ch = gzgetc(file);
                    if (ch != -1) {
                        read_chars.push_back(ch);
                    } else {
                        break;
                    }
                }
                
                // Now unget some characters in reverse order
                for (int i = read_chars.size() - 1; i >= 0 && i >= (int)read_chars.size() - 3; i--) {
                    int unget_result = gzungetc(read_chars[i], file);
                    // Test error case: try to unget EOF
                    if (i == 0) {
                        gzungetc(-1, file); // Should fail
                    }
                }
                
                // Read them back
                for (size_t i = 0; i < std::min(read_chars.size(), (size_t)3); i++) {
                    gzgetc(file);
                }
                break;
            }
            
            case 3: {
                // Test gzfread with various item sizes
                unsigned items = fdp.ConsumeIntegralInRange<unsigned>(1, 10);
                unsigned size_per_item = fdp.ConsumeIntegralInRange<unsigned>(1, 128);
                size_t total_size = items * size_per_item;
                std::vector<char> fread_buffer(total_size);
                
                size_t items_read = gzfread(fread_buffer.data(), size_per_item, items, file);
                
                // Test partial reads
                if (items_read > 0 && items_read < items) {
                    // Try to read remaining items
                    size_t remaining_items = items - items_read;
                    gzfread(fread_buffer.data() + items_read * size_per_item, 
                           size_per_item, remaining_items, file);
                }
                break;
            }
            
            case 4: {
                // Test reading past EOF and error conditions
                // First, check current EOF status
                int eof_before = gzeof(file);
                
                // Try to read a large amount (potentially past EOF)
                size_t large_read = fdp.ConsumeIntegralInRange<size_t>(1024, 8192);
                std::vector<char> large_buffer(large_read);
                int large_read_result = gzread(file, large_buffer.data(), large_read);
                
                int eof_after = gzeof(file);
                
                // Check error
                int err_num;
                const char* error_msg = gzerror(file, &err_num);
                
                // Clear error if any
                if (err_num != Z_OK) {
                    gzclearerr(file);
                }
                break;
            }
            
            case 5: {
                // Test error injection scenarios
                switch (error_injection) {
                    case 0:
                        // Read with NULL buffer
                        gzread(file, NULL, 10);
                        break;
                    case 1:
                        // Read with zero size
                        gzread(file, test_data.data(), 0);
                        break;
                    case 2:
                        // gzgets with NULL buffer
                        gzgets(file, NULL, 10);
                        break;
                    case 3:
                        // gzgets with zero length
                        char dummy_buf[1];
                        gzgets(file, dummy_buf, 0);
                        break;
                }
                
                // Check and clear any errors
                int err_num;
                const char* error_msg = gzerror(file, &err_num);
                if (err_num != Z_OK) {
                    gzclearerr(file);
                }
                break;
            }
            
            case 6: {
                // Test mixed operations
                // Read some bytes
                char mixed_buf[64];
                int bytes_read = gzread(file, mixed_buf, sizeof(mixed_buf));
                
                if (bytes_read > 0) {
                    // Get a character
                    int ch = gzgetc(file);
                    if (ch != -1) {
                        // Unget it
                        gzungetc(ch, file);
                        // Read it back
                        gzgetc(file);
                    }
                    
                    // Try to read a line
                    char line_buf[128];
                    gzgets(file, line_buf, sizeof(line_buf));
                    
                    // Check EOF
                    gzeof(file);
                    
                    // Check if direct
                    gzdirect(file);
                }
                break;
            }
            
            case 7: {
                // Test boundary conditions
                // Read single bytes repeatedly
                for (int i = 0; i < 5; i++) {
                    int ch = gzgetc(file);
                    if (ch == -1) break;
                    
                    // Occasionally unget
                    if (i % 2 == 0) {
                        gzungetc(ch, file);
                        gzgetc(file); // Read it back
                    }
                }
                
                // Try gzfread with single byte items
                char single_items[5];
                gzfread(single_items, 1, 5, file);
                
                // Check error status
                int err_num;
                gzerror(file, &err_num);
                break;
            }
        }
        
        // Check EOF periodically
        if (op % 2 == 0) {
            gzeof(file);
        }
        
        // Check if still direct
        if (op % 3 == 0) {
            gzdirect(file);
        }
    }
    
    // Final error check and cleanup
    int final_err_num;
    const char* final_error = gzerror(file, &final_err_num);
    if (final_err_num != Z_OK) {
        gzclearerr(file);
    }
    
    // Close file
    gzclose(file);
    
    // Clean up temporary files
    remove("temp_gz_read_test.gz");
    remove("empty_gz_test.gz");
    
    return 0;
}

} // namespace harness_013
