#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <climits>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis/liblouis.h"

namespace harness_012 {
/*
 * liblouis Stress Testing and Boundary Condition Fuzzing Harness (harness_012.cpp)
 * 
 * This harness focuses on stress testing and boundary condition testing to uncover
 * vulnerabilities related to memory management, buffer overflows, and extreme inputs.
 * 
 * Seven stress testing areas:
 * 1) Very large input sizes (up to 1MB practical limits)
 * 2) Very long table names and paths (up to 4096 characters)
 * 3) Unicode handling with various encodings and surrogate pairs
 * 4) Buffer edge cases (exact buffer sizes, off-by-one errors)
 * 5) Malformed but syntactically valid table files
 * 6) Repeated API calls in loops to check for resource leaks
 * 7) Extreme mode flag combinations (bitwise combinations of all translation modes)
 * 
 * Semantic differentiation from previous harnesses:
 * - harness_000-011: Focus on functional correctness and API coverage
 * - This harness: Focuses exclusively on stress, boundary conditions, and resilience testing
 */


extern "C" {
}

// Simple string to widechar conversion for fuzzing with Unicode support
static int unicode_string_to_widechar(const char* input, size_t input_len, widechar* output, size_t output_max) {
    size_t i = 0, j = 0;
    while (i < input_len && j < output_max) {
        // Simple conversion: treat each byte as a character (for ASCII)
        // For Unicode testing, we'll generate surrogate pairs from fuzzer input
        output[j] = (widechar)(unsigned char)input[i];
        i++;
        j++;
    }
    return (int)j;
}

// Generate a string with potential Unicode surrogate pairs
static std::string generate_unicode_string(FuzzedDataProvider& fdp, size_t max_len) {
    std::string result;
    size_t target_len = fdp.ConsumeIntegralInRange<size_t>(1, max_len);
    
    for (size_t i = 0; i < target_len; i++) {
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (char_type) {
            case 0: // ASCII
                result.push_back(fdp.ConsumeIntegralInRange<char>(32, 126));
                break;
            case 1: // Extended ASCII
                result.push_back(fdp.ConsumeIntegralInRange<char>(128, 255));
                break;
            case 2: // Potential UTF-8 multi-byte (simplified)
                result.push_back(0xC2); // Lead byte for 2-byte UTF-8
                result.push_back(fdp.ConsumeIntegralInRange<char>(0x80, 0xBF));
                i++; // Count both bytes
                break;
            case 3: // Potential surrogate pair markers (for testing)
                result.push_back(0xED);
                result.push_back(fdp.ConsumeIntegralInRange<char>(0xA0, 0xBF));
                result.push_back(0xED);
                result.push_back(fdp.ConsumeIntegralInRange<char>(0xB0, 0xBF));
                i += 3; // Count all bytes
                break;
        }
        if (fdp.remaining_bytes() < 4) break;
    }
    return result;
}

// Generate an extremely long path/table name
static std::string generate_long_path(FuzzedDataProvider& fdp, size_t min_len, size_t max_len) {
    size_t path_len = fdp.ConsumeIntegralInRange<size_t>(min_len, max_len);
    std::string path;
    path.reserve(path_len);
    
    // Start with /tmp/ to ensure writable directory
    path = "/tmp/";
    
    // Add random characters for the rest of the path
    while (path.length() < path_len && fdp.remaining_bytes() > 0) {
        char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
        path.push_back(c);
        
        // Occasionally add directory separators for deep paths
        if (fdp.ConsumeProbability<float>() < 0.01f && path.length() < path_len - 1) {
            path.push_back('/');
        }
    }
    
    // Ensure it ends with .ctb for table files
    if (path.length() >= 4 && path.compare(path.length() - 4, 4, ".ctb") != 0) {
        path.append(".ctb");
    }
    
    return path;
}

// Generate a malformed but syntactically valid table file
static std::vector<uint8_t> generate_malformed_table(FuzzedDataProvider& fdp, size_t size) {
    std::vector<uint8_t> table_data;
    table_data.reserve(size);
    
    // Start with valid CTB header
    std::string header = "ctb\n";
    table_data.insert(table_data.end(), header.begin(), header.end());
    
    // Add random table content that's syntactically valid but semantically weird
    size_t remaining = size - header.size();
    while (remaining > 0 && fdp.remaining_bytes() > 0) {
        uint8_t line_type = fdp.ConsumeIntegral<uint8_t>() % 10;
        
        switch (line_type) {
            case 0: // Comment line
                table_data.push_back('#');
                for (int i = 0; i < fdp.ConsumeIntegralInRange<int>(1, 50) && remaining > 1; i++) {
                    table_data.push_back(fdp.ConsumeIntegral<uint8_t>());
                    remaining--;
                }
                table_data.push_back('\n');
                remaining--;
                break;
                
            case 1: // Empty line
                table_data.push_back('\n');
                remaining--;
                break;
                
            case 2: // Rule with weird patterns
                {
                    std::string rule = "always ";
                    for (char c : rule) {
                        table_data.push_back(c);
                        remaining--;
                    }
                    for (int i = 0; i < fdp.ConsumeIntegralInRange<int>(1, 20) && remaining > 0; i++) {
                        table_data.push_back(fdp.ConsumeIntegralInRange<uint8_t>('a', 'z'));
                        remaining--;
                    }
                    table_data.push_back('\n');
                    remaining--;
                }
                break;
                
            default: // Random bytes
                table_data.push_back(fdp.ConsumeIntegral<uint8_t>());
                remaining--;
                break;
        }
    }
    
    return table_data;
}

// Log callback to suppress output during stress testing
static void stress_log_callback(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
    // Completely suppress logging to avoid I/O overhead during stress tests
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    static int initialized = 0;
    static int file_counter = 0;
    
    // Initialize liblouis once with stress logging callback
    if (!initialized) {
        lou_registerLogCallback(stress_log_callback);
        lou_setLogLevel(LOU_LOG_OFF); // Minimize logging impact
        initialized = 1;
    }
    
    // Need minimum input for stress testing configuration
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Determine stress test type
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 7;
    
    // 2. Consume configuration parameters
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(100, 1024 * 1024); // Up to 1MB
    input_size = std::min(input_size, fdp.remaining_bytes());
    
    int loop_count = fdp.ConsumeIntegralInRange<int>(1, 1000);
    int mode_flags = fdp.ConsumeIntegral<int>();
    
    // 3. Generate test input based on test type
    std::vector<uint8_t> test_input;
    
    switch (test_type) {
        case 0: // Very large input
            test_input = fdp.ConsumeBytes<uint8_t>(input_size);
            break;
            
        case 1: // Unicode handling
            {
                std::string unicode_str = generate_unicode_string(fdp, input_size);
                test_input.assign(unicode_str.begin(), unicode_str.end());
            }
            break;
            
        case 2: // Buffer edge cases (exact sizes)
            test_input = fdp.ConsumeBytes<uint8_t>(input_size);
            // We'll handle buffer sizing separately
            break;
            
        default: // General stress test
            test_input = fdp.ConsumeRemainingBytes<uint8_t>();
            break;
    }
    
    if (test_input.empty()) {
        return 0;
    }
    
    // 4. Create a temporary table file for testing
    int counter = __sync_fetch_and_add(&file_counter, 1);
    std::string table_path;
    
    if (test_type == 3) { // Long table names
        table_path = generate_long_path(fdp, 256, 4096);
    } else {
        char filename[256];
        snprintf(filename, sizeof(filename), "/tmp/liblouis_stress_%d.ctb", counter);
        table_path = filename;
    }
    
    // Generate table content
    std::vector<uint8_t> table_data;
    if (test_type == 4) { // Malformed table
        table_data = generate_malformed_table(fdp, fdp.ConsumeIntegralInRange<size_t>(100, 10000));
    } else {
        // Simple valid table
        std::string simple_table = "ctb\ninclude en-us-g2.ctb\n";
        table_data.assign(simple_table.begin(), simple_table.end());
    }
    
    // Write table file
    FILE* table_file = fopen(table_path.c_str(), "wb");
    if (!table_file) {
        return 0;
    }
    fwrite(table_data.data(), 1, table_data.size(), table_file);
    fclose(table_file);
    
    // 5. Perform stress testing with the table
    int overall_result = 0;
    
    for (int loop = 0; loop < loop_count; loop++) {
        // Convert input to widechar
        size_t input_len = test_input.size();
        widechar* input_buf = (widechar*)malloc((input_len + 1) * sizeof(widechar));
        if (!input_buf) {
            unlink(table_path.c_str());
            return 0;
        }
        
        int actual_len = unicode_string_to_widechar((const char*)test_input.data(), input_len, input_buf, input_len);
        
        // Test different buffer sizing strategies
        int output_size;
        if (test_type == 2) { // Buffer edge cases
            uint8_t buffer_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (buffer_type) {
                case 0: // Exact size
                    output_size = actual_len * 16;
                    break;
                case 1: // Off-by-one small
                    output_size = actual_len * 16 - 1;
                    break;
                case 2: // Off-by-one large
                    output_size = actual_len * 16 + 1;
                    break;
                default: // Very small buffer
                    output_size = std::max(1, actual_len / 2);
                    break;
            }
        } else {
            output_size = actual_len * 16;
        }
        
        widechar* output_buf = (widechar*)malloc((output_size + 1) * sizeof(widechar));
        if (!output_buf) {
            free(input_buf);
            unlink(table_path.c_str());
            return 0;
        }
        
        // Prepare parameters
        int inlen = actual_len;
        int outlen = output_size;
        
        // Test extreme mode combinations
        int test_mode = mode_flags;
        if (test_type == 6) { // Extreme mode combinations
            // Combine all possible mode flags
            test_mode = noContractions | compbrlAtCursor | dotsIO | compbrlLeftCursor | ucBrl | noUndefined | partialTrans;
            // Also test random combinations
            test_mode |= fdp.ConsumeIntegral<int>();
        }
        
        // Perform translation
        int result = lou_translateString(table_path.c_str(), input_buf, &inlen, 
                                         output_buf, &outlen, NULL, NULL, test_mode);
        
        // Also test back translation for comprehensive stress
        if (result && outlen > 0) {
            int back_inlen = outlen;
            int back_outlen = actual_len;
            widechar* back_output = (widechar*)malloc((back_outlen + 1) * sizeof(widechar));
            if (back_output) {
                lou_backTranslateString(table_path.c_str(), output_buf, &back_inlen,
                                       back_output, &back_outlen, NULL, NULL, test_mode);
                free(back_output);
            }
        }
        
        // Test with cursor tracking API for additional stress
        if (actual_len > 0 && output_size >= actual_len) {
            int* output_pos = (int*)malloc(actual_len * sizeof(int));
            int* input_pos = (int*)malloc(output_size * sizeof(int));
            int cursor_pos = 0;
            
            if (output_pos && input_pos) {
                int trans_inlen = actual_len;
                int trans_outlen = output_size;
                lou_translate(table_path.c_str(), input_buf, &trans_inlen, output_buf,
                             &trans_outlen, NULL, NULL, output_pos, input_pos, &cursor_pos, test_mode);
            }
            
            free(output_pos);
            free(input_pos);
        }
        
        free(output_buf);
        free(input_buf);
        
        // Break early if we're running out of resources
        if (loop > 0 && loop % 100 == 0) {
            lou_free(); // Force cleanup to test resource management
        }
    }
    
    // 6. Cleanup
    unlink(table_path.c_str());
    
    // Force a global cleanup to check for resource leaks
    if (test_type == 5) { // Resource leak testing
        for (int i = 0; i < 10; i++) {
            lou_free();
        }
    }
    
    return 0;
}

} // namespace harness_012
