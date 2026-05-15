#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include "cjson/cJSON.h"

namespace harness_013 {
/*
 * harness_013.cpp - Unicode escape sequence parsing harness targeting utf16_literal_to_utf8 and parse_hex4
 * 
 * This harness specifically targets the internal functions utf16_literal_to_utf8 and parse_hex4
 * which have 0% coverage and 48 blocked branches handling Unicode escape sequences in cJSON.
 * 
 * Focus on JSON parsing with Unicode escape sequences (\uXXXX) via cJSON_Parse* APIs with
 * cleanup via cJSON_Delete. Target scenarios including:
 * 1) Valid Unicode escape sequences (\uXXXX)
 * 2) Invalid Unicode sequences (malformed hex, wrong ranges)
 * 3) Mixed ASCII and Unicode characters
 * 4) Surrogate pairs (\uD800-\uDBFF followed by \uDC00-\uDFFF)
 * 5) Edge cases with escape sequences in strings
 * 6) Nested Unicode in arrays/objects
 * 
 * Based on analysis of cJSON source code, we need to test:
 * - parse_hex4() function for parsing 4-digit hexadecimal numbers
 * - utf16_literal_to_utf8() function for converting UTF-16 to UTF-8
 * - Invalid UTF-16 sequences (lone surrogates, out-of-range values)
 * - Valid UTF-16 sequences (BMP characters, surrogate pairs)
 * - Edge cases: incomplete sequences, wrong hex digits
 * - Various JSON structures containing Unicode strings
 * 
 * Strategy: Generate diverse JSON strings with Unicode escape sequences using
 * FuzzedDataProvider, focusing on the 48 blocked branches in these functions.
 */


extern "C" {
}

// Helper function to generate a Unicode escape sequence
static std::string generate_unicode_escape(FuzzedDataProvider& fdp, bool allow_surrogate = false) {
    std::string result = "\\u";
    
    // Generate 4 hex digits
    for (int i = 0; i < 4; i++) {
        uint8_t hex_digit_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        char hex_char;
        
        switch (hex_digit_type) {
            case 0: // 0-9
                hex_char = '0' + (fdp.ConsumeIntegral<uint8_t>() % 10);
                break;
            case 1: // A-F
                hex_char = 'A' + (fdp.ConsumeIntegral<uint8_t>() % 6);
                break;
            case 2: // a-f
                hex_char = 'a' + (fdp.ConsumeIntegral<uint8_t>() % 6);
                break;
            default:
                hex_char = '0';
        }
        
        // Occasionally generate invalid hex digits to test error paths
        if (fdp.ConsumeProbability<float>() < 0.1) {
            uint8_t invalid_char_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (invalid_char_type) {
                case 0: hex_char = 'G'; break;  // Invalid uppercase
                case 1: hex_char = 'g'; break;  // Invalid lowercase
                case 2: hex_char = '!'; break;  // Non-alphanumeric
            }
        }
        
        result += hex_char;
    }
    
    return result;
}

// Helper to generate a surrogate pair
static std::string generate_surrogate_pair(FuzzedDataProvider& fdp) {
    std::string result;
    
    // Generate high surrogate (D800-DBFF)
    result += "\\uD";
    uint8_t high_surrogate_nibble = 8 + (fdp.ConsumeIntegral<uint8_t>() % 4); // 8, 9, A, B
    char high_char;
    if (high_surrogate_nibble < 10) {
        high_char = '0' + high_surrogate_nibble;
    } else {
        high_char = 'A' + (high_surrogate_nibble - 10);
    }
    result += high_char;
    
    // Last two hex digits of high surrogate (00-FF)
    for (int i = 0; i < 2; i++) {
        uint8_t hex_val = fdp.ConsumeIntegral<uint8_t>() % 16;
        char hex_char = (hex_val < 10) ? ('0' + hex_val) : ('A' + (hex_val - 10));
        result += hex_char;
    }
    
    // Generate low surrogate (DC00-DFFF)
    result += "\\uD";
    uint8_t low_surrogate_nibble = 0xC + (fdp.ConsumeIntegral<uint8_t>() % 4); // C, D, E, F
    char low_char;
    if (low_surrogate_nibble < 10) {
        low_char = '0' + low_surrogate_nibble;
    } else {
        low_char = 'A' + (low_surrogate_nibble - 10);
    }
    result += low_char;
    
    // Last two hex digits of low surrogate (00-FF)
    for (int i = 0; i < 2; i++) {
        uint8_t hex_val = fdp.ConsumeIntegral<uint8_t>() % 16;
        char hex_char = (hex_val < 10) ? ('0' + hex_val) : ('A' + (hex_val - 10));
        result += hex_char;
    }
    
    return result;
}

// Helper to generate JSON string with Unicode content
static std::string generate_unicode_json(FuzzedDataProvider& fdp, int depth) {
    if (depth <= 0 || fdp.remaining_bytes() < 16) {
        // Generate simple string value with possible Unicode
        std::string result = "\"";
        
        // Add some ASCII characters
        int ascii_len = fdp.ConsumeIntegralInRange<int>(0, 10);
        for (int i = 0; i < ascii_len; i++) {
            char c = fdp.ConsumeIntegralInRange<char>(32, 126);
            if (c == '\"' || c == '\\') {
                result += '\\';
            }
            result += c;
        }
        
        // Add Unicode escape sequences
        int unicode_count = fdp.ConsumeIntegralInRange<int>(0, 5);
        for (int i = 0; i < unicode_count && fdp.remaining_bytes() > 4; i++) {
            uint8_t unicode_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            
            switch (unicode_type) {
                case 0: // Regular Unicode escape
                    result += generate_unicode_escape(fdp);
                    break;
                case 1: // Surrogate pair
                    if (fdp.ConsumeBool()) {
                        result += generate_surrogate_pair(fdp);
                    } else {
                        // Lone surrogate (should fail)
                        result += generate_unicode_escape(fdp, true);
                    }
                    break;
                case 2: // Possibly invalid Unicode
                    result += generate_unicode_escape(fdp);
                    break;
            }
        }
        
        result += "\"";
        return result;
    }
    
    uint8_t structure_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    if (structure_type == 0) {
        // Generate array with Unicode strings
        int num_items = fdp.ConsumeIntegralInRange<int>(1, 5);
        std::string result = "[";
        
        for (int i = 0; i < num_items && fdp.remaining_bytes() > 8; i++) {
            if (i > 0) result += ",";
            result += generate_unicode_json(fdp, depth - 1);
        }
        
        result += "]";
        return result;
    } else if (structure_type == 1) {
        // Generate object with Unicode keys and values
        int num_pairs = fdp.ConsumeIntegralInRange<int>(1, 5);
        std::string result = "{";
        
        for (int i = 0; i < num_pairs && fdp.remaining_bytes() > 16; i++) {
            if (i > 0) result += ",";
            
            // Generate key with possible Unicode
            std::string key = "\"";
            int key_len = fdp.ConsumeIntegralInRange<int>(1, 8);
            for (int j = 0; j < key_len; j++) {
                if (fdp.ConsumeProbability<float>() < 0.3 && fdp.remaining_bytes() > 4) {
                    key += generate_unicode_escape(fdp);
                } else {
                    char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
                    key += c;
                }
            }
            key += "\"";
            
            result += key + ":";
            result += generate_unicode_json(fdp, depth - 1);
        }
        
        result += "}";
        return result;
    } else {
        // Mixed structure
        return generate_unicode_json(fdp, 0);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 8) {
        return 0;  // Minimum size for meaningful Unicode testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume parsing method selection
    uint8_t method = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume require_null_terminated flag
    bool require_null_terminated = fdp.ConsumeBool();
    
    // Consume Unicode test scenario type
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Generate JSON with Unicode content based on scenario
    std::string json_input;
    
    switch (scenario) {
        case 0: { // Valid Unicode sequences
            std::string prefix = fdp.ConsumeRandomLengthString(10);
            std::string suffix = fdp.ConsumeRandomLengthString(10);
            json_input = "{\"text\": \"" + prefix;
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 4; i++) {
                json_input += generate_unicode_escape(fdp);
            }
            json_input += suffix + "\"}";
            break;
        }
            
        case 1: { // Invalid Unicode sequences
            std::string label = fdp.ConsumeRandomLengthString(8);
            json_input = "{\"" + label + "\": \"";
            
            // Add some intentionally invalid sequences
            json_input += "\\uGGGG\\u!@#$\\u123";
            
            // Also add some fuzzed potentially invalid sequences
            if (fdp.remaining_bytes() > 10) {
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 4; i++) {
                    json_input += generate_unicode_escape(fdp);
                    // Corrupt some sequences
                    if (fdp.ConsumeBool()) {
                        json_input.pop_back(); // Remove last char
                        json_input += 'X'; // Invalid hex
                    }
                }
            }
            json_input += "\"}";
            break;
        }
            
        case 2: { // Mixed ASCII and Unicode
            std::string ascii_part = fdp.ConsumeRandomLengthString(15);
            std::string unicode_part;
            for (int i = 0; i < 5 && fdp.remaining_bytes() > 4; i++) {
                if (fdp.ConsumeBool()) {
                    char c = fdp.ConsumeIntegralInRange<char>(32, 126);
                    if (c == '\"' || c == '\\') unicode_part += '\\';
                    unicode_part += c;
                } else {
                    unicode_part += generate_unicode_escape(fdp);
                }
            }
            std::string end_part = fdp.ConsumeRandomLengthString(10);
            json_input = "\"" + ascii_part + unicode_part + end_part + "\"";
            break;
        }
            
        case 3: { // Surrogate pairs
            std::string key = fdp.ConsumeRandomLengthString(10);
            json_input = "{\"" + key + "\": \"";
            if (fdp.ConsumeBool()) {
                // Valid surrogate pair
                json_input += generate_surrogate_pair(fdp);
            } else {
                // Lone surrogate (invalid)
                json_input += generate_unicode_escape(fdp, true);
            }
            json_input += "\"}";
            break;
        }
            
        case 4: { // Escape sequences in strings with other escapes
            std::string line1 = fdp.ConsumeRandomLengthString(5);
            std::string line2 = fdp.ConsumeRandomLengthString(5);
            std::string tab_label = fdp.ConsumeRandomLengthString(3);
            json_input = "{\"complex\": \"" + line1 + "\\n" + line2 + "\\t" + tab_label;
            for (int i = 0; i < 2 && fdp.remaining_bytes() > 4; i++) {
                json_input += generate_unicode_escape(fdp);
            }
            json_input += "\\\"Quote\\\"\\\\Backslash\"}";
            break;
        }
            
        case 5: // Nested Unicode in arrays/objects
            json_input = generate_unicode_json(fdp, 2);
            break;
            
        default:
            json_input = "\"Test\\u0041\\u0042\\u0043\"";
    }
    
    cJSON* json_obj = nullptr;
    const char* parse_end = nullptr;
    
    switch (method) {
        case 0:
            // Test cJSON_Parse - basic parsing
            json_obj = cJSON_Parse(json_input.c_str());
            break;
            
        case 1:
            // Test cJSON_ParseWithLength - parsing with explicit length
            json_obj = cJSON_ParseWithLength(json_input.c_str(), json_input.size());
            break;
            
        case 2:
            // Test cJSON_ParseWithOpts - parsing with options
            json_obj = cJSON_ParseWithOpts(json_input.c_str(), &parse_end, require_null_terminated);
            break;
            
        case 3:
            // Test cJSON_ParseWithLengthOpts - parsing with length and options
            json_obj = cJSON_ParseWithLengthOpts(json_input.c_str(), json_input.size(), &parse_end, require_null_terminated);
            break;
    }
    
    // If parsing succeeded, perform additional operations to increase coverage
    if (json_obj != nullptr) {
        // Test string extraction to exercise Unicode decoding
        if (cJSON_IsString(json_obj)) {
            const char* str_value = cJSON_GetStringValue(json_obj);
            (void)str_value; // Use value to avoid compiler warnings
        }
        
        // Test printing to ensure Unicode is preserved
        char* printed = cJSON_Print(json_obj);
        if (printed != nullptr) {
            free(printed);
        }
        
        // Test unformatted printing
        char* printed_unformatted = cJSON_PrintUnformatted(json_obj);
        if (printed_unformatted != nullptr) {
            free(printed_unformatted);
        }
        
        // Test minify with Unicode strings
        if (!json_input.empty()) {
            char* input_copy = (char*)malloc(json_input.size() + 1);
            if (input_copy != nullptr) {
                memcpy(input_copy, json_input.c_str(), json_input.size());
                input_copy[json_input.size()] = '\0';
                cJSON_Minify(input_copy);
                free(input_copy);
            }
        }
        
        cJSON_Delete(json_obj);
    }
    
    return 0;
}

} // namespace harness_013
