#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <cstdio>
#include "cjson/cJSON.h"

namespace harness_022 {
/*
 * Harness 022: Invalid type handling for cJSON_Compare and edge case parsing/printing
 * 
 * Primary Target: cJSON_Compare with 3/76 branches remaining (96% coverage)
 * Focus on invalid type handling to hit default switch cases (lines 3099-3100, 3199-3200)
 * which are completely uncovered. Create invalid JSON types deliberately by
 * manipulating type fields to test these default cases.
 * 
 * Secondary Targets: 
 * - parse_number (49/54 blocked): Test edge case parsing with decimal numbers,
 *   varying precision, scientific notation
 * - print_string_ptr (48/54 blocked): Test string printing with special characters/escapes
 * - parse_string (43/48 blocked): Test string parsing with edge cases
 * - print_object (42/46 blocked): Test complex object printing scenarios
 * 
 * APIs to use:
 * - cJSON_Compare (primary)
 * - cJSON_Parse/cJSON_ParseWithOpts
 * - cJSON_CreateObject/Array
 * - cJSON_AddItemToObject/Array
 * - cJSON_CreateString/Number/Bool
 * - cJSON_Delete
 */



// Helper to create a cJSON object with an invalid type by manipulating the type field
static cJSON* create_invalid_type_object(FuzzedDataProvider& fdp) {
    // Create a valid object first
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    // Add some valid content
    cJSON_AddItemToObject(obj, "valid_key", cJSON_CreateString("valid_value"));
    
    // Now manipulate the type field to an invalid value
    // Valid types: 0, 1, 2, 4, 8, 16, 32, 64, 128 (cJSON_Invalid through cJSON_Raw)
    // Generate an invalid type (not matching any valid type)
    uint8_t invalid_type = fdp.ConsumeIntegral<uint8_t>();
    
    // Ensure it's not a valid type
    // Valid types are powers of 2 from 0 to 128
    while (invalid_type == 0 || 
           invalid_type == 1 || invalid_type == 2 || invalid_type == 4 ||
           invalid_type == 8 || invalid_type == 16 || invalid_type == 32 ||
           invalid_type == 64 || invalid_type == 128) {
        invalid_type = fdp.ConsumeIntegral<uint8_t>();
    }
    
    // Set the invalid type
    obj->type = invalid_type;
    
    return obj;
}

// Helper to create partially invalid object (child with invalid type)
static cJSON* create_object_with_invalid_child(FuzzedDataProvider& fdp) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    // Add some valid children
    cJSON_AddItemToObject(obj, "num", cJSON_CreateNumber(42.5));
    cJSON_AddItemToObject(obj, "str", cJSON_CreateString("test"));
    
    // Create a child with manipulated invalid type
    cJSON* invalid_child = cJSON_CreateBool(1); // Start with valid bool
    if (invalid_child) {
        // Set to invalid type
        uint8_t invalid_type = fdp.ConsumeIntegral<uint8_t>() | 0x40; // Ensure not valid
        invalid_child->type = invalid_type;
        
        cJSON_AddItemToObject(obj, "invalid", invalid_child);
    }
    
    return obj;
}

// Test parse_number edge cases
static void test_parse_number_edge_cases(FuzzedDataProvider& fdp) {
    // Generate various number formats
    std::vector<std::string> number_formats;
    
    // Consume some fuzz data to create number strings
    int num_cases = fdp.ConsumeIntegralInRange<int>(1, 10);
    for (int i = 0; i < num_cases; i++) {
        uint8_t format_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        std::string num_str;
        
        switch (format_type) {
            case 0: // Decimal with varying precision
                {
                    double num = fdp.ConsumeFloatingPoint<double>();
                    char buf[64];
                    int precision = fdp.ConsumeIntegralInRange<int>(0, 15);
                    snprintf(buf, sizeof(buf), "%.*f", precision, num);
                    num_str = buf;
                }
                break;
            case 1: // Scientific notation
                {
                    double num = fdp.ConsumeFloatingPoint<double>();
                    char buf[64];
                    snprintf(buf, sizeof(buf), "%e", num);
                    num_str = buf;
                }
                break;
            case 2: // Integer
                {
                    int64_t num = fdp.ConsumeIntegral<int64_t>();
                    num_str = std::to_string(num);
                }
                break;
            case 3: // Mixed with signs and decimal
                {
                    bool negative = fdp.ConsumeBool();
                    bool has_decimal = fdp.ConsumeBool();
                    bool has_exponent = fdp.ConsumeBool();
                    
                    num_str = negative ? "-" : "";
                    num_str += std::to_string(fdp.ConsumeIntegralInRange<int64_t>(0, 1000000));
                    
                    if (has_decimal) {
                        num_str += ".";
                        num_str += std::to_string(fdp.ConsumeIntegralInRange<int64_t>(0, 999999));
                    }
                    
                    if (has_exponent) {
                        num_str += "e";
                        num_str += (fdp.ConsumeBool() ? "+" : "-");
                        num_str += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 10));
                    }
                }
                break;
        }
        
        if (!num_str.empty()) {
            number_formats.push_back(num_str);
        }
    }
    
    // Parse these numbers
    for (const auto& num_str : number_formats) {
        // Create JSON string with the number
        std::string json_str = "{\"value\": " + num_str + "}";
        cJSON* parsed = cJSON_Parse(json_str.c_str());
        if (parsed) {
            cJSON_Delete(parsed);
        }
    }
}

// Test string parsing and printing edge cases
static void test_string_edge_cases(FuzzedDataProvider& fdp) {
    // Generate strings with special characters and escapes
    int num_strings = fdp.ConsumeIntegralInRange<int>(1, 20);
    
    for (int i = 0; i < num_strings; i++) {
        // Create string with various special characters
        std::string str;
        int str_len = fdp.ConsumeIntegralInRange<int>(1, 100);
        
        for (int j = 0; j < str_len; j++) {
            uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 6;
            switch (char_type) {
                case 0: // Control characters
                    str.push_back(fdp.ConsumeIntegralInRange<char>(0, 31));
                    break;
                case 1: // Printable ASCII
                    str.push_back(fdp.ConsumeIntegralInRange<char>(32, 126));
                    break;
                case 2: // UTF-8 start bytes (partial)
                    str.push_back(fdp.ConsumeIntegralInRange<char>(0xC0, 0xDF));
                    break;
                case 3: // Quote and backslash (need escaping)
                    str.push_back(fdp.ConsumeBool() ? '\"' : '\\');
                    break;
                case 4: // Newline and tab
                    str.push_back(fdp.ConsumeBool() ? '\n' : '\t');
                    break;
                case 5: // Null byte
                    str.push_back('\0');
                    break;
            }
        }
        
        // Create JSON with this string
        cJSON* obj = cJSON_CreateObject();
        if (obj) {
            cJSON_AddItemToObject(obj, "test_string", cJSON_CreateString(str.c_str()));
            
            // Print it (tests print_string_ptr)
            char* printed = cJSON_Print(obj);
            if (printed) {
                free(printed);
            }
            
            cJSON_Delete(obj);
        }
    }
}

// Test complex object printing scenarios
static void test_complex_object_printing(FuzzedDataProvider& fdp) {
    // Create nested objects with various depths
    int depth = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    cJSON* root = cJSON_CreateObject();
    cJSON* current = root;
    
    for (int i = 0; i < depth; i++) {
        cJSON* child = cJSON_CreateObject();
        if (!child) break;
        
        // Add some items to the object
        int num_items = fdp.ConsumeIntegralInRange<int>(1, 5);
        for (int j = 0; j < num_items; j++) {
            uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            
            switch (item_type) {
                case 0:
                    cJSON_AddItemToObject(child, 
                        fdp.ConsumeRandomLengthString(10).c_str(),
                        cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                    break;
                case 1:
                    cJSON_AddItemToObject(child,
                        fdp.ConsumeRandomLengthString(10).c_str(),
                        cJSON_CreateString(fdp.ConsumeRandomLengthString(20).c_str()));
                    break;
                case 2:
                    cJSON_AddItemToObject(child,
                        fdp.ConsumeRandomLengthString(10).c_str(),
                        cJSON_CreateBool(fdp.ConsumeBool()));
                    break;
                case 3:
                    {
                        cJSON* arr = cJSON_CreateArray();
                        if (arr) {
                            int arr_size = fdp.ConsumeIntegralInRange<int>(0, 3);
                            for (int k = 0; k < arr_size; k++) {
                                cJSON_AddItemToArray(arr, 
                                    cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                            }
                            cJSON_AddItemToObject(child,
                                fdp.ConsumeRandomLengthString(10).c_str(),
                                arr);
                        }
                    }
                    break;
            }
        }
        
        // Add child to current level
        char key[32];
        snprintf(key, sizeof(key), "level_%d", i);
        cJSON_AddItemToObject(current, key, child);
        
        // Move to child for next level (if not last iteration)
        if (i < depth - 1) {
            current = child;
        }
    }
    
    // Test printing with various formats
    if (root) {
        // Unformatted print
        char* unformatted = cJSON_PrintUnformatted(root);
        if (unformatted) {
            free(unformatted);
        }
        
        // Formatted print
        char* formatted = cJSON_Print(root);
        if (formatted) {
            free(formatted);
        }
        
        // Buffered print
        char* buffered = cJSON_PrintBuffered(root, 1024, fdp.ConsumeBool());
        if (buffered) {
            free(buffered);
        }
        
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test 1: Invalid type comparisons for cJSON_Compare
    // Create objects with invalid types and compare them
    cJSON* invalid_obj1 = create_invalid_type_object(fdp);
    cJSON* invalid_obj2 = create_invalid_type_object(fdp);
    cJSON* valid_obj = cJSON_CreateObject();
    
    if (invalid_obj1 && invalid_obj2 && valid_obj) {
        // Compare invalid with invalid
        cJSON_Compare(invalid_obj1, invalid_obj2, fdp.ConsumeBool());
        
        // Compare invalid with valid
        cJSON_Compare(invalid_obj1, valid_obj, fdp.ConsumeBool());
        cJSON_Compare(valid_obj, invalid_obj2, fdp.ConsumeBool());
        
        // Test object with invalid child
        cJSON* obj_with_invalid_child = create_object_with_invalid_child(fdp);
        if (obj_with_invalid_child) {
            // Compare with valid object
            cJSON_Compare(obj_with_invalid_child, valid_obj, fdp.ConsumeBool());
            cJSON_Compare(valid_obj, obj_with_invalid_child, fdp.ConsumeBool());
            
            // Compare with itself
            cJSON_Compare(obj_with_invalid_child, obj_with_invalid_child, fdp.ConsumeBool());
            
            cJSON_Delete(obj_with_invalid_child);
        }
    }
    
    // Clean up
    if (invalid_obj1) cJSON_Delete(invalid_obj1);
    if (invalid_obj2) cJSON_Delete(invalid_obj2);
    if (valid_obj) cJSON_Delete(valid_obj);
    
    // Test 2: Edge case number parsing (parse_number)
    if (fdp.remaining_bytes() > 100) {
        test_parse_number_edge_cases(fdp);
    }
    
    // Test 3: String edge cases (parse_string, print_string_ptr)
    if (fdp.remaining_bytes() > 100) {
        test_string_edge_cases(fdp);
    }
    
    // Test 4: Complex object printing (print_object)
    if (fdp.remaining_bytes() > 200) {
        test_complex_object_printing(fdp);
    }
    
    // Additional tests: Normal comparison scenarios with varied parameters
    // to ensure we also exercise the valid comparison paths
    if (fdp.remaining_bytes() > 50) {
        cJSON* obj1 = cJSON_CreateObject();
        cJSON* obj2 = cJSON_CreateObject();
        
        if (obj1 && obj2) {
            // Add some items
            cJSON_AddItemToObject(obj1, "key1", cJSON_CreateString("value1"));
            cJSON_AddItemToObject(obj2, "key1", cJSON_CreateString("value1"));
            cJSON_AddItemToObject(obj1, "key2", cJSON_CreateNumber(42.0));
            cJSON_AddItemToObject(obj2, "key2", cJSON_CreateNumber(42.0));
            
            // Test case sensitive and insensitive
            cJSON_Compare(obj1, obj2, true);   // case sensitive
            cJSON_Compare(obj1, obj2, false);  // case insensitive
            
            // Test with different values
            cJSON* obj3 = cJSON_CreateObject();
            if (obj3) {
                cJSON_AddItemToObject(obj3, "key1", cJSON_CreateString("DIFFERENT"));
                cJSON_AddItemToObject(obj3, "key2", cJSON_CreateNumber(99.0));
                
                cJSON_Compare(obj1, obj3, fdp.ConsumeBool());
                cJSON_Delete(obj3);
            }
        }
        
        if (obj1) cJSON_Delete(obj1);
        if (obj2) cJSON_Delete(obj2);
    }
    
    // Test parsing with various inputs
    if (fdp.remaining_bytes() > 10) {
        std::string json_input = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
        cJSON* parsed = cJSON_Parse(json_input.c_str());
        if (parsed) {
            cJSON_Delete(parsed);
        }
        
        // Also test with parse opts
        if (fdp.remaining_bytes() > 10) {
            const char* parse_end = NULL;
            std::string json_input2 = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
            cJSON* parsed2 = cJSON_ParseWithOpts(json_input2.c_str(), &parse_end, fdp.ConsumeBool());
            if (parsed2) {
                cJSON_Delete(parsed2);
            }
        }
    }
    
    return 0;
}

} // namespace harness_022
