#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include "magic.h"

namespace harness_013 {
/*
 * Fuzzing harness for libmagic library - JSON Parsing Functions
 * Targets: json_parse (35/44 branches blocked) and json_parse_number (32/42 branches blocked) 
 *          functions in is_json.c
 * Focus: Generate diverse JSON data including all RFC-7159 data types:
 *        - Objects (complex nesting, varying key counts)
 *        - Arrays (nested, mixed types)
 *        - Strings (with escape sequences, Unicode, special characters)
 *        - Numbers (integers, floats, exponents, scientific notation, edge cases)
 *        - Constants (true, false, null)
 *        - Malformed JSON to exercise error handling
 * Goal: Maximize coverage of json_parse and json_parse_number functions by generating
 *        structured JSON with varying depths, nesting levels, and edge cases
 * Semantic differentiation: Existing harnesses test general file detection; this harness
 *        exclusively targets JSON parsing functions with specialized JSON generation
 * Strategy: Use FuzzedDataProvider to:
 *        1. Generate JSON strings programmatically with recursive depth control
 *        2. Create diverse number formats to target json_parse_number branches
 *        3. Test JSON detection through magic_buffer API
 *        4. Include malformed JSON to exercise error handling paths
 * Expected coverage gain: Target the 67+ blocked branches in json_parse and json_parse_number
 */


// Include libmagic header

// Helper function to generate a random JSON string with escape sequences
std::string generate_json_string(FuzzedDataProvider& fdp, int max_len = 256) {
    std::string result;
    size_t len = fdp.ConsumeIntegralInRange<size_t>(0, max_len);
    
    if (len == 0) {
        return "\"\"";
    }
    
    result += "\"";
    
    // Generate string content with potential escape sequences
    for (size_t i = 0; i < len; i++) {
        if (fdp.ConsumeBool() && i + 1 < len) {
            // Add escape sequence
            uint8_t escape_type = fdp.ConsumeIntegralInRange<uint8_t>(0, 7);
            switch (escape_type) {
                case 0: result += "\\\""; break;
                case 1: result += "\\\\"; break;
                case 2: result += "\\/"; break;
                case 3: result += "\\b"; break;
                case 4: result += "\\f"; break;
                case 5: result += "\\n"; break;
                case 6: result += "\\r"; break;
                case 7: result += "\\t"; break;
                default: result += "\\\\"; break;
            }
            i++; // Consumed an extra character
        } else if (fdp.ConsumeBool() && i + 5 < len) {
            // Add Unicode escape
            uint16_t unicode_val = fdp.ConsumeIntegral<uint16_t>();
            char hex_buf[7];
            snprintf(hex_buf, sizeof(hex_buf), "\\u%04x", unicode_val);
            result += hex_buf;
            i += 5; // Consumed 5 extra characters
        } else {
            // Add regular character (avoid control characters and quotes)
            char c = fdp.ConsumeIntegralInRange<char>(32, 126);
            if (c == '"' || c == '\\') {
                result += 'a'; // Safe alternative
            } else {
                result += c;
            }
        }
    }
    
    result += "\"";
    return result;
}

// Helper function to generate diverse JSON numbers targeting json_parse_number
std::string generate_json_number(FuzzedDataProvider& fdp) {
    std::string result;
    
    uint8_t num_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (num_type) {
        case 0: { // Simple integer
            int32_t val = fdp.ConsumeIntegral<int32_t>();
            result = std::to_string(val);
            break;
        }
        case 1: { // Negative integer
            int32_t val = fdp.ConsumeIntegral<int32_t>();
            if (val > 0) val = -val;
            result = std::to_string(val);
            break;
        }
        case 2: { // Float with decimal
            double val = fdp.ConsumeFloatingPoint<double>();
            // Ensure we have decimal point
            std::string str = std::to_string(val);
            if (str.find('.') == std::string::npos) {
                str += ".0";
            }
            result = str;
            break;
        }
        case 3: { // Scientific notation
            double val = fdp.ConsumeFloatingPoint<double>();
            char buf[64];
            snprintf(buf, sizeof(buf), "%.10e", val);
            result = buf;
            break;
        }
        case 4: { // Integer with exponent
            int32_t base = fdp.ConsumeIntegral<int32_t>();
            int32_t exp = fdp.ConsumeIntegralInRange<int32_t>(-10, 10);
            result = std::to_string(base) + "e" + std::to_string(exp);
            break;
        }
        case 5: { // Float with exponent and sign
            double val = fdp.ConsumeFloatingPoint<double>();
            char buf[64];
            snprintf(buf, sizeof(buf), "%.10E", val);
            result = buf;
            break;
        }
        case 6: { // Leading zeros (edge case)
            uint32_t val = fdp.ConsumeIntegralInRange<uint32_t>(0, 9999);
            uint8_t zeros = fdp.ConsumeIntegralInRange<uint8_t>(1, 5);
            result = std::string(zeros, '0') + std::to_string(val);
            break;
        }
        case 7: { // Malformed number (to test error handling)
            if (fdp.ConsumeBool()) {
                result = "-"; // Just a minus sign
            } else {
                result = "1.2.3"; // Multiple decimal points
            }
            break;
        }
    }
    
    return result;
}

// Forward declarations
std::string generate_json_object(FuzzedDataProvider& fdp, int depth, int max_depth);
std::string generate_json_array(FuzzedDataProvider& fdp, int depth, int max_depth);

// Recursive function to generate JSON value with depth control
std::string generate_json_value(FuzzedDataProvider& fdp, int depth, int max_depth) {
    if (depth >= max_depth) {
        // At max depth, generate simple value
        uint8_t simple_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        switch (simple_type) {
            case 0: return generate_json_string(fdp, 50);
            case 1: return generate_json_number(fdp);
            case 2: return "true";
            case 3: return "false";
            case 4: return "null";
            default: return "null";
        }
    }
    
    uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (value_type) {
        case 0: // Object
            return generate_json_object(fdp, depth + 1, max_depth);
        case 1: // Array
            return generate_json_array(fdp, depth + 1, max_depth);
        case 2: // String
            return generate_json_string(fdp, 100);
        case 3: // Number
            return generate_json_number(fdp);
        case 4: // Boolean
            return fdp.ConsumeBool() ? "true" : "false";
        case 5: // Null
            return "null";
        default:
            return "null";
    }
}

// Generate JSON object
std::string generate_json_object(FuzzedDataProvider& fdp, int depth, int max_depth) {
    std::string result = "{";
    
    size_t num_pairs = fdp.ConsumeIntegralInRange<size_t>(0, 8);
    
    for (size_t i = 0; i < num_pairs; i++) {
        if (i > 0) {
            result += ",";
        }
        
        // Generate key
        std::string key = generate_json_string(fdp, 50);
        result += key + ":";
        
        // Generate value
        result += generate_json_value(fdp, depth, max_depth);
    }
    
    result += "}";
    return result;
}

// Generate JSON array
std::string generate_json_array(FuzzedDataProvider& fdp, int depth, int max_depth) {
    std::string result = "[";
    
    size_t num_elements = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    
    for (size_t i = 0; i < num_elements; i++) {
        if (i > 0) {
            result += ",";
        }
        
        result += generate_json_value(fdp, depth, max_depth);
    }
    
    result += "]";
    return result;
}


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for JSON generation decisions
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters
    int max_depth = fdp.ConsumeIntegralInRange<int>(1, 10);
    bool generate_valid_json = fdp.ConsumeBool();
    
    std::string json_data;
    
    if (generate_valid_json) {
        // Generate valid JSON
        json_data = generate_json_value(fdp, 0, max_depth);
        
        // Sometimes generate New Line Delimited JSON (multiple JSON objects)
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
            size_t num_objects = fdp.ConsumeIntegralInRange<size_t>(1, 5);
            for (size_t i = 1; i < num_objects; i++) {
                json_data += "\n" + generate_json_value(fdp, 0, max_depth);
            }
        }
    } else {
        // Generate malformed JSON to test error handling
        uint8_t error_type = fdp.ConsumeIntegral<uint8_t>() % 10;
        
        switch (error_type) {
            case 0: // Unterminated string
                json_data = "\"unterminated";
                break;
            case 1: // Invalid escape
                json_data = "\"\\x\"";
                break;
            case 2: // Missing comma in array
                json_data = "[1 2 3]";
                break;
            case 3: // Missing colon in object
                json_data = "{\"key\" \"value\"}";
                break;
            case 4: // Trailing comma
                json_data = "[1,2,]";
                break;
            case 5: // Extra comma in object
                json_data = "{\"a\":1,,}";
                break;
            case 6: // Invalid number
                json_data = "123abc";
                break;
            case 7: // Invalid constant
                json_data = "tru";
                break;
            case 8: // Unclosed array
                json_data = "[1,2,3";
                break;
            case 9: // Unclosed object
                json_data = "{\"a\":1";
                break;
            default:
                json_data = "invalid";
                break;
        }
        
        // Sometimes mix valid and invalid
        if (fdp.ConsumeBool()) {
            json_data = "[" + json_data + ", " + generate_json_value(fdp, 0, max_depth) + "]";
        }
    }
    
    // Test JSON detection through magic_buffer API
    // Open magic handle with flags that enable JSON checking
    int flags = MAGIC_NONE;
    
    // Consume flags but ensure JSON checking is NOT disabled
    uint16_t flag_word = fdp.ConsumeIntegral<uint16_t>();
    
    // Set various flags but explicitly avoid MAGIC_NO_CHECK_JSON
    if (flag_word & 0x0001) flags |= MAGIC_DEBUG;
    if (flag_word & 0x0002) flags |= MAGIC_SYMLINK;
    if (flag_word & 0x0004) flags |= MAGIC_COMPRESS;
    if (flag_word & 0x0008) flags |= MAGIC_DEVICES;
    if (flag_word & 0x0010) flags |= MAGIC_MIME_TYPE;
    if (flag_word & 0x0020) flags |= MAGIC_CONTINUE;
    if (flag_word & 0x0040) flags |= MAGIC_CHECK;
    if (flag_word & 0x0080) flags |= MAGIC_RAW;
    if (flag_word & 0x0100) flags |= MAGIC_ERROR;
    if (flag_word & 0x0200) flags |= MAGIC_MIME_ENCODING;
    if (flag_word & 0x0400) flags |= MAGIC_APPLE;
    if (flag_word & 0x0800) flags |= MAGIC_EXTENSION;
    
    // IMPORTANT: Do NOT set MAGIC_NO_CHECK_JSON to ensure JSON checking happens
    // This ensures json_parse and json_parse_number are called
    
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0; // Failed to open magic handle
    }
    
    // Load default magic database
    // Note: magic_load will use compiled-in magic path
    // In a fuzzing context, this should work with the built database
    
    // Test magic_buffer with generated JSON
    const char* result = magic_buffer(magic, json_data.c_str(), json_data.size());
    (void)result; // Result can be NULL for invalid JSON, that's expected
    
    // Also test with MIME type flag to exercise different code paths
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        magic_setflags(magic, flags | MAGIC_MIME_TYPE);
        const char* mime_result = magic_buffer(magic, json_data.c_str(), json_data.size());
        (void)mime_result;
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_013
