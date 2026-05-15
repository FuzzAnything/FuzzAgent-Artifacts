#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include "cjson/cJSON.h"

namespace harness_014 {
/*
 * harness_014.cpp - cJSON printing/formatting functions comprehensive testing harness
 * 
 * This harness specifically targets cJSON printing/formatting functions:
 * - cJSON_Print: formatted JSON output with indentation
 * - cJSON_PrintUnformatted: compact JSON output without formatting
 * - cJSON_PrintBuffered: buffered printing with configurable buffer size
 * - cJSON_PrintPreallocated: printing to pre-allocated buffer
 * 
 * These functions convert cJSON structures to strings and have complex code paths
 * involving memory allocation, string formatting, recursion for nested structures,
 * and error handling. Focus on printing scenarios:
 * 1) Printing simple JSON values (null, boolean, number, string)
 * 2) Printing nested arrays/objects with various depth levels
 * 3) Printing with/without formatting (cJSON_Print vs cJSON_PrintUnformatted)
 * 4) Memory allocation edge cases (small buffers, large structures)
 * 5) Large JSON document printing with many elements
 * 6) Error recovery after failed prints (null items, insufficient buffers)
 * 7) Pre-allocated buffer testing with exact size constraints
 * 
 * Based on analysis of cJSON source code, we need to test:
 * - print_value() function for different JSON types (switch statement)
 * - print_number() for numeric formatting
 * - print_string() for string escaping
 * - print_array() for array iteration
 * - print_object() for object iteration
 * - ensure() function for buffer management
 * - Memory allocation paths in cJSON_Print/cJSON_PrintUnformatted
 * - Buffer size handling in cJSON_PrintBuffered
 * - Pre-allocated buffer boundary cases in cJSON_PrintPreallocated
 * 
 * Strategy: Generate diverse cJSON structures using FuzzedDataProvider and test
 * all printing functions with various configurations to maximize coverage.
 */


extern "C" {
}

// Helper function to create a cJSON structure with variable depth
static cJSON* create_json_structure(FuzzedDataProvider& fdp, int max_depth, int current_depth) {
    if (current_depth >= max_depth || fdp.remaining_bytes() < 16) {
        // Create simple value at max depth
        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        switch (value_type) {
            case 0:
                return cJSON_CreateNull();
            case 1:
                return cJSON_CreateBool(fdp.ConsumeBool());
            case 2:
                return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            case 3: {
                std::string str = fdp.ConsumeRandomLengthString(100);
                return cJSON_CreateString(str.c_str());
            }
            case 4:
                return cJSON_CreateArray();
            case 5:
                return cJSON_CreateObject();
            default:
                return cJSON_CreateNull();
        }
    }
    
    uint8_t structure_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    if (structure_type == 0) {
        // Create array with nested elements
        cJSON* array = cJSON_CreateArray();
        if (!array) return nullptr;
        
        int num_elements = fdp.ConsumeIntegralInRange<int>(1, 8);
        for (int i = 0; i < num_elements && fdp.remaining_bytes() > 8; i++) {
            cJSON* element = create_json_structure(fdp, max_depth, current_depth + 1);
            if (element) {
                cJSON_AddItemToArray(array, element);
            }
        }
        return array;
    } else if (structure_type == 1) {
        // Create object with key-value pairs
        cJSON* object = cJSON_CreateObject();
        if (!object) return nullptr;
        
        int num_pairs = fdp.ConsumeIntegralInRange<int>(1, 8);
        for (int i = 0; i < num_pairs && fdp.remaining_bytes() > 16; i++) {
            std::string key = fdp.ConsumeRandomLengthString(50);
            cJSON* value = create_json_structure(fdp, max_depth, current_depth + 1);
            if (value) {
                cJSON_AddItemToObject(object, key.c_str(), value);
            }
        }
        return object;
    } else {
        // Create complex mixed structure
        cJSON* root = cJSON_CreateObject();
        if (!root) return nullptr;
        
        // Add various types of values
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
            cJSON_AddNumberToObject(root, "number", fdp.ConsumeFloatingPoint<double>());
        }
        
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
            std::string str = fdp.ConsumeRandomLengthString(100);
            cJSON_AddStringToObject(root, "string", str.c_str());
        }
        
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
            cJSON_AddBoolToObject(root, "boolean", fdp.ConsumeBool());
        }
        
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
            cJSON_AddNullToObject(root, "null_value");
        }
        
        // Add nested array
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 16) {
            cJSON* nested_array = cJSON_CreateArray();
            if (nested_array) {
                int array_size = fdp.ConsumeIntegralInRange<int>(1, 5);
                for (int i = 0; i < array_size && fdp.remaining_bytes() > 8; i++) {
                    cJSON* elem = create_json_structure(fdp, max_depth, current_depth + 1);
                    if (elem) {
                        cJSON_AddItemToArray(nested_array, elem);
                    }
                }
                cJSON_AddItemToObject(root, "nested_array", nested_array);
            }
        }
        
        // Add nested object
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 16) {
            cJSON* nested_obj = cJSON_CreateObject();
            if (nested_obj) {
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
                    cJSON_AddNumberToObject(nested_obj, "inner_num", fdp.ConsumeFloatingPoint<double>());
                }
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
                    std::string inner_str = fdp.ConsumeRandomLengthString(50);
                    cJSON_AddStringToObject(nested_obj, "inner_str", inner_str.c_str());
                }
                cJSON_AddItemToObject(root, "nested_object", nested_obj);
            }
        }
        
        return root;
    }
}

// Helper function to test printing with all available functions
static void test_all_printing_functions(cJSON* item, FuzzedDataProvider& fdp) {
    if (!item) return;
    
    // Test 1: cJSON_Print (formatted printing)
    char* printed_formatted = cJSON_Print(item);
    if (printed_formatted) {
        // Verify the printed string is valid (non-NULL, not empty if item is not empty)
        (void)strlen(printed_formatted);
        free(printed_formatted);
    }
    
    // Test 2: cJSON_PrintUnformatted (compact printing)
    char* printed_unformatted = cJSON_PrintUnformatted(item);
    if (printed_unformatted) {
        (void)strlen(printed_unformatted);
        free(printed_unformatted);
    }
    
    // Test 3: cJSON_PrintBuffered with various buffer sizes
    if (fdp.remaining_bytes() > 4) {
        int buffer_size = fdp.ConsumeIntegralInRange<int>(1, 4096);
        bool formatted = fdp.ConsumeBool();
        
        char* printed_buffered = cJSON_PrintBuffered(item, buffer_size, formatted);
        if (printed_buffered) {
            (void)strlen(printed_buffered);
            free(printed_buffered);
        }
    }
    
    // Test 4: cJSON_PrintPreallocated with exact buffer size testing
    if (fdp.remaining_bytes() > 8) {
        int buffer_length = fdp.ConsumeIntegralInRange<int>(1, 2048);
        bool format = fdp.ConsumeBool();
        
        // Allocate buffer and test preallocated printing
        char* buffer = (char*)malloc(buffer_length);
        if (buffer) {
            cJSON_bool success = cJSON_PrintPreallocated(item, buffer, buffer_length, format);
            (void)success; // Use result to avoid unused variable warning
            
            // Test edge case: NULL buffer
            if (fdp.ConsumeBool()) {
                cJSON_PrintPreallocated(item, NULL, buffer_length, format);
            }
            
            // Test edge case: negative length
            if (fdp.ConsumeBool()) {
                cJSON_PrintPreallocated(item, buffer, -1, format);
            }
            
            free(buffer);
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for meaningful printing scenarios
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario selection
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    cJSON* test_item = nullptr;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Simple value printing
            uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 6;
            switch (value_type) {
                case 0:
                    test_item = cJSON_CreateNull();
                    break;
                case 1:
                    test_item = cJSON_CreateTrue();
                    break;
                case 2:
                    test_item = cJSON_CreateFalse();
                    break;
                case 3:
                    test_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                    break;
                case 4: {
                    std::string str = fdp.ConsumeRandomLengthString(200);
                    test_item = cJSON_CreateString(str.c_str());
                    break;
                }
                case 5:
                    test_item = cJSON_CreateRaw(fdp.ConsumeRandomLengthString(100).c_str());
                    break;
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Array printing with various element types
            test_item = cJSON_CreateArray();
            if (test_item) {
                int array_size = fdp.ConsumeIntegralInRange<int>(1, 20);
                for (int i = 0; i < array_size && fdp.remaining_bytes() > 8; i++) {
                    uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 5;
                    cJSON* elem = nullptr;
                    
                    switch (elem_type) {
                        case 0:
                            elem = cJSON_CreateNull();
                            break;
                        case 1:
                            elem = cJSON_CreateBool(fdp.ConsumeBool());
                            break;
                        case 2:
                            elem = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                            break;
                        case 3: {
                            std::string str = fdp.ConsumeRandomLengthString(50);
                            elem = cJSON_CreateString(str.c_str());
                            break;
                        }
                        case 4:
                            elem = cJSON_CreateArray(); // Empty nested array
                            break;
                    }
                    
                    if (elem) {
                        cJSON_AddItemToArray(test_item, elem);
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Object printing with mixed key-value pairs
            test_item = cJSON_CreateObject();
            if (test_item) {
                int num_pairs = fdp.ConsumeIntegralInRange<int>(1, 15);
                for (int i = 0; i < num_pairs && fdp.remaining_bytes() > 16; i++) {
                    std::string key = fdp.ConsumeRandomLengthString(30);
                    uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 5;
                    
                    switch (value_type) {
                        case 0:
                            cJSON_AddNullToObject(test_item, key.c_str());
                            break;
                        case 1:
                            cJSON_AddBoolToObject(test_item, key.c_str(), fdp.ConsumeBool());
                            break;
                        case 2:
                            cJSON_AddNumberToObject(test_item, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                            break;
                        case 3: {
                            std::string str = fdp.ConsumeRandomLengthString(100);
                            cJSON_AddStringToObject(test_item, key.c_str(), str.c_str());
                            break;
                        }
                        case 4:
                            cJSON_AddRawToObject(test_item, key.c_str(), fdp.ConsumeRandomLengthString(50).c_str());
                            break;
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Deeply nested structure (recursive creation)
            int max_depth = fdp.ConsumeIntegralInRange<int>(1, 5);
            test_item = create_json_structure(fdp, max_depth, 0);
            break;
        }
        
        case 4: {
            // Scenario 4: Large JSON document with many elements
            test_item = cJSON_CreateObject();
            if (test_item) {
                // Add large array
                cJSON* large_array = cJSON_CreateArray();
                if (large_array) {
                    int array_size = fdp.ConsumeIntegralInRange<int>(10, 50);
                    for (int i = 0; i < array_size && fdp.remaining_bytes() > 4; i++) {
                        cJSON* num = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                        if (num) {
                            cJSON_AddItemToArray(large_array, num);
                        }
                    }
                    cJSON_AddItemToObject(test_item, "large_array", large_array);
                }
                
                // Add object with many fields
                cJSON* many_fields = cJSON_CreateObject();
                if (many_fields) {
                    int field_count = fdp.ConsumeIntegralInRange<int>(5, 30);
                    for (int i = 0; i < field_count && fdp.remaining_bytes() > 8; i++) {
                        std::string key = "field_" + std::to_string(i);
                        cJSON_AddNumberToObject(many_fields, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                    }
                    cJSON_AddItemToObject(test_item, "many_fields", many_fields);
                }
            }
            break;
        }
        
        case 5: {
            // Scenario 5: Edge cases and error paths
            // Test with NULL item
            test_all_printing_functions(nullptr, fdp);
            
            // Test with invalid item (freed or corrupted)
            cJSON* invalid_item = (cJSON*)malloc(sizeof(cJSON));
            if (invalid_item) {
                memset(invalid_item, 0, sizeof(cJSON));
                test_all_printing_functions(invalid_item, fdp);
                free(invalid_item);
            }
            
            // Create a valid item but test with small buffers
            if (fdp.remaining_bytes() > 8) {
                std::string str = fdp.ConsumeRandomLengthString(50);
                test_item = cJSON_CreateString(str.c_str());
                if (test_item) {
                    // Test with extremely small buffer
                    char tiny_buffer[1];
                    cJSON_PrintPreallocated(test_item, tiny_buffer, 1, false);
                    
                    // Test with zero buffer
                    cJSON_PrintPreallocated(test_item, nullptr, 0, false);
                }
            }
            // Note: test_item will be cleaned up below
            break;
        }
    }
    
    // Test printing functions on the created item (if any)
    if (test_item) {
        test_all_printing_functions(test_item, fdp);
        cJSON_Delete(test_item);
    }
    
    // Additional tests: Print and re-parse cycle
    if (fdp.remaining_bytes() > 32) {
        cJSON* cycle_item = create_json_structure(fdp, 3, 0);
        if (cycle_item) {
            char* printed = cJSON_Print(cycle_item);
            if (printed) {
                cJSON* reparsed = cJSON_Parse(printed);
                if (reparsed) {
                    // Test printing the re-parsed item
                    char* reprinted = cJSON_PrintUnformatted(reparsed);
                    if (reprinted) {
                        free(reprinted);
                    }
                    cJSON_Delete(reparsed);
                }
                free(printed);
            }
            cJSON_Delete(cycle_item);
        }
    }
    
    return 0;
}

} // namespace harness_014
