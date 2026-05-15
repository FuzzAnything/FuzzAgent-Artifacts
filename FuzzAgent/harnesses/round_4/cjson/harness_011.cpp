#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_011 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for comprehensive minification testing with comments
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Determine test scenario for comment processing
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (scenario) {
        case 0: {
            // Test 1: JSON with various single-line comments (//)
            std::string json_with_comments;
            
            // Start with basic JSON structure
            json_with_comments = "{";
            
            // Add single-line comment before a value
            if (fdp.ConsumeBool()) {
                json_with_comments += " // This is a single-line comment\n";
            }
            
            // Add key-value pairs with comments in different positions
            size_t num_pairs = fdp.ConsumeIntegralInRange<size_t>(1, 5);
            for (size_t i = 0; i < num_pairs && fdp.remaining_bytes() > 0; i++) {
                // Generate key
                size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (fdp.remaining_bytes() < key_len) break;
                std::string key = fdp.ConsumeBytesAsString(key_len);
                
                json_with_comments += "\"" + key + "\": ";
                
                // Add comment after key but before value
                if (fdp.ConsumeBool()) {
                    json_with_comments += " // comment before value\n";
                }
                
                // Add value
                uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 5;
                switch (value_type) {
                    case 0:
                        json_with_comments += "null";
                        break;
                    case 1:
                        json_with_comments += fdp.ConsumeBool() ? "true" : "false";
                        break;
                    case 2: {
                        double num = fdp.ConsumeFloatingPoint<double>();
                        json_with_comments += std::to_string(num);
                        break;
                    }
                    case 3: {
                        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                        if (fdp.remaining_bytes() < str_len) break;
                        std::string str_val = fdp.ConsumeBytesAsString(str_len);
                        json_with_comments += "\"" + str_val + "\"";
                        break;
                    }
                    case 4:
                        json_with_comments += "[]";
                        break;
                }
                
                // Add comment after value
                if (fdp.ConsumeBool()) {
                    json_with_comments += " // comment after value\n";
                }
                
                if (i < num_pairs - 1) {
                    json_with_comments += ",";
                }
            }
            
            json_with_comments += "}";
            
            // Add trailing comment
            if (fdp.ConsumeBool()) {
                json_with_comments += " // trailing comment\n";
            }
            
            // Test minification
            char* minify_buffer = strdup(json_with_comments.c_str());
            if (minify_buffer != NULL) {
                cJSON_Minify(minify_buffer);
                free(minify_buffer);
            }
            break;
        }
        
        case 1: {
            // Test 2: JSON with multiline comments (/* */)
            std::string json_with_comments;
            
            json_with_comments = "/* Multi-line comment at start */\n";
            json_with_comments += "{";
            
            size_t num_pairs = fdp.ConsumeIntegralInRange<size_t>(1, 4);
            for (size_t i = 0; i < num_pairs && fdp.remaining_bytes() > 0; i++) {
                // Generate key
                size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (fdp.remaining_bytes() < key_len) break;
                std::string key = fdp.ConsumeBytesAsString(key_len);
                
                json_with_comments += "\"" + key + "\": ";
                
                // Add multiline comment before value
                if (fdp.ConsumeBool()) {
                    json_with_comments += "/* multiline\ncomment\nbefore value */";
                }
                
                // Add value
                uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (value_type) {
                    case 0:
                        json_with_comments += "null";
                        break;
                    case 1:
                        json_with_comments += fdp.ConsumeBool() ? "true" : "false";
                        break;
                    case 2: {
                        double num = fdp.ConsumeFloatingPoint<double>();
                        json_with_comments += std::to_string(num);
                        break;
                    }
                    case 3: {
                        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                        if (fdp.remaining_bytes() < str_len) break;
                        std::string str_val = fdp.ConsumeBytesAsString(str_len);
                        json_with_comments += "\"" + str_val + "\"";
                        break;
                    }
                }
                
                // Add multiline comment after value
                if (fdp.ConsumeBool()) {
                    json_with_comments += "/* comment after value */";
                }
                
                if (i < num_pairs - 1) {
                    json_with_comments += ",";
                    // Add comment between items
                    if (fdp.ConsumeBool()) {
                        json_with_comments += " /* comment between items */ ";
                    }
                }
            }
            
            json_with_comments += "}\n";
            json_with_comments += "/* Multi-line comment at end */";
            
            // Test minification
            char* minify_buffer = strdup(json_with_comments.c_str());
            if (minify_buffer != NULL) {
                cJSON_Minify(minify_buffer);
                free(minify_buffer);
            }
            break;
        }
        
        case 2: {
            // Test 3: Mixed single-line and multiline comments
            std::string json_with_comments;
            
            json_with_comments = "// Single-line comment at start\n";
            json_with_comments += "/* Multiline comment at start */\n";
            json_with_comments += "{";
            
            size_t num_items = fdp.ConsumeIntegralInRange<size_t>(1, 3);
            for (size_t i = 0; i < num_items && fdp.remaining_bytes() > 0; i++) {
                // Generate key
                size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                if (fdp.remaining_bytes() < key_len) break;
                std::string key = fdp.ConsumeBytesAsString(key_len);
                
                json_with_comments += "\n  // Comment before key\n";
                json_with_comments += "  \"" + key + "\": ";
                
                // Mix comment types
                if (fdp.ConsumeBool()) {
                    json_with_comments += "// Single-line before value\n  ";
                }
                if (fdp.ConsumeBool()) {
                    json_with_comments += "/* Multiline before value */ ";
                }
                
                // Simple value
                json_with_comments += fdp.ConsumeBool() ? "true" : "false";
                
                if (fdp.ConsumeBool()) {
                    json_with_comments += " // Single-line after value";
                }
                if (fdp.ConsumeBool()) {
                    json_with_comments += " /* Multiline after value */";
                }
                
                if (i < num_items - 1) {
                    json_with_comments += ",";
                }
            }
            
            json_with_comments += "\n}\n";
            json_with_comments += "// Trailing single-line comment\n";
            json_with_comments += "/* Trailing multiline comment */";
            
            // Test minification
            char* minify_buffer = strdup(json_with_comments.c_str());
            if (minify_buffer != NULL) {
                cJSON_Minify(minify_buffer);
                free(minify_buffer);
            }
            break;
        }
        
        case 3: {
            // Test 4: Malformed/unterminated comments
            std::string json_with_bad_comments;
            
            // Start with valid JSON
            json_with_bad_comments = "{ \"key\": \"value\" }";
            
            // Add various malformed comment patterns
            uint8_t bad_comment_type = fdp.ConsumeIntegral<uint8_t>() % 5;
            
            switch (bad_comment_type) {
                case 0:
                    // Unterminated multiline comment
                    json_with_bad_comments = "/* Unterminated comment " + json_with_bad_comments;
                    break;
                case 1:
                    // Single slash (not a comment)
                    json_with_bad_comments = "/ " + json_with_bad_comments;
                    break;
                case 2:
                    // Nested comments (not supported in JSON)
                    json_with_bad_comments = "/* Outer /* Inner */ Still outer? */" + json_with_bad_comments;
                    break;
                case 3:
                    // Comment in string (should be preserved)
                    json_with_bad_comments = "{ \"key\": \"value // not a comment\" }";
                    break;
                case 4:
                    // Multiple slashes
                    json_with_bad_comments = "/// Triple slash" + json_with_bad_comments;
                    break;
            }
            
            // Also test with whitespace variations
            std::string with_whitespace;
            for (char c : json_with_bad_comments) {
                with_whitespace += c;
                if (fdp.ConsumeBool()) {
                    uint8_t ws_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                    switch (ws_type) {
                        case 0: with_whitespace += ' '; break;
                        case 1: with_whitespace += '\t'; break;
                        case 2: with_whitespace += '\n'; break;
                        case 3: with_whitespace += '\r'; break;
                    }
                }
            }
            
            // Test minification on malformed input
            char* minify_buffer = strdup(with_whitespace.c_str());
            if (minify_buffer != NULL) {
                cJSON_Minify(minify_buffer);
                free(minify_buffer);
            }
            break;
        }
        
        case 4: {
            // Test 5: Comments with special characters
            std::string json_with_special_comments;
            
            json_with_special_comments = "{";
            
            // Create comment with special characters
            std::string special_chars = "/* Comment with special chars: ";
            special_chars += "\\\" quotes, \\\\ backslash, \\/ slash, ";
            special_chars += "unicode: \u00E9, ";
            special_chars += "control chars maybe */";
            
            json_with_special_comments += special_chars + "\n";
            
            // Add a simple key-value pair
            json_with_special_comments += "\"test\": true";
            
            // Add another comment with different special chars
            json_with_special_comments += " // Line comment with \t tabs, \n newlines (escaped)";
            
            json_with_special_comments += "}";
            
            // Test minification
            char* minify_buffer = strdup(json_with_special_comments.c_str());
            if (minify_buffer != NULL) {
                cJSON_Minify(minify_buffer);
                free(minify_buffer);
            }
            break;
        }
        
        case 5: {
            // Test 6: Full integration test - parse, print, then minify
            // First create a JSON object using cJSON APIs
            cJSON* root = cJSON_CreateObject();
            if (root == NULL) return 0;
            
            // Add some items based on fuzzer input
            size_t num_items = fdp.ConsumeIntegralInRange<size_t>(1, 3);
            for (size_t i = 0; i < num_items && fdp.remaining_bytes() > 0; i++) {
                size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                if (fdp.remaining_bytes() < key_len) break;
                std::string key = fdp.ConsumeBytesAsString(key_len);
                
                uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (value_type) {
                    case 0:
                        cJSON_AddNullToObject(root, key.c_str());
                        break;
                    case 1:
                        cJSON_AddBoolToObject(root, key.c_str(), fdp.ConsumeBool());
                        break;
                    case 2: {
                        double num = fdp.ConsumeFloatingPoint<double>();
                        cJSON_AddNumberToObject(root, key.c_str(), num);
                        break;
                    }
                    case 3: {
                        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                        if (fdp.remaining_bytes() < str_len) break;
                        std::string str_val = fdp.ConsumeBytesAsString(str_len);
                        cJSON_AddStringToObject(root, key.c_str(), str_val.c_str());
                        break;
                    }
                }
            }
            
            // Print the JSON (unformatted to have less whitespace)
            char* json_str = cJSON_PrintUnformatted(root);
            if (json_str != NULL) {
                // Create a version with comments added
                std::string with_comments = "// Generated by fuzzer\n";
                with_comments += "/* Object with ";
                with_comments += std::to_string(num_items);
                with_comments += " items */\n";
                with_comments += json_str;
                
                // Minify the version with comments
                char* minify_buffer = strdup(with_comments.c_str());
                if (minify_buffer != NULL) {
                    cJSON_Minify(minify_buffer);
                    
                    // Try to parse the minified result
                    cJSON* minified_parsed = cJSON_Parse(minify_buffer);
                    if (minified_parsed != NULL) {
                        cJSON_Delete(minified_parsed);
                    }
                    
                    free(minify_buffer);
                }
                
                free(json_str);
            }
            
            cJSON_Delete(root);
            break;
        }
    }
    
    return 0;
}

} // namespace harness_011
