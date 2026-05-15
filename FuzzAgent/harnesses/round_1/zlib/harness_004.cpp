#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_004 {
/*
 * Fuzzing harness for zlib gzip formatted output APIs targeting gzwrite.c module
 * Targets gzip formatted output APIs with various data types, formats, buffer sizes, and error conditions
 * APIs: gzprintf, gzvprintf, gzputc, gzputs, gzfwrite, gzgetc
 * Focus: Testing gzip formatted output with semantic diversity from existing harnesses
 * Coverage target: gzwrite.c module with 0% coverage and 1000+ undiscovered branches
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration and test data
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int operation_type = fdp.ConsumeIntegralInRange<int>(0, 5);  // Which API to test
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);  // Compression level
    int buffer_size = fdp.ConsumeIntegralInRange<int>(128, 8192);  // Buffer size for gzbuffer
    int format_type = fdp.ConsumeIntegralInRange<int>(0, 7);  // Format type for gzprintf
    bool test_error_conditions = fdp.ConsumeBool();  // Whether to test error conditions
    bool test_boundary_conditions = fdp.ConsumeBool();  // Test edge cases
    bool use_direct_mode = fdp.ConsumeBool();  // Use direct (uncompressed) mode
    
    // Consume string data for testing
    size_t string_data_size = fdp.ConsumeIntegralInRange<size_t>(32, 512);
    if (fdp.remaining_bytes() < string_data_size) {
        return 0;
    }
    std::string test_string = fdp.ConsumeBytesAsString(string_data_size);
    
    // Consume binary data for gzfwrite/gzwrite testing
    size_t binary_data_size = fdp.ConsumeIntegralInRange<size_t>(64, 2048);
    if (fdp.remaining_bytes() < binary_data_size) {
        return 0;
    }
    std::vector<uint8_t> test_binary_data = fdp.ConsumeBytes<uint8_t>(binary_data_size);
    
    // Consume numeric values for gzprintf formatting
    int int_value = fdp.ConsumeIntegral<int>();
    unsigned uint_value = fdp.ConsumeIntegral<unsigned>();
    long long_value = fdp.ConsumeIntegral<long>();
    double double_value = fdp.ConsumeFloatingPoint<double>();
    
    // Create a temporary file for gzip operations
    char fname[] = "/tmp/gzwrite_fuzz_XXXXXX";
    int fd = mkstemp(fname);
    if (fd < 0) {
        return 0;
    }
    close(fd);  // Close file descriptor, gzopen will reopen
    
    // Determine mode based on fuzzer input
    std::string mode = "wb";
    if (use_direct_mode) {
        mode += "T";  // Transparent (uncompressed) mode
    } else {
        mode += std::to_string(compression_level);
    }
    
    // Open the file for writing with gzip
    gzFile file = gzopen(fname, mode.c_str());
    if (!file) {
        unlink(fname);
        return 0;
    }
    
    // Set buffer size for gzip operations
    gzbuffer(file, buffer_size);
    
    int result = 0;
    size_t items_written = 0;
    
    // Test different gzip output operations based on fuzzer input
    switch (operation_type) {
        case 0:  // Test gzprintf with various format strings
        {
            // Select format string based on fuzzer input
            const char* format = nullptr;
            switch (format_type) {
                case 0: format = "Test string: %s, int: %d, uint: %u\n"; break;
                case 1: format = "Long: %ld, double: %.2f\n"; break;
                case 2: format = "Hex: 0x%x, Octal: %o\n"; break;
                case 3: format = "Char: %c, String: %.*s\n"; break;
                case 4: format = "Pointer: %p, Scientific: %e\n"; break;
                case 5: format = "Width: %10d, Precision: %.3f\n"; break;
                case 6: format = "Mixed: %s %d %.2f %c\n"; break;
                case 7: format = "%% escaped percent sign\n"; break;
            }
            
            if (format) {
                // Test gzprintf with the selected format - provide arguments based on format type
                switch (format_type) {
                    case 0:  // "Test string: %s, int: %d, uint: %u\n"
                        result = gzprintf(file, format, 
                            test_string.c_str(), int_value, uint_value);
                        break;
                    case 1:  // "Long: %ld, double: %.2f\n"
                        result = gzprintf(file, format, 
                            long_value, double_value);
                        break;
                    case 2:  // "Hex: 0x%x, Octal: %o\n"
                        result = gzprintf(file, format, 
                            uint_value, uint_value);
                        break;
                    case 3:  // "Char: %c, String: %.*s\n"
                        result = gzprintf(file, format, 
                            int_value % 256, 5, test_string.c_str());
                        break;
                    case 4:  // "Pointer: %p, Scientific: %e\n"
                        result = gzprintf(file, format, 
                            &result, double_value);
                        break;
                    case 5:  // "Width: %10d, Precision: %.3f\n"
                        result = gzprintf(file, format, 
                            int_value, double_value);
                        break;
                    case 6:  // "Mixed: %s %d %.2f %c\n"
                        result = gzprintf(file, format, 
                            test_string.c_str(), int_value, double_value, int_value % 256);
                        break;
                    case 7:  // "%% escaped percent sign\n"
                        result = gzprintf(file, format);
                        break;
                }
                
                // Test with fewer arguments for some formats
                if (format_type == 0) {
                    result = gzprintf(file, "Simple: %s %d", test_string.c_str(), int_value);
                }
            }
            break;
        }
        
        case 1:  // Test gzvprintf (va_list version)
        {
            // Create a format string for testing
            const char* format = "vprintf test: %s %d %f";
            
            // Since we can't easily create a proper va_list dynamically in a fuzzer,
            // we'll test gzvprintf indirectly through gzprintf which calls it internally.
            // The gzvprintf function is called by gzprintf, so testing gzprintf exercises
            // the gzvprintf code path.
            result = gzprintf(file, format, test_string.c_str(), int_value, double_value);
            
            // Test with different format strings to exercise more of gzvprintf
            if (test_string.length() > 0) {
                const char* alt_format = "Alternate: %.*s %ld %e";
                result = gzprintf(file, alt_format, 
                    (int)std::min(test_string.length(), (size_t)20), 
                    test_string.c_str(), long_value, double_value);
            }
            break;
        }
        
        case 2:  // Test gzputc - single character writing
        {
            // Write multiple characters from the test string
            for (size_t i = 0; i < test_string.length() && i < 100; i++) {
                int char_result = gzputc(file, test_string[i]);
                if (char_result == -1 && test_error_conditions) {
                    // Check error state
                    int errnum;
                    const char* errmsg = gzerror(file, &errnum);
                    break;
                }
            }
            
            // Test edge cases
            if (test_boundary_conditions) {
                gzputc(file, 0);  // Null character
                gzputc(file, 255);  // Maximum byte value
                gzputc(file, '\n');  // Newline
                gzputc(file, EOF);  // EOF (should be converted to unsigned char)
            }
            break;
        }
        
        case 3:  // Test gzputs - string writing
        {
            // Write the test string
            result = gzputs(file, test_string.c_str());
            
            // Write multiple strings
            if (test_string.length() > 10) {
                std::string part1 = test_string.substr(0, test_string.length() / 2);
                std::string part2 = test_string.substr(test_string.length() / 2);
                
                gzputs(file, part1.c_str());
                gzputs(file, part2.c_str());
                gzputs(file, "\n");  // Add newline
            }
            
            // Test empty string
            if (test_boundary_conditions) {
                gzputs(file, "");
            }
            break;
        }
        
        case 4:  // Test gzfwrite - structured binary writing
        {
            // Write binary data in various chunk sizes
            size_t total_written = 0;
            size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            
            while (total_written < test_binary_data.size()) {
                size_t to_write = std::min(chunk_size, test_binary_data.size() - total_written);
                size_t items = 1;  // Write as single item
                size_t size = to_write;  // Size of each item
                
                items_written = gzfwrite(&test_binary_data[total_written], size, items, file);
                if (items_written == 0) {
                    // Check for errors
                    int errnum;
                    const char* errmsg = gzerror(file, &errnum);
                    break;
                }
                total_written += items_written * size;
                
                // Change chunk size for next iteration
                chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            }
            
            // Test with size=1 (like gzputc but for binary)
            if (test_boundary_conditions && !test_binary_data.empty()) {
                for (size_t i = 0; i < std::min(test_binary_data.size(), (size_t)100); i++) {
                    items_written = gzfwrite(&test_binary_data[i], 1, 1, file);
                }
            }
            break;
        }
        
        case 5:  // Test gzgetc after writing (read back written data)
        {
            // First write some data
            gzputs(file, test_string.c_str());
            gzflush(file, Z_SYNC_FLUSH);
            
            // Close and reopen for reading
            gzclose(file);
            file = gzopen(fname, "rb");
            if (!file) {
                unlink(fname);
                return 0;
            }
            
            // Read back characters with gzgetc
            int ch;
            int read_count = 0;
            while ((ch = gzgetc(file)) != -1 && read_count < 1000) {
                read_count++;
                
                // Test gzungetc if we have a character
                if (test_boundary_conditions && read_count % 10 == 0) {
                    gzungetc(ch, file);
                    int ch2 = gzgetc(file);  // Should get the same character
                }
            }
            
            // Test error conditions
            if (test_error_conditions) {
                // Try to read from write-only file (will fail)
                gzclose(file);
                file = gzopen(fname, "wb");
                if (file) {
                    ch = gzgetc(file);  // Should return -1
                }
            }
            break;
        }
    }
    
    // Test flushing operations
    if (!test_error_conditions) {
        gzflush(file, Z_SYNC_FLUSH);
        
        // Test intermediate flush modes
        int flush_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
        switch (flush_mode) {
            case 0: gzflush(file, Z_NO_FLUSH); break;
            case 1: gzflush(file, Z_PARTIAL_FLUSH); break;
            case 2: gzflush(file, Z_FULL_FLUSH); break;
            case 3: gzflush(file, Z_FINISH); break;
        }
    }
    
    // Test error retrieval
    if (test_error_conditions) {
        int errnum;
        const char* errmsg = gzerror(file, &errnum);
        
        // Clear errors
        gzclearerr(file);
        
        // Test with invalid file
        gzFile invalid_file = nullptr;
        gzerror(invalid_file, &errnum);
    }
    
    // Test file position
    if (!test_error_conditions) {
        z_off_t pos = gztell(file);
        
        // Try seeking if we have a position
        if (pos > 0) {
            z_off_t seek_result = gzseek(file, pos / 2, SEEK_SET);
            z_off_t new_pos = gztell(file);
        }
    }
    
    // Clean up
    gzclose(file);
    unlink(fname);
    
    return 0;
}

} // namespace harness_004
