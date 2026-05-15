#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "cjson/cJSON.h"

namespace harness_027 {
/*
 * harness_027.cpp - Targeted string formatting & escaping coverage testing
 *                   targeting print_string_ptr function with 50 blocked branches
 * 
 * This harness targets the 50/54 blocked branches in print_string_ptr with focus on:
 * 1) Strings with various escape sequences: \\", \\\\, \\b, \\f, \\n, \\r, \\t
 * 2) Unicode characters requiring UTF-16 escape sequences (uXXXX)
 * 3) Strings with control characters (< 32)
 * 4) Empty strings and NULL string handling
 * 5) Buffer size edge cases for cJSON_PrintBuffered
 * 
 * Public APIs targeted:
 * - cJSON_PrintBuffered (Primary)
 * - cJSON_PrintPreallocated
 * - cJSON_Print
 * - cJSON_PrintUnformatted
 * 
 * Semantic differentiation from existing harnesses:
 * - Systematic testing of all escape sequence types
 * - Focus on control character boundary (character < 32)
 * - Testing NULL string pointers via cJSON_CreateStringReference(NULL)
 * - Buffer size edge cases (small, exact, large) for buffered printing
 * - Preallocated buffer testing with exact sizing scenarios
 * 
 * Coverage gaps specifically targeted:
 * 1) All switch cases in print_string_ptr escape handling (lines 986-1066)
 * 2) NULL input path (lines 969-979)
 * 3) No escape characters optimization (lines 1014-1022)
 * 4) UTF-16 escape sequence generation for control characters (lines 999-1001)
 * 5) Buffer size edge cases in ensure() calls within print_string_ptr
 */


extern "C" {
}

// Helper to create strings with specific escape character patterns using fuzzer input
static std::string create_string_with_escape_pattern(FuzzedDataProvider& fdp, uint8_t pattern_type) {
    std::string result;
    
    switch (pattern_type % 8) {
        case 0:
            // Basic escape sequences - generate string with random placement of escape chars
            {
                std::string base = fdp.ConsumeRandomLengthString(64);
                // Insert escape characters at random positions
                size_t len = base.length();
                if (len > 0) {
                    // Randomly replace some characters with escape sequences
                    size_t num_escapes = fdp.ConsumeIntegralInRange<size_t>(0, std::min(len, static_cast<size_t>(10)));
                    for (size_t i = 0; i < num_escapes; i++) {
                        size_t pos = fdp.ConsumeIntegralInRange<size_t>(0, len - 1);
                        uint8_t escape_type = fdp.ConsumeIntegral<uint8_t>() % 7;
                        switch (escape_type) {
                            case 0: base[pos] = '\"'; break;
                            case 1: base[pos] = '\\'; break;
                            case 2: base[pos] = '\b'; break;
                            case 3: base[pos] = '\f'; break;
                            case 4: base[pos] = '\n'; break;
                            case 5: base[pos] = '\r'; break;
                            case 6: base[pos] = '\t'; break;
                        }
                    }
                }
                result = base;
            }
            break;
        case 1:
            // Control characters (0-31) - generate string with control chars
            {
                size_t length = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                result.reserve(length);
                for (size_t i = 0; i < length; i++) {
                    uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                    if (char_type == 0) {
                        // Control character < 32
                        result.push_back(static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(0, 31)));
                    } else if (char_type == 1) {
                        // Normal printable character
                        result.push_back(static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(32, 126)));
                    } else {
                        // Escape sequence character
                        uint8_t escape_type = fdp.ConsumeIntegral<uint8_t>() % 7;
                        switch (escape_type) {
                            case 0: result.push_back('\"'); break;
                            case 1: result.push_back('\\'); break;
                            case 2: result.push_back('\b'); break;
                            case 3: result.push_back('\f'); break;
                            case 4: result.push_back('\n'); break;
                            case 5: result.push_back('\r'); break;
                            case 6: result.push_back('\t'); break;
                        }
                    }
                }
            }
            break;
        case 2:
            // Mixed escape and normal characters - use consumed bytes directly
            result = fdp.ConsumeRandomLengthString(64);
            break;
        case 3:
            // Unicode-like sequences - simulate with control chars that need uXXXX escape
            {
                size_t length = fdp.ConsumeIntegralInRange<size_t>(1, 30);
                result.reserve(length);
                for (size_t i = 0; i < length; i++) {
                    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
                        // Add a control character that requires uXXXX escape
                        result.push_back(static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(0, 31)));
                    } else {
                        // Add normal text
                        result.push_back(static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(32, 126)));
                    }
                }
            }
            break;
        case 4:
            // Many quotes - generate string with many quotes
            {
                size_t num_quotes = fdp.ConsumeIntegralInRange<size_t>(1, 20);
                result.reserve(num_quotes + 10);
                for (size_t i = 0; i < num_quotes; i++) {
                    result.push_back('\"');
                    if (fdp.ConsumeBool() && i < num_quotes - 1) {
                        result.push_back(static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(32, 126)));
                    }
                }
            }
            break;
        case 5:
            // Many backslashes - generate string with many backslashes
            {
                size_t num_backslashes = fdp.ConsumeIntegralInRange<size_t>(1, 20);
                result.reserve(num_backslashes + 10);
                for (size_t i = 0; i < num_backslashes; i++) {
                    result.push_back('\\');
                    if (fdp.ConsumeBool() && i < num_backslashes - 1) {
                        result.push_back(static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(32, 126)));
                    }
                }
            }
            break;
        case 6:
            // All escape types - mix all types
            {
                size_t length = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                result.reserve(length);
                for (size_t i = 0; i < length; i++) {
                    uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 10;
                    if (char_type < 7) {
                        // One of the 7 standard escape sequences
                        switch (char_type) {
                            case 0: result.push_back('\"'); break;
                            case 1: result.push_back('\\'); break;
                            case 2: result.push_back('\b'); break;
                            case 3: result.push_back('\f'); break;
                            case 4: result.push_back('\n'); break;
                            case 5: result.push_back('\r'); break;
                            case 6: result.push_back('\t'); break;
                        }
                    } else if (char_type == 7 || char_type == 8) {
                        // Control character < 32
                        result.push_back(static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(0, 31)));
                    } else {
                        // Normal printable character
                        result.push_back(static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(32, 126)));
                    }
                }
            }
            break;
        case 7:
            // Empty or minimal
            if (fdp.ConsumeBool()) {
                result = "";
            } else {
                result = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 10));
            }
            break;
    }
    
    // Possibly add random suffix if we have enough bytes
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        size_t suffix_len = fdp.ConsumeIntegralInRange<size_t>(0, std::min(static_cast<size_t>(32), fdp.remaining_bytes()));
        result += fdp.ConsumeBytesAsString(suffix_len);
    }
    
    return result;
}

// Helper to create JSON objects with strings targeting different scenarios
static cJSON* create_string_test_object(FuzzedDataProvider& fdp) {
    cJSON* root = cJSON_CreateObject();
    if (!root) return nullptr;
    
    // Add strings with various escape patterns
    uint8_t num_strings = fdp.ConsumeIntegralInRange<uint8_t>(1, 5);
    for (uint8_t i = 0; i < num_strings; i++) {
        std::string key = "str" + std::to_string(i);
        std::string value = create_string_with_escape_pattern(fdp, i);
        
        // Randomly use regular string or string reference
        if (fdp.ConsumeBool()) {
            cJSON_AddStringToObject(root, key.c_str(), value.c_str());
        } else {
            cJSON* str_obj = cJSON_CreateString(value.c_str());  // FIXED: Use CreateString instead of CreateStringReference
            if (str_obj) {
                cJSON_AddItemToObject(root, key.c_str(), str_obj);
            }
        }
    }
    
    // Add a NULL string reference (NULL is okay since it's a constant)
    if (fdp.ConsumeBool()) {
        cJSON* null_str = cJSON_CreateStringReference(nullptr);
        if (null_str) {
            cJSON_AddItemToObject(root, "null_str", null_str);
        }
    }
    
    // Add other value types for context
    if (fdp.ConsumeBool()) {
        cJSON_AddNumberToObject(root, "number", fdp.ConsumeFloatingPoint<double>());
    }
    if (fdp.ConsumeBool()) {
        cJSON_AddBoolToObject(root, "boolean", fdp.ConsumeBool());
    }
    if (fdp.ConsumeBool()) {
        cJSON_AddNullToObject(root, "null_value");
    }
    
    return root;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 4) {
        return 0;  // Minimum size for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test configuration
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    int prebuffer_size = fdp.ConsumeIntegralInRange<int>(-10, 2048);  // Negative for edge cases
    bool formatted = fdp.ConsumeBool();
    
    // Create test object with string scenarios
    cJSON* test_obj = create_string_test_object(fdp);
    if (!test_obj) {
        return 0;
    }
    
    // Test different printing APIs based on scenario
    switch (test_scenario) {
        case 0: {
            // Test cJSON_PrintBuffered with various buffer sizes
            int effective_buffer = prebuffer_size;
            if (effective_buffer < 1) {
                effective_buffer = 1;  // Minimum valid buffer
            }
            
            char* buffered = cJSON_PrintBuffered(test_obj, effective_buffer, formatted);
            if (buffered) {
                free(buffered);
            }
            break;
        }
        
        case 1: {
            // Test cJSON_PrintPreallocated with exact buffer sizing
            // First get required size by printing unformatted
            char* temp = formatted ? cJSON_Print(test_obj) : cJSON_PrintUnformatted(test_obj);
            if (temp) {
                size_t required_len = strlen(temp) + 1;
                free(temp);
                
                // Test with different buffer sizes
                std::vector<char> buffer(required_len + prebuffer_size);
                if (!buffer.empty()) {
                    cJSON_PrintPreallocated(test_obj, buffer.data(), buffer.size(), formatted);
                }
                
                // Also test with small buffer
                if (required_len > 5) {
                    std::vector<char> small_buffer(5);
                    cJSON_PrintPreallocated(test_obj, small_buffer.data(), small_buffer.size(), formatted);
                }
            }
            break;
        }
        
        case 2: {
            // Test regular printing functions
            if (formatted) {
                char* printed = cJSON_Print(test_obj);
                if (printed) {
                    free(printed);
                }
            } else {
                char* printed = cJSON_PrintUnformatted(test_obj);
                if (printed) {
                    free(printed);
                }
            }
            break;
        }
        
        case 3: {
            // Test all printing functions in sequence
            char* printed1 = cJSON_Print(test_obj);
            if (printed1) free(printed1);
            
            char* printed2 = cJSON_PrintUnformatted(test_obj);
            if (printed2) free(printed2);
            
            char* printed3 = cJSON_PrintBuffered(test_obj, 
                fdp.ConsumeIntegralInRange<int>(1, 1024), 
                fdp.ConsumeBool());
            if (printed3) free(printed3);
            
            // Try preallocated with conservative buffer
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
            std::vector<char> buffer(buffer_size);
            if (!buffer.empty()) {
                cJSON_PrintPreallocated(test_obj, buffer.data(), buffer.size(), 
                    fdp.ConsumeBool());
            }
            break;
        }
    }
    
    // Cleanup
    cJSON_Delete(test_obj);
    
    return 0;
}

} // namespace harness_027
