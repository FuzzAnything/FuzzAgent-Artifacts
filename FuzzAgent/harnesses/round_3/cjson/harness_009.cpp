#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_009 {
/*
 * Harness 009: Comprehensive Unicode and String Escaping Tests for cJSON
 * 
 * Target: print_string_ptr function (47/54 blocked branches)
 * 
 * This harness focuses on testing Unicode handling, string escaping,
 * and control character processing in cJSON's printing functions.
 * 
 * Key coverage targets:
 * 1. NULL string input handling (lines 969-982: 0 hits)
 * 2. Control characters (<32) requiring UTF-16 escape sequences (lines 1000-1005)
 * 3. Various escape character cases (lines 988-995)
 * 4. Mixed ASCII/Unicode content
 * 5. Buffer boundary conditions
 * 6. Strings triggering buffer reallocation in ensure()
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 4) {
        return 0;  // Need minimum input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    uint8_t buffer_size_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool use_preallocated = fdp.ConsumeBool();
    bool include_null_string = fdp.ConsumeBool();
    bool test_round_trip = fdp.ConsumeBool();
    
    // Determine string length and content strategy based on test type
    std::string test_string;
    
    switch (test_type) {
        case 0: { // Empty string and NULL testing
            if (include_null_string) {
                // Test NULL string case
                cJSON* null_string_item = cJSON_CreateString(NULL);
                if (null_string_item != NULL) {
                    char* printed = cJSON_Print(null_string_item);
                    if (printed != NULL) {
                        free(printed);
                    }
                    cJSON_Delete(null_string_item);
                }
            }
            
            // Test empty string
            cJSON* empty_string_item = cJSON_CreateString("");
            if (empty_string_item != NULL) {
                char* printed = cJSON_Print(empty_string_item);
                if (printed != NULL) {
                    free(printed);
                }
                cJSON_Delete(empty_string_item);
            }
            return 0;
        }
            
        case 1: { // Control characters only (0-31)
            size_t control_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
            test_string.reserve(control_len);
            for (size_t i = 0; i < control_len && fdp.remaining_bytes() > 0; i++) {
                uint8_t control_char = fdp.ConsumeIntegralInRange<uint8_t>(0, 31);
                test_string.push_back(static_cast<char>(control_char));
            }
            break;
        }
            
        case 2: { // Special escape sequences only
            const char escape_chars[] = {'\"', '\\', '\b', '\f', '\n', '\r', '\t'};
            size_t escape_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
            test_string.reserve(escape_len);
            for (size_t i = 0; i < escape_len && fdp.remaining_bytes() > 0; i++) {
                uint8_t idx = fdp.ConsumeIntegral<uint8_t>() % 7;
                test_string.push_back(escape_chars[idx]);
            }
            break;
        }
            
        case 3: { // Mixed control and escape characters
            size_t mixed_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
            test_string.reserve(mixed_len);
            for (size_t i = 0; i < mixed_len && fdp.remaining_bytes() > 0; i++) {
                uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                if (char_type == 0) {
                    // Control character
                    test_string.push_back(static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(0, 31)));
                } else if (char_type == 1) {
                    // Escape character
                    const char escape_chars[] = {'\"', '\\', '\b', '\f', '\n', '\r', '\t'};
                    test_string.push_back(escape_chars[fdp.ConsumeIntegral<uint8_t>() % 7]);
                } else {
                    // Normal printable ASCII
                    test_string.push_back(static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(32, 126)));
                }
            }
            break;
        }
            
        case 4: { // Unicode characters (UTF-8)
            size_t unicode_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
            test_string.reserve(unicode_len * 4); // Maximum UTF-8 bytes per character
            
            // Generate UTF-8 sequences
            while (test_string.length() < unicode_len * 2 && fdp.remaining_bytes() >= 3) {
                uint8_t utf8_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                
                if (utf8_type == 0 && fdp.remaining_bytes() >= 2) {
                    // 2-byte UTF-8: U+0080 to U+07FF
                    uint16_t code_point = fdp.ConsumeIntegralInRange<uint16_t>(0x80, 0x7FF);
                    test_string.push_back(static_cast<char>(0xC0 | (code_point >> 6)));
                    test_string.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
                } else if (utf8_type == 1 && fdp.remaining_bytes() >= 3) {
                    // 3-byte UTF-8: U+0800 to U+FFFF
                    uint16_t code_point = fdp.ConsumeIntegralInRange<uint16_t>(0x800, 0xFFFF);
                    test_string.push_back(static_cast<char>(0xE0 | (code_point >> 12)));
                    test_string.push_back(static_cast<char>(0x80 | ((code_point >> 6) & 0x3F)));
                    test_string.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
                } else {
                    // Single byte (ASCII)
                    test_string.push_back(static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(32, 126)));
                }
            }
            break;
        }
            
        case 5: { // Maximum escape density (every character needs escaping)
            size_t max_escape_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
            test_string.reserve(max_escape_len);
            for (size_t i = 0; i < max_escape_len && fdp.remaining_bytes() > 0; i++) {
                // Alternate between control chars and escape chars
                if (i % 2 == 0) {
                    test_string.push_back(static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(0, 31)));
                } else {
                    const char escape_chars[] = {'\"', '\\', '\b', '\f', '\n', '\r', '\t'};
                    test_string.push_back(escape_chars[fdp.ConsumeIntegral<uint8_t>() % 7]);
                }
            }
            break;
        }
            
        case 6: { // Buffer boundary testing (very long strings)
            size_t long_len = fdp.ConsumeIntegralInRange<size_t>(100, 1000);
            test_string.reserve(long_len);
            for (size_t i = 0; i < long_len && fdp.remaining_bytes() > 0; i++) {
                uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 10;
                if (char_type < 2) {
                    // 20% control characters
                    test_string.push_back(static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(0, 31)));
                } else if (char_type < 4) {
                    // 20% escape characters
                    const char escape_chars[] = {'\"', '\\', '\b', '\f', '\n', '\r', '\t'};
                    test_string.push_back(escape_chars[fdp.ConsumeIntegral<uint8_t>() % 7]);
                } else {
                    // 60% normal characters
                    test_string.push_back(static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(32, 126)));
                }
            }
            break;
        }
            
        case 7: { // Random mixed content
            size_t random_len = fdp.ConsumeIntegralInRange<size_t>(1, 200);
            test_string = fdp.ConsumeRandomLengthString(random_len);
            break;
        }
    }
    
    // Create cJSON string object
    cJSON* string_item = cJSON_CreateString(test_string.c_str());
    if (string_item == NULL) {
        return 0;
    }
    
    // Determine buffer size for testing
    int prebuffer_size;
    switch (buffer_size_type) {
        case 0:  // Very small buffer (likely triggers reallocation)
            prebuffer_size = fdp.ConsumeIntegralInRange<int>(1, 10);
            break;
        case 1:  // Small buffer
            prebuffer_size = fdp.ConsumeIntegralInRange<int>(11, 100);
            break;
        case 2:  // Medium buffer
            prebuffer_size = fdp.ConsumeIntegralInRange<int>(101, 1000);
            break;
        case 3:  // Large buffer
        default:
            prebuffer_size = fdp.ConsumeIntegralInRange<int>(1001, 10000);
            break;
    }
    
    // Test cJSON_PrintBuffered with various buffer sizes
    char* buffered_print = cJSON_PrintBuffered(string_item, prebuffer_size, false);
    if (buffered_print != NULL) {
        // Test round-trip parsing if enabled
        if (test_round_trip && fdp.remaining_bytes() > 0) {
            cJSON* parsed = cJSON_Parse(buffered_print);
            if (parsed != NULL) {
                // Compare the parsed result with original (simplified comparison)
                if (cJSON_IsString(parsed)) {
                    const char* parsed_string = cJSON_GetStringValue(parsed);
                    // Note: We don't compare strings directly because escaping changes them
                    // The important part is that parsing succeeded
                }
                cJSON_Delete(parsed);
            }
        }
        free(buffered_print);
    }
    
    // Test cJSON_PrintPreallocated if enabled
    if (use_preallocated && fdp.remaining_bytes() > 0) {
        // Allocate buffer for preallocated printing
        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(10, 10000);
        char* prealloc_buffer = (char*)malloc(buffer_size);
        if (prealloc_buffer != NULL) {
            cJSON_bool print_success = cJSON_PrintPreallocated(string_item, prealloc_buffer, buffer_size, false);
            // Success or failure are both valid test outcomes
            free(prealloc_buffer);
        }
    }
    
    // Test regular cJSON_Print
    char* regular_print = cJSON_Print(string_item);
    if (regular_print != NULL) {
        free(regular_print);
    }
    
    // Test cJSON_PrintUnformatted
    char* unformatted_print = cJSON_PrintUnformatted(string_item);
    if (unformatted_print != NULL) {
        free(unformatted_print);
    }
    
    // Cleanup
    cJSON_Delete(string_item);
    
    return 0;
}

} // namespace harness_009
