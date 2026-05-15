#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis.h"

namespace harness_006 {
// Targeted liblouis fuzzing harness for comprehensive hyphenation and character size testing
// Harness_006: Focuses on lou_hyphenate (47 undiscovered branches) and lou_charSize (0% covered)
// Differentiation from harness_005: Uses lou_findTable for dynamic table discovery, includes lou_charSize,
// and performs more systematic hyphenation testing with edge cases
// Strategy: Deep testing of hyphenation logic with dynamic table selection and character size integration




// Log callback to suppress output during fuzzing
static void fuzz_log_callback(logLevels level, const char* message) {
    (void)level;
    (void)message;
    // Suppress all logging during fuzzing
}

// Convert std::string to widechar buffer (simple ASCII/Latin-1 conversion)
static widechar* string_to_widechar(const std::string& str, int* out_len) {
    if (str.empty()) {
        *out_len = 0;
        return nullptr;
    }
    
    widechar* wbuf = (widechar*)malloc((str.size() + 1) * sizeof(widechar));
    if (!wbuf) {
        *out_len = 0;
        return nullptr;
    }
    
    for (size_t i = 0; i < str.size(); i++) {
        wbuf[i] = (widechar)(unsigned char)str[i];
    }
    wbuf[str.size()] = 0;
    *out_len = (int)str.size();
    return wbuf;
}

// Generate random hyphen queries for lou_findTable
static std::string generate_table_query(FuzzedDataProvider& fdp) {
    std::string query;
    
    // Choose query type
    uint8_t query_type = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    switch (query_type) {
        case 0:
            query = "en";  // English tables
            break;
        case 1:
            query = "g2";  // Grade 2 tables
            break;
        case 2:
            query = "comp"; // Computer braille
            break;
        case 3:
            query = "us";  // US tables
            break;
        case 4:
            // Random string query
            if (fdp.remaining_bytes() > 1) {
                size_t len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                query = fdp.ConsumeBytesAsString(len);
            } else {
                query = "en";
            }
            break;
    }
    
    return query;
}

// Generate hyphen array for hyphenation testing
static char* generate_hyphen_array(FuzzedDataProvider& fdp, int length) {
    if (length <= 0) return nullptr;
    
    char* hyphens = (char*)malloc(length * sizeof(char));
    if (!hyphens) return nullptr;
    
    for (int i = 0; i < length; i++) {
        // Generate valid hyphen characters: '0', '1', or other valid hyphen markers
        // According to liblouis documentation, hyphens array contains '0' or '1'
        uint8_t hyphen_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (hyphen_type) {
            case 0:
                hyphens[i] = '0';  // No hyphen
                break;
            case 1:
                hyphens[i] = '1';  // Hyphen
                break;
            case 2:
                hyphens[i] = fdp.ConsumeIntegralInRange<char>('0', '1');
                break;
        }
    }
    return hyphens;
}

// Generate test text with various characteristics for hyphenation testing
static std::string generate_test_text(FuzzedDataProvider& fdp) {
    size_t remaining = fdp.remaining_bytes();
    if (remaining < 1) return "";
    
    // Determine text length (bounded to prevent excessive memory usage)
    size_t max_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    if (max_len > remaining) {
        max_len = remaining;
    }
    
    // Generate text with various characteristics
    uint8_t text_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (text_type) {
        case 0:
            // Normal text
            return fdp.ConsumeBytesAsString(max_len);
        case 1:
            // Text with spaces (simulating words)
            {
                std::string result;
                while (result.length() < max_len && fdp.remaining_bytes() > 0) {
                    size_t word_len = fdp.ConsumeIntegralInRange<size_t>(1, 12);
                    if (word_len > fdp.remaining_bytes()) word_len = fdp.remaining_bytes();
                    if (word_len > max_len - result.length()) word_len = max_len - result.length();
                    
                    result += fdp.ConsumeBytesAsString(word_len);
                    if (result.length() < max_len && fdp.ConsumeBool()) {
                        result += " ";
                    }
                }
                return result;
            }
        case 2:
            // Text with punctuation (edge cases for hyphenation)
            {
                std::string result;
                const char* punctuation = ".,;:!?-'\"";
                while (result.length() < max_len && fdp.remaining_bytes() > 0) {
                    if (fdp.ConsumeBool() && result.length() + 1 < max_len) {
                        uint8_t punct_idx = fdp.ConsumeIntegral<uint8_t>() % 9;
                        result += punctuation[punct_idx];
                    } else {
                        size_t chunk_len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
                        if (chunk_len > fdp.remaining_bytes()) chunk_len = fdp.remaining_bytes();
                        if (chunk_len > max_len - result.length()) chunk_len = max_len - result.length();
                        
                        result += fdp.ConsumeBytesAsString(chunk_len);
                    }
                }
                return result;
            }
        case 3:
            // Mixed case text
            {
                std::string result = fdp.ConsumeBytesAsString(max_len);
                // Randomly change case
                for (size_t i = 0; i < result.length(); i++) {
                    if (fdp.ConsumeBool()) {
                        if (result[i] >= 'a' && result[i] <= 'z') {
                            result[i] = result[i] - 'a' + 'A';
                        } else if (result[i] >= 'A' && result[i] <= 'Z') {
                            result[i] = result[i] - 'A' + 'a';
                        }
                    }
                }
                return result;
            }
    }
    
    return fdp.ConsumeBytesAsString(max_len);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for comprehensive hyphenation testing
    // We need: log level (4 bytes) + mode (4 bytes) + operation count (4 bytes) + at least 1 byte text
    if (size < 13) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Set log level - consume from valid range
    logLevels log_level = static_cast<logLevels>(
        fdp.ConsumeIntegralInRange<int>(LOU_LOG_ALL, LOU_LOG_OFF));
    lou_setLogLevel(log_level);
    
    // 2. Register log callback to suppress output
    lou_registerLogCallback(fuzz_log_callback);
    
    // 3. Find a table that supports hyphenation using lou_findTable
    std::string table_query = generate_table_query(fdp);
    char* found_table = lou_findTable(table_query.c_str());
    
    const char* table_to_use = nullptr;
    if (found_table) {
        table_to_use = found_table;
    } else {
        // If no table found, use a default known table
        // Choose from common tables that likely support hyphenation
        uint8_t default_table = fdp.ConsumeIntegral<uint8_t>() % 5;
        switch (default_table) {
            case 0: table_to_use = "en-us-g2.ctb"; break;
            case 1: table_to_use = "en-ueb-g2.ctb"; break;
            case 2: table_to_use = "de-g0.utb"; break;
            case 3: table_to_use = "fr-bfu-comp6.ctb"; break;
            case 4: table_to_use = "sv-g1.ctb"; break;
        }
    }
    
    // 4. Get the table pointer using lou_getTable (if table is valid)
    const void* table_ptr = nullptr;
    if (table_to_use) {
        table_ptr = lou_getTable(table_to_use);
        if (!table_ptr) {
            // If we can't get the table, we can't proceed with hyphenation
            if (found_table) lou_freeTableFile(found_table);
            return 0;
        }
    } else {
        // No table available
        return 0;
    }
    
    // 5. Call lou_charSize to understand character dimensions
    int char_size = lou_charSize();
    (void)char_size; // Use result to avoid unused variable warning
    
    // 6. Determine how many hyphenation operations to perform
    int operation_count = fdp.ConsumeIntegralInRange<int>(1, 10);
    
    // 7. Consume translation mode (deprecated but still used)
    int mode = fdp.ConsumeIntegral<int>();
    
    // Track success of at least one operation
    int any_success = 0;
    
    // 8. Perform multiple hyphenation operations with varied inputs
    for (int op = 0; op < operation_count; op++) {
        // Check if we have enough data for another operation
        if (fdp.remaining_bytes() < 4) break;
        
        // Generate test text for this operation
        std::string text_str = generate_test_text(fdp);
        if (text_str.empty()) {
            continue;
        }
        
        // Convert to widechar format
        int text_len = 0;
        widechar* text_buf = string_to_widechar(text_str, &text_len);
        if (!text_buf || text_len == 0) {
            free(text_buf);
            continue;
        }
        
        // Choose hyphenation scenario
        uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 5;
        
        switch (scenario) {
            case 0: {
                // Basic hyphenation test
                char* hyphens = (char*)malloc((text_len + 1) * sizeof(char));
                if (hyphens) {
                    int result = lou_hyphenate(table_to_use, text_buf, text_len, hyphens, mode);
                    any_success |= result;
                    free(hyphens);
                }
                break;
            }
            
            case 1: {
                // Hyphenation with pre-generated hyphen array
                char* hyphens = generate_hyphen_array(fdp, text_len);
                if (hyphens) {
                    int result = lou_hyphenate(table_to_use, text_buf, text_len, hyphens, mode);
                    any_success |= result;
                    free(hyphens);
                }
                break;
            }
            
            case 2: {
                // Test with empty or single character text (edge case)
                if (text_len <= 1) {
                    char* hyphens = (char*)malloc((text_len + 1) * sizeof(char));
                    if (hyphens) {
                        int result = lou_hyphenate(table_to_use, text_buf, text_len, hyphens, mode);
                        any_success |= result;
                        free(hyphens);
                    }
                }
                break;
            }
            
            case 3: {
                // Test with modified mode flags
                int modified_mode = mode;
                // Toggle various mode flags
                if (fdp.ConsumeBool()) modified_mode |= compbrlAtCursor;
                if (fdp.ConsumeBool()) modified_mode |= dotsIO;
                if (fdp.ConsumeBool()) modified_mode |= noUndefined;
                
                char* hyphens = (char*)malloc((text_len + 1) * sizeof(char));
                if (hyphens) {
                    int result = lou_hyphenate(table_to_use, text_buf, text_len, hyphens, modified_mode);
                    any_success |= result;
                    free(hyphens);
                }
                break;
            }
            
            case 4: {
                // Test hyphenation with different input lengths
                // Use substring of the text to test different lengths
                if (text_len > 1) {
                    int sub_len = fdp.ConsumeIntegralInRange<int>(1, text_len);
                    char* hyphens = (char*)malloc((sub_len + 1) * sizeof(char));
                    if (hyphens) {
                        int result = lou_hyphenate(table_to_use, text_buf, sub_len, hyphens, mode);
                        any_success |= result;
                        free(hyphens);
                    }
                }
                break;
            }
        }
        
        // Free text buffer for this operation
        free(text_buf);
        
        // Check if we should continue based on remaining input
        if (fdp.remaining_bytes() < 8) break;
    }
    
    // 9. Clean up dynamically found table
    if (found_table) {
        lou_freeTableFile(found_table);
    }
    
    // 10. Free liblouis resources
    lou_free();
    
    return 0;
}

} // namespace harness_006
