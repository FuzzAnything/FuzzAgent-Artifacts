#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <climits>
#include <cfloat>
#include "cjson/cJSON.h"

namespace harness_011 {
/*
 * harness_011.cpp - cJSON_Compare deep nested structure and edge case testing harness
 * 
 * This harness targets remaining blocked branches in cJSON_Compare (64 blocked branches)
 * with focus on areas not covered by harness_010:
 * 1) Deep nested structure comparisons (arrays/objects within arrays/objects)
 * 2) Edge cases with special characters in strings (unicode, control chars, null bytes)
 * 3) Floating-point number precision edge cases (very small differences near DBL_EPSILON)
 * 4) Array/object size mismatch scenarios  
 * 5) Complex type mismatch scenarios (invalid types, mixed types)
 * 6) Null/boolean edge cases including identical object pointer comparison (a == b)
 * 7) Testing O(n^2) object comparison algorithm with various key arrangements
 * 
 * Strategy: Create complex nested structures, test edge cases in compare_double(),
 * exercise strcmp() paths with challenging strings, and test type validation logic.
 */


extern "C" {
}

// Helper to create deeply nested arrays
static cJSON* create_deep_nested_array(FuzzedDataProvider& fdp, int depth) {
    if (depth <= 0 || fdp.remaining_bytes() < 16) {
        // Create leaf nodes
        uint8_t leaf_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (leaf_type) {
            case 0: return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            case 1: return cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
            case 2: return cJSON_CreateBool(fdp.ConsumeBool());
            case 3: return cJSON_CreateNull();
            default: return cJSON_CreateNull();
        }
    }
    
    cJSON* array = cJSON_CreateArray();
    if (!array) return nullptr;
    
    int num_items = fdp.ConsumeIntegralInRange<int>(1, 5);
    for (int i = 0; i < num_items && fdp.remaining_bytes() > 8; i++) {
        cJSON* item = create_deep_nested_array(fdp, depth - 1);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    
    return array;
}

// Helper to create deeply nested objects
static cJSON* create_deep_nested_object(FuzzedDataProvider& fdp, int depth) {
    if (depth <= 0 || fdp.remaining_bytes() < 16) {
        // Create leaf nodes with simple values
        cJSON* obj = cJSON_CreateObject();
        if (!obj) return nullptr;
        
        std::string key = "k" + std::to_string(fdp.ConsumeIntegral<uint32_t>() % 100);
        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (value_type) {
            case 0:
                cJSON_AddNumberToObject(obj, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                break;
            case 1:
                cJSON_AddStringToObject(obj, key.c_str(), fdp.ConsumeRandomLengthString(32).c_str());
                break;
            case 2:
                cJSON_AddBoolToObject(obj, key.c_str(), fdp.ConsumeBool());
                break;
            case 3:
                cJSON_AddNullToObject(obj, key.c_str());
                break;
        }
        return obj;
    }
    
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return nullptr;
    
    int num_items = fdp.ConsumeIntegralInRange<int>(1, 5);
    for (int i = 0; i < num_items && fdp.remaining_bytes() > 16; i++) {
        std::string key = "key" + std::to_string(i) + "_" + 
                         std::to_string(fdp.ConsumeIntegral<uint32_t>() % 1000);
        
        // 50% chance of nested object, 50% chance of other type
        if (fdp.ConsumeBool()) {
            cJSON* nested = create_deep_nested_object(fdp, depth - 1);
            if (nested) {
                cJSON_AddItemToObject(obj, key.c_str(), nested);
            }
        } else {
            uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 5;
            switch (value_type) {
                case 0:
                    cJSON_AddNumberToObject(obj, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                    break;
                case 1:
                    cJSON_AddStringToObject(obj, key.c_str(), fdp.ConsumeRandomLengthString(64).c_str());
                    break;
                case 2:
                    cJSON_AddBoolToObject(obj, key.c_str(), fdp.ConsumeBool());
                    break;
                case 3:
                    cJSON_AddNullToObject(obj, key.c_str());
                    break;
                case 4:
                    cJSON_AddRawToObject(obj, key.c_str(), fdp.ConsumeRandomLengthString(64).c_str());
                    break;
            }
        }
    }
    
    return obj;
}

// Test special string edge cases
static void test_string_edge_cases(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    // Test with strings containing special characters
    std::vector<std::string> special_strings;
    
    // Generate strings with various special characters
    for (int i = 0; i < 5 && fdp.remaining_bytes() > 8; i++) {
        std::string str;
        int length = fdp.ConsumeIntegralInRange<int>(1, 50);
        
        for (int j = 0; j < length && fdp.remaining_bytes() > 0; j++) {
            uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 8;
            switch (char_type) {
                case 0: str += '\0'; break;  // Null byte
                case 1: str += '\n'; break;  // Newline
                case 2: str += '\t'; break;  // Tab
                case 3: str += '\\'; break;  // Backslash
                case 4: str += '\"'; break;  // Quote
                case 5: 
                    // Unicode-like characters (simplified)
                    str += static_cast<char>(0xC0 + fdp.ConsumeIntegral<uint8_t>() % 64);
                    break;
                case 6:
                    str += static_cast<char>(0x80 + fdp.ConsumeIntegral<uint8_t>() % 64);
                    break;
                case 7:
                    str += static_cast<char>(' ' + fdp.ConsumeIntegral<uint8_t>() % 95);
                    break;
            }
        }
        special_strings.push_back(str);
    }
    
    // Compare these strings in various combinations
    for (size_t i = 0; i < special_strings.size(); i++) {
        for (size_t j = 0; j < special_strings.size(); j++) {
            cJSON* str1 = cJSON_CreateString(special_strings[i].c_str());
            cJSON* str2 = cJSON_CreateString(special_strings[j].c_str());
            
            if (str1 && str2) {
                cJSON_Compare(str1, str2, case_sensitive);
                
                // Also test as raw JSON
                cJSON* raw1 = cJSON_CreateRaw(special_strings[i].c_str());
                cJSON* raw2 = cJSON_CreateRaw(special_strings[j].c_str());
                
                if (raw1 && raw2) {
                    cJSON_Compare(raw1, raw2, case_sensitive);
                }
                
                if (raw1) cJSON_Delete(raw1);
                if (raw2) cJSON_Delete(raw2);
            }
            
            if (str1) cJSON_Delete(str1);
            if (str2) cJSON_Delete(str2);
        }
    }
}

// Test floating-point precision edge cases near DBL_EPSILON
static void test_float_precision_edge_cases(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    // Test numbers that are very close but not equal according to compare_double
    double base = fdp.ConsumeFloatingPoint<double>();
    
    // Test with DBL_EPSILON multiples
    const double eps = DBL_EPSILON;
    std::vector<double> test_values = {
        base,
        base * (1.0 + eps),        // Just outside tolerance
        base * (1.0 + eps * 0.5),  // Just inside tolerance  
        base * (1.0 - eps),
        base * (1.0 - eps * 0.5),
        base + eps * fabs(base),
        base - eps * fabs(base),
        base + eps * fabs(base) * 2.0,  // Definitely different
        base - eps * fabs(base) * 2.0,
    };
    
    // Also test special floating point values
    test_values.push_back(NAN);
    test_values.push_back(INFINITY);
    test_values.push_back(-INFINITY);
    test_values.push_back(0.0);
    test_values.push_back(-0.0);
    test_values.push_back(DBL_MIN);
    test_values.push_back(DBL_MAX);
    
    // Compare all combinations
    for (size_t i = 0; i < test_values.size(); i++) {
        for (size_t j = 0; j < test_values.size(); j++) {
            cJSON* num1 = cJSON_CreateNumber(test_values[i]);
            cJSON* num2 = cJSON_CreateNumber(test_values[j]);
            
            if (num1 && num2) {
                cJSON_Compare(num1, num2, case_sensitive);
            }
            
            if (num1) cJSON_Delete(num1);
            if (num2) cJSON_Delete(num2);
        }
    }
}

// Test identical object pointer case (a == b)
static void test_identical_pointer_case(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    // Create objects and compare them with themselves
    cJSON* obj = cJSON_CreateObject();
    if (obj) {
        // Add some items
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 8; i++) {
            std::string key = "key" + std::to_string(i);
            cJSON_AddNumberToObject(obj, key.c_str(), fdp.ConsumeFloatingPoint<double>());
        }
        
        // Test identical pointer comparison
        cJSON_Compare(obj, obj, case_sensitive);
        cJSON_Delete(obj);
    }
    
    // Test with other types too
    cJSON* arr = cJSON_CreateArray();
    if (arr) {
        cJSON_Compare(arr, arr, case_sensitive);
        cJSON_Delete(arr);
    }
    
    cJSON* str = cJSON_CreateString("test");
    if (str) {
        cJSON_Compare(str, str, case_sensitive);
        cJSON_Delete(str);
    }
    
    cJSON* num = cJSON_CreateNumber(42.0);
    if (num) {
        cJSON_Compare(num, num, case_sensitive);
        cJSON_Delete(num);
    }
}

// Test type mismatch scenarios
static void test_type_mismatches(FuzzedDataProvider& fdp, cJSON_bool case_sensitive) {
    // Create objects of different types and compare them
    std::vector<cJSON*> objects;
    
    // Create various types
    objects.push_back(cJSON_CreateNull());
    objects.push_back(cJSON_CreateTrue());
    objects.push_back(cJSON_CreateFalse());
    objects.push_back(cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
    objects.push_back(cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str()));
    objects.push_back(cJSON_CreateRaw(fdp.ConsumeRandomLengthString(32).c_str()));
    
    cJSON* arr = cJSON_CreateArray();
    if (arr) {
        cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
        objects.push_back(arr);
    }
    
    cJSON* obj = cJSON_CreateObject();
    if (obj) {
        cJSON_AddNumberToObject(obj, "key", 1.0);
        objects.push_back(obj);
    }
    
    // Compare all combinations (should mostly return false due to type mismatch)
    for (size_t i = 0; i < objects.size(); i++) {
        for (size_t j = 0; j < objects.size(); j++) {
            if (objects[i] && objects[j]) {
                cJSON_Compare(objects[i], objects[j], case_sensitive);
            }
        }
    }
    
    // Clean up
    for (cJSON* obj : objects) {
        if (obj) cJSON_Delete(obj);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for complex nested structures
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario selection
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Consume case sensitivity flag
    cJSON_bool case_sensitive = fdp.ConsumeBool() ? 1 : 0;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Deep nested array comparisons
            int depth1 = fdp.ConsumeIntegralInRange<int>(1, 5);
            int depth2 = fdp.ConsumeIntegralInRange<int>(1, 5);
            
            cJSON* arr1 = create_deep_nested_array(fdp, depth1);
            cJSON* arr2 = create_deep_nested_array(fdp, depth2);
            
            if (arr1 && arr2) {
                cJSON_Compare(arr1, arr2, case_sensitive);
            }
            
            if (arr1) cJSON_Delete(arr1);
            if (arr2) cJSON_Delete(arr2);
            break;
        }
        
        case 1: {
            // Scenario 1: Deep nested object comparisons with various key arrangements
            int depth1 = fdp.ConsumeIntegralInRange<int>(1, 4);
            int depth2 = fdp.ConsumeIntegralInRange<int>(1, 4);
            
            cJSON* obj1 = create_deep_nested_object(fdp, depth1);
            cJSON* obj2 = create_deep_nested_object(fdp, depth2);
            
            if (obj1 && obj2) {
                cJSON_Compare(obj1, obj2, case_sensitive);
                
                // Also test with same object but different case sensitivity
                if (fdp.ConsumeBool()) {
                    cJSON_Compare(obj1, obj2, !case_sensitive);
                }
            }
            
            if (obj1) cJSON_Delete(obj1);
            if (obj2) cJSON_Delete(obj2);
            break;
        }
        
        case 2: {
            // Scenario 2: String edge cases with special characters
            test_string_edge_cases(fdp, case_sensitive);
            break;
        }
        
        case 3: {
            // Scenario 3: Floating-point precision edge cases
            test_float_precision_edge_cases(fdp, case_sensitive);
            break;
        }
        
        case 4: {
            // Scenario 4: Identical pointer and type mismatch tests
            test_identical_pointer_case(fdp, case_sensitive);
            test_type_mismatches(fdp, case_sensitive);
            break;
        }
        
        case 5: {
            // Scenario 5: Mixed complex structures - arrays of objects, objects with arrays
            cJSON* mixed1 = cJSON_CreateObject();
            cJSON* mixed2 = cJSON_CreateObject();
            
            if (mixed1 && mixed2) {
                // Create array inside object
                cJSON* arr1 = cJSON_CreateArray();
                cJSON* arr2 = cJSON_CreateArray();
                
                if (arr1 && arr2) {
                    // Fill arrays with mixed content
                    for (int i = 0; i < 3 && fdp.remaining_bytes() > 16; i++) {
                        cJSON* nested_obj = cJSON_CreateObject();
                        if (nested_obj) {
                            cJSON_AddNumberToObject(nested_obj, "nested_key", 
                                                   fdp.ConsumeFloatingPoint<double>());
                            cJSON_AddItemToArray(arr1, nested_obj);
                        }
                        
                        cJSON* nested_arr = cJSON_CreateArray();
                        if (nested_arr) {
                            cJSON_AddItemToArray(nested_arr, 
                                                cJSON_CreateString(fdp.ConsumeRandomLengthString(16).c_str()));
                            cJSON_AddItemToArray(arr2, nested_arr);
                        }
                    }
                    
                    cJSON_AddItemToObject(mixed1, "array_field", arr1);
                    cJSON_AddItemToObject(mixed2, "array_field", arr2);
                } else {
                    if (arr1) cJSON_Delete(arr1);
                    if (arr2) cJSON_Delete(arr2);
                }
                
                // Compare the mixed structures
                cJSON_Compare(mixed1, mixed2, case_sensitive);
            }
            
            if (mixed1) cJSON_Delete(mixed1);
            if (mixed2) cJSON_Delete(mixed2);
            break;
        }
    }
    
    return 0;
}

} // namespace harness_011
