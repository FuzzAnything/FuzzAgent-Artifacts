#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include "cjson/cJSON.h"

namespace harness_029 {
/*
 * Harness 029: Comprehensive cJSON_Minify testing with focus on comment handling
 * 
 * Primary targets with low/0% coverage:
 * 1) skip_multiline_comment function (lines 2896-2907, 0% coverage) - Handles multiline comments (slash-star style)
 * 2) skip_oneline_comment function (lines 2879-2890) - Handles single-line comments (double-slash style)
 * 3) minify_string function (lines 2906-2926) - Handles string escaping and quotes
 * 4) cJSON_Minify edge cases (NULL input, various whitespace, escaped strings)
 * 
 * Coverage analysis from existing harnesses:
 * - harness_000 tests cJSON_Minify but only with random JSON input
 * - No harness specifically tests comment-heavy JSON or explores comment parsing logic
 * - According to coverage data, skip_multiline_comment has 0 hits (never executed)
 * - skip_oneline_comment likely also has low coverage
 * - minify_string function may have partial coverage but needs escaped string testing
 * 
 * Semantic diversity from existing 28 harnesses:
 * - harness_000 does basic minify testing without targeted comment generation
 * - No harness generates JSON with intentional multiline comments
 * - No harness tests edge cases like unterminated comments, nested comments, comments in strings
 * - This harness focuses on systematic testing of minify's comment handling capabilities
 * 
 * Strategy:
 * 1) Generate JSON-like input with various comment patterns (single-line, multiline)
 * 2) Test escaped strings and quote handling within minify_string function
 * 3) Test edge cases: NULL input, empty input, pure whitespace, pure comments
 * 4) Test mixed whitespace patterns (spaces, tabs, newlines, carriage returns)
 * 5) Test comment nesting and edge cases (unterminated comments, comments in strings)
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    if (size < 4) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Part 1: Test NULL and edge cases
    // ---------------------------------
    // Test with NULL input (should return immediately)
    cJSON_Minify(NULL);
    
    // Test with empty string
    if (size >= 1) {
        char empty_str[] = "";
        cJSON_Minify(empty_str);
    }
    
    // Test with pure whitespace
    if (size >= 8) {
        std::string whitespace_test = fdp.ConsumeRandomLengthString(8);
        // Replace all characters with whitespace
        for (char& c : whitespace_test) {
            uint8_t whitespace_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (whitespace_type) {
                case 0: c = ' '; break;
                case 1: c = '\t'; break;
                case 2: c = '\n'; break;
                case 3: c = '\r'; break;
            }
        }
        char* whitespace_buffer = (char*)malloc(whitespace_test.size() + 1);
        if (whitespace_buffer != NULL) {
            memcpy(whitespace_buffer, whitespace_test.c_str(), whitespace_test.size() + 1);
            cJSON_Minify(whitespace_buffer);
            free(whitespace_buffer);
        }
    }
    
    // Reset fdp for main testing
    FuzzedDataProvider fdp2(data, size);
    
    // Part 2: Generate structured JSON with comments for minify testing
    // -----------------------------------------------------------------
    
    // Determine test pattern based on input
    uint8_t test_pattern = fdp2.ConsumeIntegral<uint8_t>() % 8;
    
    std::string json_with_comments;
    
    switch (test_pattern) {
        case 0: {
            // Pattern 0: JSON with single-line comments (//)
            std::string key = fdp2.ConsumeRandomLengthString(16);
            std::string value = fdp2.ConsumeRandomLengthString(16);
            std::string comment1 = fdp2.ConsumeRandomLengthString(32);
            std::string comment2 = fdp2.ConsumeRandomLengthString(32);
            
            json_with_comments = "{\n";
            json_with_comments += "  // " + comment1 + "\n";
            json_with_comments += "  \"" + key + "\": \"" + value + "\",\n";
            json_with_comments += "  // " + comment2 + "\n";
            json_with_comments += "  \"number\": " + std::to_string(fdp2.ConsumeIntegral<int>()) + "\n";
            json_with_comments += "}";
            break;
        }
        
        case 1: {
            // Pattern 1: JSON with multiline comments (/* */) - TARGETS skip_multiline_comment
            std::string key = fdp2.ConsumeRandomLengthString(16);
            std::string value = fdp2.ConsumeRandomLengthString(16);
            std::string multiline_comment = fdp2.ConsumeRandomLengthString(64);
            
            json_with_comments = "/* Start of JSON document */\n";
            json_with_comments += "{\n";
            json_with_comments += "  /* Key: " + key + " */\n";
            json_with_comments += "  \"" + key + "\": \"" + value + "\",\n";
            json_with_comments += "  /* Multiline comment\n";
            json_with_comments += "     spanning multiple\n";
            json_with_comments += "     lines: " + multiline_comment + " */\n";
            json_with_comments += "  \"array\": [1, 2, 3]\n";
            json_with_comments += "}\n";
            json_with_comments += "/* End of document */";
            break;
        }
        
        case 2: {
            // Pattern 2: Mixed single-line and multiline comments
            json_with_comments = "// Single line comment at start\n";
            json_with_comments += "/* Multi-line\n";
            json_with_comments += "   comment */\n";
            json_with_comments += "{\n";
            json_with_comments += "  // Comment before key\n";
            json_with_comments += "  \"test\": \"value\", /* inline comment */\n";
            json_with_comments += "  \"num\": 42 // trailing comment\n";
            json_with_comments += "}\n";
            json_with_comments += "// Final comment";
            break;
        }
        
        case 3: {
            // Pattern 3: JSON with escaped strings - TARGETS minify_string function
            std::string escaped_key = fdp2.ConsumeRandomLengthString(16);
            std::string escaped_value = fdp2.ConsumeRandomLengthString(32);
            
            // Add escape sequences to value
            escaped_value = "Value with \\\"quotes\\\" and \\\\backslashes\\\\ and \\nnewline";
            
            json_with_comments = "{\n";
            json_with_comments += "  \"" + escaped_key + "\": \"" + escaped_value + "\",\n";
            json_with_comments += "  \"normal\": \"plain string\",\n";
            json_with_comments += "  \"empty\": \"\",\n";
            json_with_comments += "  \"quotes\": \"string with \\\"nested quotes\\\"\"\n";
            json_with_comments += "}";
            break;
        }
        
        case 4: {
            // Pattern 4: Comments inside strings (should NOT be treated as comments)
            json_with_comments = "{\n";
            json_with_comments += "  \"commentInString\": \"This is // not a comment and /* not */ either\",\n";
            json_with_comments += "  \"path\": \"C:\\\\Users\\\\test // still string\",\n";
            json_with_comments += "  \"url\": \"https://example.com/path\" // THIS is a comment\n";
            json_with_comments += "}";
            break;
        }
        
        case 5: {
            // Pattern 5: Complex nested structure with comments
            json_with_comments = "/* Outer comment */\n";
            json_with_comments += "{\n";
            json_with_comments += "  \"object\": { // nested object\n";
            json_with_comments += "    /* inner comment */\n";
            json_with_comments += "    \"key\": \"value\"\n";
            json_with_comments += "  },\n";
            json_with_comments += "  \"array\": [ /* array comment */\n";
            json_with_comments += "    1,\n";
            json_with_comments += "    2, // element comment\n";
            json_with_comments += "    /* multi\n";
            json_with_comments += "       line\n";
            json_with_comments += "       comment */\n";
            json_with_comments += "    3\n";
            json_with_comments += "  ]\n";
            json_with_comments += "}\n";
            json_with_comments += "// EOF";
            break;
        }
        
        case 6: {
            // Pattern 6: Edge cases - almost comments
            json_with_comments = "{\n";
            json_with_comments += "  \"key\": \"/not a comment\",\n";
            json_with_comments += "  \"path\": \"path/to//something\",\n";
            json_with_comments += "  \"regex\": \"/.*/g\",\n";
            json_with_comments += "  \"almostComment\": \"/* not closed\",\n";
            json_with_comments += "  \"almostComment2\": \"not opened */\"\n";
            json_with_comments += "}";
            break;
        }
        
        case 7: {
            // Pattern 7: Minimal valid JSON with various whitespace
            std::vector<std::string> whitespace_chars = {" ", "\t", "\n", "\r"};
            json_with_comments = "{";
            for (int i = 0; i < 8 && fdp2.remaining_bytes() > 0; i++) {
                size_t ws_idx = fdp2.ConsumeIntegralInRange<size_t>(0, whitespace_chars.size() - 1);
                json_with_comments += whitespace_chars[ws_idx];
            }
            json_with_comments += "\"key\"";
            for (int i = 0; i < 4 && fdp2.remaining_bytes() > 0; i++) {
                size_t ws_idx = fdp2.ConsumeIntegralInRange<size_t>(0, whitespace_chars.size() - 1);
                json_with_comments += whitespace_chars[ws_idx];
            }
            json_with_comments += ":";
            for (int i = 0; i < 4 && fdp2.remaining_bytes() > 0; i++) {
                size_t ws_idx = fdp2.ConsumeIntegralInRange<size_t>(0, whitespace_chars.size() - 1);
                json_with_comments += whitespace_chars[ws_idx];
            }
            json_with_comments += "\"value\"";
            for (int i = 0; i < 8 && fdp2.remaining_bytes() > 0; i++) {
                size_t ws_idx = fdp2.ConsumeIntegralInRange<size_t>(0, whitespace_chars.size() - 1);
                json_with_comments += whitespace_chars[ws_idx];
            }
            json_with_comments += "}";
            break;
        }
    }
    
    // Apply minify to the generated JSON
    char* minify_buffer = (char*)malloc(json_with_comments.size() + 1);
    if (minify_buffer != NULL) {
        memcpy(minify_buffer, json_with_comments.c_str(), json_with_comments.size() + 1);
        cJSON_Minify(minify_buffer);
        
        // Optional: Verify the minified result is still valid JSON by trying to parse it
        // (but only if we have enough remaining fuzzer input)
        if (fdp2.remaining_bytes() > 10) {
            // Try parsing the minified output
            cJSON* parsed = cJSON_Parse(minify_buffer);
            if (parsed != NULL) {
                cJSON_Delete(parsed);
            }
            
            // Also test with ParseWithOpts
            const char* parse_end = NULL;
            cJSON_bool require_null_terminated = fdp2.ConsumeBool();
            cJSON* parsed_with_opts = cJSON_ParseWithOpts(minify_buffer, &parse_end, require_null_terminated);
            if (parsed_with_opts != NULL) {
                cJSON_Delete(parsed_with_opts);
            }
        }
        
        free(minify_buffer);
    }
    
    // Part 3: Test with raw fuzzer input as JSON (may contain comment-like sequences)
    // --------------------------------------------------------------------------------
    if (size > 16) {
        FuzzedDataProvider fdp3(data, size);
        std::string raw_input = fdp3.ConsumeRemainingBytesAsString();
        
        char* raw_buffer = (char*)malloc(raw_input.size() + 1);
        if (raw_buffer != NULL) {
            memcpy(raw_buffer, raw_input.c_str(), raw_input.size() + 1);
            cJSON_Minify(raw_buffer);
            free(raw_buffer);
        }
    }
    
    // Part 4: Test comment edge cases
    // --------------------------------
    // Test with unterminated multiline comment
    if (size > 20) {
        std::string unterminated = "/* This comment never ends... {\"key\": \"value\"}";
        char* unterminated_buffer = (char*)malloc(unterminated.size() + 1);
        if (unterminated_buffer != NULL) {
            memcpy(unterminated_buffer, unterminated.c_str(), unterminated.size() + 1);
            cJSON_Minify(unterminated_buffer);
            free(unterminated_buffer);
        }
    }
    
    // Test with nested comments (not standard JSON but test parsing)
    if (size > 30) {
        std::string nested_comments = "/* Outer /* Inner */ Still outer? */ {}";
        char* nested_buffer = (char*)malloc(nested_comments.size() + 1);
        if (nested_buffer != NULL) {
            memcpy(nested_buffer, nested_comments.c_str(), nested_comments.size() + 1);
            cJSON_Minify(nested_buffer);
            free(nested_buffer);
        }
    }
    
    // Test empty comment blocks
    std::string empty_comment = "/**/ {}";
    char* empty_comment_buffer = (char*)malloc(empty_comment.size() + 1);
    if (empty_comment_buffer != NULL) {
        memcpy(empty_comment_buffer, empty_comment.c_str(), empty_comment.size() + 1);
        cJSON_Minify(empty_comment_buffer);
        free(empty_comment_buffer);
    }
    
    // Test single slash (not a comment)
    std::string single_slash = "/ {\"key\": \"value\"}";
    char* single_slash_buffer = (char*)malloc(single_slash.size() + 1);
    if (single_slash_buffer != NULL) {
        memcpy(single_slash_buffer, single_slash.c_str(), single_slash.size() + 1);
        cJSON_Minify(single_slash_buffer);
        free(single_slash_buffer);
    }

    return 0;
}

} // namespace harness_029
