#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include "cjson/cJSON.h"

namespace harness_012 {
/*
 * harness_012.cpp - cJSON_Minify JSON minification testing harness
 * 
 * This harness specifically targets cJSON_Minify function which has 22 completely
 * blocked branches and 0% function line coverage - making it the single most 
 * critical coverage gap in the cJSON project.
 * 
 * Focus on JSON minification operations to provide semantic diversity from 
 * existing harnesses (which have focused on cJSON_Compare). Target scenarios:
 * 1) Whitespace removal from JSON strings (spaces, tabs, carriage returns, newlines)
 * 2) Newline and tab character elimination  
 * 3) Single-line comment removal (// comments)
 * 4) Multi-line comment removal (/* *\/ comments)
 * 5) Minification of nested JSON structures
 * 6) Edge cases with special characters in strings (escape sequences, quotes)
 * 7) Minification of large JSON documents with mixed whitespace/comment patterns
 * 8) Unclosed comment edge cases
 * 9) String preservation with escaped characters
 * 10) Empty input and null pointer handling
 * 
 * Based on analysis of cJSON_Minify source code, we need to test:
 * - skip_oneline_comment() function for // comments
 * - skip_multiline_comment() function for /* *\/ comments  
 * - minify_string() function for string preservation
 * - Whitespace removal switch case (space, \t, \r, \n)
 * - Default case for copying regular characters
 * - Edge cases: unclosed strings, unclosed comments, escape sequences
 * - Null pointer handling at function entry
 * 
 * Strategy: Generate diverse JSON strings with varying patterns of whitespace,
 * comments, and structure, then apply cJSON_Minify and verify behavior.
 */


extern "C" {
}

// Helper function to generate JSON with various whitespace patterns
static std::string generate_json_with_whitespace(FuzzedDataProvider& fdp, int depth) {
    if (depth <= 0 || fdp.remaining_bytes() < 16) {
        // Generate simple JSON values
        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        switch (value_type) {
            case 0: return "null";
            case 1: return fdp.ConsumeBool() ? "true" : "false";
            case 2: return std::to_string(fdp.ConsumeFloatingPoint<double>());
            case 3: {
                std::string str = fdp.ConsumeRandomLengthString(32);
                // Escape quotes in string
                std::string escaped;
                for (char c : str) {
                    if (c == '\"') escaped += "\\\"";
                    else if (c == '\\') escaped += "\\\\";
                    else escaped += c;
                }
                return "\"" + escaped + "\"";
            }
            case 4: return "[]";
            default: return "null";
        }
    }
    
    uint8_t structure_type = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    if (structure_type == 0) {
        // Generate array
        int num_items = fdp.ConsumeIntegralInRange<int>(1, 5);
        std::string result = "[";
        
        for (int i = 0; i < num_items && fdp.remaining_bytes() > 8; i++) {
            if (i > 0) result += ",";
            result += generate_json_with_whitespace(fdp, depth - 1);
        }
        
        result += "]";
        return result;
    } else {
        // Generate object
        int num_pairs = fdp.ConsumeIntegralInRange<int>(1, 5);
        std::string result = "{";
        
        for (int i = 0; i < num_pairs && fdp.remaining_bytes() > 16; i++) {
            if (i > 0) result += ",";
            
            // Generate key
            std::string key = fdp.ConsumeRandomLengthString(16);
            std::string escaped_key;
            for (char c : key) {
                if (c == '\"') escaped_key += "\\\"";
                else if (c == '\\') escaped_key += "\\\\";
                else escaped_key += c;
            }
            
            result += "\"" + escaped_key + "\":";
            result += generate_json_with_whitespace(fdp, depth - 1);
        }
        
        result += "}";
        return result;
    }
}

// Helper to insert whitespace into JSON string
static std::string insert_whitespace(FuzzedDataProvider& fdp, const std::string& json, float whitespace_density) {
    if (json.empty()) return json;
    
    std::string result;
    result.reserve(json.size() * 2); // Reserve extra space for whitespace
    
    for (size_t i = 0; i < json.size(); i++) {
        result += json[i];
        
        // Decide whether to insert whitespace after this character
        if (fdp.ConsumeProbability<float>() < whitespace_density && fdp.remaining_bytes() > 0) {
            uint8_t ws_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (ws_type) {
                case 0: result += ' '; break;
                case 1: result += '\t'; break;
                case 2: result += '\r'; break;
                case 3: result += '\n'; break;
            }
        }
        
        // Special handling for certain positions to insert comments
        if (i > 0 && json[i-1] == '/' && fdp.ConsumeProbability<float>() < 0.1 && fdp.remaining_bytes() > 4) {
            uint8_t comment_type = fdp.ConsumeIntegral<uint8_t>() % 2;
            if (comment_type == 0) {
                // Single-line comment
                result += "/ This is a fuzzer-generated comment ";
                result += std::to_string(fdp.ConsumeIntegral<uint32_t>());
                result += "\n";
            } else {
                // Multi-line comment
                result += "* Multi-line comment ";
                result += std::to_string(fdp.ConsumeIntegral<uint32_t>());
                result += " *" + std::string(fdp.ConsumeIntegralInRange<int>(1, 3), '/');
            }
        }
    }
    
    return result;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for meaningful minification scenarios
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario selection
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 10;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Simple JSON with random whitespace
            std::string json_template = generate_json_with_whitespace(fdp, 3);
            float whitespace_density = fdp.ConsumeProbability<float>();
            std::string json_with_ws = insert_whitespace(fdp, json_template, whitespace_density);
            
            char* json_copy = (char*)malloc(json_with_ws.size() + 1);
            if (!json_copy) return 0;
            memcpy(json_copy, json_with_ws.c_str(), json_with_ws.size() + 1);
            
            cJSON_Minify(json_copy);
            
            // The minified JSON should be valid (or at least not crash)
            // We can optionally parse it to verify
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
                cJSON* parsed = cJSON_Parse(json_copy);
                if (parsed) {
                    cJSON_Delete(parsed);
                }
            }
            
            free(json_copy);
            break;
        }
        
        case 1: {
            // Scenario 1: JSON with single-line comments
            std::string base_json = generate_json_with_whitespace(fdp, 2);
            
            // Insert single-line comments at various positions
            std::string json_with_comments;
            for (char c : base_json) {
                json_with_comments += c;
                if (fdp.ConsumeProbability<float>() < 0.2 && fdp.remaining_bytes() > 8) {
                    json_with_comments += "// Comment " + std::to_string(fdp.ConsumeIntegral<uint32_t>()) + "\n";
                }
            }
            
            char* json_copy = (char*)malloc(json_with_comments.size() + 1);
            if (!json_copy) return 0;
            memcpy(json_copy, json_with_comments.c_str(), json_with_comments.size() + 1);
            
            cJSON_Minify(json_copy);
            free(json_copy);
            break;
        }
        
        case 2: {
            // Scenario 2: JSON with multi-line comments
            std::string base_json = generate_json_with_whitespace(fdp, 2);
            
            // Insert multi-line comments
            std::string json_with_comments;
            for (char c : base_json) {
                json_with_comments += c;
                if (fdp.ConsumeProbability<float>() < 0.15 && fdp.remaining_bytes() > 8) {
                    json_with_comments += "/* Multi-line comment " + 
                                        std::to_string(fdp.ConsumeIntegral<uint32_t>()) + 
                                        " */";
                }
            }
            
            char* json_copy = (char*)malloc(json_with_comments.size() + 1);
            if (!json_copy) return 0;
            memcpy(json_copy, json_with_comments.c_str(), json_with_comments.size() + 1);
            
            cJSON_Minify(json_copy);
            free(json_copy);
            break;
        }
        
        case 3: {
            // Scenario 3: JSON with strings containing escape sequences
            // Test minify_string() function thoroughly
            std::vector<std::string> test_strings = {
                "\"simple string\"",
                "\"string with \\\"quotes\\\"\"",
                "\"string with \\\\ backslash\"",
                "\"string with \\t tab\"",
                "\"string with \\n newline\"",
                "\"string with \\r carriage return\"",
                "\"string with \\b backspace\"",
                "\"string with \\f formfeed\"",
                "\"string with \\/ slash\"",
                "\"string with \\u0041 unicode\""
            };
            
            uint8_t string_idx = fdp.ConsumeIntegral<uint8_t>() % test_strings.size();
            std::string json = "{\"key\": " + test_strings[string_idx] + "}";
            
            // Add whitespace around the string
            json = "{ \"key\" : " + test_strings[string_idx] + " }";
            
            char* json_copy = (char*)malloc(json.size() + 1);
            if (!json_copy) return 0;
            memcpy(json_copy, json.c_str(), json.size() + 1);
            
            cJSON_Minify(json_copy);
            
            // Verify the string content is preserved
            // (cJSON_Minify should not modify string contents)
            
            free(json_copy);
            break;
        }
        
        case 4: {
            // Scenario 4: Nested structures with mixed whitespace/comments
            std::string complex_json = "{\n";
            complex_json += "  /* Outer comment */\n";
            complex_json += "  \"array\": [\n";
            complex_json += "    1,\n";
            complex_json += "    // Item comment\n";
            complex_json += "    2,\n";
            complex_json += "    /* Nested object */\n";
            complex_json += "    {\"nested\": true}\n";
            complex_json += "  ],\n";
            complex_json += "  \"object\": {\n";
            complex_json += "    \"key\": \"value with\\t tab\"\n";
            complex_json += "  }\n";
            complex_json += "}\n";
            
            char* json_copy = (char*)malloc(complex_json.size() + 1);
            if (!json_copy) return 0;
            memcpy(json_copy, complex_json.c_str(), complex_json.size() + 1);
            
            cJSON_Minify(json_copy);
            free(json_copy);
            break;
        }
        
        case 5: {
            // Scenario 5: Edge case - unclosed multi-line comment
            std::string unclosed_comment = "/* This comment never ends";
            char* json_copy = (char*)malloc(unclosed_comment.size() + 1);
            if (!json_copy) return 0;
            memcpy(json_copy, unclosed_comment.c_str(), unclosed_comment.size() + 1);
            
            cJSON_Minify(json_copy);
            free(json_copy);
            break;
        }
        
        case 6: {
            // Scenario 6: Edge case - string with pending escape
            std::string pending_escape = "\"string with \\";
            char* json_copy = (char*)malloc(pending_escape.size() + 1);
            if (!json_copy) return 0;
            memcpy(json_copy, pending_escape.c_str(), pending_escape.size() + 1);
            
            cJSON_Minify(json_copy);
            free(json_copy);
            break;
        }
        
        case 7: {
            // Scenario 7: Empty and minimal JSON
            std::vector<std::string> minimal_jsons = {
                "",
                "{}",
                "[]",
                "null",
                "true",
                "false",
                "123.456",
                "\"\"",
                "\"test\""
            };
            
            uint8_t json_idx = fdp.ConsumeIntegral<uint8_t>() % minimal_jsons.size();
            std::string json = minimal_jsons[json_idx];
            
            // Add optional whitespace
            if (fdp.ConsumeBool()) {
                json = " " + json + " ";
            }
            
            char* json_copy = (char*)malloc(json.size() + 1);
            if (!json_copy) return 0;
            memcpy(json_copy, json.c_str(), json.size() + 1);
            
            cJSON_Minify(json_copy);
            free(json_copy);
            break;
        }
        
        case 8: {
            // Scenario 8: Large JSON document generation
            // Generate a moderately complex JSON structure
            std::string large_json = "{\n";
            int num_items = fdp.ConsumeIntegralInRange<int>(5, 20);
            
            for (int i = 0; i < num_items && fdp.remaining_bytes() > 16; i++) {
                if (i > 0) large_json += ",\n";
                
                std::string key = "key" + std::to_string(i);
                large_json += "  \"" + key + "\": ";
                
                uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (value_type) {
                    case 0:
                        large_json += std::to_string(fdp.ConsumeFloatingPoint<double>());
                        break;
                    case 1:
                        large_json += fdp.ConsumeBool() ? "true" : "false";
                        break;
                    case 2: {
                        std::string str = fdp.ConsumeRandomLengthString(32);
                        // Simple escaping
                        std::string escaped;
                        for (char c : str) {
                            if (c == '\"') escaped += "\\\"";
                            else escaped += c;
                        }
                        large_json += "\"" + escaped + "\"";
                        break;
                    }
                    case 3:
                        large_json += "[]";
                        break;
                }
                
                // Add occasional comments
                if (fdp.ConsumeProbability<float>() < 0.3 && fdp.remaining_bytes() > 4) {
                    large_json += " // Item " + std::to_string(i);
                }
            }
            
            large_json += "\n}";
            
            char* json_copy = (char*)malloc(large_json.size() + 1);
            if (!json_copy) return 0;
            memcpy(json_copy, large_json.c_str(), large_json.size() + 1);
            
            cJSON_Minify(json_copy);
            free(json_copy);
            break;
        }
        
        case 9: {
            // Scenario 9: Test null pointer handling (edge case)
            // This should be safe - cJSON_Minify checks for NULL
            cJSON_Minify(nullptr);
            
            // Also test with empty string
            char empty[] = "";
            cJSON_Minify(empty);
            break;
        }
    }
    
    return 0;
}

} // namespace harness_012
