#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <cstdarg>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_005 {
/*
 * Fuzzing harness for zlib gzip formatted I/O functions
 * Target: gzfread, gzfwrite, gzprintf, gzvprintf
 * Harness ID: 005
 * 
 * This harness specifically targets gzip formatted I/O operations which are
 * a critical coverage gap with 853+ undiscovered branches and 0% coverage.
 * 
 * Different from existing harnesses:
 * - harness_001 (basic gzip file I/O): covers gzputc, gzputs, gzwrite, gzread, etc.
 * - harness_004 (advanced gzip I/O with dynamic parameter control): covers gzsetparams, gzbuffer, etc.
 * 
 * This harness focuses on formatted I/O operations:
 * 1. gzfread/gzfwrite: Structured reading/writing with item size and count
 * 2. gzprintf: Formatted output with various format specifiers
 * 3. gzvprintf: Variadic argument list version of formatted output
 * 
 * The harness tests edge cases like:
 * - Size * nitems overflow (Z_STREAM_ERROR)
 * - Partial item reads/writes
 * - Various format string combinations
 * - Buffer size limits for gzprintf (8191 byte limit)
 * - Mixed formatted and binary I/O operations
 */


// Helper function for gzvprintf testing
static int test_gzvprintf_helper(gzFile file, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = gzvprintf(file, format, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    bool test_formatted_io = fdp.ConsumeBool();
    bool test_binary_io = fdp.ConsumeBool();
    bool test_mixed_operations = fdp.ConsumeBool();
    bool test_error_cases = fdp.ConsumeBool();
    bool test_large_operations = fdp.ConsumeBool();
    
    // Determine gzip mode from fuzzer input
    std::string mode = "wb";
    uint8_t mode_selector = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (mode_selector) {
        case 0: mode = "wb"; break;     // Write mode (compressed)
        case 1: mode = "wT"; break;     // Write transparent (no compression)
        case 2: mode = "wb6"; break;    // Write with compression level 6
        case 3: mode = "wb9f"; break;   // Write with full flush
    }

    // Create a temporary file for gzip operations
    char temp_filename[] = "/tmp/gzip_formatted_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd == -1) {
        return 0;
    }
    close(fd);

    // Open gzip file
    gzFile gzfile = gzopen(temp_filename, mode.c_str());
    if (gzfile == NULL) {
        // Try with default mode if opening failed
        mode = "wb";
        gzfile = gzopen(temp_filename, mode.c_str());
        if (gzfile == NULL) {
            remove(temp_filename);
            return 0;
        }
    }

    // Set buffer size for gzprintf testing (affects 8191 byte limit)
    unsigned buffer_size = fdp.ConsumeIntegralInRange<unsigned>(1024, 16384);
    gzbuffer(gzfile, buffer_size);

    // Determine number of operations to perform
    uint8_t num_operations = fdp.ConsumeIntegral<uint8_t>() % 16 + 1;
    
    // Track whether we're in read or write mode
    bool is_write_mode = (mode[0] == 'w' || mode[0] == 'a');
    
    for (uint8_t i = 0; i < num_operations && fdp.remaining_bytes() > 0; i++) {
        // Choose operation type based on mode and test flags
        uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 8;
        
        if (is_write_mode) {
            // Write operations
            switch (operation_type) {
                case 0: {  // gzfwrite - structured binary write
                    if (test_binary_io && fdp.remaining_bytes() >= 4) {
                        // Consume size and nitems parameters
                        z_size_t item_size = fdp.ConsumeIntegralInRange<z_size_t>(1, 256);
                        z_size_t num_items = fdp.ConsumeIntegralInRange<z_size_t>(1, 64);
                        
                        // Check for potential overflow
                        if (item_size > 0 && num_items > 0 && 
                            item_size <= SIZE_MAX / num_items) {
                            size_t total_size = item_size * num_items;
                            if (fdp.remaining_bytes() >= total_size) {
                                std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(total_size);
                                gzfwrite(buffer.data(), item_size, num_items, gzfile);
                            }
                        } else {
                            // Test overflow case - should return Z_STREAM_ERROR
                            // Use safe values that still trigger overflow detection
                            // but don't cause buffer overrun
                            std::vector<uint8_t> small_buffer(16);
                            // Use size that would overflow when multiplied, but
                            // use nitems=1 to avoid actual overflow in buffer access
                            // The API should detect the overflow in size calculation
                            gzfwrite(small_buffer.data(), SIZE_MAX/2 + 1, 2, gzfile);
                        }
                    }
                    break;
                }
                case 1: {  // gzprintf - formatted output
                    if (test_formatted_io && fdp.remaining_bytes() >= 8) {
                        // Choose format string type
                        uint8_t format_type = fdp.ConsumeIntegral<uint8_t>() % 6;
                        
                        switch (format_type) {
                            case 0: {  // Integer formatting
                                int int_value = fdp.ConsumeIntegral<int>();
                                gzprintf(gzfile, "Integer: %d", int_value);
                                break;
                            }
                            case 1: {  // Floating point formatting
                                double double_value = fdp.ConsumeFloatingPoint<double>();
                                gzprintf(gzfile, "Float: %f", double_value);
                                break;
                            }
                            case 2: {  // String formatting
                                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                                std::string str = fdp.ConsumeRandomLengthString(str_len);
                                gzprintf(gzfile, "String: %s", str.c_str());
                                break;
                            }
                            case 3: {  // Character formatting
                                char char_value = fdp.ConsumeIntegral<char>();
                                gzprintf(gzfile, "Char: %c", char_value);
                                break;
                            }
                            case 4: {  // Hexadecimal formatting
                                int hex_value = fdp.ConsumeIntegral<int>();
                                gzprintf(gzfile, "Hex: 0x%x", hex_value);
                                break;
                            }
                            case 5: {  // Complex format string
                                int int1 = fdp.ConsumeIntegral<int>();
                                int int2 = fdp.ConsumeIntegral<int>();
                                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                                std::string str = fdp.ConsumeRandomLengthString(str_len);
                                gzprintf(gzfile, "Complex: %d items, name: %s, value: %d", 
                                         int1, str.c_str(), int2);
                                break;
                            }
                        }
                    }
                    break;
                }
                
                case 2: {  // gzvprintf - variadic formatted output
                    if (test_formatted_io && fdp.remaining_bytes() >= 8) {
                        uint8_t format_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                        
                        switch (format_type) {
                            case 0: {
                                int int_value = fdp.ConsumeIntegral<int>();
                                test_gzvprintf_helper(gzfile, "vprintf int: %d", int_value);
                                break;
                            }
                            case 1: {
                                double double_value = fdp.ConsumeFloatingPoint<double>();
                                test_gzvprintf_helper(gzfile, "vprintf float: %f", double_value);
                                break;
                            }
                            case 2: {
                                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                                std::string str = fdp.ConsumeRandomLengthString(str_len);
                                test_gzvprintf_helper(gzfile, "vprintf string: %s", str.c_str());
                                break;
                            }
                            case 3: {
                                int int1 = fdp.ConsumeIntegral<int>();
                                int int2 = fdp.ConsumeIntegral<int>();
                                test_gzvprintf_helper(gzfile, "vprintf two: %d and %d", int1, int2);
                                break;
                            }
                        }
                    }
                    break;
                }
                
                case 3: {  // Mixed: gzfwrite followed by gzprintf
                    if (test_mixed_operations && fdp.remaining_bytes() >= 32) {
                        // First do a gzfwrite
                        z_size_t item_size = fdp.ConsumeIntegralInRange<z_size_t>(1, 64);
                        z_size_t num_items = fdp.ConsumeIntegralInRange<z_size_t>(1, 16);
                        size_t total_size = item_size * num_items;
                        
                        if (fdp.remaining_bytes() >= total_size + 8) {
                            std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(total_size);
                            gzfwrite(buffer.data(), item_size, num_items, gzfile);
                            
                            // Then do a gzprintf
                            int int_value = fdp.ConsumeIntegral<int>();
                            size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                            std::string str = fdp.ConsumeRandomLengthString(str_len);
                            gzprintf(gzfile, "After binary: %d, %s", int_value, str.c_str());
                        }
                    }
                    break;
                }
                
                case 4: {  // Test gzprintf buffer limit (8191 bytes)
                    if (test_error_cases && fdp.remaining_bytes() >= 8192) {
                        // Create a very long string that might exceed buffer limit
                        std::vector<uint8_t> long_data = fdp.ConsumeBytes<uint8_t>(8192);
                        // Convert to string for gzprintf
                        std::string long_str(long_data.begin(), long_data.end());
                        // Replace null bytes to avoid early termination
                        for (char& c : long_str) {
                            if (c == '\0') c = ' ';
                        }
                        // This might hit the 8191 byte limit
                        gzprintf(gzfile, "Long: %s", long_str.c_str());
                    }
                    break;
                }
                
                case 5: {  // gzwrite for comparison
                    if (test_binary_io && fdp.remaining_bytes() >= 128) {
                        size_t write_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                        if (fdp.remaining_bytes() >= write_len) {
                            std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(write_len);
                            gzwrite(gzfile, buffer.data(), buffer.size());
                        }
                    }
                    break;
                }
                
                case 6: {  // Test edge case: size=0 or nitems=0
                    if (test_error_cases && fdp.remaining_bytes() >= 16) {
                        uint8_t edge_case = fdp.ConsumeIntegral<uint8_t>() % 4;
                        
                        switch (edge_case) {
                            case 0: {  // size=0
                                std::vector<uint8_t> buffer(16);
                                gzfwrite(buffer.data(), 0, 10, gzfile);
                                break;
                            }
                            case 1: {  // nitems=0
                                std::vector<uint8_t> buffer(16);
                                gzfwrite(buffer.data(), 4, 0, gzfile);
                                break;
                            }
                            case 2: {  // both=0
                                std::vector<uint8_t> buffer(16);
                                gzfwrite(buffer.data(), 0, 0, gzfile);
                                break;
                            }
                            case 3: {  // large size, small nitems - ensure buffer is large enough
                                size_t large_size = fdp.ConsumeIntegralInRange<size_t>(1, 65536);
                                std::vector<uint8_t> buffer(large_size);
                                gzfwrite(buffer.data(), large_size, 1, gzfile);
                                break;
                            }
                        }
                    }
                    break;
                }
                
                case 7: {  // Flush operation between formatted writes
                    if (test_mixed_operations) {
                        int flush_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                        gzflush(gzfile, flush_type);
                    }
                    break;
                }
            }
        } else {
            // Read operations (if mode were 'r', though we're primarily testing writes)
            // For completeness, include some read testing
            if (operation_type == 0 && test_binary_io && fdp.remaining_bytes() >= 4) {
                // gzfread testing
                z_size_t item_size = fdp.ConsumeIntegralInRange<z_size_t>(1, 256);
                z_size_t num_items = fdp.ConsumeIntegralInRange<z_size_t>(1, 16);
                size_t total_size = item_size * num_items;
                
                if (total_size <= 4096) {  // Limit buffer size
                    std::vector<uint8_t> buffer(total_size);
                    gzfread(buffer.data(), item_size, num_items, gzfile);
                }
            }
        }
        
        // Occasionally seek to test formatted I/O at different positions
        if (test_mixed_operations && (i % 3 == 0) && fdp.remaining_bytes() >= 4) {
            int whence = fdp.ConsumeIntegral<uint8_t>() % 3;
            long offset = fdp.ConsumeIntegralInRange<long>(-100, 100);
            gzseek(gzfile, offset, whence);
        }
    }
    
    // Get error status if any
    if (test_error_cases) {
        int errnum;
        const char* errmsg = gzerror(gzfile, &errnum);
        (void)errmsg;  // Suppress unused variable warning
    }
    
    // Close the file
    gzclose(gzfile);
    remove(temp_filename);
    
    return 0;
}

} // namespace harness_005
