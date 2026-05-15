#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include "cjson/cJSON.h"

namespace harness_023 {
/*
 * harness_023.cpp - Targeted testing of print_string_ptr function for branch coverage
 * 
 * This harness systematically tests the print_string_ptr function which has
 * 50/54 blocked branches (80.89% coverage). The function handles string
 * escaping for JSON output, including:
 * 1) NULL input buffers (returns false)
 * 2) Empty strings (prints "")
 * 3) Normal characters (copied directly)
 * 4) Escape sequences: ", \, \b, \f, \n, \r, \t
 * 5) Characters < 32 (UTF-16 escape sequences uXXXX)
 * 
 * Since print_string_ptr is a static function, we test it indirectly through
 * public APIs that call it:
 * - cJSON_Print / cJSON_PrintUnformatted (via print_string)
 * - cJSON_PrintBuffered / cJSON_PrintPreallocated (via print_string)
 * - Object key printing (via print_object)
 * 
 * Coverage gaps to address:
 * 1) The switch statement in print_string_ptr (lines 1038-1066)
 * 2) The early return paths for NULL output_buffer (line 963)
 * 3) The empty string handling (lines 969-978)
 * 4) The escape character counting loop (lines 982-1004)
 * 5) The no-escape fast path (lines 1014-1022)
 * 
 * Strategy: Generate diverse string content with systematic edge cases
 * and use different printing methods to maximize branch coverage.
 */


extern "C" {
}

// Generate strings with specific character classes to test different escape paths
static std::string generate_test_string(FuzzedDataProvider& fdp) {
    uint8_t string_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    if (string_type == 0) {
        return std::string();  // NULL string (handled specially in caller)
    } else if (string_type == 1) {
        return "";  // Empty string
    } else if (string_type == 2) {
        return fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 100));
    } else if (string_type == 3) {
        // String with common escape sequences
        std::string escape_result;
        size_t length = fdp.ConsumeIntegralInRange<size_t>(5, 50);
        for (size_t i = 0; i < length; i++) {
            uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 7;
            switch (char_type) {
                case 0: escape_result += '\"'; break;   // double quote
                case 1: escape_result += '\\'; break;   // backslash
                case 2: escape_result += '\b'; break;   // backspace
                case 3: escape_result += '\f'; break;   // form feed
                case 4: escape_result += '\n'; break;   // newline
                case 5: escape_result += '\r'; break;   // carriage return
                case 6: escape_result += '\t'; break;   // tab
            }
        }
        return escape_result;
    } else if (string_type == 4) {
        // String with control characters (< 32) that need UTF-16 escaping
        std::string control_result;
        size_t length = fdp.ConsumeIntegralInRange<size_t>(3, 30);
        for (size_t i = 0; i < length; i++) {
            char c = static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(0, 31));
            if (c == 0) c = 1;  // Skip null terminator
            control_result += c;
        }
        return control_result;
    } else if (string_type == 5) {
        // Mixed content: normal chars + escape sequences + control chars
        std::string mixed_result = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(10, 100));
        if (!mixed_result.empty()) {
            size_t insert_pos = fdp.ConsumeIntegralInRange<size_t>(0, mixed_result.length());
            if (insert_pos < mixed_result.length()) {
                mixed_result.insert(insert_pos, 1, '\"');
            }
            insert_pos = fdp.ConsumeIntegralInRange<size_t>(0, mixed_result.length());
            if (insert_pos < mixed_result.length()) {
                mixed_result.insert(insert_pos, 1, '\n');
            }
            insert_pos = fdp.ConsumeIntegralInRange<size_t>(0, mixed_result.length());
            if (insert_pos < mixed_result.length()) {
                mixed_result.insert(insert_pos, 1, static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(1, 31)));
            }
        }
        return mixed_result;
    } else if (string_type == 6) {
        // String that triggers the no-escape fast path
        std::string fast_result;
        size_t length = fdp.ConsumeIntegralInRange<size_t>(1, 100);
        for (size_t i = 0; i < length; i++) {
            char c;
            do {
                c = static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(32, 126));
            } while (c == '\"' || c == '\\');
            fast_result += c;
        }
        return fast_result;
    } else if (string_type == 7) {
        // Very long string to test buffer allocation
        return fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1000, 10000));
    }
    
    return "default";
}

// Test different printing methods
static void test_printing_method(FuzzedDataProvider& fdp, cJSON* string_item, uint8_t method) {
    if (string_item == nullptr) return;
    
    switch (method % 5) {
        case 0: // cJSON_Print (formatted)
        {
            char* printed = cJSON_Print(string_item);
            if (printed != nullptr) {
                free(printed);
            }
            break;
        }
        
        case 1: // cJSON_PrintUnformatted
        {
            char* printed = cJSON_PrintUnformatted(string_item);
            if (printed != nullptr) {
                free(printed);
            }
            break;
        }
        
        case 2: // cJSON_PrintBuffered with random buffer size
        {
            int buffer_size = fdp.ConsumeIntegralInRange<int>(1, 10000);
            bool formatted = fdp.ConsumeBool();
            char* printed = cJSON_PrintBuffered(string_item, buffer_size, formatted ? 1 : 0);
            if (printed != nullptr) {
                free(printed);
            }
            break;
        }
        
        case 3: // cJSON_PrintPreallocated with allocated buffer
        {
            // Estimate required buffer size: string length * 2 (for escapes) + quotes + null
            const char* str = cJSON_GetStringValue(string_item);
            size_t estimated_size = 0;
            if (str != nullptr) {
                estimated_size = strlen(str) * 2 + 3;
            } else {
                estimated_size = 3; // For empty string ""
            }
            
            if (estimated_size > 0 && estimated_size < 100000) {
                char* buffer = (char*)malloc(estimated_size);
                if (buffer != nullptr) {
                    bool formatted = fdp.ConsumeBool();
                    cJSON_PrintPreallocated(string_item, buffer, estimated_size, formatted ? 1 : 0);
                    free(buffer);
                }
            }
            break;
        }
        
        case 4: // Test as object key (triggers print_string_ptr differently)
        {
            cJSON* obj = cJSON_CreateObject();
            if (obj != nullptr) {
                // Use the string as an object key
                const char* key_str = cJSON_GetStringValue(string_item);
                if (key_str != nullptr) {
                    cJSON_AddItemToObject(obj, key_str, cJSON_CreateNull());
                }
                
                // Print the object to trigger print_string_ptr for the key
                char* printed = cJSON_Print(obj);
                if (printed != nullptr) {
                    free(printed);
                }
                
                cJSON_Delete(obj);
            }
            break;
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 8) {
        return 0; // Need minimum input for string type selection
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Generate test string based on fuzzed input
    std::string test_string = generate_test_string(fdp);
    
    // Test different scenarios for string creation
    uint8_t creation_method = fdp.ConsumeIntegral<uint8_t>() % 4;
    cJSON* string_item = nullptr;
    
    switch (creation_method) {
        case 0: // Normal string creation
            if (!test_string.empty()) {
                string_item = cJSON_CreateString(test_string.c_str());
            }
            break;
            
        case 1: // String reference (tests different memory handling)
            if (!test_string.empty()) {
                string_item = cJSON_CreateStringReference(test_string.c_str());
            }
            break;
            
        case 2: // Test with NULL string pointer
            // Explicitly test NULL string handling
            string_item = cJSON_CreateString(nullptr);
            break;
            
        case 3: // Create string array with single element to test array printing
            if (!test_string.empty()) {
                const char* strings[] = { test_string.c_str() };
                string_item = cJSON_CreateStringArray(strings, 1);
            }
            break;
    }
    
    // If we have a valid string item, test different printing methods
    if (string_item != nullptr) {
        // Consume which printing method to test
        uint8_t print_method = fdp.ConsumeIntegral<uint8_t>() % 5;
        test_printing_method(fdp, string_item, print_method);
        
        // Also test string value retrieval
        const char* retrieved = cJSON_GetStringValue(string_item);
        (void)retrieved; // Use to avoid unused variable warning
        
        cJSON_Delete(string_item);
    } else {
        // Test edge case: printing a non-string item (should not call print_string_ptr)
        // This helps ensure we're only hitting print_string_ptr when appropriate
        cJSON* null_item = cJSON_CreateNull();
        if (null_item != nullptr) {
            char* printed = cJSON_Print(null_item);
            if (printed != nullptr) {
                free(printed);
            }
            cJSON_Delete(null_item);
        }
    }
    
    // Additional test: Direct string printing via parse/print cycle
    if (!test_string.empty() && test_string.length() < 1000) {
        // Create a JSON string literal from our test string
        std::string json_input = "\"" + test_string + "\"";
        cJSON* parsed = cJSON_Parse(json_input.c_str());
        if (parsed != nullptr) {
            char* printed = cJSON_Print(parsed);
            if (printed != nullptr) {
                free(printed);
            }
            cJSON_Delete(parsed);
        }
    }
    
    return 0;
}

} // namespace harness_023
