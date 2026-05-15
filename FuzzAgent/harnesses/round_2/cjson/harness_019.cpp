#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_019 {

extern "C" {
}

// Helper to create deeply nested JSON structures approaching CJSON_NESTING_LIMIT
cJSON* create_deeply_nested_structure(FuzzedDataProvider& fdp, int depth, bool create_object) {
    if (depth <= 0 || fdp.remaining_bytes() < 4) {
        // Create leaf node
        uint8_t type = fdp.ConsumeIntegral<uint8_t>() % 6;
        switch (type) {
            case 0: return cJSON_CreateNull();
            case 1: return cJSON_CreateFalse();
            case 2: return cJSON_CreateTrue();
            case 3: return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            case 4: {
                std::string str = fdp.ConsumeRandomLengthString(32);
                return cJSON_CreateString(str.c_str());
            }
            default: return cJSON_CreateArray(); // Empty array as leaf
        }
    }
    
    if (create_object) {
        cJSON* obj = cJSON_CreateObject();
        if (!obj) return NULL;
        
        // Create a few properties
        int prop_count = fdp.ConsumeIntegralInRange<int>(1, 3);
        for (int i = 0; i < prop_count && fdp.remaining_bytes() > 0; i++) {
            std::string key = fdp.ConsumeRandomLengthString(16);
            cJSON* child = create_deeply_nested_structure(fdp, depth - 1, !create_object);
            if (child) {
                cJSON_AddItemToObject(obj, key.c_str(), child);
            }
        }
        return obj;
    } else {
        cJSON* arr = cJSON_CreateArray();
        if (!arr) return NULL;
        
        // Create a few elements
        int elem_count = fdp.ConsumeIntegralInRange<int>(1, 3);
        for (int i = 0; i < elem_count && fdp.remaining_bytes() > 0; i++) {
            cJSON* child = create_deeply_nested_structure(fdp, depth - 1, !create_object);
            if (child) {
                cJSON_AddItemToArray(arr, child);
            }
        }
        return arr;
    }
}

// Helper to create malformed JSON strings to trigger error paths in parse_value
std::string create_malformed_json(FuzzedDataProvider& fdp) {
    uint8_t error_type = fdp.ConsumeIntegral<uint8_t>() % 12;
    
    switch (error_type) {
        case 0: // NULL input simulation (empty string)
            return "";
        case 1: // Just whitespace
            return "   \t\n\r  ";
        case 2: // Invalid type identifier
            return "invalid";
        case 3: // Partial true
            return "tru";
        case 4: // Partial false  
            return "fals";
        case 5: // Partial null
            return "nul";
        case 6: // Unclosed string
            return "\"unclosed string";
        case 7: // Unclosed array
            return "[1,2,3";
        case 8: // Unclosed object
            return "{\"key\": \"value\"";
        case 9: // Invalid number
            return "12.34.56";
        case 10: // Invalid escape in string
            return "\"test\\x\"";
        case 11: // Control characters in string (unencoded)
            {
                std::string result = "\"";
                int len = fdp.ConsumeIntegralInRange<int>(1, 10);
                for (int i = 0; i < len && fdp.remaining_bytes() > 0; i++) {
                    result.push_back(static_cast<char>(fdp.ConsumeIntegral<uint8_t>()));
                }
                result.push_back('"');
                return result;
            }
        default:
            return "null";
    }
}

// Helper to create JSON with various number formats
std::string create_number_json(FuzzedDataProvider& fdp) {
    uint8_t num_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (num_type) {
        case 0: // Regular integer
            return std::to_string(fdp.ConsumeIntegral<int64_t>());
        case 1: // Regular float
            return std::to_string(fdp.ConsumeFloatingPoint<double>());
        case 2: // Scientific notation
            return std::to_string(fdp.ConsumeFloatingPoint<double>()) + 
                   (fdp.ConsumeBool() ? "e" : "E") +
                   (fdp.ConsumeBool() ? "+" : "-") +
                   std::to_string(fdp.ConsumeIntegralInRange<int>(0, 308));
        case 3: // Very large number
            return "1" + std::string(fdp.ConsumeIntegralInRange<int>(50, 200), '0');
        case 4: // Very small number
            return "0." + std::string(fdp.ConsumeIntegralInRange<int>(50, 200), '0') + "1";
        case 5: // Negative infinity-like
            return "-1" + std::string(fdp.ConsumeIntegralInRange<int>(50, 200), '0');
        case 6: { // Multiple signs
            std::string result;
            result += (fdp.ConsumeBool() ? "+" : "-");
            result += (fdp.ConsumeBool() ? "+" : "-");
            result += "123";
            return result;
        }
        case 7: // Hex-like (invalid for JSON)
            return "0x" + std::to_string(fdp.ConsumeIntegral<int>());
        default:
            return "0";
    }
}

// Helper to create complex object for printing tests
cJSON* create_complex_print_object(FuzzedDataProvider& fdp) {
    cJSON* root = cJSON_CreateObject();
    if (!root) return NULL;
    
    // Add various types of values to exercise print_object thoroughly
    int item_count = fdp.ConsumeIntegralInRange<int>(1, 10);
    
    for (int i = 0; i < item_count && fdp.remaining_bytes() > 0; i++) {
        std::string key = fdp.ConsumeRandomLengthString(20);
        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 8;
        
        cJSON* value = NULL;
        switch (value_type) {
            case 0:
                value = cJSON_CreateNull();
                break;
            case 1:
                value = cJSON_CreateFalse();
                break;
            case 2:
                value = cJSON_CreateTrue();
                break;
            case 3:
                value = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                break;
            case 4: {
                std::string str = fdp.ConsumeRandomLengthString(64);
                value = cJSON_CreateString(str.c_str());
                break;
            }
            case 5:
                value = cJSON_CreateArray();
                if (value) {
                    // Add a few array elements
                    int arr_elements = fdp.ConsumeIntegralInRange<int>(0, 5);
                    for (int j = 0; j < arr_elements && fdp.remaining_bytes() > 0; j++) {
                        cJSON* elem = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                        if (elem) {
                            cJSON_AddItemToArray(value, elem);
                        }
                    }
                }
                break;
            case 6:
                value = cJSON_CreateObject();
                if (value) {
                    // Add nested object with one property
                    cJSON* nested = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
                    if (nested) {
                        cJSON_AddItemToObject(value, "nested", nested);
                    }
                }
                break;
            case 7: {
                // Create string with potential special characters
                std::string special_str;
                int special_len = fdp.ConsumeIntegralInRange<int>(1, 50);
                for (int j = 0; j < special_len && fdp.remaining_bytes() > 0; j++) {
                    uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 5;
                    switch (char_type) {
                        case 0: // Regular ASCII
                            special_str.push_back(fdp.ConsumeIntegralInRange<char>(32, 126));
                            break;
                        case 1: // Control character
                            special_str.push_back(fdp.ConsumeIntegralInRange<char>(1, 31));
                            break;
                        case 2: // Unicode-like (multi-byte simulation)
                            special_str.push_back(static_cast<char>(0xC0 | (fdp.ConsumeIntegral<uint8_t>() & 0x1F)));
                            special_str.push_back(static_cast<char>(0x80 | (fdp.ConsumeIntegral<uint8_t>() & 0x3F)));
                            break;
                        case 3: // Quote and backslash
                            special_str.push_back(fdp.ConsumeBool() ? '\"' : '\\');
                            break;
                        case 4: // Newline/tab
                            special_str.push_back(fdp.ConsumeBool() ? '\n' : '\t');
                            break;
                    }
                }
                value = cJSON_CreateString(special_str.c_str());
                break;
            }
        }
        
        if (value) {
            cJSON_AddItemToObject(root, key.c_str(), value);
            
            // Sometimes create objects with NULL string pointers (edge case for print_object)
            if (fdp.ConsumeBool() && value->type == cJSON_String && value->valuestring) {
                // Don't actually free to avoid double-free, just simulate edge case
                // by creating another item with potential NULL string
            }
        }
    }
    
    return root;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 8) return 0; // Minimum size for meaningful testing
    
    FuzzedDataProvider fdp(data, size);
    
    // Split fuzzer input for different test scenarios
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (test_scenario) {
        case 0: {
            // Scenario 1: Error Path Testing for parse_value
            // Test NULL inputs, zero-length buffers, invalid JSON
            
            // Consume operation type
            uint8_t error_test = fdp.ConsumeIntegral<uint8_t>() % 5;
            
            if (error_test == 0) {
                // Test with NULL buffer
                cJSON* item = cJSON_CreateNull();
                if (item) {
                    // These should return false due to NULL input
                    cJSON_ParseWithOpts(NULL, NULL, 0);
                    cJSON_ParseWithLengthOpts(NULL, 0, NULL, 0);
                    cJSON_Delete(item);
                }
            } else if (error_test == 1) {
                // Test with zero-length buffer
                cJSON_ParseWithLengthOpts("", 0, NULL, 0);
            } else if (error_test == 2) {
                // Test with malformed JSON
                std::string malformed = create_malformed_json(fdp);
                cJSON* parsed = cJSON_ParseWithLengthOpts(malformed.c_str(), malformed.length(), NULL, 0);
                if (parsed) {
                    cJSON_Delete(parsed);
                }
            } else if (error_test == 3) {
                // Test parse with return parse end
                std::string json_str = fdp.ConsumeRandomLengthString(128);
                const char* parse_end = NULL;
                cJSON* parsed = cJSON_ParseWithOpts(json_str.c_str(), &parse_end, 0);
                if (parsed) {
                    cJSON_Delete(parsed);
                }
            } else {
                // Test various invalid inputs
                std::vector<uint8_t> invalid_data = fdp.ConsumeRemainingBytes<uint8_t>();
                if (!invalid_data.empty()) {
                    // Cast to char* for parsing attempt (may be invalid)
                    cJSON* parsed = cJSON_ParseWithLengthOpts(
                        reinterpret_cast<const char*>(invalid_data.data()), 
                        invalid_data.size(), 
                        NULL, 
                        0
                    );
                    if (parsed) {
                        cJSON_Delete(parsed);
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Scenario 2: Complex Object Creation and Printing
            // Test print_object with various formatting options
            
            // Create complex object
            cJSON* obj = create_complex_print_object(fdp);
            if (!obj) break;
            
            // Test different printing methods
            uint8_t print_method = fdp.ConsumeIntegral<uint8_t>() % 4;
            
            char* printed = NULL;
            switch (print_method) {
                case 0:
                    // Regular print
                    printed = cJSON_Print(obj);
                    break;
                case 1:
                    // Unformatted print
                    printed = cJSON_PrintUnformatted(obj);
                    break;
                case 2: {
                    // Buffered print with limited buffer size (to test buffer overflow conditions)
                    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                    printed = (char*)malloc(buffer_size);
                    if (printed) {
                        cJSON_PrintPreallocated(obj, printed, buffer_size, fdp.ConsumeBool());
                        free(printed);
                        printed = NULL;
                    }
                    break;
                }
                case 3: {
                    // PrintBuffered with format option
                    printed = cJSON_PrintBuffered(obj, fdp.ConsumeBool(), fdp.ConsumeIntegralInRange<int>(1, 4096));
                    break;
                }
            }
            
            if (printed) {
                free(printed);
            }
            
            // Test printing with NULL object (should return NULL)
            char* null_print = cJSON_Print(NULL);
            if (null_print) free(null_print);
            
            cJSON_Delete(obj);
            break;
        }
        
        case 2: {
            // Scenario 3: Deep Nesting Tests (approaching CJSON_NESTING_LIMIT)
            // Create deeply nested structures
            
            int depth = fdp.ConsumeIntegralInRange<int>(1, 50); // Up to 50 levels deep
            bool start_with_object = fdp.ConsumeBool();
            
            cJSON* deep_structure = create_deeply_nested_structure(fdp, depth, start_with_object);
            if (deep_structure) {
                // Try to print it (may fail due to nesting limit)
                char* printed = cJSON_Print(deep_structure);
                if (printed) {
                    // Try to parse it back
                    cJSON* reparsed = cJSON_Parse(printed);
                    if (reparsed) {
                        cJSON_Delete(reparsed);
                    }
                    free(printed);
                }
                
                cJSON_Delete(deep_structure);
            }
            break;
        }
        
        case 3: {
            // Scenario 4: Number Parsing Edge Cases
            // Test parse_number paths through parse_value
            
            std::string number_json = create_number_json(fdp);
            cJSON* parsed = cJSON_ParseWithLengthOpts(number_json.c_str(), number_json.length(), NULL, 0);
            if (parsed) {
                // Try to print the number
                char* printed = cJSON_Print(parsed);
                if (printed) {
                    free(printed);
                }
                cJSON_Delete(parsed);
            }
            break;
        }
        
        case 4: {
            // Scenario 5: Memory Stress Tests
            // Repeated parsing/printing cycles
            
            int cycles = fdp.ConsumeIntegralInRange<int>(1, 10);
            std::string json_data = fdp.ConsumeRandomLengthString(256);
            
            for (int i = 0; i < cycles && fdp.remaining_bytes() > 0; i++) {
                cJSON* parsed = cJSON_ParseWithLengthOpts(json_data.c_str(), json_data.length(), NULL, 0);
                if (parsed) {
                    // Test different print methods
                    if (fdp.ConsumeBool()) {
                        char* printed = cJSON_Print(parsed);
                        if (printed) free(printed);
                    } else {
                        char* printed = cJSON_PrintUnformatted(parsed);
                        if (printed) free(printed);
                    }
                    
                    cJSON_Delete(parsed);
                }
                
                // Also test with modified JSON
                if (i < cycles - 1 && fdp.remaining_bytes() > 10) {
                    std::string modified_json = json_data + fdp.ConsumeRandomLengthString(32);
                    cJSON* parsed2 = cJSON_Parse(modified_json.c_str());
                    if (parsed2) cJSON_Delete(parsed2);
                }
            }
            break;
        }
        
        case 5: {
            // Scenario 6: Mixed Comprehensive Test
            // Combine multiple operations
            
            // First parse some JSON
            std::string json_to_parse = fdp.ConsumeRandomLengthString(512);
            cJSON* parsed = cJSON_ParseWithOpts(json_to_parse.c_str(), NULL, fdp.ConsumeBool());
            
            if (parsed) {
                // Create a new object and add the parsed item
                cJSON* new_obj = cJSON_CreateObject();
                if (new_obj) {
                    std::string key = fdp.ConsumeRandomLengthString(20);
                    cJSON_AddItemToObject(new_obj, key.c_str(), parsed);
                    
                    // Print with various options
                    uint8_t print_flags = fdp.ConsumeIntegral<uint8_t>() % 2;
                    char* printed = print_flags ? cJSON_Print(new_obj) : cJSON_PrintUnformatted(new_obj);
                    
                    if (printed) {
                        // Parse the printed output
                        cJSON* reparsed = cJSON_ParseWithLengthOpts(printed, strlen(printed), NULL, 0);
                        if (reparsed) {
                            cJSON_Delete(reparsed);
                        }
                        free(printed);
                    }
                    
                    // Don't delete parsed - it's now owned by new_obj
                    cJSON_Delete(new_obj);
                } else {
                    cJSON_Delete(parsed);
                }
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_019
