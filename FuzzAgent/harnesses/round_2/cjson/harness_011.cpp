#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_011 {

extern "C" {
}

// Helper function to generate complex number strings
std::string generate_number_string(FuzzedDataProvider& fdp) {
    uint8_t number_type = fdp.ConsumeIntegral<uint8_t>() % 15;
    
    switch (number_type) {
        case 0: // Regular integer
            return std::to_string(fdp.ConsumeIntegral<int>());
        case 1: // Negative integer
            return "-" + std::to_string(fdp.ConsumeIntegral<int>());
        case 2: // Float with decimal
            return std::to_string(fdp.ConsumeFloatingPoint<double>());
        case 3: // Scientific notation
            return std::to_string(fdp.ConsumeFloatingPoint<double>()) + "e" + 
                   std::to_string(fdp.ConsumeIntegralInRange<int>(-10, 10));
        case 4: // Scientific notation with capital E
            return std::to_string(fdp.ConsumeFloatingPoint<double>()) + "E" + 
                   std::to_string(fdp.ConsumeIntegralInRange<int>(-10, 10));
        case 5: // Hex number (not standard JSON but might be parsed)
            return "0x" + std::to_string(fdp.ConsumeIntegralInRange<int>(0, 255));
        case 6: // Very large number
            return "1" + std::string(fdp.ConsumeIntegralInRange<int>(1, 50), '0');
        case 7: // Very small number
            return "0." + std::string(fdp.ConsumeIntegralInRange<int>(1, 50), '0') + "1";
        case 8: // NaN string representation
            return "NaN";
        case 9: // Infinity string representation
            return "Infinity";
        case 10: // -Infinity
            return "-Infinity";
        case 11: // Number with multiple decimals
            return std::to_string(fdp.ConsumeIntegral<int>()) + ".." + 
                   std::to_string(fdp.ConsumeIntegral<int>());
        case 12: // Number starting with plus
            return "+" + std::to_string(fdp.ConsumeIntegral<int>());
        case 13: // Empty number string
            return "";
        case 14: // Number with spaces
            return " " + std::to_string(fdp.ConsumeIntegral<int>()) + " ";
        default:
            return std::to_string(fdp.ConsumeIntegral<int>());
    }
}

// Helper function to generate strings with escape sequences
std::string generate_escape_string(FuzzedDataProvider& fdp) {
    uint8_t escape_type = fdp.ConsumeIntegral<uint8_t>() % 12;
    
    switch (escape_type) {
        case 0: // Basic escape sequences
            return "\"Hello\\tWorld\\nWith\\rNewline\\fFormfeed\\bBackspace\\\\Backslash\\\"Quote\"";
        case 1: // Unicode escape sequences
            return "\"Unicode: \\u0041\\u0042\\u0043\\u0044\\u0045\"";
        case 2: // Mixed escapes
            return "\"Mixed:\\tTab\\nNewline\\u263A\\u2764\\uFE0F\"";
        case 3: // Invalid Unicode escapes
            return "\"Invalid:\\u123\\u12\\u1\\u\"";
        case 4: // High Unicode
            return "\"High:\\uD83D\\uDE00\\uD83C\\uDF89\"";
        case 5: // Empty string
            return "\"\"";
        case 6: // Control characters
            return "\"\x01\x02\x03\x04\x05\x06\x07\x08\x0B\x0C\x0E\x0F\x10\"";
        case 7: // Backslash at end
            return "\"End with backslash\\\"";
        case 8: // Multiple backslashes
            return "\"\\\\\\\\\\\\\\\"";
        case 9: // Escaped null
            return "\"\\0\"";
        case 10: // Very long string with escapes
            return "\"" + std::string(fdp.ConsumeIntegralInRange<int>(10, 100), 'a') + 
                   "\\t\\n\\r\\f\\b\\\\\\\"" + 
                   std::string(fdp.ConsumeIntegralInRange<int>(10, 100), 'b') + "\"";
        case 11: // Random string with random escapes
            {
                std::string result = "\"";
                int length = fdp.ConsumeIntegralInRange<int>(5, 50);
                for (int i = 0; i < length; i++) {
                    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
                        uint8_t escape = fdp.ConsumeIntegral<uint8_t>() % 8;
                        switch (escape) {
                            case 0: result += "\\t"; break;
                            case 1: result += "\\n"; break;
                            case 2: result += "\\r"; break;
                            case 3: result += "\\f"; break;
                            case 4: result += "\\b"; break;
                            case 5: result += "\\\\"; break;
                            case 6: result += "\\\""; break;
                            case 7: result += "\\u0041"; break;
                        }
                    } else {
                        result += static_cast<char>(fdp.ConsumeIntegralInRange<int>(32, 126));
                    }
                }
                result += "\"";
                return result;
            }
        default:
            return "\"test\"";
    }
}

// Helper function to generate nested JSON structures
std::string generate_nested_structure(FuzzedDataProvider& fdp, int depth) {
    if (depth <= 0 || fdp.remaining_bytes() < 10) {
        return "null";
    }
    
    uint8_t structure_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (structure_type) {
        case 0: // Object
            {
                std::string result = "{";
                int item_count = fdp.ConsumeIntegralInRange<int>(0, 3);
                for (int i = 0; i < item_count; i++) {
                    if (i > 0) result += ",";
                    std::string key = "\"key" + std::to_string(i) + "\"";
                    std::string value = generate_nested_structure(fdp, depth - 1);
                    result += key + ":" + value;
                }
                result += "}";
                return result;
            }
        case 1: // Array
            {
                std::string result = "[";
                int item_count = fdp.ConsumeIntegralInRange<int>(0, 3);
                for (int i = 0; i < item_count; i++) {
                    if (i > 0) result += ",";
                    result += generate_nested_structure(fdp, depth - 1);
                }
                result += "]";
                return result;
            }
        case 2: // String
            return "\"string" + std::to_string(fdp.ConsumeIntegral<int>()) + "\"";
        case 3: // Number
            return generate_number_string(fdp);
        case 4: // Boolean
            return fdp.ConsumeBool() ? "true" : "false";
        case 5: // Null
            return "null";
        default:
            return "null";
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0; // Need minimum input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Select test scenario
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Test Scenario 0: Complex number parsing (targeting parse_number)
    if (test_scenario == 0) {
        // Generate various number strings
        int num_tests = fdp.ConsumeIntegralInRange<int>(1, 10);
        
        for (int i = 0; i < num_tests && fdp.remaining_bytes() > 0; i++) {
            std::string number_str = generate_number_string(fdp);
            
            // Test parse_number indirectly through cJSON_Parse
            std::string json_str = "{\"num\":" + number_str + "}";
            cJSON* parsed = cJSON_Parse(json_str.c_str());
            
            if (parsed != NULL) {
                // If parsing succeeded, test number extraction
                cJSON* num_item = cJSON_GetObjectItem(parsed, "num");
                if (num_item != NULL && cJSON_IsNumber(num_item)) {
                    double val = cJSON_GetNumberValue(num_item);
                    // Also test printing the number
                    char* printed = cJSON_Print(num_item);
                    if (printed != NULL) {
                        free(printed);
                    }
                }
                cJSON_Delete(parsed);
            }
            
            // Also test parse_number indirectly through cJSON_CreateNumber
            cJSON* num_obj = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            if (num_obj != NULL) {
                // Test print_string_ptr indirectly through printing
                char* num_str = cJSON_Print(num_obj);
                if (num_str != NULL) {
                    free(num_str);
                }
                cJSON_Delete(num_obj);
            }
        }
        return 0;
    }
    
    // Test Scenario 1: String parsing with escape sequences (targeting parse_string)
    if (test_scenario == 1) {
        int num_tests = fdp.ConsumeIntegralInRange<int>(1, 10);
        
        for (int i = 0; i < num_tests && fdp.remaining_bytes() > 0; i++) {
            std::string escape_str = generate_escape_string(fdp);
            
            // Test parse_string indirectly through cJSON_Parse
            std::string json_str = "{\"str\":" + escape_str + "}";
            cJSON* parsed = cJSON_Parse(json_str.c_str());
            
            if (parsed != NULL) {
                // If parsing succeeded, test string extraction
                cJSON* str_item = cJSON_GetObjectItem(parsed, "str");
                if (str_item != NULL && cJSON_IsString(str_item)) {
                    char* str_val = cJSON_GetStringValue(str_item);
                    
                    // Test print_string_ptr indirectly through various printing functions
                    uint8_t print_method = fdp.ConsumeIntegral<uint8_t>() % 4;
                    char* printed = NULL;
                    
                    switch (print_method) {
                        case 0:
                            printed = cJSON_Print(str_item);
                            break;
                        case 1:
                            printed = cJSON_PrintUnformatted(str_item);
                            break;
                        case 2:
                            printed = cJSON_PrintBuffered(str_item, 
                                                         fdp.ConsumeIntegralInRange<int>(10, 1024),
                                                         fdp.ConsumeBool());
                            break;
                        case 3:
                            {
                                int buffer_size = fdp.ConsumeIntegralInRange<int>(10, 1024);
                                std::vector<char> buffer(buffer_size);
                                cJSON_PrintPreallocated(str_item, buffer.data(), buffer_size, fdp.ConsumeBool());
                            }
                            break;
                    }
                    
                    if (printed != NULL) {
                        free(printed);
                    }
                }
                cJSON_Delete(parsed);
            }
            
            // Also test cJSON_CreateString with various strings
            std::string random_str = fdp.ConsumeRandomLengthString(100);
            cJSON* str_obj = cJSON_CreateString(random_str.c_str());
            if (str_obj != NULL) {
                char* printed_str = cJSON_Print(str_obj);
                if (printed_str != NULL) {
                    free(printed_str);
                }
                cJSON_Delete(str_obj);
            }
        }
        return 0;
    }
    
    // Test Scenario 2: Value parsing with deeply nested structures (targeting parse_value)
    if (test_scenario == 2) {
        int nesting_depth = fdp.ConsumeIntegralInRange<int>(1, 10);
        
        // Generate deeply nested JSON
        std::string nested_json = generate_nested_structure(fdp, nesting_depth);
        
        // Test parse_value indirectly through cJSON_Parse
        cJSON* parsed = cJSON_Parse(nested_json.c_str());
        
        if (parsed != NULL) {
            // Test various printing methods on the parsed structure
            uint8_t print_method = fdp.ConsumeIntegral<uint8_t>() % 4;
            char* printed = NULL;
            
            switch (print_method) {
                case 0:
                    printed = cJSON_Print(parsed);
                    break;
                case 1:
                    printed = cJSON_PrintUnformatted(parsed);
                    break;
                case 2:
                    printed = cJSON_PrintBuffered(parsed,
                                                 fdp.ConsumeIntegralInRange<int>(10, 4096),
                                                 fdp.ConsumeBool());
                    break;
                case 3:
                    {
                        int buffer_size = fdp.ConsumeIntegralInRange<int>(10, 4096);
                        std::vector<char> buffer(buffer_size);
                        cJSON_PrintPreallocated(parsed, buffer.data(), buffer_size, fdp.ConsumeBool());
                    }
                    break;
            }
            
            if (printed != NULL) {
                free(printed);
            }
            
            cJSON_Delete(parsed);
        }
        
        // Also test edge cases: empty objects and arrays
        const char* edge_cases[] = {
            "{}",      // Empty object
            "[]",      // Empty array  
            "{ }",     // Object with space
            "[ ]",     // Array with space
            "{\"\":null}",  // Object with empty key
            "{\"a\":{}}",   // Nested empty object
            "{\"a\":[]}",   // Object with empty array
            "[[[]]]",       // Deep empty arrays
            "{{{}}}"        // Deep empty objects
        };
        
        for (const char* edge_case : edge_cases) {
            cJSON* edge_parsed = cJSON_Parse(edge_case);
            if (edge_parsed != NULL) {
                char* edge_printed = cJSON_Print(edge_parsed);
                if (edge_printed != NULL) {
                    free(edge_printed);
                }
                cJSON_Delete(edge_parsed);
            }
        }
        
        return 0;
    }
    
    // Test Scenario 3: Comprehensive printing tests (targeting print_string_ptr)
    if (test_scenario == 3) {
        // Create a complex JSON object with various data types
        cJSON* root = cJSON_CreateObject();
        
        if (root != NULL) {
            // Add different types of items
            cJSON_AddNullToObject(root, "null_item");
            cJSON_AddTrueToObject(root, "true_item");
            cJSON_AddFalseToObject(root, "false_item");
            cJSON_AddNumberToObject(root, "number_item", fdp.ConsumeFloatingPoint<double>());
            
            // Add string with escape sequences
            std::string escape_str = generate_escape_string(fdp);
            // Remove quotes for cJSON_CreateString
            if (escape_str.length() >= 2 && escape_str[0] == '"' && escape_str[escape_str.length()-1] == '"') {
                escape_str = escape_str.substr(1, escape_str.length() - 2);
            }
            cJSON_AddStringToObject(root, "string_item", escape_str.c_str());
            
            // Add array with mixed types
            cJSON* array = cJSON_CreateArray();
            if (array != NULL) {
                cJSON_AddItemToArray(array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                cJSON_AddItemToArray(array, cJSON_CreateString("array_string"));
                cJSON_AddItemToArray(array, cJSON_CreateBool(fdp.ConsumeBool()));
                cJSON_AddItemToObject(root, "array_item", array);
            }
            
            // Add nested object
            cJSON* nested = cJSON_CreateObject();
            if (nested != NULL) {
                cJSON_AddNumberToObject(nested, "nested_num", fdp.ConsumeFloatingPoint<double>());
                cJSON_AddItemToObject(root, "nested_obj", nested);
            }
            
            // Test all printing methods
            for (int i = 0; i < 4; i++) {
                char* printed = NULL;
                switch (i) {
                    case 0:
                        printed = cJSON_Print(root);
                        break;
                    case 1:
                        printed = cJSON_PrintUnformatted(root);
                        break;
                    case 2:
                        printed = cJSON_PrintBuffered(root, 
                                                     fdp.ConsumeIntegralInRange<int>(10, 2048),
                                                     fdp.ConsumeBool());
                        break;
                    case 3:
                        {
                            int buffer_size = fdp.ConsumeIntegralInRange<int>(10, 2048);
                            std::vector<char> buffer(buffer_size);
                            cJSON_PrintPreallocated(root, buffer.data(), buffer_size, fdp.ConsumeBool());
                        }
                        break;
                }
                
                if (printed != NULL) {
                    free(printed);
                }
            }
            
            cJSON_Delete(root);
        }
        return 0;
    }
    
    // Test Scenario 4: Malformed JSON and boundary conditions
    if (test_scenario == 4) {
        // Generate malformed JSON strings
        std::vector<std::string> malformed_jsons;
        
        // Edge cases for parse_number
        malformed_jsons.push_back("123abc");  // Number followed by letters
        malformed_jsons.push_back("12.3.4");  // Multiple decimals
        malformed_jsons.push_back("1e2e3");   // Multiple e
        malformed_jsons.push_back("e123");    // Starts with e
        malformed_jsons.push_back(".123");    // Starts with decimal
        malformed_jsons.push_back("-");       // Just minus sign
        malformed_jsons.push_back("+123");    // Plus sign (not valid in JSON)
        
        // Edge cases for parse_string
        malformed_jsons.push_back("\"unclosed");  // Unclosed string
        malformed_jsons.push_back("\"bad\\xescape\"");  // Invalid escape
        malformed_jsons.push_back("\"bad\\u123\"");  // Incomplete Unicode
        malformed_jsons.push_back("\"bad\\u123x\"");  // Invalid Unicode
        malformed_jsons.push_back("\"null\\0byte\"");  // Null byte
        
        // Edge cases for parse_value
        malformed_jsons.push_back("tru");      // Incomplete true
        malformed_jsons.push_back("fals");     // Incomplete false
        malformed_jsons.push_back("nul");      // Incomplete null
        malformed_jsons.push_back("[1,2,3");   // Unclosed array
        malformed_jsons.push_back("{\"a\":1"); // Unclosed object
        malformed_jsons.push_back("]");        // Just closing bracket
        malformed_jsons.push_back("}");        // Just closing brace
        
        // Test each malformed JSON
        for (const auto& malformed : malformed_jsons) {
            cJSON* parsed = cJSON_Parse(malformed.c_str());
            // Should return NULL for malformed JSON
            if (parsed != NULL) {
                cJSON_Delete(parsed);
            }
        }
        
        // Test boundary conditions with cJSON_PrintPreallocated
        cJSON* simple = cJSON_CreateString("test");
        if (simple != NULL) {
            // Test with buffer too small
            char small_buffer[1];
            cJSON_PrintPreallocated(simple, small_buffer, 1, true);
            
            // Test with zero buffer
            char zero_buffer[0];
            cJSON_PrintPreallocated(simple, zero_buffer, 0, true);
            
            // Test with negative buffer size
            char neg_buffer[10];
            cJSON_PrintPreallocated(simple, neg_buffer, -1, true);
            
            // Test with NULL buffer
            cJSON_PrintPreallocated(simple, NULL, 100, true);
            
            cJSON_Delete(simple);
        }
        
        // Test print_string_ptr with NULL input
        cJSON* null_string = cJSON_CreateString(NULL);
        if (null_string != NULL) {
            // This should handle NULL string gracefully
            char* printed = cJSON_Print(null_string);
            if (printed != NULL) {
                free(printed);
            }
            cJSON_Delete(null_string);
        }
        
        return 0;
    }
    
    return 0;
}

} // namespace harness_011
